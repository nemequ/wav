# Not Yet Ready For Use

While largely feature-complete, the API is not yet stable and it is
largely untested.  Please don't depend on it for anything critical yet.

Also, the project name will probably change, if anyone has any ideas
please let me know.  I don't like the current one, but I wanted to
write code not think about a name.

# WASMV

WASMV is an alternative to the wasm_simd128.h API provided by LLVM. It
is primarily designed to provide a more type-safe API.

A quick example:

```c
#include <wasmv.h>

void
add_data(
    size_t element_count,
    float sum[], float a[], float b[]) {
  for (size_t o = 0 ; o < element_count ; o += 4) {
    wasmv_f32x4_t av = wasmv_load(&(a[o]));
    wasmv_f32x4_t bv = wasmv_load(&(b[o]));

    wasmv_f32x4_t r = wasmv_add(av, bv);

    wasmv_store(&(sum[o]), r);
  }
}
```

Or, if you prefer to use non-overloaded functions:

```c
#include <wasmv.h>

void
add_data(
    size_t element_count,
    float sum[], float a[], float b[]) {
  for (size_t o = 0 ; o < element_count ; o += 4) {
    wasmv_f32x4_t av = wasmv_f32x4_load(&(a[o]));
    wasmv_f32x4_t bv = wasmv_f32x4_load(&(b[o]));

    wasmv_f32x4_t r = wasmv_f32x4_add(av, bv);

    wasmv_f32x4_store(&(sum[o]), r);
  }
}
```

In C++ there are overloaded operators in addition to the overloaded
function names (i.e., `wasmv_add(a, b) == (a + b)`).

WASMV will make it as difficult as we can to use incorrectly.  For
example:

```c
wasmv_i8x16_t
foo(wasmv_i16x4_t bar, wasmv_i32x2_t baz) {
  return wasm_add(bar, baz);
}
```

Will trigger compile-time errors. You can't add a vector of 8-bit
signed integers to a vector of 16-bit signed integers.

Note that all of this happens exclusively at compile time; there is
no run-time checking, and almost all functions compile down to a single
instruction just like if you were using `wasm_simd128.h` (there are a
few which don't, but in those cases `wasm_simd128.h` doesn't either).

WASMV also provides functions to convert between its types and
`v128_t` from `wasm_simd128.h`, so you can freely switch between APIs
if you need to:

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
 * `loadu`/`storeu`/`loadu_zero` -- these are really closer to what the
   WASM SIMD spec does than WASMV's `load`/`store`, but they're not
   type-safe.

## FAQ

* *Can I use it without targeting WASM?*
   
  No, WASMV isn't an abstraction layer, though honestly the effort
  required to turn it into one is pretty low… *Most* functions are
  implemented using portable code instead of calling builtins.

* *Why not just use `wasm_simd128.h`?*

  `wasm_simd128.h` is a straightforward, consise way to expose the WASM
  SIMD specification in C/C++.  It does that extremely well; the
  semantics mirror the specification very closely.

  WASMV, on the other hand, has a different goal.  It tries to create a
  an API to expose the WASM SIMD specification in the safest way it can
  without creating any run-time overhead.

* *So it's a wrapper around `wasm_simd128.h`?*

  Not really, no.  We do `#include <wasm_simd128.h>` but that's only
  so we can create the functions to convert to/from `v128_t`.  We don't
  use the header for anything else; functions are either implemented
  using portable code which clang recognizes and generates the correct
  instructions for or we call the builtins directly.

  It could be made into a wrapper of `wasm_simd128.h`.  It could also
  provide the same API as `wasm_simd128.h` using the WASMV functions.

* *"WASMV" is a stupid name.*

  Yes it is. I don't like it either.  If you can think of something
  better let me know, I want to change the name ASAP (before people
  start really using it).
