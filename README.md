# WAV: a safer C/C++ API for WASM SIMD

WAV is an alternative to the
[`wasm_simd128.h`](https://github.com/llvm/llvm-project/blob/main/clang/lib/Headers/wasm_simd128.h)
header which ships with LLVM.  It is intended to be safer and easier to
use, while offering exactly the same performance.

While feature-complete, the API is not yet stable.

## Why?

`wasm_simd128.h` is not a bad API.  Frankly, I'm impressed with how
succinctly it is able to express the underlying WASM SIMD
instructions, and how closely it matches the semantics.  If the goal
is to expose the underlying WASM SIMD in the most straightforward way
I'd say they did a phenomenal job.

WAV, on the other hand, takes a very different approach.  WAV's
primary goal is safety and, to the extent it doesn't sacrifice safety,
convenience.  The result is an API which is pleasant to use and helps
you avoid shooting yourself in the foot.

Both APIs are limited to what the underlying [WASM SIMD
specification](https://github.com/WebAssembly/simd/) supports.  For
example, WASM SIMD only supports saturating integer addition of 16-bit
numbers.  We could easily add some code to emulate saturated 32-bit
addition, but that's not our place; neither API is an abstraction
layer, they are an interface to the WASM SIMD functionality that
actually exists.

Both APIs also offer exactly the same performance.  Every function in
WAV is tested to make sure it outputs the expected WebAssembly
instruction(s), which are the same ones `wasm_simd128.h` outputs.

So, what makes WAV so different?

### Safety First

The primary goal is a safer API which is easy to use, but also hard to
*misuse*.  Okay, it's still C/C++, so maybe "less unsafe" would be
more appropriate (less inappropriate?), but you get the idea… don't @
me, rustaceans!

Where `wasm_simd128.h` offers one type (`v128_t`), WAV has 14:

 * Signed integers
   * `wav_i8x16_t`
   * `wav_i16x8_t`
   * `wav_i32x4_t`
   * `wav_i64x2_t`
 * Unsigned integers
   * `wav_u8x16_t`
   * `wav_u16x8_t`
   * `wav_u32x4_t`
   * `wav_u64x2_t`
 * Booleans
   * `wav_b8x16_t`
   * `wav_b16x8_t`
   * `wav_b32x4_t`
   * `wav_b64x2_t`
 * Floating-point
   * `wav_f32x4_t`
   * `wav_f64x2_t`

This allows us to enlist the compiler's help to make sure we don't
accidentally try to, for example, add a vector of 32-bit floats to a
vector of 16-bit integers.  Here are the relevant functions from
`wasm_simd128.h`:

```c
v128_t wasm_i8x16_add(v128_t a, v128_t b);
v128_t wasm_f32x4_add(v128_t a, v128_t b);
```

And the relevant functions from WAV:

```c
wav_i8x16_t wav_i8x16_add(wav_i8x16_t a, wav_i8x16_t b);
wav_f32x4_t wav_f32x4_add(wav_f32x4_t a, wav_f32x4_t b);
```

As you can see, nothing prevents you from passing a vector of 16 8-bit
signed integers to `wasm_f32x4_add`; 4 8-bit values will be
interpreted as (not converted to) a single 32-bit floating point
value, and the result will likely be complete garbage.

On the WAV side of things, however, you'll end up with an error from
the compiler about passing a `wav_i8x16_t` to a parameter which
expects a `wav_f32x4_t`.

Of course, sometimes you really do need to treat a bunch of 8-bit
integers (or, more likely, 32-bit integers) as floats.  WAV includes
functions to do that for you (without aliasing violations / undefined
behavior; please don't just do `*((wav_f32x4_t*) &value)`!).  For
example:

```c
wav_f32x4_t wav_i8x16_as_f32x4(wav_i8x16_t value);
wav_i8x16_t wav_f32x4_as_i8x16(wav_f32x4_t value);
```

Yes, it's a bit more verbose than just passing the value directly to
`wasm_f32x4_add`, and some people will hate that.  However, I think
most people will find that the API is actually very easy to use, and
well worth the safety features.

The idea of a type-safe SIMD API isn't new.  For example:

 * NEON has types like `int8x16_t` and `float32x4_t`.
 * SVE has `svint8x16_t` and `svfloat32x4_t`.
 * AltiVec and System Z add a `vector` keyword to the language, so
   they have types like `vector signed char` and `vector float`.
 * MIPS uses `v16i8` and `v4f32`.

Even the x86 APIs (SSE, AVX, AVX-512, etc.) aren't as aggressively
uni-typed as `wasm_simd128.h` as it has distinct types for floats
(`__m128`) and doubles (`__m128d`); only integer vectors use one type
(`__m128i`) for everything.  `wasm_simd128.h` is really the exception,
not the rule.

### Overload Overload

Lots of types aren't the only thing which distinguishes WAV from
`wasm_simd128.h`.  WAV also includes overloaded APIs, which you can
use like this:

```c
wav_f32x4_t
multiply_add (wav_f32x4_t a, wav_f32x4_t b, wav_f32x4_t) {
  return wav_add(wav_mul(a, b), c);
}
```

This works in both C and C++.  In C++ WAV also supports operator
overloading, so if you prefer you can also do this:

```c
wav_f32x4_t
multiply_add (wav_f32x4_t a, wav_f32x4_t b, wav_f32x4_t) {
  return (a * b) + c;
}
```

Operator overloading doesn't work in C, so if you want your code to
work in both languages you'll have to use the named versions.
However, the *named* overloads (*e.g.*, `wav_add`) do work in C.

Of course the type system still protects you; you can't pass a
`wav_f32x4_t` and a `wav_i32x4_t` to `wav_add` and expect it to
work… the types have to match.  Critically, we also don't support
operations which WASM SIMD128 doesn't support; for example, attempting
to multiply vectors of 8-bit integers will result in an error.  This is
in contrast to raw GCC-style vector extensions which will emulate the
instruction in software, often resulting in unexpected performance
problems.

Between the distinct types and the overloading, one way to look at
this is that WAV takes the opposite approach as `wasm_simd128.h`.
With `wasm_simd128.h`, the type is overloaded but the functions
aren't.  With WAV, the functions are overloaded but the types
aren't.

The nice thing about WAV's approach is that, since it knows what
type of vector you're using, the compiler can save you from making
many horrible mistakes by combining them in invalid ways.

### It's True, WAV Has Boolean Types

Another significant difference from `wasm_simd128.h` is the presence
of boolean types (`wav_b8x16_t`, `wav_b16x8_t`, `wav_b32x4_t`,
and `wav_b64x2_t`).

Boolean types are used for the results of comparison functions, and
the value of each lane is assumed to have either all bits unset (`0`)
or all bits set (`~0`).

You can convert between boolean types and other types (using
`wav_*_as_*`), but this usually isn't necessary; operations where it
makes sense to mix boolean and non-boolean types generally have
functions to do just that.  For example:

```c
wav_f32x4_t wav_f32x4_and_b32x4(wav_f32x4_t a, wav_b32x4_t b);
wav_f32x4_t wav_b32x4_and_f32x4(wav_b32x4_t a, wav_f32x4_t b);
```

And of course the overloaded functions work as expected as well, so
you can pass a vector of booleans and a vector of floats to `wav_and`
(or, in C++, the `&` operator) and everything will work.  However,
WAV will stop you from passing the wrong size boolean vector (*e.g.*,
mixing a `wav_f32x4_t` and a `wav_b16x8_t`); if you want to do that
you'll need to make it explicit by using a function like
`wav_b16x8_as_b32x4`.

### Can't We All Just Get Along?

Interoperability with `wasm_simd128.h` is critical.  We don't expect
everyone to switch over to WAV and never look back; sometimes you'll
want to call `wasm_simd128.h` code from code using WAV, and at other
times you'll want to call a WAV implementation from code using
`wasm_simd128.h`.

In addition to converting between WAV's types, WAV also contains
functions to convert to/from `wasm_simd128.h`'s type (`v128_t`).  For
example:

```c
v128_t      wav_f32x4_as_v128(wav_f32x4_t value);
wav_f32x4_t wav_v128_as_f32x4(     v128_t value);
```

This makes it easy to convert between the two APIs so you can freely
mix code written to use `wasm_simd128.h` and WAV:

```
wav_i8x16_t
add_using_wasm_simd128(wav_i8x16_t a, wav_i8x16_t b) {
  return
    wav_v128_as_i8x16(
      wav_add_i8x16(
        wav_i8x16_as_v128(a),
        wav_i8x16_as_v128(b)
      );
    );
}

v128_t
add_using_wav(v128_t a, v128_t b) {
  return
    wav_i8x16_to_v128(
      wasm_i8x16_add(
        wav_v128_to_i8x16(a),
        wav_v128_to_i8x16(b)
      )
    );
}
```

Most likely you'll be doing more work than a single add function at a
time, but the idea is the same no matter how much code there is.

## Emulation

WAV allows you to define `WAV_EMULATION` to 1 prior to including wav.h
to signal that WASM SIMD128 is not required.  In this mode WAV should
work on any target, including non-WASM targets like x86 or Arm, though
performance may be sub-optimal since we do no use target-specific
intrinsics.  This is primarily used to make development a bit more
convenient, though it can also be used as an easy way to fall back on
normal WebAssembly if SIMD128 is not suppored.

Note that in this mode, modifying the target using pragmas is
unreliable as WAV will always use the portable implementaions; LLVM may
generate optimal instructions, but it may not.

## Future Directions

WAV is feature-complete.  The functionality in WAV should match the
functionality in WASM SIMD.  WAV is, however, not yet API stable.
Depending on feedback things may change.  I don't expect many
API-breaking changes, but can't promise that yet.

As I mentioned earlier, adding functionality beyond what is provided
by WebAssembly SIMD is outside of WAV's scope, which means once
WAV's API is stable and complete we should be done, at least until
changes are made to the specification or additional specifications
(e.g., relaxed SIMD) are released.
