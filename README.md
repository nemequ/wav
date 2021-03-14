# NOT READY YET

This is currently nowhere near complete, and the API is not stable.

Also, the project name will probably change, if anyone has any ideas
please let me know.  I don't like the current one, but I wanted to
write code not think about a name.

# WASMV

WASMV is an alternative to the wasm_simd128.h API provided by LLVM. It
is primarily designed to provide a more type-safe API.

The API is a bit of a blending of NEON, AltiVec, and SSE:

 * The types feel a bit like NEON; it has different vector types for
   each element type (8/16/32/64-bit ints, signed and unsigned, as
   well as 32/64-bit floats) without requiring a keyword (like
   "vector" in AltiVec).
 * The (optional) type-generic functions feel like AltiVec (e.g.,
   `wasmv_add` is similar to `vec_add`).
 * It only provides the features of the underlying WASM SIMD
   instruction set, which means annoying gaps in the API which provide
   a definite SSE/AVX feel as well.

The end result is something which looks like this:

```c
#include <wasmv.h>

void
add_data(
    size_t element_count,
    float sum[], float a[], float b[]) {
  for (size_t o = 0 ; o < element_count ; o += 4) {
    wasmv_f32x4_t av = wasmv_load(&(a[i]));
    wasmv_f32x4_t bv = wasmv_load(&(b[i]));

    wasmv_f32x4_t r = wasmv_add(av, bv);

    wasmv_store(&(sum[i]), r);
  }
}
```

Or, if you prefer:

```c
#include <wasmv.h>

void
add_data(
    size_t element_count,
    float sum[], float a[], float b[]) {
  for (size_t o = 0 ; o < element_count ; o += 4) {
    wasmv_f32x4_t av = wasmv_f32x4_load(&(a[i]));
    wasmv_f32x4_t bv = wasmv_f32x4_load(&(b[i]));

    wasmv_f32x4_t r = wasmv_f32x4_add(av, bv);

    wasmv_f32x4_store(&(sum[i]), r);
  }
}
```

In C++ there are overloaded operators in addition to the overloaded function names (i.e., `wasmv_add(a, b) == (a + b)`).

WASMV will make it as difficult as we can to use incorrectly.  For
example:

```c
wasmv_i8x16_t foo(wasmv_i16x4_t bar, wasmv_i32x2_t baz) {
  return wasm_add(bar, baz);
}
```

Will trigger compile-time errors. You can't add a vector of 8-bit
signed integers to a vector of 16-bit signed integers.

Note that all of this happens exclusively at compile time; this really
is intended as an alternative to wasm_simd128.h, and the most
important part of that is retaining the performance.  Functions almost
always compile to a single instruction just like they do with
wasm_simd128.h (there are a few which don't, but wasm_simd128.h
doesn't either).

WASMV also provides functions to convert between its types and
`v128_t` from wasm_simd128.h, so you can freely switch between APIs if
you need to:

```c
wasmv_i8x16_t
add_using_wasm_simd128(wasmv_i8x16_t a, wasmv_i8x16_t b) {
  return
    wasmv_v128_as_i8x16(
      wasmv_add_i8x16(
        wasmv_i8x16_as_v128(a),
        wasmv_i8x16_as_v128(b)
      );
    );
}

v128_t
add_using_wasmv(v128_t a, v128_t b) {
  return
    wasmv_i8x16_to_v128(
      wasm_i8x16_add(
        wasmv_v128_to_i8x16(a),
        wasmv_v128_to_i8x16(b)
      )
    );
}
```

Obviously you'll want to operate on larger sections of code than a
single addition, but if you have bunch of existing wasm_simd128.h code
this is a pretty straightforward way to integrate bidirectionally.

## Boolean Types

WASMV has dedicated boolean types (`wasmv_b8x16_t`, `wasmv_b16x8_t`,
etc.).  These are intended to only hold values where every lane
contains either all zeros or all ones.  While it is possible to violate
this assumption by using the `wasmv_*_as_*` functions, the API
considers this to be undefined behavior.

## Non-standard names

WASMV adds some other interfaces which aren't present in WASM SIMD.
These are usually just friendlier ways to access functionality which
does exist, but these APIs should be considered to be especially
unstable since we may change the names in the future to match
`wasm_simd128.h` if they adopt similar interfaces.

 * `make` -- present in `wasm_simd128.h`, but not WASM SIMD.
 * `blend` -- similar to `bitselect`, but slightly different semantics.
 * `move` -- `load_zero`, but takes value types instead of pointer
   types.
 * `loadu`/`storeu` -- there are really closer to what the WASM SIMD
   spec does than WASMV's `load`/`store`, but they're not type-safe.
 * `loadu_zero` -- same situation as `loadu`/`storeu`.

## FAQ

* *Can I use it without targeting WASM?*
   
  No, WASMV isn't an abstraction layer, though honestly the effort
  required to turn it into one is pretty low… *Most* functions are
  implemented using portable code instead of calling builtins.

  If WASMV becomes popular I will probably add an implementation
  to [SIMDe](https://github.com/simd-everywhere/simde), so if you're
  using it please let us know!

* *Why not just use `wasm_simd128.h`?*

  `wasm_simd128.h` is intended as a straightforward translation of the
  WASM SIMD specification into C.  WASMV, on the other hand, looks at
  the WASM SIMD spec and asks how to present it in a way which makes it
  easy to use and difficult to misuse by enlisting the compiler's help
  as much as possible.

  Frankly, it comes down to personal prefernce (or at least it will
  once WASMV implements the full specrification).  I prefer a style
  more like WASMV, which I why I'm writing it, but I'm sure others will
  prefer the `wasm_simd128.h` style.

* *So it's a wrapper around `wasm_simd128.h`?*

  Not really, no.  We do `#include <wasm_simd128.h>` but that's only
  so we can create the functions to convert to/from `v128_t`.  We don't
  use the header for anything else; functions are either implemented
  using portable code which clang recognizes and generates the correct
  instructions for or we call the builtins directly.

* *"WASMV" is a stupid name.*

  Yes it is. I don't like it either.  If you can think of something
  better let me know, I want to change the name ASAP (before people
  start really using it).
