# Not Yet Ready For Use

While largely feature-complete, the API is not yet stable and it is
largely untested.  Please don't depend on it for anything critical
yet.

Also, the project name will probably change, if anyone has any ideas
please let me know.  I don't like the current one, but I wanted to
write code not think about a name.

# WASMV: a safer way to target WASM SIMD from C and C++

WASMV is an alternative to the
[`wasm_simd128.h`](https://github.com/llvm/llvm-project/blob/main/clang/lib/Headers/wasm_simd128.h)
header which ships with LLVM.

## Why?

`wasm_simd128.h` is not a bad API.  Frankly, I'm impressed with how
succinctly it is able to express the underlying WASM SIMD
instructions, and how closely it matches the semantics.  If the goal
is to expose the underlying WASM SIMD in the most straightforward way
I'd say they did a phenomenal job.

WASMV, on the other hand, takes a very different approach.  WASMV's
primary goal is safety and, to the extent it doesn't sacrifice safety,
convenience.  The result is an API which is (IMHO) pleasant to use and
helps you avoid shooting yourself in the foot.  While the API is
simple and unsurprising, `wasmv.h` is currently almost 6 times the
size of `wasm_simd128.h`.

Both APIs are limited to what the underlying [WASM SIMD
specification](https://github.com/WebAssembly/simd/) supports.  For
example, WASM SIMD only supports saturating integer addition of 16-bit
numbers.  We could easily add some code to emulate saturated 32-bit
addition, but that's not our place; neither API is an abstraction
layer, they are an interface to the WASM SIMD functionality that
actually exists.

So, what makes WASMV so different?

### Safety First

The primary goal is a safer API which is easy to use, but also hard to
*misuse*.  Okay, it's still C/C++, so maybe "less unsafe" would be
more appropriate (less inappropriate?), but you get the idea… don't @
me, rustaceans!

Where `wasm_simd128.h` offers one type (`v128_t`), WASMV has 14:

 * Signed integers
   * `wasmv_i8x16_t`
   * `wasmv_i16x8_t`
   * `wasmv_i32x4_t`
   * `wasmv_i64x2_t`
 * Unsigned integers
   * `wasmv_u8x16_t`
   * `wasmv_u16x8_t`
   * `wasmv_u32x4_t`
   * `wasmv_u64x2_t`
 * Booleans
   * `wasmv_b8x16_t`
   * `wasmv_b16x8_t`
   * `wasmv_b32x4_t`
   * `wasmv_b64x2_t`
 * Floating-point
   * `wasmv_f32x4_t`
   * `wasmv_f64x2_t`

This allows us to enlist the compiler's help to make sure we don't
accidentally try to add, for example, a vector of 32-bit floats to a
vector of 16-bit integers.  Here are the relevant functions from
`wasm_simd128.h`:

```c
v128_t wasm_i8x16_add(v128_t a, v128_t b);
v128_t wasm_f32x4_add(v128_t a, v128_t b);
```

And the relevant functions from WASMV:

```c
wasmv_i8x16_t wasmv_i8x16_add(wasmv_i8x16_t a, wasmv_i8x16_t b);
wasmv_f32x4_t wasmv_f32x4_add(wasmv_f32x4_t a, wasmv_f32x4_t b);
```

As you can see, nothing prevents you from passing a vector of 16 8-bit
signed integers to `wasm_f32x4_add`; 4 8-bit values will be
reinterpreted as (not converted to) a single 32-bit floating point
value, and the result will likely be complete garbage.

On the WASMV side of things, however, you'll end up with an error from
the compiler about passing a `wasmv_i8x16_t` to a parameter which
expects a `wasmv_f32x4_t`.

Of course, sometimes you really do need to treat a bunch of 8-bit
integers (or, more likely, 32-bit integers) as floats.  WASMV includes
functions to do that for you (without aliasing violations / undefined
behavior; please don't just do `*((wasmv_f32x4_t*) &value)`!).  For
example:

```c
wasmv_f32x4_t wasmv_i8x16_as_f32x4(wasmv_i8x16_t value);
wasmv_i8x16_t wasmv_f32x4_as_i8x16(wasmv_f32x4_t value);
```

Yes, it's a bit more verbose than just passing the value directly to
`wasm_f32x4_add`, and some people will hate that.  If that's you then
you'll probably be happier with `wasm_simd128.h`.

The idea of a type-safe SIMD API isn't new.  For example:

 * NEON has, for example, `int8x16_t` and `float32x4_t`.
 * SVE has `svint8x16_t` and `svfloat32x4_t`.
 * AltiVec and System Z add a `vector` keyword to the language, so
   they have types like `vector signed char` and `vector float`.
 * MIPS uses `v16i8` and `v4f32`.

Even the x86 APIs (SSE, AVX, AVX-512, etc.) aren't as aggressively
untyped as `wasm_simd128.h` as it has distinct types for floats
(`__m128`) and doubles (`__m128d`); only integer vectors use one type
(`__m128i`) for everything.  `wasm_simd128.h` is really the exception,
not the rule.

### Overload Overload

Lots of types aren't the only thing which distinguishes WASMV from
`wasm_simd128.h`.  WASMV also includes overloaded APIs, which you can
use like this:

```c
wasmv_f32x4_t
multiply_add (wasmv_f32x4_t a, wasmv_f32x4_t b, wasmv_f32x4_t) {
  return wasmv_add(wasmv_mul(a, b), c);
}
```

This is implemented using C11 generic selections or C++ method
overloading.  In C++ WASMV also supports operator overloading, so if
you prefer you also do this:

```c
wasmv_f32x4_t
multiply_add (wasmv_f32x4_t a, wasmv_f32x4_t b, wasmv_f32x4_t) {
  return (a * b) + c;
}
```

Operator overloading doesn't work in C, so if you want your code to
work in both languages you'll have to use the named versions.
However, the *named* overloads (*e.g.*, `wasmv_add`) do work in C,
even if you're not targeting C11.

Of course the type system still protects you; you can't pass a
`wasmv_f32x4_t` and a `wasmv_i32x4_t` to `wasmv_add` and expect it to
work… the types have to match.

Since (AFAIK) LLVM is the only game in town for compiling C/C++ to
WebAssembly SIMD, WASMV doesn't have to worry about compatibility with
other compilers, which means we can actually use C11 generic
selections even in earlier modes; we just have to suppress the
`-Wc11-extensions` diagnostic that clang wants to emit (which we do in
`wasmv.h`, you don't have to worry about it).  That means that you get
the overloaded methods in C even if you're not working in C11.

Between the distinct types and the overloading, one way to look at
this is that WASMV takes the opposite approach as `wasm_simd128.h`.
With `wasm_simd128.h`, the type is overloaded but the functions
aren't.  With WASMV, the functions are overloaded but the types
aren't.

The nice thing about WASMV's approach is that, since it knows what
type of vector you're using, the compiler can save you from making
many horrible mistakes by combining them in invalid ways.

### It's True, WASMV Has Boolean Types

Another significant difference from `wasm_simd128.h` is the presence
of boolean types (`wasmv_b8x16_t`, `wasmv_b16x8_t`, `wasmv_32x4_t`,
and `wasmv_b64x2_t`).

Boolean types are used for the results of comparison functions, and
the value of each lane is assumed to have either all bits unset (`0`)
or all bits set (`~0`).

### Can't We All Just Get Along?

Interoperability with `wasm_simd128.h` is critical.  We don't expect
everyone to switch over to WASMV and never look back; sometimes you'll
want to call `wasm_simd128.h` code from code using WASMV, and at other
times you'll want to call a WASMV implementation from code using
`wasm_simd128.h`.

In addition to converting between WASMV's types, WASMV also contains
functions to convert to/from `wasm_simd128.h`'s type (`v128_t`).  For
example:

```c
v128_t        wasmv_f32x4_as_v128(wasmv_f32x4_t value);
wasmv_f32x4_t wasmv_v128_as_f32x4(       v128_t value);
```

This makes it easy to convert between the two APIs so you can freely
mix code written to use `wasm_simd128.h` and WASMV:

```
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

Most likely you'll be doing more work than a single add function at a
time, but the idea is the same no matter how much code there is.

## Future Directions

WASMV is fairly complete; the functionality in WASMV should match the
functionality in WASM SIMD (I may have missed a function here and
there, please file an issue if you find one!), so it is largely
feature complete.  There are a few function which need to be
implemented in LLVM before we can really make use of them in WASMV,
though some can be emulated if you pass `-munimplemented-simd128` to
the compiler.


WASMV is, however, not yet API stable.  Depending on feedback things
may change.  I don't expect many API-breaking changes, but one
significant change I'm considering is to remove the lane count from
everything.  For example, `wasmv_f32_t` instead of `wasmv_f32x4_t`,
and `wasmv_f32_add` instead of `wasmv_f32x4_add`.  That would be
fairly disruptive, though easy to adapt to.

Moving forward, I think the next priority is going to be writing
tests.  A **lot** of tests.  We need to verify that every single
function compiles to the intended instruction(s) in WebAssembly.

As I mentioned earlier, adding functionality beyond what is provided
by WebAssembly SIMD is outside of WASMV's scope, which means once
WASMV's API is stable and complete we should be done, at least until
changes are made to the specification.
