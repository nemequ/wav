/* WAV
 * <https://github.com/nemequ/wav>
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Copyright:
 *   2021      Evan Nemerson <evan@nemerson.com>
 * */

#if !defined(WAV_H)
#define WAV_H

#include <stdint.h>
#include <stdbool.h>

/* This is intended to be compatible with the v128_t type from
 * wasm_simd128.h. */
#if defined(__WASM_SIMD128_H)
  typedef v128_t wav_v128_t;
#else
  typedef int32_t wav_v128_t __attribute__((__vector_size__(16), __aligned__(16)));
#endif

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc99-extensions"
#pragma clang diagnostic ignored "-Wc11-extensions"
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wgcc-compat"

#if !defined(WAV_OPERATOR_OVERLOADING)
  #if defined(__cplusplus)
    #define WAV_OPERATOR_OVERLOADING 1
  #else
    #define WAV_OPERATOR_OVERLOADING 0
  #endif
#endif

/* These are for internal use only. */
typedef  int8_t wav_v_i8x16_ __attribute__((__vector_size__(16),__aligned__(16)));
typedef int16_t wav_v_i16x8_ __attribute__((__vector_size__(16),__aligned__(16)));
typedef int32_t wav_v_i32x4_ __attribute__((__vector_size__(16),__aligned__(16)));
typedef int64_t wav_v_i64x2_ __attribute__((__vector_size__(16),__aligned__(16)));

typedef double wav_v_f64x2_ __attribute__((__vector_size__(16),__aligned__(16)));

/* We use structs to make sure types are incompatible.  If we just used
 * raw vectors the boolean types would be compatible with the unsigned
 * types.  This also lets us restrict the use of operators to
 * functionality actually present in WASM SIMD128; for example, clang
 * will happily multiply two vectors of doubles, but WASM SIMD128
 * doesn't support it so it will fall back on a scalar version. */

typedef struct {   int8_t values __attribute__((__vector_size__(16),__aligned__(16))); } wav_i8x16_t;
typedef struct {  int16_t values __attribute__((__vector_size__(16),__aligned__(16))); } wav_i16x8_t;
typedef struct {  int32_t values __attribute__((__vector_size__(16),__aligned__(16))); } wav_i32x4_t;
typedef struct {  int64_t values __attribute__((__vector_size__(16),__aligned__(16))); } wav_i64x2_t;

typedef struct {  uint8_t values __attribute__((__vector_size__(16),__aligned__(16))); } wav_u8x16_t;
typedef struct { uint16_t values __attribute__((__vector_size__(16),__aligned__(16))); } wav_u16x8_t;
typedef struct { uint32_t values __attribute__((__vector_size__(16),__aligned__(16))); } wav_u32x4_t;
typedef struct { uint64_t values __attribute__((__vector_size__(16),__aligned__(16))); } wav_u64x2_t;

typedef struct {  uint8_t values __attribute__((__vector_size__(16),__aligned__(16))); } wav_b8x16_t;
typedef struct { uint16_t values __attribute__((__vector_size__(16),__aligned__(16))); } wav_b16x8_t;
typedef struct { uint32_t values __attribute__((__vector_size__(16),__aligned__(16))); } wav_b32x4_t;
typedef struct { uint64_t values __attribute__((__vector_size__(16),__aligned__(16))); } wav_b64x2_t;

typedef struct {    float values __attribute__((__vector_size__(16),__aligned__(16))); } wav_f32x4_t;
typedef struct {   double values __attribute__((__vector_size__(16),__aligned__(16))); } wav_f64x2_t;

/* If desired WAV can work even when the WASM SIMD128 target is not
 * enabled.  WASM SIMD128 instructions will *not* be generated, but
 * this makes it a bit easier to develop code using WAV; you can simply
 * use your normal development environment.  You can also share code,
 * though this should be done with caution since we don't make any
 * effort to make sure the instructions generated are optimaal, only
 * that the implementation is correct.
 *
 * To enable this functionality, define WAV_EMULATION to 1 prior to
 * including this header (e.g., -DWAV_EMULATION=1).
 *
 * Note that, while you can define WAV_EMULATION unconditionally (WAV
 * will still use WASM SIMD128 if it is detected), this is not
 * recommended if you use pragmas to enable the simd128 target as
 * WAV will still use the portable versions. */
#if !defined(__wasm_simd128__) && (defined(WAV_EMULATION) && (WAV_EMULATION == 1))
  /* The WAV_PORTABLE_SLOW macro is used to mark sections of code where
   * the "portable" implementations (which work on other targets, but
   * still require clang) don't yet(?) generate the instruction(s) we
   * want for the WASM SIMD128 target.  If LLVM does generate the desired
   * instruction we just go ahead and use that everywhere instead.  The
   * bugnum parameter is used to document the associated bug number in
   * LLVM (see <https://bugs.llvm.org/> for the reports). */
  #define WAV_PORTABLE_SLOW(bugnum) 1
  #define WAV_FUNCTION_ATTRIBUTES static inline __attribute__((__always_inline__,__artificial__))
  #define WAV_OVERLOAD_ATTRIBUTES static inline __attribute__((__always_inline__,__overloadable__))
#else
  #define WAV_PORTABLE_SLOW(bugnum) 0
  #define WAV_FUNCTION_ATTRIBUTES static inline __attribute__(( __artificial__ ,__target__("simd128"),__always_inline__))
  #define WAV_OVERLOAD_ATTRIBUTES static inline __attribute__((__overloadable__,__target__("simd128"),__always_inline__))
#endif

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
  #define WAV_ARRAY_LENGTH(Length) Length
#else
  #define WAV_ARRAY_LENGTH(Length)
#endif

#define WAV_ALIGNED(alignment) __attribute__((__aligned__(alignment)))

#define WAV_REQUIRE_CONSTANT(value) \
  _Static_assert(__builtin_constant_p(value), #value " is not constant")

#define WAV_REQUIRE_CONSTANT_RANGE(value, min, max) (__extension__ ({ \
    WAV_REQUIRE_CONSTANT(value); \
    _Static_assert((value <= max) && (value >= min), #value " is not between " #min " and " #max "."); \
  }))

#define WAV_REQUIRE_CONSTANT_PARAM(parameter) \
  __attribute__((__diagnose_if__(!__builtin_constant_p(parameter), #parameter " is not constant", "error")))

#define WAV_REQUIRE_CONSTANT_RANGE_PARAM(parameter, min, max) \
  __attribute__((__diagnose_if__(!__builtin_constant_p(parameter), #parameter " is not constant", "error"), __diagnose_if__(((parameter < min) || (parameter > max)), #parameter " is not between " #min " and " #max, "error")))

#define WAV_REQUIRE_TYPE(Type, Value) (__extension__ ({ \
    const Type wav_require_type_value_ = Value; \
    wav_require_type_value_; \
  }))

#define WAV_DEFINE_REINTERPRET_FUNCTION_(Type_To, Type_From, Function_Name) \
WAV_OVERLOAD_ATTRIBUTES \
  Type_To \
  Function_Name(Type_From value) { \
    _Static_assert(sizeof(Type_To) == sizeof(Type_From), #Type_To " and " #Type_From " must be the same size."); \
    Type_To r; \
    __builtin_memcpy(&r, &value, sizeof(value)); \
    return r; \
  }

WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i8x16_t, wav_i16x8_t, wav_i16x8_as_i8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i8x16_t, wav_i32x4_t, wav_i32x4_as_i8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i8x16_t, wav_i64x2_t, wav_i64x2_as_i8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i8x16_t, wav_u8x16_t, wav_u8x16_as_i8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i8x16_t, wav_u16x8_t, wav_u16x8_as_i8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i8x16_t, wav_u32x4_t, wav_u32x4_as_i8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i8x16_t, wav_u64x2_t, wav_u64x2_as_i8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i8x16_t, wav_f32x4_t, wav_f32x4_as_i8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i8x16_t, wav_f64x2_t, wav_f64x2_as_i8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i8x16_t, wav_b8x16_t, wav_b8x16_as_i8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i8x16_t, wav_b16x8_t, wav_b16x8_as_i8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i8x16_t, wav_b32x4_t, wav_b32x4_as_i8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i8x16_t, wav_b64x2_t, wav_b64x2_as_i8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i8x16_t,  wav_v128_t,  wav_v128_as_i8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i16x8_t, wav_i8x16_t, wav_i8x16_as_i16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i16x8_t, wav_i32x4_t, wav_i32x4_as_i16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i16x8_t, wav_i64x2_t, wav_i64x2_as_i16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i16x8_t, wav_u8x16_t, wav_u8x16_as_i16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i16x8_t, wav_u16x8_t, wav_u16x8_as_i16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i16x8_t, wav_u32x4_t, wav_u32x4_as_i16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i16x8_t, wav_u64x2_t, wav_u64x2_as_i16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i16x8_t, wav_f32x4_t, wav_f32x4_as_i16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i16x8_t, wav_f64x2_t, wav_f64x2_as_i16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i16x8_t, wav_b8x16_t, wav_b8x16_as_i16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i16x8_t, wav_b16x8_t, wav_b16x8_as_i16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i16x8_t, wav_b32x4_t, wav_b32x4_as_i16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i16x8_t, wav_b64x2_t, wav_b64x2_as_i16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i16x8_t,  wav_v128_t,  wav_v128_as_i16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i32x4_t, wav_i8x16_t, wav_i8x16_as_i32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i32x4_t, wav_i16x8_t, wav_i16x8_as_i32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i32x4_t, wav_i64x2_t, wav_i64x2_as_i32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i32x4_t, wav_u8x16_t, wav_u8x16_as_i32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i32x4_t, wav_u16x8_t, wav_u16x8_as_i32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i32x4_t, wav_u32x4_t, wav_u32x4_as_i32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i32x4_t, wav_u64x2_t, wav_u64x2_as_i32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i32x4_t, wav_f32x4_t, wav_f32x4_as_i32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i32x4_t, wav_f64x2_t, wav_f64x2_as_i32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i32x4_t, wav_b8x16_t, wav_b8x16_as_i32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i32x4_t, wav_b16x8_t, wav_b16x8_as_i32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i32x4_t, wav_b32x4_t, wav_b32x4_as_i32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i32x4_t, wav_b64x2_t, wav_b64x2_as_i32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i32x4_t,  wav_v128_t,  wav_v128_as_i32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i64x2_t, wav_i8x16_t, wav_i8x16_as_i64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i64x2_t, wav_i16x8_t, wav_i16x8_as_i64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i64x2_t, wav_i32x4_t, wav_i32x4_as_i64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i64x2_t, wav_u8x16_t, wav_u8x16_as_i64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i64x2_t, wav_u16x8_t, wav_u16x8_as_i64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i64x2_t, wav_u32x4_t, wav_u32x4_as_i64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i64x2_t, wav_u64x2_t, wav_u64x2_as_i64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i64x2_t, wav_f32x4_t, wav_f32x4_as_i64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i64x2_t, wav_f64x2_t, wav_f64x2_as_i64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i64x2_t, wav_b8x16_t, wav_b8x16_as_i64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i64x2_t, wav_b16x8_t, wav_b16x8_as_i64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i64x2_t, wav_b32x4_t, wav_b32x4_as_i64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i64x2_t, wav_b64x2_t, wav_b64x2_as_i64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i64x2_t,  wav_v128_t,  wav_v128_as_i64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u8x16_t, wav_i8x16_t, wav_i8x16_as_u8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u8x16_t, wav_i16x8_t, wav_i16x8_as_u8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u8x16_t, wav_i32x4_t, wav_i32x4_as_u8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u8x16_t, wav_i64x2_t, wav_i64x2_as_u8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u8x16_t, wav_u16x8_t, wav_u16x8_as_u8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u8x16_t, wav_u32x4_t, wav_u32x4_as_u8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u8x16_t, wav_u64x2_t, wav_u64x2_as_u8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u8x16_t, wav_f32x4_t, wav_f32x4_as_u8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u8x16_t, wav_f64x2_t, wav_f64x2_as_u8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u8x16_t, wav_b8x16_t, wav_b8x16_as_u8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u8x16_t, wav_b16x8_t, wav_b16x8_as_u8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u8x16_t, wav_b32x4_t, wav_b32x4_as_u8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u8x16_t, wav_b64x2_t, wav_b64x2_as_u8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u8x16_t,  wav_v128_t,  wav_v128_as_u8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u16x8_t, wav_i8x16_t, wav_i8x16_as_u16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u16x8_t, wav_i16x8_t, wav_i16x8_as_u16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u16x8_t, wav_i32x4_t, wav_i32x4_as_u16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u16x8_t, wav_i64x2_t, wav_i64x2_as_u16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u16x8_t, wav_u8x16_t, wav_u8x16_as_u16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u16x8_t, wav_u32x4_t, wav_u32x4_as_u16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u16x8_t, wav_u64x2_t, wav_u64x2_as_u16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u16x8_t, wav_f32x4_t, wav_f32x4_as_u16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u16x8_t, wav_f64x2_t, wav_f64x2_as_u16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u16x8_t, wav_b8x16_t, wav_b8x16_as_u16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u16x8_t, wav_b16x8_t, wav_b16x8_as_u16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u16x8_t, wav_b32x4_t, wav_b32x4_as_u16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u16x8_t, wav_b64x2_t, wav_b64x2_as_u16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u16x8_t,  wav_v128_t,  wav_v128_as_u16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u32x4_t, wav_i8x16_t, wav_i8x16_as_u32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u32x4_t, wav_i16x8_t, wav_i16x8_as_u32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u32x4_t, wav_i32x4_t, wav_i32x4_as_u32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u32x4_t, wav_i64x2_t, wav_i64x2_as_u32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u32x4_t, wav_u8x16_t, wav_u8x16_as_u32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u32x4_t, wav_u16x8_t, wav_u16x8_as_u32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u32x4_t, wav_u64x2_t, wav_u64x2_as_u32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u32x4_t, wav_f32x4_t, wav_f32x4_as_u32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u32x4_t, wav_f64x2_t, wav_f64x2_as_u32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u32x4_t, wav_b8x16_t, wav_b8x16_as_u32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u32x4_t, wav_b16x8_t, wav_b16x8_as_u32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u32x4_t, wav_b32x4_t, wav_b32x4_as_u32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u32x4_t, wav_b64x2_t, wav_b64x2_as_u32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u32x4_t,  wav_v128_t,  wav_v128_as_u32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u64x2_t, wav_i8x16_t, wav_i8x16_as_u64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u64x2_t, wav_i16x8_t, wav_i16x8_as_u64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u64x2_t, wav_i32x4_t, wav_i32x4_as_u64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u64x2_t, wav_i64x2_t, wav_i64x2_as_u64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u64x2_t, wav_u8x16_t, wav_u8x16_as_u64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u64x2_t, wav_u16x8_t, wav_u16x8_as_u64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u64x2_t, wav_u32x4_t, wav_u32x4_as_u64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u64x2_t, wav_f32x4_t, wav_f32x4_as_u64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u64x2_t, wav_f64x2_t, wav_f64x2_as_u64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u64x2_t, wav_b8x16_t, wav_b8x16_as_u64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u64x2_t, wav_b16x8_t, wav_b16x8_as_u64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u64x2_t, wav_b32x4_t, wav_b32x4_as_u64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u64x2_t, wav_b64x2_t, wav_b64x2_as_u64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u64x2_t,  wav_v128_t,  wav_v128_as_u64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_f32x4_t, wav_i8x16_t, wav_i8x16_as_f32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_f32x4_t, wav_i16x8_t, wav_i16x8_as_f32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_f32x4_t, wav_i32x4_t, wav_i32x4_as_f32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_f32x4_t, wav_i64x2_t, wav_i64x2_as_f32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_f32x4_t, wav_u8x16_t, wav_u8x16_as_f32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_f32x4_t, wav_u16x8_t, wav_u16x8_as_f32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_f32x4_t, wav_u32x4_t, wav_u32x4_as_f32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_f32x4_t, wav_u64x2_t, wav_u64x2_as_f32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_f32x4_t, wav_f64x2_t, wav_f64x2_as_f32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_f32x4_t, wav_b8x16_t, wav_b8x16_as_f32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_f32x4_t, wav_b16x8_t, wav_b16x8_as_f32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_f32x4_t, wav_b32x4_t, wav_b32x4_as_f32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_f32x4_t, wav_b64x2_t, wav_b64x2_as_f32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_f32x4_t,  wav_v128_t,  wav_v128_as_f32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_f64x2_t, wav_i8x16_t, wav_i8x16_as_f64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_f64x2_t, wav_i16x8_t, wav_i16x8_as_f64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_f64x2_t, wav_i32x4_t, wav_i32x4_as_f64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_f64x2_t, wav_i64x2_t, wav_i64x2_as_f64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_f64x2_t, wav_u8x16_t, wav_u8x16_as_f64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_f64x2_t, wav_u16x8_t, wav_u16x8_as_f64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_f64x2_t, wav_u32x4_t, wav_u32x4_as_f64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_f64x2_t, wav_u64x2_t, wav_u64x2_as_f64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_f64x2_t, wav_f32x4_t, wav_f32x4_as_f64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_f64x2_t, wav_b8x16_t, wav_b8x16_as_f64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_f64x2_t, wav_b16x8_t, wav_b16x8_as_f64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_f64x2_t, wav_b32x4_t, wav_b32x4_as_f64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_f64x2_t, wav_b64x2_t, wav_b64x2_as_f64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_f64x2_t,  wav_v128_t,  wav_v128_as_f64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b8x16_t, wav_i8x16_t, wav_i8x16_as_b8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b8x16_t, wav_i16x8_t, wav_i16x8_as_b8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b8x16_t, wav_i32x4_t, wav_i32x4_as_b8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b8x16_t, wav_i64x2_t, wav_i64x2_as_b8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b8x16_t, wav_u8x16_t, wav_u8x16_as_b8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b8x16_t, wav_u16x8_t, wav_u16x8_as_b8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b8x16_t, wav_u32x4_t, wav_u32x4_as_b8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b8x16_t, wav_u64x2_t, wav_u64x2_as_b8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b8x16_t, wav_f32x4_t, wav_f32x4_as_b8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b8x16_t, wav_f64x2_t, wav_f64x2_as_b8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b8x16_t, wav_b16x8_t, wav_b16x8_as_b8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b8x16_t, wav_b32x4_t, wav_b32x4_as_b8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b8x16_t, wav_b64x2_t, wav_b64x2_as_b8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b8x16_t,  wav_v128_t,  wav_v128_as_b8x16)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b16x8_t, wav_i8x16_t, wav_i8x16_as_b16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b16x8_t, wav_i16x8_t, wav_i16x8_as_b16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b16x8_t, wav_i32x4_t, wav_i32x4_as_b16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b16x8_t, wav_i64x2_t, wav_i64x2_as_b16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b16x8_t, wav_u8x16_t, wav_u8x16_as_b16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b16x8_t, wav_u16x8_t, wav_u16x8_as_b16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b16x8_t, wav_u32x4_t, wav_u32x4_as_b16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b16x8_t, wav_u64x2_t, wav_u64x2_as_b16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b16x8_t, wav_f32x4_t, wav_f32x4_as_b16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b16x8_t, wav_f64x2_t, wav_f64x2_as_b16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b16x8_t, wav_b8x16_t, wav_b8x16_as_b16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b16x8_t, wav_b32x4_t, wav_b32x4_as_b16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b16x8_t, wav_b64x2_t, wav_b64x2_as_b16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b16x8_t,  wav_v128_t , wav_v128_as_b16x8)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b32x4_t, wav_i8x16_t, wav_i8x16_as_b32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b32x4_t, wav_i16x8_t, wav_i16x8_as_b32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b32x4_t, wav_i32x4_t, wav_i32x4_as_b32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b32x4_t, wav_i64x2_t, wav_i64x2_as_b32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b32x4_t, wav_u8x16_t, wav_u8x16_as_b32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b32x4_t, wav_u16x8_t, wav_u16x8_as_b32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b32x4_t, wav_u32x4_t, wav_u32x4_as_b32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b32x4_t, wav_u64x2_t, wav_u64x2_as_b32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b32x4_t, wav_f32x4_t, wav_f32x4_as_b32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b32x4_t, wav_f64x2_t, wav_f64x2_as_b32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b32x4_t, wav_b8x16_t, wav_b8x16_as_b32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b32x4_t, wav_b16x8_t, wav_b16x8_as_b32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b32x4_t, wav_b64x2_t, wav_b64x2_as_b32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b32x4_t,  wav_v128_t,  wav_v128_as_b32x4)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b64x2_t, wav_i8x16_t, wav_i8x16_as_b64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b64x2_t, wav_i16x8_t, wav_i16x8_as_b64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b64x2_t, wav_i32x4_t, wav_i32x4_as_b64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b64x2_t, wav_i64x2_t, wav_i64x2_as_b64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b64x2_t, wav_u8x16_t, wav_u8x16_as_b64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b64x2_t, wav_u16x8_t, wav_u16x8_as_b64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b64x2_t, wav_u32x4_t, wav_u32x4_as_b64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b64x2_t, wav_u64x2_t, wav_u64x2_as_b64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b64x2_t, wav_f32x4_t, wav_f32x4_as_b64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b64x2_t, wav_f64x2_t, wav_f64x2_as_b64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b64x2_t, wav_b8x16_t, wav_b8x16_as_b64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b64x2_t, wav_b16x8_t, wav_b16x8_as_b64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b64x2_t, wav_b32x4_t, wav_b32x4_as_b64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b64x2_t,  wav_v128_t,  wav_v128_as_b64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_( wav_v128_t, wav_i8x16_t, wav_i8x16_as_v128)
WAV_DEFINE_REINTERPRET_FUNCTION_( wav_v128_t, wav_i16x8_t, wav_i16x8_as_v128)
WAV_DEFINE_REINTERPRET_FUNCTION_( wav_v128_t, wav_i32x4_t, wav_i32x4_as_v128)
WAV_DEFINE_REINTERPRET_FUNCTION_( wav_v128_t, wav_i64x2_t, wav_i64x2_as_v128)
WAV_DEFINE_REINTERPRET_FUNCTION_( wav_v128_t, wav_u8x16_t, wav_u8x16_as_v128)
WAV_DEFINE_REINTERPRET_FUNCTION_( wav_v128_t, wav_u16x8_t, wav_u16x8_as_v128)
WAV_DEFINE_REINTERPRET_FUNCTION_( wav_v128_t, wav_u32x4_t, wav_u32x4_as_v128)
WAV_DEFINE_REINTERPRET_FUNCTION_( wav_v128_t, wav_u64x2_t, wav_u64x2_as_v128)
WAV_DEFINE_REINTERPRET_FUNCTION_( wav_v128_t, wav_f32x4_t, wav_f32x4_as_v128)
WAV_DEFINE_REINTERPRET_FUNCTION_( wav_v128_t, wav_f64x2_t, wav_f64x2_as_v128)
WAV_DEFINE_REINTERPRET_FUNCTION_( wav_v128_t, wav_b8x16_t, wav_b8x16_as_v128)
WAV_DEFINE_REINTERPRET_FUNCTION_( wav_v128_t, wav_b16x8_t, wav_b16x8_as_v128)
WAV_DEFINE_REINTERPRET_FUNCTION_( wav_v128_t, wav_b32x4_t, wav_b32x4_as_v128)
WAV_DEFINE_REINTERPRET_FUNCTION_( wav_v128_t, wav_b64x2_t, wav_b64x2_as_v128)

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_as_i8x16(wav_i16x8_t value) { return wav_i16x8_as_i8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_as_i8x16(wav_i32x4_t value) { return wav_i32x4_as_i8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_as_i8x16(wav_i64x2_t value) { return wav_i64x2_as_i8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_as_i8x16(wav_u8x16_t value) { return wav_u8x16_as_i8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_as_i8x16(wav_u16x8_t value) { return wav_u16x8_as_i8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_as_i8x16(wav_u32x4_t value) { return wav_u32x4_as_i8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_as_i8x16(wav_u64x2_t value) { return wav_u64x2_as_i8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_as_i8x16(wav_f32x4_t value) { return wav_f32x4_as_i8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_as_i8x16(wav_f64x2_t value) { return wav_f64x2_as_i8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_as_i8x16(wav_b8x16_t value) { return wav_b8x16_as_i8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_as_i8x16(wav_b16x8_t value) { return wav_b16x8_as_i8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_as_i8x16(wav_b32x4_t value) { return wav_b32x4_as_i8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_as_i8x16(wav_b64x2_t value) { return wav_b64x2_as_i8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_as_i8x16( wav_v128_t value) { return  wav_v128_as_i8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_as_i16x8(wav_i8x16_t value) { return wav_i8x16_as_i16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_as_i16x8(wav_i32x4_t value) { return wav_i32x4_as_i16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_as_i16x8(wav_i64x2_t value) { return wav_i64x2_as_i16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_as_i16x8(wav_u8x16_t value) { return wav_u8x16_as_i16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_as_i16x8(wav_u16x8_t value) { return wav_u16x8_as_i16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_as_i16x8(wav_u32x4_t value) { return wav_u32x4_as_i16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_as_i16x8(wav_u64x2_t value) { return wav_u64x2_as_i16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_as_i16x8(wav_f32x4_t value) { return wav_f32x4_as_i16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_as_i16x8(wav_f64x2_t value) { return wav_f64x2_as_i16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_as_i16x8(wav_b8x16_t value) { return wav_b8x16_as_i16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_as_i16x8(wav_b16x8_t value) { return wav_b16x8_as_i16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_as_i16x8(wav_b32x4_t value) { return wav_b32x4_as_i16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_as_i16x8(wav_b64x2_t value) { return wav_b64x2_as_i16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_as_i16x8( wav_v128_t value) { return  wav_v128_as_i16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_as_i32x4(wav_i8x16_t value) { return wav_i8x16_as_i32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_as_i32x4(wav_i16x8_t value) { return wav_i16x8_as_i32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_as_i32x4(wav_i64x2_t value) { return wav_i64x2_as_i32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_as_i32x4(wav_u8x16_t value) { return wav_u8x16_as_i32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_as_i32x4(wav_u16x8_t value) { return wav_u16x8_as_i32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_as_i32x4(wav_u32x4_t value) { return wav_u32x4_as_i32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_as_i32x4(wav_u64x2_t value) { return wav_u64x2_as_i32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_as_i32x4(wav_f32x4_t value) { return wav_f32x4_as_i32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_as_i32x4(wav_f64x2_t value) { return wav_f64x2_as_i32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_as_i32x4(wav_b8x16_t value) { return wav_b8x16_as_i32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_as_i32x4(wav_b16x8_t value) { return wav_b16x8_as_i32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_as_i32x4(wav_b32x4_t value) { return wav_b32x4_as_i32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_as_i32x4(wav_b64x2_t value) { return wav_b64x2_as_i32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_as_i32x4( wav_v128_t value) { return  wav_v128_as_i32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_as_i64x2(wav_i8x16_t value) { return wav_i8x16_as_i64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_as_i64x2(wav_i16x8_t value) { return wav_i16x8_as_i64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_as_i64x2(wav_i32x4_t value) { return wav_i32x4_as_i64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_as_i64x2(wav_u8x16_t value) { return wav_u8x16_as_i64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_as_i64x2(wav_u16x8_t value) { return wav_u16x8_as_i64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_as_i64x2(wav_u32x4_t value) { return wav_u32x4_as_i64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_as_i64x2(wav_u64x2_t value) { return wav_u64x2_as_i64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_as_i64x2(wav_f32x4_t value) { return wav_f32x4_as_i64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_as_i64x2(wav_f64x2_t value) { return wav_f64x2_as_i64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_as_i64x2(wav_b8x16_t value) { return wav_b8x16_as_i64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_as_i64x2(wav_b16x8_t value) { return wav_b16x8_as_i64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_as_i64x2(wav_b32x4_t value) { return wav_b32x4_as_i64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_as_i64x2(wav_b64x2_t value) { return wav_b64x2_as_i64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_as_i64x2( wav_v128_t value) { return  wav_v128_as_i64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_as_u8x16(wav_i8x16_t value) { return wav_i8x16_as_u8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_as_u8x16(wav_i16x8_t value) { return wav_i16x8_as_u8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_as_u8x16(wav_i32x4_t value) { return wav_i32x4_as_u8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_as_u8x16(wav_i64x2_t value) { return wav_i64x2_as_u8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_as_u8x16(wav_u16x8_t value) { return wav_u16x8_as_u8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_as_u8x16(wav_u32x4_t value) { return wav_u32x4_as_u8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_as_u8x16(wav_u64x2_t value) { return wav_u64x2_as_u8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_as_u8x16(wav_f32x4_t value) { return wav_f32x4_as_u8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_as_u8x16(wav_f64x2_t value) { return wav_f64x2_as_u8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_as_u8x16(wav_b8x16_t value) { return wav_b8x16_as_u8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_as_u8x16(wav_b16x8_t value) { return wav_b16x8_as_u8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_as_u8x16(wav_b32x4_t value) { return wav_b32x4_as_u8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_as_u8x16(wav_b64x2_t value) { return wav_b64x2_as_u8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_as_u8x16( wav_v128_t value) { return  wav_v128_as_u8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_as_u16x8(wav_i8x16_t value) { return wav_i8x16_as_u16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_as_u16x8(wav_i16x8_t value) { return wav_i16x8_as_u16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_as_u16x8(wav_i32x4_t value) { return wav_i32x4_as_u16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_as_u16x8(wav_i64x2_t value) { return wav_i64x2_as_u16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_as_u16x8(wav_u8x16_t value) { return wav_u8x16_as_u16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_as_u16x8(wav_u32x4_t value) { return wav_u32x4_as_u16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_as_u16x8(wav_u64x2_t value) { return wav_u64x2_as_u16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_as_u16x8(wav_f32x4_t value) { return wav_f32x4_as_u16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_as_u16x8(wav_f64x2_t value) { return wav_f64x2_as_u16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_as_u16x8(wav_b8x16_t value) { return wav_b8x16_as_u16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_as_u16x8(wav_b16x8_t value) { return wav_b16x8_as_u16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_as_u16x8(wav_b32x4_t value) { return wav_b32x4_as_u16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_as_u16x8(wav_b64x2_t value) { return wav_b64x2_as_u16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_as_u16x8( wav_v128_t value) { return  wav_v128_as_u16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_as_u32x4(wav_i8x16_t value) { return wav_i8x16_as_u32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_as_u32x4(wav_i16x8_t value) { return wav_i16x8_as_u32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_as_u32x4(wav_i32x4_t value) { return wav_i32x4_as_u32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_as_u32x4(wav_i64x2_t value) { return wav_i64x2_as_u32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_as_u32x4(wav_u8x16_t value) { return wav_u8x16_as_u32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_as_u32x4(wav_u16x8_t value) { return wav_u16x8_as_u32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_as_u32x4(wav_u64x2_t value) { return wav_u64x2_as_u32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_as_u32x4(wav_f32x4_t value) { return wav_f32x4_as_u32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_as_u32x4(wav_f64x2_t value) { return wav_f64x2_as_u32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_as_u32x4(wav_b8x16_t value) { return wav_b8x16_as_u32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_as_u32x4(wav_b16x8_t value) { return wav_b16x8_as_u32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_as_u32x4(wav_b32x4_t value) { return wav_b32x4_as_u32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_as_u32x4(wav_b64x2_t value) { return wav_b64x2_as_u32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_as_u32x4( wav_v128_t value) { return  wav_v128_as_u32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_as_u64x2(wav_i8x16_t value) { return wav_i8x16_as_u64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_as_u64x2(wav_i16x8_t value) { return wav_i16x8_as_u64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_as_u64x2(wav_i32x4_t value) { return wav_i32x4_as_u64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_as_u64x2(wav_i64x2_t value) { return wav_i64x2_as_u64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_as_u64x2(wav_u8x16_t value) { return wav_u8x16_as_u64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_as_u64x2(wav_u16x8_t value) { return wav_u16x8_as_u64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_as_u64x2(wav_u32x4_t value) { return wav_u32x4_as_u64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_as_u64x2(wav_f32x4_t value) { return wav_f32x4_as_u64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_as_u64x2(wav_f64x2_t value) { return wav_f64x2_as_u64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_as_u64x2(wav_b8x16_t value) { return wav_b8x16_as_u64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_as_u64x2(wav_b16x8_t value) { return wav_b16x8_as_u64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_as_u64x2(wav_b32x4_t value) { return wav_b32x4_as_u64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_as_u64x2(wav_b64x2_t value) { return wav_b64x2_as_u64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_as_u64x2( wav_v128_t value) { return  wav_v128_as_u64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_as_f32x4(wav_i8x16_t value) { return wav_i8x16_as_f32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_as_f32x4(wav_i16x8_t value) { return wav_i16x8_as_f32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_as_f32x4(wav_i32x4_t value) { return wav_i32x4_as_f32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_as_f32x4(wav_i64x2_t value) { return wav_i64x2_as_f32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_as_f32x4(wav_u8x16_t value) { return wav_u8x16_as_f32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_as_f32x4(wav_u16x8_t value) { return wav_u16x8_as_f32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_as_f32x4(wav_u32x4_t value) { return wav_u32x4_as_f32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_as_f32x4(wav_u64x2_t value) { return wav_u64x2_as_f32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_as_f32x4(wav_f64x2_t value) { return wav_f64x2_as_f32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_as_f32x4(wav_b8x16_t value) { return wav_b8x16_as_f32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_as_f32x4(wav_b16x8_t value) { return wav_b16x8_as_f32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_as_f32x4(wav_b32x4_t value) { return wav_b32x4_as_f32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_as_f32x4(wav_b64x2_t value) { return wav_b64x2_as_f32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_as_f32x4( wav_v128_t value) { return  wav_v128_as_f32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_as_f64x2(wav_i8x16_t value) { return wav_i8x16_as_f64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_as_f64x2(wav_i16x8_t value) { return wav_i16x8_as_f64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_as_f64x2(wav_i32x4_t value) { return wav_i32x4_as_f64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_as_f64x2(wav_i64x2_t value) { return wav_i64x2_as_f64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_as_f64x2(wav_u8x16_t value) { return wav_u8x16_as_f64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_as_f64x2(wav_u16x8_t value) { return wav_u16x8_as_f64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_as_f64x2(wav_u32x4_t value) { return wav_u32x4_as_f64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_as_f64x2(wav_u64x2_t value) { return wav_u64x2_as_f64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_as_f64x2(wav_f32x4_t value) { return wav_f32x4_as_f64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_as_f64x2(wav_b8x16_t value) { return wav_b8x16_as_f64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_as_f64x2(wav_b16x8_t value) { return wav_b16x8_as_f64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_as_f64x2(wav_b32x4_t value) { return wav_b32x4_as_f64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_as_f64x2(wav_b64x2_t value) { return wav_b64x2_as_f64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_as_f64x2( wav_v128_t value) { return  wav_v128_as_f64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t wav_as_b8x16(wav_i8x16_t value) { return wav_i8x16_as_b8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t wav_as_b8x16(wav_i16x8_t value) { return wav_i16x8_as_b8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t wav_as_b8x16(wav_i32x4_t value) { return wav_i32x4_as_b8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t wav_as_b8x16(wav_i64x2_t value) { return wav_i64x2_as_b8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t wav_as_b8x16(wav_u8x16_t value) { return wav_u8x16_as_b8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t wav_as_b8x16(wav_u16x8_t value) { return wav_u16x8_as_b8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t wav_as_b8x16(wav_u32x4_t value) { return wav_u32x4_as_b8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t wav_as_b8x16(wav_u64x2_t value) { return wav_u64x2_as_b8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t wav_as_b8x16(wav_f32x4_t value) { return wav_f32x4_as_b8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t wav_as_b8x16(wav_f64x2_t value) { return wav_f64x2_as_b8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t wav_as_b8x16(wav_b16x8_t value) { return wav_b16x8_as_b8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t wav_as_b8x16(wav_b32x4_t value) { return wav_b32x4_as_b8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t wav_as_b8x16(wav_b64x2_t value) { return wav_b64x2_as_b8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t wav_as_b8x16( wav_v128_t value) { return  wav_v128_as_b8x16(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t wav_as_b16x8(wav_i8x16_t value) { return wav_i8x16_as_b16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t wav_as_b16x8(wav_i16x8_t value) { return wav_i16x8_as_b16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t wav_as_b16x8(wav_i32x4_t value) { return wav_i32x4_as_b16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t wav_as_b16x8(wav_i64x2_t value) { return wav_i64x2_as_b16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t wav_as_b16x8(wav_u8x16_t value) { return wav_u8x16_as_b16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t wav_as_b16x8(wav_u16x8_t value) { return wav_u16x8_as_b16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t wav_as_b16x8(wav_u32x4_t value) { return wav_u32x4_as_b16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t wav_as_b16x8(wav_u64x2_t value) { return wav_u64x2_as_b16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t wav_as_b16x8(wav_f32x4_t value) { return wav_f32x4_as_b16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t wav_as_b16x8(wav_f64x2_t value) { return wav_f64x2_as_b16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t wav_as_b16x8(wav_b8x16_t value) { return wav_b8x16_as_b16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t wav_as_b16x8(wav_b32x4_t value) { return wav_b32x4_as_b16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t wav_as_b16x8(wav_b64x2_t value) { return wav_b64x2_as_b16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t wav_as_b16x8( wav_v128_t value) { return  wav_v128_as_b16x8(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_as_b32x4(wav_i8x16_t value) { return wav_i8x16_as_b32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_as_b32x4(wav_i16x8_t value) { return wav_i16x8_as_b32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_as_b32x4(wav_i32x4_t value) { return wav_i32x4_as_b32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_as_b32x4(wav_i64x2_t value) { return wav_i64x2_as_b32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_as_b32x4(wav_u8x16_t value) { return wav_u8x16_as_b32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_as_b32x4(wav_u16x8_t value) { return wav_u16x8_as_b32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_as_b32x4(wav_u32x4_t value) { return wav_u32x4_as_b32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_as_b32x4(wav_u64x2_t value) { return wav_u64x2_as_b32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_as_b32x4(wav_f32x4_t value) { return wav_f32x4_as_b32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_as_b32x4(wav_f64x2_t value) { return wav_f64x2_as_b32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_as_b32x4(wav_b8x16_t value) { return wav_b8x16_as_b32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_as_b32x4(wav_b16x8_t value) { return wav_b16x8_as_b32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_as_b32x4(wav_b64x2_t value) { return wav_b64x2_as_b32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_as_b32x4( wav_v128_t value) { return  wav_v128_as_b32x4(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_as_b64x2(wav_i8x16_t value) { return wav_i8x16_as_b64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_as_b64x2(wav_i16x8_t value) { return wav_i16x8_as_b64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_as_b64x2(wav_i32x4_t value) { return wav_i32x4_as_b64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_as_b64x2(wav_i64x2_t value) { return wav_i64x2_as_b64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_as_b64x2(wav_u8x16_t value) { return wav_u8x16_as_b64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_as_b64x2(wav_u16x8_t value) { return wav_u16x8_as_b64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_as_b64x2(wav_u32x4_t value) { return wav_u32x4_as_b64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_as_b64x2(wav_u64x2_t value) { return wav_u64x2_as_b64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_as_b64x2(wav_f32x4_t value) { return wav_f32x4_as_b64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_as_b64x2(wav_f64x2_t value) { return wav_f64x2_as_b64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_as_b64x2(wav_b8x16_t value) { return wav_b8x16_as_b64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_as_b64x2(wav_b16x8_t value) { return wav_b16x8_as_b64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_as_b64x2(wav_b32x4_t value) { return wav_b32x4_as_b64x2(value); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_as_b64x2( wav_v128_t value) { return  wav_v128_as_b64x2(value); }
WAV_OVERLOAD_ATTRIBUTES  wav_v128_t  wav_as_v128(wav_i8x16_t value) { return  wav_i8x16_as_v128(value); }
WAV_OVERLOAD_ATTRIBUTES  wav_v128_t  wav_as_v128(wav_i16x8_t value) { return  wav_i16x8_as_v128(value); }
WAV_OVERLOAD_ATTRIBUTES  wav_v128_t  wav_as_v128(wav_i32x4_t value) { return  wav_i32x4_as_v128(value); }
WAV_OVERLOAD_ATTRIBUTES  wav_v128_t  wav_as_v128(wav_i64x2_t value) { return  wav_i64x2_as_v128(value); }
WAV_OVERLOAD_ATTRIBUTES  wav_v128_t  wav_as_v128(wav_u8x16_t value) { return  wav_u8x16_as_v128(value); }
WAV_OVERLOAD_ATTRIBUTES  wav_v128_t  wav_as_v128(wav_u16x8_t value) { return  wav_u16x8_as_v128(value); }
WAV_OVERLOAD_ATTRIBUTES  wav_v128_t  wav_as_v128(wav_u32x4_t value) { return  wav_u32x4_as_v128(value); }
WAV_OVERLOAD_ATTRIBUTES  wav_v128_t  wav_as_v128(wav_u64x2_t value) { return  wav_u64x2_as_v128(value); }
WAV_OVERLOAD_ATTRIBUTES  wav_v128_t  wav_as_v128(wav_f32x4_t value) { return  wav_f32x4_as_v128(value); }
WAV_OVERLOAD_ATTRIBUTES  wav_v128_t  wav_as_v128(wav_f64x2_t value) { return  wav_f64x2_as_v128(value); }
WAV_OVERLOAD_ATTRIBUTES  wav_v128_t  wav_as_v128(wav_b8x16_t value) { return  wav_b8x16_as_v128(value); }
WAV_OVERLOAD_ATTRIBUTES  wav_v128_t  wav_as_v128(wav_b16x8_t value) { return  wav_b16x8_as_v128(value); }
WAV_OVERLOAD_ATTRIBUTES  wav_v128_t  wav_as_v128(wav_b32x4_t value) { return  wav_b32x4_as_v128(value); }
WAV_OVERLOAD_ATTRIBUTES  wav_v128_t  wav_as_v128(wav_b64x2_t value) { return  wav_b64x2_as_v128(value); }

/* load
 *
 * Load data.
 *
 * WASM can load and store unaligned data, so these are really only
 * here to provide a type safe API. */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_load(const int8_t src[WAV_ARRAY_LENGTH(16)]) {
  wav_i8x16_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_load(const int16_t src[WAV_ARRAY_LENGTH(8)]) {
  wav_i16x8_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_load(const int32_t src[WAV_ARRAY_LENGTH(4)]) {
  wav_i32x4_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_load(const int64_t src[WAV_ARRAY_LENGTH(2)]) {
  wav_i64x2_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_load(const uint8_t src[WAV_ARRAY_LENGTH(16)]) {
  wav_u8x16_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_load(const uint16_t src[WAV_ARRAY_LENGTH(8)]) {
  wav_u16x8_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_load(const uint32_t src[WAV_ARRAY_LENGTH(4)]) {
  wav_u32x4_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_load(const uint64_t src[WAV_ARRAY_LENGTH(2)]) {
  wav_u64x2_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_load(const float src[WAV_ARRAY_LENGTH(4)]) {
  wav_f32x4_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_load(const double src[WAV_ARRAY_LENGTH(2)]) {
  wav_f64x2_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_load(const   int8_t src[WAV_ARRAY_LENGTH(16)]) { return wav_i8x16_load(src); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_load(const  int16_t src[WAV_ARRAY_LENGTH( 8)]) { return wav_i16x8_load(src); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_load(const  int32_t src[WAV_ARRAY_LENGTH( 4)]) { return wav_i32x4_load(src); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_load(const  int64_t src[WAV_ARRAY_LENGTH( 2)]) { return wav_i64x2_load(src); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_load(const  uint8_t src[WAV_ARRAY_LENGTH(16)]) { return wav_u8x16_load(src); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_load(const uint16_t src[WAV_ARRAY_LENGTH( 8)]) { return wav_u16x8_load(src); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_load(const uint32_t src[WAV_ARRAY_LENGTH( 4)]) { return wav_u32x4_load(src); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_load(const uint64_t src[WAV_ARRAY_LENGTH( 2)]) { return wav_u64x2_load(src); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_load(const    float src[WAV_ARRAY_LENGTH( 4)]) { return wav_f32x4_load(src); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_load(const   double src[WAV_ARRAY_LENGTH( 2)]) { return wav_f64x2_load(src); }

/* loadu
 *
 * Load unaligned data.
 *
 * Note that there are no overloads for these functions; the input type
 * is always a void pointer, so there is nothing to use to determine
 * the type. */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_loadu(const void * src) {
  wav_i8x16_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_loadu(const void * src) {
  wav_i16x8_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_loadu(const void * src) {
  wav_i32x4_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_loadu(const void * src) {
  wav_i64x2_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_loadu(const void * src) {
  wav_u8x16_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_loadu(const void * src) {
  wav_u16x8_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_loadu(const void * src) {
  wav_u32x4_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_loadu(const void * src) {
  wav_u64x2_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_loadu(const void * src) {
  wav_f32x4_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_loadu(const void * src) {
  wav_f64x2_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

/* loada
 *
 * Load unaligned data.
 *
 * Note that there are no overloads for these functions; the input type
 * is always a void pointer, so there is nothing to use to determine
 * the type. */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_loada(WAV_ALIGNED(16) const int8_t src[WAV_ARRAY_LENGTH(16)]) {
  wav_i8x16_t dest;
  __builtin_memcpy(&dest, __builtin_assume_aligned(src, __alignof__(dest)), sizeof(dest));
  return dest;
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_loada(WAV_ALIGNED(16) const int16_t src[WAV_ARRAY_LENGTH(8)]) {
  wav_i16x8_t dest;
  __builtin_memcpy(&dest, __builtin_assume_aligned(src, __alignof__(dest)), sizeof(dest));
  return dest;
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_loada(WAV_ALIGNED(16) const int32_t src[WAV_ARRAY_LENGTH(4)]) {
  wav_i32x4_t dest;
  __builtin_memcpy(&dest, __builtin_assume_aligned(src, __alignof__(dest)), sizeof(dest));
  return dest;
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_loada(WAV_ALIGNED(16) const int64_t src[WAV_ARRAY_LENGTH(2)]) {
  wav_i64x2_t dest;
  __builtin_memcpy(&dest, __builtin_assume_aligned(src, __alignof__(dest)), sizeof(dest));
  return dest;
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_loada(WAV_ALIGNED(16) const uint8_t src[WAV_ARRAY_LENGTH(16)]) {
  wav_u8x16_t dest;
  __builtin_memcpy(&dest, __builtin_assume_aligned(src, __alignof__(dest)), sizeof(dest));
  return dest;
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_loada(WAV_ALIGNED(16) const uint16_t src[WAV_ARRAY_LENGTH(8)]) {
  wav_u16x8_t dest;
  __builtin_memcpy(&dest, __builtin_assume_aligned(src, __alignof__(dest)), sizeof(dest));
  return dest;
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_loada(WAV_ALIGNED(16) const uint32_t src[WAV_ARRAY_LENGTH(4)]) {
  wav_u32x4_t dest;
  __builtin_memcpy(&dest, __builtin_assume_aligned(src, __alignof__(dest)), sizeof(dest));
  return dest;
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_loada(WAV_ALIGNED(16) const uint64_t src[WAV_ARRAY_LENGTH(2)]) {
  wav_u64x2_t dest;
  __builtin_memcpy(&dest, __builtin_assume_aligned(src, __alignof__(dest)), sizeof(dest));
  return dest;
}

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_loada(WAV_ALIGNED(16) const float src[WAV_ARRAY_LENGTH(4)]) {
  wav_f32x4_t dest;
  __builtin_memcpy(&dest, __builtin_assume_aligned(src, __alignof__(dest)), sizeof(dest));
  return dest;
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_loada(WAV_ALIGNED(16) const double src[WAV_ARRAY_LENGTH(2)]) {
  wav_f64x2_t dest;
  __builtin_memcpy(&dest, __builtin_assume_aligned(src, __alignof__(dest)), sizeof(dest));
  return dest;
}

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_loada(WAV_ALIGNED(16) const   int8_t src[WAV_ARRAY_LENGTH(16)]) { return wav_i8x16_loada(src); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_loada(WAV_ALIGNED(16) const  int16_t src[WAV_ARRAY_LENGTH( 8)]) { return wav_i16x8_loada(src); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_loada(WAV_ALIGNED(16) const  int32_t src[WAV_ARRAY_LENGTH( 4)]) { return wav_i32x4_loada(src); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_loada(WAV_ALIGNED(16) const  int64_t src[WAV_ARRAY_LENGTH( 2)]) { return wav_i64x2_loada(src); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_loada(WAV_ALIGNED(16) const  uint8_t src[WAV_ARRAY_LENGTH(16)]) { return wav_u8x16_loada(src); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_loada(WAV_ALIGNED(16) const uint16_t src[WAV_ARRAY_LENGTH( 8)]) { return wav_u16x8_loada(src); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_loada(WAV_ALIGNED(16) const uint32_t src[WAV_ARRAY_LENGTH( 4)]) { return wav_u32x4_loada(src); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_loada(WAV_ALIGNED(16) const uint64_t src[WAV_ARRAY_LENGTH( 2)]) { return wav_u64x2_loada(src); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_loada(WAV_ALIGNED(16) const    float src[WAV_ARRAY_LENGTH( 4)]) { return wav_f32x4_loada(src); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_loada(WAV_ALIGNED(16) const   double src[WAV_ARRAY_LENGTH( 2)]) { return wav_f64x2_loada(src); }

/* store
 *
 * Store data. */

WAV_FUNCTION_ATTRIBUTES
void
wav_i8x16_store(int8_t dest[WAV_ARRAY_LENGTH(16)], wav_i8x16_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_i16x8_store(int16_t dest[WAV_ARRAY_LENGTH(8)], wav_i16x8_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_i32x4_store(int32_t dest[WAV_ARRAY_LENGTH(4)], wav_i32x4_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_i64x2_store(int64_t dest[WAV_ARRAY_LENGTH(2)], wav_i64x2_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_u8x16_store(uint8_t dest[WAV_ARRAY_LENGTH(8)], wav_u8x16_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_u16x8_store(uint16_t dest[WAV_ARRAY_LENGTH(4)], wav_u16x8_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_u32x4_store(uint32_t dest[WAV_ARRAY_LENGTH(2)], wav_u32x4_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_u64x2_store(uint64_t dest[WAV_ARRAY_LENGTH(4)], wav_u64x2_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_f32x4_store(float dest[WAV_ARRAY_LENGTH(2)], wav_f32x4_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_f64x2_store(double dest[WAV_ARRAY_LENGTH(16)], wav_f64x2_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WAV_OVERLOAD_ATTRIBUTES void wav_store(  int8_t dest[WAV_ARRAY_LENGTH(16)], wav_i8x16_t src) { wav_i8x16_store(dest, src); }
WAV_OVERLOAD_ATTRIBUTES void wav_store( int16_t dest[WAV_ARRAY_LENGTH( 8)], wav_i16x8_t src) { wav_i16x8_store(dest, src); }
WAV_OVERLOAD_ATTRIBUTES void wav_store( int32_t dest[WAV_ARRAY_LENGTH( 4)], wav_i32x4_t src) { wav_i32x4_store(dest, src); }
WAV_OVERLOAD_ATTRIBUTES void wav_store( int64_t dest[WAV_ARRAY_LENGTH( 2)], wav_i64x2_t src) { wav_i64x2_store(dest, src); }
WAV_OVERLOAD_ATTRIBUTES void wav_store( uint8_t dest[WAV_ARRAY_LENGTH(16)], wav_u8x16_t src) { wav_u8x16_store(dest, src); }
WAV_OVERLOAD_ATTRIBUTES void wav_store(uint16_t dest[WAV_ARRAY_LENGTH( 8)], wav_u16x8_t src) { wav_u16x8_store(dest, src); }
WAV_OVERLOAD_ATTRIBUTES void wav_store(uint32_t dest[WAV_ARRAY_LENGTH( 4)], wav_u32x4_t src) { wav_u32x4_store(dest, src); }
WAV_OVERLOAD_ATTRIBUTES void wav_store(uint64_t dest[WAV_ARRAY_LENGTH( 2)], wav_u64x2_t src) { wav_u64x2_store(dest, src); }
WAV_OVERLOAD_ATTRIBUTES void wav_store(   float dest[WAV_ARRAY_LENGTH( 4)], wav_f32x4_t src) { wav_f32x4_store(dest, src); }
WAV_OVERLOAD_ATTRIBUTES void wav_store(  double dest[WAV_ARRAY_LENGTH( 2)], wav_f64x2_t src) { wav_f64x2_store(dest, src); }

/* storea
 *
 * Store data to 16-byte-aligned memory. */

WAV_FUNCTION_ATTRIBUTES
void
wav_i8x16_storea(WAV_ALIGNED(16) int8_t dest[WAV_ARRAY_LENGTH(16)], wav_i8x16_t src) {
  __builtin_memcpy(__builtin_assume_aligned(dest, __alignof__(src)), &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_i16x8_storea(WAV_ALIGNED(16) int16_t dest[WAV_ARRAY_LENGTH(8)], wav_i16x8_t src) {
  __builtin_memcpy(__builtin_assume_aligned(dest, __alignof__(src)), &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_i32x4_storea(WAV_ALIGNED(16) int32_t dest[WAV_ARRAY_LENGTH(4)], wav_i32x4_t src) {
  __builtin_memcpy(__builtin_assume_aligned(dest, __alignof__(src)), &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_i64x2_storea(WAV_ALIGNED(16) int64_t dest[WAV_ARRAY_LENGTH(2)], wav_i64x2_t src) {
  __builtin_memcpy(__builtin_assume_aligned(dest, __alignof__(src)), &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_u8x16_storea(WAV_ALIGNED(16) uint8_t dest[WAV_ARRAY_LENGTH(16)], wav_u8x16_t src) {
  __builtin_memcpy(__builtin_assume_aligned(dest, __alignof__(src)), &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_u16x8_storea(WAV_ALIGNED(16) uint16_t dest[WAV_ARRAY_LENGTH(8)], wav_u16x8_t src) {
  __builtin_memcpy(__builtin_assume_aligned(dest, __alignof__(src)), &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_u32x4_storea(WAV_ALIGNED(16) uint32_t dest[WAV_ARRAY_LENGTH(4)], wav_u32x4_t src) {
  __builtin_memcpy(__builtin_assume_aligned(dest, __alignof__(src)), &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_u64x2_storea(WAV_ALIGNED(16) uint64_t dest[WAV_ARRAY_LENGTH(2)], wav_u64x2_t src) {
  __builtin_memcpy(__builtin_assume_aligned(dest, __alignof__(src)), &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_f32x4_storea(WAV_ALIGNED(16) float dest[WAV_ARRAY_LENGTH(4)], wav_f32x4_t src) {
  __builtin_memcpy(__builtin_assume_aligned(dest, __alignof__(src)), &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_f64x2_storea(WAV_ALIGNED(16) double dest[WAV_ARRAY_LENGTH(2)], wav_f64x2_t src) {
  __builtin_memcpy(__builtin_assume_aligned(dest, __alignof__(src)), &src, sizeof(src));
}

WAV_OVERLOAD_ATTRIBUTES void wav_storea(  int8_t dest[WAV_ARRAY_LENGTH(16)], wav_i8x16_t src) { wav_i8x16_storea(dest, src); }
WAV_OVERLOAD_ATTRIBUTES void wav_storea( int16_t dest[WAV_ARRAY_LENGTH( 8)], wav_i16x8_t src) { wav_i16x8_storea(dest, src); }
WAV_OVERLOAD_ATTRIBUTES void wav_storea( int32_t dest[WAV_ARRAY_LENGTH( 4)], wav_i32x4_t src) { wav_i32x4_storea(dest, src); }
WAV_OVERLOAD_ATTRIBUTES void wav_storea( int64_t dest[WAV_ARRAY_LENGTH( 2)], wav_i64x2_t src) { wav_i64x2_storea(dest, src); }
WAV_OVERLOAD_ATTRIBUTES void wav_storea( uint8_t dest[WAV_ARRAY_LENGTH(16)], wav_u8x16_t src) { wav_u8x16_storea(dest, src); }
WAV_OVERLOAD_ATTRIBUTES void wav_storea(uint16_t dest[WAV_ARRAY_LENGTH( 8)], wav_u16x8_t src) { wav_u16x8_storea(dest, src); }
WAV_OVERLOAD_ATTRIBUTES void wav_storea(uint32_t dest[WAV_ARRAY_LENGTH( 4)], wav_u32x4_t src) { wav_u32x4_storea(dest, src); }
WAV_OVERLOAD_ATTRIBUTES void wav_storea(uint64_t dest[WAV_ARRAY_LENGTH( 2)], wav_u64x2_t src) { wav_u64x2_storea(dest, src); }
WAV_OVERLOAD_ATTRIBUTES void wav_storea(   float dest[WAV_ARRAY_LENGTH( 4)], wav_f32x4_t src) { wav_f32x4_storea(dest, src); }
WAV_OVERLOAD_ATTRIBUTES void wav_storea(  double dest[WAV_ARRAY_LENGTH( 2)], wav_f64x2_t src) { wav_f64x2_storea(dest, src); }

/* storeu
 *
 * Store unaligned data. */

WAV_FUNCTION_ATTRIBUTES
void
wav_i8x16_storeu(void * dest, wav_i8x16_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_i16x8_storeu(void * dest, wav_i16x8_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_i32x4_storeu(void * dest, wav_i32x4_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_i64x2_storeu(void * dest, wav_i64x2_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_u8x16_storeu(void * dest, wav_u8x16_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_u16x8_storeu(void * dest, wav_u16x8_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_u32x4_storeu(void * dest, wav_u32x4_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_u64x2_storeu(void * dest, wav_u64x2_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_f32x4_storeu(void * dest, wav_f32x4_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_f64x2_storeu(void * dest, wav_f64x2_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WAV_OVERLOAD_ATTRIBUTES void wav_storeu(void * dest, wav_i8x16_t src) { wav_i8x16_storeu(dest, src); }
WAV_OVERLOAD_ATTRIBUTES void wav_storeu(void * dest, wav_i16x8_t src) { wav_i16x8_storeu(dest, src); }
WAV_OVERLOAD_ATTRIBUTES void wav_storeu(void * dest, wav_i32x4_t src) { wav_i32x4_storeu(dest, src); }
WAV_OVERLOAD_ATTRIBUTES void wav_storeu(void * dest, wav_i64x2_t src) { wav_i64x2_storeu(dest, src); }
WAV_OVERLOAD_ATTRIBUTES void wav_storeu(void * dest, wav_u8x16_t src) { wav_u8x16_storeu(dest, src); }
WAV_OVERLOAD_ATTRIBUTES void wav_storeu(void * dest, wav_u16x8_t src) { wav_u16x8_storeu(dest, src); }
WAV_OVERLOAD_ATTRIBUTES void wav_storeu(void * dest, wav_u32x4_t src) { wav_u32x4_storeu(dest, src); }
WAV_OVERLOAD_ATTRIBUTES void wav_storeu(void * dest, wav_u64x2_t src) { wav_u64x2_storeu(dest, src); }
WAV_OVERLOAD_ATTRIBUTES void wav_storeu(void * dest, wav_f32x4_t src) { wav_f32x4_storeu(dest, src); }
WAV_OVERLOAD_ATTRIBUTES void wav_storeu(void * dest, wav_f64x2_t src) { wav_f64x2_storeu(dest, src); }

/* splat
 *
 * Create vector with identical lanes.
 */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_splat(int8_t value) {
  return (wav_i8x16_t) { {
    value, value, value, value, value, value, value, value,
    value, value, value, value, value, value, value, value } };
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_splat(int16_t value) {
  return (wav_i16x8_t) { {
    value, value, value, value, value, value, value, value } };
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_splat(int32_t value) {
  return (wav_i32x4_t) { { value, value, value, value } };
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_splat(int64_t value) {
  return (wav_i64x2_t) { { value, value } };
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_splat(uint8_t value) {
  return (wav_u8x16_t) { {
    value, value, value, value, value, value, value, value,
    value, value, value, value, value, value, value, value } };
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_splat(uint16_t value) {
  return (wav_u16x8_t) { {
    value, value, value, value, value, value, value, value } };
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_splat(uint32_t value) {
  return (wav_u32x4_t) { { value, value, value, value } };
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_splat(uint64_t value) {
  return (wav_u64x2_t) { { value, value } };
}

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_splat(float value) {
  return (wav_f32x4_t) { { value, value, value, value } };
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_splat(double value) {
  return (wav_f64x2_t) { { value, value } };
}

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_splat(  int8_t value) { return wav_i8x16_splat(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_splat( int16_t value) { return wav_i16x8_splat(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_splat( int32_t value) { return wav_i32x4_splat(value); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_splat( int64_t value) { return wav_i64x2_splat(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_splat( uint8_t value) { return wav_u8x16_splat(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_splat(uint16_t value) { return wav_u16x8_splat(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_splat(uint32_t value) { return wav_u32x4_splat(value); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_splat(uint64_t value) { return wav_u64x2_splat(value); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_splat(   float value) { return wav_f32x4_splat(value); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_splat(  double value) { return wav_f64x2_splat(value); }

/* make
 *
 * Create vector.
 *
 * Slow; splat + 15 replace_lanes
 */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_make(
    int8_t c0, int8_t c1, int8_t  c2, int8_t  c3, int8_t  c4, int8_t  c5, int8_t  c6, int8_t  c7,
    int8_t c8, int8_t c9, int8_t c10, int8_t c11, int8_t c12, int8_t c13, int8_t c14, int8_t c15) {
  return (wav_i8x16_t) { {
    c0, c1,  c2,  c3,  c4,  c5,  c6,  c7,
    c8, c9, c10, c11, c12, c13, c14, c15 } };
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_make(
    int16_t c0, int16_t c1, int16_t c2, int16_t c3, int16_t c4, int16_t c5, int16_t c6, int16_t c7) {
  return (wav_i16x8_t) { {
    c0, c1, c2, c3, c4, c5, c6, c7 } };
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_make(int32_t c0, int32_t c1, int32_t c2, int32_t c3) {
  return (wav_i32x4_t) { { c0, c1, c2, c3 } };
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_make(int64_t c0, int64_t c1) {
  return (wav_i64x2_t) { { c0, c1 } };
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_make(
    uint8_t c0, uint8_t c1, uint8_t  c2, uint8_t  c3, uint8_t  c4, uint8_t  c5, uint8_t  c6, uint8_t  c7,
    uint8_t c8, uint8_t c9, uint8_t c10, uint8_t c11, uint8_t c12, uint8_t c13, uint8_t c14, uint8_t c15) {
  return (wav_u8x16_t) { {
    c0, c1,  c2,  c3,  c4,  c5,  c6,  c7,
    c8, c9, c10, c11, c12, c13, c14, c15 } };
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_make(
    uint16_t c0, uint16_t c1, uint16_t c2, uint16_t c3, uint16_t c4, uint16_t c5, uint16_t c6, uint16_t c7) {
  return (wav_u16x8_t) { {
    c0, c1, c2, c3, c4, c5, c6, c7 } };
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_make(uint32_t c0, uint32_t c1, uint32_t c2, uint32_t c3) {
  return (wav_u32x4_t) { { c0, c1, c2, c3 } };
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_make(uint64_t c0, uint64_t c1) {
  return (wav_u64x2_t) { { c0, c1 } };
}

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_make(float c0, float c1, float c2, float c3) {
  return (wav_f32x4_t) { { c0, c1, c2, c3 } };
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_make(double c0, double c1) {
  return (wav_f64x2_t) { { c0, c1 } };
}

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_make(
    int8_t c0, int8_t c1, int8_t  c2, int8_t  c3, int8_t  c4, int8_t  c5, int8_t  c6, int8_t  c7,
    int8_t c8, int8_t c9, int8_t c10, int8_t c11, int8_t c12, int8_t c13, int8_t c14, int8_t c15) {
  return wav_i8x16_make(
    c0, c1,  c2,  c3,  c4,  c5,  c6,  c7,
    c8, c9, c10, c11, c12, c13, c14, c15);
}
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_make(
    int16_t c0, int16_t c1, int16_t c2, int16_t c3, int16_t c4, int16_t c5, int16_t c6, int16_t c7) {
  return wav_i16x8_make(c0, c1, c2, c3, c4, c5, c6, c7);
}
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_make(int32_t c0, int32_t c1, int32_t c2, int32_t c3) {
  return wav_i32x4_make(c0, c1, c2, c3);
}
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_make(int64_t c0, int64_t c1) {
  return wav_i64x2_make(c0, c1);
}
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_make(
    uint8_t c0, uint8_t c1, uint8_t  c2, uint8_t  c3, uint8_t  c4, uint8_t  c5, uint8_t  c6, uint8_t  c7,
    uint8_t c8, uint8_t c9, uint8_t c10, uint8_t c11, uint8_t c12, uint8_t c13, uint8_t c14, uint8_t c15) {
  return wav_u8x16_make(
    c0, c1,  c2,  c3,  c4,  c5,  c6,  c7,
    c8, c9, c10, c11, c12, c13, c14, c15);
}
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_make(
    uint16_t c0, uint16_t c1, uint16_t c2, uint16_t c3, uint16_t c4, uint16_t c5, uint16_t c6, uint16_t c7) {
  return wav_u16x8_make(c0, c1, c2, c3, c4, c5, c6, c7);
}
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_make(uint32_t c0, uint32_t c1, uint32_t c2, uint32_t c3) {
  return wav_u32x4_make(c0, c1, c2, c3);
}
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_make(uint64_t c0, uint64_t c1) {
  return wav_u64x2_make(c0, c1);
}
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_make(float c0, float c1, float c2, float c3) {
  return wav_f32x4_make(c0, c1, c2, c3);
}
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_make(double c0, double c1) {
  return wav_f64x2_make(c0, c1);
}

/* const
 *
 * Create a vector with all lanes are known at compile time.
 */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_const(
    int8_t c0, int8_t c1, int8_t  c2, int8_t  c3, int8_t  c4, int8_t  c5, int8_t  c6, int8_t  c7,
    int8_t c8, int8_t c9, int8_t c10, int8_t c11, int8_t c12, int8_t c13, int8_t c14, int8_t c15)
  WAV_REQUIRE_CONSTANT_PARAM(c0)
  WAV_REQUIRE_CONSTANT_PARAM(c1)
  WAV_REQUIRE_CONSTANT_PARAM(c2)
  WAV_REQUIRE_CONSTANT_PARAM(c3)
  WAV_REQUIRE_CONSTANT_PARAM(c4)
  WAV_REQUIRE_CONSTANT_PARAM(c5)
  WAV_REQUIRE_CONSTANT_PARAM(c6)
  WAV_REQUIRE_CONSTANT_PARAM(c7)
  WAV_REQUIRE_CONSTANT_PARAM(c8)
  WAV_REQUIRE_CONSTANT_PARAM(c9)
  WAV_REQUIRE_CONSTANT_PARAM(c10)
  WAV_REQUIRE_CONSTANT_PARAM(c11)
  WAV_REQUIRE_CONSTANT_PARAM(c12)
  WAV_REQUIRE_CONSTANT_PARAM(c13)
  WAV_REQUIRE_CONSTANT_PARAM(c14)
  WAV_REQUIRE_CONSTANT_PARAM(c15) {
  return (wav_i8x16_t) { {
    c0, c1,  c2,  c3,  c4,  c5,  c6,  c7,
    c8, c9, c10, c11, c12, c13, c14, c15
  } };
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_const(
    int16_t c0, int16_t c1, int16_t  c2, int16_t  c3, int16_t  c4, int16_t  c5, int16_t  c6, int16_t  c7)
  WAV_REQUIRE_CONSTANT_PARAM(c0)
  WAV_REQUIRE_CONSTANT_PARAM(c1)
  WAV_REQUIRE_CONSTANT_PARAM(c2)
  WAV_REQUIRE_CONSTANT_PARAM(c3)
  WAV_REQUIRE_CONSTANT_PARAM(c4)
  WAV_REQUIRE_CONSTANT_PARAM(c5)
  WAV_REQUIRE_CONSTANT_PARAM(c6)
  WAV_REQUIRE_CONSTANT_PARAM(c7) {
  return (wav_i16x8_t) { {
    c0, c1,  c2,  c3,  c4,  c5,  c6,  c7
  } };
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_const(int32_t c0, int32_t c1, int32_t c2, int32_t c3)
  WAV_REQUIRE_CONSTANT_PARAM(c0)
  WAV_REQUIRE_CONSTANT_PARAM(c1)
  WAV_REQUIRE_CONSTANT_PARAM(c2)
  WAV_REQUIRE_CONSTANT_PARAM(c3) {
  return (wav_i32x4_t) { { c0, c1, c2, c3 } };
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_const(int64_t c0, int64_t c1)
  WAV_REQUIRE_CONSTANT_PARAM(c0)
  WAV_REQUIRE_CONSTANT_PARAM(c1) {
  return (wav_i64x2_t) { { c0, c1 } };
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_const(
    uint8_t c0, uint8_t c1, uint8_t  c2, uint8_t  c3, uint8_t  c4, uint8_t  c5, uint8_t  c6, uint8_t  c7,
    uint8_t c8, uint8_t c9, uint8_t c10, uint8_t c11, uint8_t c12, uint8_t c13, uint8_t c14, uint8_t c15)
  WAV_REQUIRE_CONSTANT_PARAM(c0)
  WAV_REQUIRE_CONSTANT_PARAM(c1)
  WAV_REQUIRE_CONSTANT_PARAM(c2)
  WAV_REQUIRE_CONSTANT_PARAM(c3)
  WAV_REQUIRE_CONSTANT_PARAM(c4)
  WAV_REQUIRE_CONSTANT_PARAM(c5)
  WAV_REQUIRE_CONSTANT_PARAM(c6)
  WAV_REQUIRE_CONSTANT_PARAM(c7)
  WAV_REQUIRE_CONSTANT_PARAM(c8)
  WAV_REQUIRE_CONSTANT_PARAM(c9)
  WAV_REQUIRE_CONSTANT_PARAM(c10)
  WAV_REQUIRE_CONSTANT_PARAM(c11)
  WAV_REQUIRE_CONSTANT_PARAM(c12)
  WAV_REQUIRE_CONSTANT_PARAM(c13)
  WAV_REQUIRE_CONSTANT_PARAM(c14)
  WAV_REQUIRE_CONSTANT_PARAM(c15) {
  return (wav_u8x16_t) { {
    c0, c1,  c2,  c3,  c4,  c5,  c6,  c7,
    c8, c9, c10, c11, c12, c13, c14, c15
  } };
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_const(
    uint16_t c0, uint16_t c1, uint16_t  c2, uint16_t  c3, uint16_t  c4, uint16_t  c5, uint16_t  c6, uint16_t  c7)
  WAV_REQUIRE_CONSTANT_PARAM(c0)
  WAV_REQUIRE_CONSTANT_PARAM(c1)
  WAV_REQUIRE_CONSTANT_PARAM(c2)
  WAV_REQUIRE_CONSTANT_PARAM(c3)
  WAV_REQUIRE_CONSTANT_PARAM(c4)
  WAV_REQUIRE_CONSTANT_PARAM(c5)
  WAV_REQUIRE_CONSTANT_PARAM(c6)
  WAV_REQUIRE_CONSTANT_PARAM(c7) {
  return (wav_u16x8_t) { {
    c0, c1,  c2,  c3,  c4,  c5,  c6,  c7
  } };
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_const(uint32_t c0, uint32_t c1, uint32_t c2, uint32_t c3)
  WAV_REQUIRE_CONSTANT_PARAM(c0)
  WAV_REQUIRE_CONSTANT_PARAM(c1)
  WAV_REQUIRE_CONSTANT_PARAM(c2)
  WAV_REQUIRE_CONSTANT_PARAM(c3) {
  return (wav_u32x4_t) { { c0, c1, c2, c3 } };
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_const(uint64_t c0, uint64_t c1)
  WAV_REQUIRE_CONSTANT_PARAM(c0)
  WAV_REQUIRE_CONSTANT_PARAM(c1) {
  return (wav_u64x2_t) { { c0, c1 } };
}

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_const(float c0, float c1, float c2, float c3)
  WAV_REQUIRE_CONSTANT_PARAM(c0)
  WAV_REQUIRE_CONSTANT_PARAM(c1)
  WAV_REQUIRE_CONSTANT_PARAM(c2)
  WAV_REQUIRE_CONSTANT_PARAM(c3) {
  return (wav_f32x4_t) { { c0, c1, c2, c3 } };
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_const(double c0, double c1)
  WAV_REQUIRE_CONSTANT_PARAM(c0)
  WAV_REQUIRE_CONSTANT_PARAM(c1) {
  return (wav_f64x2_t) { { c0, c1 } };
}

WAV_OVERLOAD_ATTRIBUTES
wav_i8x16_t
wav_const(
    int8_t c0, int8_t c1, int8_t  c2, int8_t  c3, int8_t  c4, int8_t  c5, int8_t  c6, int8_t  c7,
    int8_t c8, int8_t c9, int8_t c10, int8_t c11, int8_t c12, int8_t c13, int8_t c14, int8_t c15)
  WAV_REQUIRE_CONSTANT_PARAM(c0)
  WAV_REQUIRE_CONSTANT_PARAM(c1)
  WAV_REQUIRE_CONSTANT_PARAM(c2)
  WAV_REQUIRE_CONSTANT_PARAM(c3)
  WAV_REQUIRE_CONSTANT_PARAM(c4)
  WAV_REQUIRE_CONSTANT_PARAM(c5)
  WAV_REQUIRE_CONSTANT_PARAM(c6)
  WAV_REQUIRE_CONSTANT_PARAM(c7)
  WAV_REQUIRE_CONSTANT_PARAM(c8)
  WAV_REQUIRE_CONSTANT_PARAM(c9)
  WAV_REQUIRE_CONSTANT_PARAM(c10)
  WAV_REQUIRE_CONSTANT_PARAM(c11)
  WAV_REQUIRE_CONSTANT_PARAM(c12)
  WAV_REQUIRE_CONSTANT_PARAM(c13)
  WAV_REQUIRE_CONSTANT_PARAM(c14)
  WAV_REQUIRE_CONSTANT_PARAM(c15) {
  return (wav_i8x16_t) { {
    c0, c1,  c2,  c3,  c4,  c5,  c6,  c7,
    c8, c9, c10, c11, c12, c13, c14, c15
  } };
}

WAV_OVERLOAD_ATTRIBUTES
wav_i16x8_t
wav_const(
    int16_t c0, int16_t c1, int16_t  c2, int16_t  c3, int16_t  c4, int16_t  c5, int16_t  c6, int16_t  c7)
  WAV_REQUIRE_CONSTANT_PARAM(c0)
  WAV_REQUIRE_CONSTANT_PARAM(c1)
  WAV_REQUIRE_CONSTANT_PARAM(c2)
  WAV_REQUIRE_CONSTANT_PARAM(c3)
  WAV_REQUIRE_CONSTANT_PARAM(c4)
  WAV_REQUIRE_CONSTANT_PARAM(c5)
  WAV_REQUIRE_CONSTANT_PARAM(c6)
  WAV_REQUIRE_CONSTANT_PARAM(c7) {
  return (wav_i16x8_t) { {
    c0, c1,  c2,  c3,  c4,  c5,  c6,  c7
  } };
}

WAV_OVERLOAD_ATTRIBUTES
wav_i32x4_t
wav_const(int32_t c0, int32_t c1, int32_t c2, int32_t c3)
  WAV_REQUIRE_CONSTANT_PARAM(c0)
  WAV_REQUIRE_CONSTANT_PARAM(c1)
  WAV_REQUIRE_CONSTANT_PARAM(c2)
  WAV_REQUIRE_CONSTANT_PARAM(c3) {
  return (wav_i32x4_t) { { c0, c1, c2, c3 } };
}

WAV_OVERLOAD_ATTRIBUTES
wav_i64x2_t
wav_const(int64_t c0, int64_t c1)
  WAV_REQUIRE_CONSTANT_PARAM(c0)
  WAV_REQUIRE_CONSTANT_PARAM(c1) {
  return (wav_i64x2_t) { { c0, c1 } };
}

WAV_OVERLOAD_ATTRIBUTES
wav_u8x16_t
wav_const(
    uint8_t c0, uint8_t c1, uint8_t  c2, uint8_t  c3, uint8_t  c4, uint8_t  c5, uint8_t  c6, uint8_t  c7,
    uint8_t c8, uint8_t c9, uint8_t c10, uint8_t c11, uint8_t c12, uint8_t c13, uint8_t c14, uint8_t c15)
  WAV_REQUIRE_CONSTANT_PARAM(c0)
  WAV_REQUIRE_CONSTANT_PARAM(c1)
  WAV_REQUIRE_CONSTANT_PARAM(c2)
  WAV_REQUIRE_CONSTANT_PARAM(c3)
  WAV_REQUIRE_CONSTANT_PARAM(c4)
  WAV_REQUIRE_CONSTANT_PARAM(c5)
  WAV_REQUIRE_CONSTANT_PARAM(c6)
  WAV_REQUIRE_CONSTANT_PARAM(c7)
  WAV_REQUIRE_CONSTANT_PARAM(c8)
  WAV_REQUIRE_CONSTANT_PARAM(c9)
  WAV_REQUIRE_CONSTANT_PARAM(c10)
  WAV_REQUIRE_CONSTANT_PARAM(c11)
  WAV_REQUIRE_CONSTANT_PARAM(c12)
  WAV_REQUIRE_CONSTANT_PARAM(c13)
  WAV_REQUIRE_CONSTANT_PARAM(c14)
  WAV_REQUIRE_CONSTANT_PARAM(c15) {
  return (wav_u8x16_t) { {
    c0, c1,  c2,  c3,  c4,  c5,  c6,  c7,
    c8, c9, c10, c11, c12, c13, c14, c15
  } };
}

WAV_OVERLOAD_ATTRIBUTES
wav_u16x8_t
wav_const(
    uint16_t c0, uint16_t c1, uint16_t  c2, uint16_t  c3, uint16_t  c4, uint16_t  c5, uint16_t  c6, uint16_t  c7)
  WAV_REQUIRE_CONSTANT_PARAM(c0)
  WAV_REQUIRE_CONSTANT_PARAM(c1)
  WAV_REQUIRE_CONSTANT_PARAM(c2)
  WAV_REQUIRE_CONSTANT_PARAM(c3)
  WAV_REQUIRE_CONSTANT_PARAM(c4)
  WAV_REQUIRE_CONSTANT_PARAM(c5)
  WAV_REQUIRE_CONSTANT_PARAM(c6)
  WAV_REQUIRE_CONSTANT_PARAM(c7) {
  return (wav_u16x8_t) { {
    c0, c1,  c2,  c3,  c4,  c5,  c6,  c7
  } };
}

WAV_OVERLOAD_ATTRIBUTES
wav_u32x4_t
wav_const(uint32_t c0, uint32_t c1, uint32_t c2, uint32_t c3)
  WAV_REQUIRE_CONSTANT_PARAM(c0)
  WAV_REQUIRE_CONSTANT_PARAM(c1)
  WAV_REQUIRE_CONSTANT_PARAM(c2)
  WAV_REQUIRE_CONSTANT_PARAM(c3) {
  return (wav_u32x4_t) { { c0, c1, c2, c3 } };
}

WAV_OVERLOAD_ATTRIBUTES
wav_u64x2_t
wav_const(uint64_t c0, uint64_t c1)
  WAV_REQUIRE_CONSTANT_PARAM(c0)
  WAV_REQUIRE_CONSTANT_PARAM(c1) {
  return (wav_u64x2_t) { { c0, c1 } };
}

WAV_OVERLOAD_ATTRIBUTES
wav_f32x4_t
wav_const(float c0, float c1, float c2, float c3)
  WAV_REQUIRE_CONSTANT_PARAM(c0)
  WAV_REQUIRE_CONSTANT_PARAM(c1)
  WAV_REQUIRE_CONSTANT_PARAM(c2)
  WAV_REQUIRE_CONSTANT_PARAM(c3) {
  return (wav_f32x4_t) { { c0, c1, c2, c3 } };
}

WAV_OVERLOAD_ATTRIBUTES
wav_f64x2_t
wav_const(double c0, double c1)
  WAV_REQUIRE_CONSTANT_PARAM(c0)
  WAV_REQUIRE_CONSTANT_PARAM(c1) {
  return (wav_f64x2_t) { { c0, c1 } };
}

/* extract_lane -- Extract lane as a scalar
 *
 * Extract the scalar value of lane specified in the immediate mode
 * operand lane in v.
 */

WAV_FUNCTION_ATTRIBUTES   int8_t wav_i8x16_extract_lane(wav_i8x16_t v, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0, 15) { return v.values[lane]; }
WAV_FUNCTION_ATTRIBUTES  int16_t wav_i16x8_extract_lane(wav_i16x8_t v, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  7) { return v.values[lane]; }
WAV_FUNCTION_ATTRIBUTES  int32_t wav_i32x4_extract_lane(wav_i32x4_t v, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  3) { return v.values[lane]; }
WAV_FUNCTION_ATTRIBUTES  int64_t wav_i64x2_extract_lane(wav_i64x2_t v, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  1) { return v.values[lane]; }
WAV_FUNCTION_ATTRIBUTES  uint8_t wav_u8x16_extract_lane(wav_u8x16_t v, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0, 15) { return v.values[lane]; }
WAV_FUNCTION_ATTRIBUTES uint16_t wav_u16x8_extract_lane(wav_u16x8_t v, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  7) { return v.values[lane]; }
WAV_FUNCTION_ATTRIBUTES uint32_t wav_u32x4_extract_lane(wav_u32x4_t v, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  3) { return v.values[lane]; }
WAV_FUNCTION_ATTRIBUTES uint64_t wav_u64x2_extract_lane(wav_u64x2_t v, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  1) { return v.values[lane]; }
WAV_FUNCTION_ATTRIBUTES    float wav_f32x4_extract_lane(wav_f32x4_t v, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  3) { return v.values[lane]; }
WAV_FUNCTION_ATTRIBUTES   double wav_f64x2_extract_lane(wav_f64x2_t v, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  1) { return v.values[lane]; }

WAV_OVERLOAD_ATTRIBUTES   int8_t wav_extract_lane(wav_i8x16_t v, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0, 15) { return v.values[lane]; }
WAV_OVERLOAD_ATTRIBUTES  int16_t wav_extract_lane(wav_i16x8_t v, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  7) { return v.values[lane]; }
WAV_OVERLOAD_ATTRIBUTES  int32_t wav_extract_lane(wav_i32x4_t v, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  3) { return v.values[lane]; }
WAV_OVERLOAD_ATTRIBUTES  int64_t wav_extract_lane(wav_i64x2_t v, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  1) { return v.values[lane]; }
WAV_OVERLOAD_ATTRIBUTES  uint8_t wav_extract_lane(wav_u8x16_t v, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0, 15) { return v.values[lane]; }
WAV_OVERLOAD_ATTRIBUTES uint16_t wav_extract_lane(wav_u16x8_t v, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  7) { return v.values[lane]; }
WAV_OVERLOAD_ATTRIBUTES uint32_t wav_extract_lane(wav_u32x4_t v, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  3) { return v.values[lane]; }
WAV_OVERLOAD_ATTRIBUTES uint64_t wav_extract_lane(wav_u64x2_t v, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  1) { return v.values[lane]; }
WAV_OVERLOAD_ATTRIBUTES    float wav_extract_lane(wav_f32x4_t v, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  3) { return v.values[lane]; }
WAV_OVERLOAD_ATTRIBUTES   double wav_extract_lane(wav_f64x2_t v, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  1) { return v.values[lane]; }

/* replace_lane -- Replace lane value
 *
 * Return a new vector with lanes identical to v, except for the lane
 * specified in the immediate mode operand lane which has the value
 * value.
 */

WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_i8x16_replace_lane(wav_i8x16_t v, const int lane,   int8_t value) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0, 15) { v.values[lane] = value; return v; }
WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_i16x8_replace_lane(wav_i16x8_t v, const int lane,  int16_t value) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  7) { v.values[lane] = value; return v; }
WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_i32x4_replace_lane(wav_i32x4_t v, const int lane,  int32_t value) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  3) { v.values[lane] = value; return v; }
WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_i64x2_replace_lane(wav_i64x2_t v, const int lane,  int64_t value) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  1) { v.values[lane] = value; return v; }
WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_u8x16_replace_lane(wav_u8x16_t v, const int lane,  uint8_t value) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0, 15) { v.values[lane] = value; return v; }
WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_u16x8_replace_lane(wav_u16x8_t v, const int lane, uint16_t value) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  7) { v.values[lane] = value; return v; }
WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_u32x4_replace_lane(wav_u32x4_t v, const int lane, uint32_t value) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  3) { v.values[lane] = value; return v; }
WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_u64x2_replace_lane(wav_u64x2_t v, const int lane, uint64_t value) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  1) { v.values[lane] = value; return v; }
WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_f32x4_replace_lane(wav_f32x4_t v, const int lane,    float value) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  3) { v.values[lane] = value; return v; }
WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_f64x2_replace_lane(wav_f64x2_t v, const int lane,   double value) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  1) { v.values[lane] = value; return v; }

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_replace_lane(wav_i8x16_t v, const int lane,   int8_t value) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0, 15) { v.values[lane] = value; return v; }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_replace_lane(wav_i16x8_t v, const int lane,  int16_t value) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  7) { v.values[lane] = value; return v; }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_replace_lane(wav_i32x4_t v, const int lane,  int32_t value) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  3) { v.values[lane] = value; return v; }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_replace_lane(wav_i64x2_t v, const int lane,  int64_t value) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  1) { v.values[lane] = value; return v; }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_replace_lane(wav_u8x16_t v, const int lane,  uint8_t value) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0, 15) { v.values[lane] = value; return v; }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_replace_lane(wav_u16x8_t v, const int lane, uint16_t value) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  7) { v.values[lane] = value; return v; }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_replace_lane(wav_u32x4_t v, const int lane, uint32_t value) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  3) { v.values[lane] = value; return v; }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_replace_lane(wav_u64x2_t v, const int lane, uint64_t value) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  1) { v.values[lane] = value; return v; }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_replace_lane(wav_f32x4_t v, const int lane,    float value) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  3) { v.values[lane] = value; return v; }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_replace_lane(wav_f64x2_t v, const int lane,   double value) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  1) { v.values[lane] = value; return v; }

/* and -- Bitwise AND */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_and(wav_i8x16_t a, wav_i8x16_t b) {
  return (wav_i8x16_t) { a.values & b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_and(wav_i16x8_t a, wav_i16x8_t b) {
  return (wav_i16x8_t) { a.values & b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_and(wav_i32x4_t a, wav_i32x4_t b) {
  return (wav_i32x4_t) { a.values & b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_and(wav_i64x2_t a, wav_i64x2_t b) {
  return (wav_i64x2_t) { a.values & b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_and(wav_u8x16_t a, wav_u8x16_t b) {
  return (wav_u8x16_t) { a.values & b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_and(wav_u16x8_t a, wav_u16x8_t b) {
  return (wav_u16x8_t) { a.values & b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_and(wav_u32x4_t a, wav_u32x4_t b) {
  return (wav_u32x4_t) { a.values & b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_and(wav_u64x2_t a, wav_u64x2_t b) {
  return (wav_u64x2_t) { a.values & b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_b8x16_and(wav_b8x16_t a, wav_b8x16_t b) {
  return (wav_b8x16_t) { a.values & b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_b16x8_and(wav_b16x8_t a, wav_b16x8_t b) {
  return (wav_b16x8_t) { a.values & b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_b32x4_and(wav_b32x4_t a, wav_b32x4_t b) {
  return (wav_b32x4_t) { a.values & b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_b64x2_and(wav_b64x2_t a, wav_b64x2_t b) {
  return (wav_b64x2_t) { a.values & b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_and_b8x16(wav_i8x16_t a, wav_b8x16_t b) {
  return (wav_i8x16_t) { a.values & b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_and_b16x8(wav_i16x8_t a, wav_b16x8_t b) {
  return (wav_i16x8_t) { a.values & b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_and_b32x4(wav_i32x4_t a, wav_b32x4_t b) {
  return (wav_i32x4_t) { a.values & b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_and_b64x2(wav_i64x2_t a, wav_b64x2_t b) {
  return (wav_i64x2_t) { a.values & b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_and_b8x16(wav_u8x16_t a, wav_b8x16_t b) {
  return (wav_u8x16_t) { a.values & b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_and_b16x8(wav_u16x8_t a, wav_b16x8_t b) {
  return (wav_u16x8_t) { a.values & b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_and_b32x4(wav_u32x4_t a, wav_b32x4_t b) {
  return (wav_u32x4_t) { a.values & b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_and_b64x2(wav_u64x2_t a, wav_b64x2_t b) {
  return (wav_u64x2_t) { a.values & b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_and_b32x4(wav_f32x4_t a, wav_b32x4_t b) {
  return wav_u32x4_as_f32x4(wav_u32x4_and_b32x4(wav_f32x4_as_u32x4(a), b));
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_and_b64x2(wav_f64x2_t a, wav_b64x2_t b) {
  return wav_u64x2_as_f64x2(wav_u64x2_and_b64x2(wav_f64x2_as_u64x2(a), b));
}

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_b8x16_and_i8x16(wav_b8x16_t a, wav_i8x16_t b) {
  return wav_i8x16_and_b8x16(b, a);
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_b16x8_and_i16x8(wav_b16x8_t a, wav_i16x8_t b) {
  return wav_i16x8_and_b16x8(b, a);
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_b32x4_and_i32x4(wav_b32x4_t a, wav_i32x4_t b) {
  return wav_i32x4_and_b32x4(b, a);
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_b64x2_and_i64x2(wav_b64x2_t a, wav_i64x2_t b) {
  return wav_i64x2_and_b64x2(b, a);
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_b8x16_and_u8x16(wav_b8x16_t a, wav_u8x16_t b) {
  return wav_u8x16_and_b8x16(b, a);
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_b16x8_and_u16x8(wav_b16x8_t a, wav_u16x8_t b) {
  return wav_u16x8_and_b16x8(b, a);
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_b32x4_and_u32x4(wav_b32x4_t a, wav_u32x4_t b) {
  return wav_u32x4_and_b32x4(b, a);
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_b64x2_and_u64x2(wav_b64x2_t a, wav_u64x2_t b) {
  return wav_u64x2_and_b64x2(b, a);
}

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_b32x4_and_f32x4(wav_b32x4_t a, wav_f32x4_t b) {
  return wav_f32x4_and_b32x4(b, a);
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_b64x2_and_f64x2(wav_b64x2_t a, wav_f64x2_t b) {
  return wav_f64x2_and_b64x2(b, a);
}

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_and(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_and(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_and(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_and(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_and(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_and(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_and(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_and(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_and(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_and(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_and(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_and(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_and(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_and(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_and(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_and(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t wav_and(wav_b8x16_t a, wav_b8x16_t b) { return wav_b8x16_and(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t wav_and(wav_b16x8_t a, wav_b16x8_t b) { return wav_b16x8_and(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_and(wav_b32x4_t a, wav_b32x4_t b) { return wav_b32x4_and(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_and(wav_b64x2_t a, wav_b64x2_t b) { return wav_b64x2_and(a, b); }

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_and(wav_i8x16_t a, wav_b8x16_t b) { return wav_i8x16_and_b8x16(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_and(wav_i16x8_t a, wav_b16x8_t b) { return wav_i16x8_and_b16x8(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_and(wav_i32x4_t a, wav_b32x4_t b) { return wav_i32x4_and_b32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_and(wav_i64x2_t a, wav_b64x2_t b) { return wav_i64x2_and_b64x2(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_and(wav_u8x16_t a, wav_b8x16_t b) { return wav_u8x16_and_b8x16(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_and(wav_u16x8_t a, wav_b16x8_t b) { return wav_u16x8_and_b16x8(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_and(wav_u32x4_t a, wav_b32x4_t b) { return wav_u32x4_and_b32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_and(wav_u64x2_t a, wav_b64x2_t b) { return wav_u64x2_and_b64x2(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_and(wav_f32x4_t a, wav_b32x4_t b) { return wav_f32x4_and_b32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_and(wav_f64x2_t a, wav_b64x2_t b) { return wav_f64x2_and_b64x2(a, b); }

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_and(wav_b8x16_t a, wav_i8x16_t b) { return wav_b8x16_and_i8x16(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_and(wav_b16x8_t a, wav_i16x8_t b) { return wav_b16x8_and_i16x8(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_and(wav_b32x4_t a, wav_i32x4_t b) { return wav_b32x4_and_i32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_and(wav_b64x2_t a, wav_i64x2_t b) { return wav_b64x2_and_i64x2(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_and(wav_b8x16_t a, wav_u8x16_t b) { return wav_b8x16_and_u8x16(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_and(wav_b16x8_t a, wav_u16x8_t b) { return wav_b16x8_and_u16x8(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_and(wav_b32x4_t a, wav_u32x4_t b) { return wav_b32x4_and_u32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_and(wav_b64x2_t a, wav_u64x2_t b) { return wav_b64x2_and_u64x2(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_and(wav_b32x4_t a, wav_f32x4_t b) { return wav_b32x4_and_f32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_and(wav_b64x2_t a, wav_f64x2_t b) { return wav_b64x2_and_f64x2(a, b); }

#if WAV_OPERATOR_OVERLOADING
WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t operator&(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_and(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t operator&(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_and(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t operator&(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_and(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t operator&(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_and(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t operator&(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_and(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t operator&(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_and(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t operator&(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_and(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t operator&(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_and(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t operator&(wav_b8x16_t a, wav_b8x16_t b) { return wav_b8x16_and(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t operator&(wav_b16x8_t a, wav_b16x8_t b) { return wav_b16x8_and(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t operator&(wav_b32x4_t a, wav_b32x4_t b) { return wav_b32x4_and(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t operator&(wav_b64x2_t a, wav_b64x2_t b) { return wav_b64x2_and(a, b); }

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t operator&(wav_i8x16_t a, wav_b8x16_t b) { return wav_i8x16_and_b8x16(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t operator&(wav_i16x8_t a, wav_b16x8_t b) { return wav_i16x8_and_b16x8(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t operator&(wav_i32x4_t a, wav_b32x4_t b) { return wav_i32x4_and_b32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t operator&(wav_i64x2_t a, wav_b64x2_t b) { return wav_i64x2_and_b64x2(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t operator&(wav_u8x16_t a, wav_b8x16_t b) { return wav_u8x16_and_b8x16(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t operator&(wav_u16x8_t a, wav_b16x8_t b) { return wav_u16x8_and_b16x8(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t operator&(wav_u32x4_t a, wav_b32x4_t b) { return wav_u32x4_and_b32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t operator&(wav_u64x2_t a, wav_b64x2_t b) { return wav_u64x2_and_b64x2(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t operator&(wav_f32x4_t a, wav_b32x4_t b) { return wav_f32x4_and_b32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t operator&(wav_f64x2_t a, wav_b64x2_t b) { return wav_f64x2_and_b64x2(a, b); }

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t operator&(wav_b8x16_t a, wav_i8x16_t b) { return wav_b8x16_and_i8x16(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t operator&(wav_b16x8_t a, wav_i16x8_t b) { return wav_b16x8_and_i16x8(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t operator&(wav_b32x4_t a, wav_i32x4_t b) { return wav_b32x4_and_i32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t operator&(wav_b64x2_t a, wav_i64x2_t b) { return wav_b64x2_and_i64x2(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t operator&(wav_b8x16_t a, wav_u8x16_t b) { return wav_b8x16_and_u8x16(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t operator&(wav_b16x8_t a, wav_u16x8_t b) { return wav_b16x8_and_u16x8(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t operator&(wav_b32x4_t a, wav_u32x4_t b) { return wav_b32x4_and_u32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t operator&(wav_b64x2_t a, wav_u64x2_t b) { return wav_b64x2_and_u64x2(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t operator&(wav_b32x4_t a, wav_f32x4_t b) { return wav_b32x4_and_f32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t operator&(wav_b64x2_t a, wav_f64x2_t b) { return wav_b64x2_and_f64x2(a, b); }
#endif

/* or -- Bitwise OR */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_or(wav_i8x16_t a, wav_i8x16_t b) {
  return (wav_i8x16_t) { a.values | b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_or(wav_i16x8_t a, wav_i16x8_t b) {
  return (wav_i16x8_t) { a.values | b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_or(wav_i32x4_t a, wav_i32x4_t b) {
  return (wav_i32x4_t) { a.values | b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_or(wav_i64x2_t a, wav_i64x2_t b) {
  return (wav_i64x2_t) { a.values | b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_or(wav_u8x16_t a, wav_u8x16_t b) {
  return (wav_u8x16_t) { a.values | b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_or(wav_u16x8_t a, wav_u16x8_t b) {
  return (wav_u16x8_t) { a.values | b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_or(wav_u32x4_t a, wav_u32x4_t b) {
  return (wav_u32x4_t) { a.values | b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_or(wav_u64x2_t a, wav_u64x2_t b) {
  return (wav_u64x2_t) { a.values | b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_b8x16_or(wav_b8x16_t a, wav_b8x16_t b) {
  return (wav_b8x16_t) { a.values | b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_b16x8_or(wav_b16x8_t a, wav_b16x8_t b) {
  return (wav_b16x8_t) { a.values | b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_b32x4_or(wav_b32x4_t a, wav_b32x4_t b) {
  return (wav_b32x4_t) { a.values | b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_b64x2_or(wav_b64x2_t a, wav_b64x2_t b) {
  return (wav_b64x2_t) { a.values | b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_or_b8x16(wav_i8x16_t a, wav_b8x16_t b) {
  return (wav_i8x16_t) { a.values | b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_or_b16x8(wav_i16x8_t a, wav_b16x8_t b) {
  return (wav_i16x8_t) { a.values | b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_or_b32x4(wav_i32x4_t a, wav_b32x4_t b) {
  return (wav_i32x4_t) { a.values | b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_or_b64x2(wav_i64x2_t a, wav_b64x2_t b) {
  return (wav_i64x2_t) { a.values | b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_or_b8x16(wav_u8x16_t a, wav_b8x16_t b) {
  return (wav_u8x16_t) { a.values | b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_or_b16x8(wav_u16x8_t a, wav_b16x8_t b) {
  return (wav_u16x8_t) { a.values | b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_or_b32x4(wav_u32x4_t a, wav_b32x4_t b) {
  return (wav_u32x4_t) { a.values | b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_or_b64x2(wav_u64x2_t a, wav_b64x2_t b) {
  return (wav_u64x2_t) { a.values | b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_or_b32x4(wav_f32x4_t a, wav_b32x4_t b) {
  return wav_u32x4_as_f32x4(wav_u32x4_or_b32x4(wav_f32x4_as_u32x4(a), b));
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_or_b64x2(wav_f64x2_t a, wav_b64x2_t b) {
  return wav_u64x2_as_f64x2(wav_u64x2_or_b64x2(wav_f64x2_as_u64x2(a), b));
}

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_b8x16_or_i8x16(wav_b8x16_t a, wav_i8x16_t b) {
  return wav_i8x16_or_b8x16(b, a);
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_b16x8_or_i16x8(wav_b16x8_t a, wav_i16x8_t b) {
  return wav_i16x8_or_b16x8(b, a);
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_b32x4_or_i32x4(wav_b32x4_t a, wav_i32x4_t b) {
  return wav_i32x4_or_b32x4(b, a);
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_b64x2_or_i64x2(wav_b64x2_t a, wav_i64x2_t b) {
  return wav_i64x2_or_b64x2(b, a);
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_b8x16_or_u8x16(wav_b8x16_t a, wav_u8x16_t b) {
  return wav_u8x16_or_b8x16(b, a);
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_b16x8_or_u16x8(wav_b16x8_t a, wav_u16x8_t b) {
  return wav_u16x8_or_b16x8(b, a);
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_b32x4_or_u32x4(wav_b32x4_t a, wav_u32x4_t b) {
  return wav_u32x4_or_b32x4(b, a);
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_b64x2_or_u64x2(wav_b64x2_t a, wav_u64x2_t b) {
  return wav_u64x2_or_b64x2(b, a);
}

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_b32x4_or_f32x4(wav_b32x4_t a, wav_f32x4_t b) {
  return wav_f32x4_or_b32x4(b, a);
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_b64x2_or_f64x2(wav_b64x2_t a, wav_f64x2_t b) {
  return wav_f64x2_or_b64x2(b, a);
}

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_or(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_or(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_or(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_or(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_or(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_or(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_or(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_or(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_or(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_or(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_or(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_or(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_or(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_or(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_or(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_or(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t wav_or(wav_b8x16_t a, wav_b8x16_t b) { return wav_b8x16_or(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t wav_or(wav_b16x8_t a, wav_b16x8_t b) { return wav_b16x8_or(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_or(wav_b32x4_t a, wav_b32x4_t b) { return wav_b32x4_or(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_or(wav_b64x2_t a, wav_b64x2_t b) { return wav_b64x2_or(a, b); }

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_or(wav_i8x16_t a, wav_b8x16_t b) { return wav_i8x16_or_b8x16(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_or(wav_i16x8_t a, wav_b16x8_t b) { return wav_i16x8_or_b16x8(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_or(wav_i32x4_t a, wav_b32x4_t b) { return wav_i32x4_or_b32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_or(wav_i64x2_t a, wav_b64x2_t b) { return wav_i64x2_or_b64x2(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_or(wav_u8x16_t a, wav_b8x16_t b) { return wav_u8x16_or_b8x16(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_or(wav_u16x8_t a, wav_b16x8_t b) { return wav_u16x8_or_b16x8(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_or(wav_u32x4_t a, wav_b32x4_t b) { return wav_u32x4_or_b32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_or(wav_u64x2_t a, wav_b64x2_t b) { return wav_u64x2_or_b64x2(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_or(wav_f32x4_t a, wav_b32x4_t b) { return wav_f32x4_or_b32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_or(wav_f64x2_t a, wav_b64x2_t b) { return wav_f64x2_or_b64x2(a, b); }

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_or(wav_b8x16_t a, wav_i8x16_t b) { return wav_b8x16_or_i8x16(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_or(wav_b16x8_t a, wav_i16x8_t b) { return wav_b16x8_or_i16x8(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_or(wav_b32x4_t a, wav_i32x4_t b) { return wav_b32x4_or_i32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_or(wav_b64x2_t a, wav_i64x2_t b) { return wav_b64x2_or_i64x2(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_or(wav_b8x16_t a, wav_u8x16_t b) { return wav_b8x16_or_u8x16(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_or(wav_b16x8_t a, wav_u16x8_t b) { return wav_b16x8_or_u16x8(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_or(wav_b32x4_t a, wav_u32x4_t b) { return wav_b32x4_or_u32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_or(wav_b64x2_t a, wav_u64x2_t b) { return wav_b64x2_or_u64x2(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_or(wav_b32x4_t a, wav_f32x4_t b) { return wav_b32x4_or_f32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_or(wav_b64x2_t a, wav_f64x2_t b) { return wav_b64x2_or_f64x2(a, b); }

#if WAV_OPERATOR_OVERLOADING
WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t operator|(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_or(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t operator|(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_or(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t operator|(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_or(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t operator|(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_or(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t operator|(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_or(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t operator|(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_or(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t operator|(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_or(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t operator|(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_or(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t operator|(wav_b8x16_t a, wav_b8x16_t b) { return wav_b8x16_or(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t operator|(wav_b16x8_t a, wav_b16x8_t b) { return wav_b16x8_or(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t operator|(wav_b32x4_t a, wav_b32x4_t b) { return wav_b32x4_or(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t operator|(wav_b64x2_t a, wav_b64x2_t b) { return wav_b64x2_or(a, b); }

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t operator|(wav_i8x16_t a, wav_b8x16_t b) { return wav_i8x16_or_b8x16(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t operator|(wav_i16x8_t a, wav_b16x8_t b) { return wav_i16x8_or_b16x8(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t operator|(wav_i32x4_t a, wav_b32x4_t b) { return wav_i32x4_or_b32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t operator|(wav_i64x2_t a, wav_b64x2_t b) { return wav_i64x2_or_b64x2(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t operator|(wav_u8x16_t a, wav_b8x16_t b) { return wav_u8x16_or_b8x16(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t operator|(wav_u16x8_t a, wav_b16x8_t b) { return wav_u16x8_or_b16x8(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t operator|(wav_u32x4_t a, wav_b32x4_t b) { return wav_u32x4_or_b32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t operator|(wav_u64x2_t a, wav_b64x2_t b) { return wav_u64x2_or_b64x2(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t operator|(wav_f32x4_t a, wav_b32x4_t b) { return wav_f32x4_or_b32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t operator|(wav_f64x2_t a, wav_b64x2_t b) { return wav_f64x2_or_b64x2(a, b); }

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t operator|(wav_b8x16_t a, wav_i8x16_t b) { return wav_b8x16_or_i8x16(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t operator|(wav_b16x8_t a, wav_i16x8_t b) { return wav_b16x8_or_i16x8(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t operator|(wav_b32x4_t a, wav_i32x4_t b) { return wav_b32x4_or_i32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t operator|(wav_b64x2_t a, wav_i64x2_t b) { return wav_b64x2_or_i64x2(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t operator|(wav_b8x16_t a, wav_u8x16_t b) { return wav_b8x16_or_u8x16(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t operator|(wav_b16x8_t a, wav_u16x8_t b) { return wav_b16x8_or_u16x8(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t operator|(wav_b32x4_t a, wav_u32x4_t b) { return wav_b32x4_or_u32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t operator|(wav_b64x2_t a, wav_u64x2_t b) { return wav_b64x2_or_u64x2(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t operator|(wav_b32x4_t a, wav_f32x4_t b) { return wav_b32x4_or_f32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t operator|(wav_b64x2_t a, wav_f64x2_t b) { return wav_b64x2_or_f64x2(a, b); }
#endif

/* xor -- Bitwise XOR */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_xor(wav_i8x16_t a, wav_i8x16_t b) {
  return (wav_i8x16_t) { a.values ^ b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_xor(wav_i16x8_t a, wav_i16x8_t b) {
  return (wav_i16x8_t) { a.values ^ b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_xor(wav_i32x4_t a, wav_i32x4_t b) {
  return (wav_i32x4_t) { a.values ^ b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_xor(wav_i64x2_t a, wav_i64x2_t b) {
  return (wav_i64x2_t) { a.values ^ b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_xor(wav_u8x16_t a, wav_u8x16_t b) {
  return (wav_u8x16_t) { a.values ^ b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_xor(wav_u16x8_t a, wav_u16x8_t b) {
  return (wav_u16x8_t) { a.values ^ b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_xor(wav_u32x4_t a, wav_u32x4_t b) {
  return (wav_u32x4_t) { a.values ^ b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_xor(wav_u64x2_t a, wav_u64x2_t b) {
  return (wav_u64x2_t) { a.values ^ b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_b8x16_xor(wav_b8x16_t a, wav_b8x16_t b) {
  return (wav_b8x16_t) { a.values ^ b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_b16x8_xor(wav_b16x8_t a, wav_b16x8_t b) {
  return (wav_b16x8_t) { a.values ^ b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_b32x4_xor(wav_b32x4_t a, wav_b32x4_t b) {
  return (wav_b32x4_t) { a.values ^ b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_b64x2_xor(wav_b64x2_t a, wav_b64x2_t b) {
  return (wav_b64x2_t) { a.values ^ b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_xor_b8x16(wav_i8x16_t a, wav_b8x16_t b) {
  return (wav_i8x16_t) { a.values ^ b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_xor_b16x8(wav_i16x8_t a, wav_b16x8_t b) {
  return (wav_i16x8_t) { a.values ^ b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_xor_b32x4(wav_i32x4_t a, wav_b32x4_t b) {
  return (wav_i32x4_t) { a.values ^ b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_xor_b64x2(wav_i64x2_t a, wav_b64x2_t b) {
  return (wav_i64x2_t) { a.values ^ b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_xor_b8x16(wav_u8x16_t a, wav_b8x16_t b) {
  return (wav_u8x16_t) { a.values ^ b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_xor_b16x8(wav_u16x8_t a, wav_b16x8_t b) {
  return (wav_u16x8_t) { a.values ^ b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_xor_b32x4(wav_u32x4_t a, wav_b32x4_t b) {
  return (wav_u32x4_t) { a.values ^ b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_xor_b64x2(wav_u64x2_t a, wav_b64x2_t b) {
  return (wav_u64x2_t) { a.values ^ b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_xor_b32x4(wav_f32x4_t a, wav_b32x4_t b) {
  return wav_u32x4_as_f32x4(wav_u32x4_xor_b32x4(wav_f32x4_as_u32x4(a), b));
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_xor_b64x2(wav_f64x2_t a, wav_b64x2_t b) {
  return wav_u64x2_as_f64x2(wav_u64x2_xor_b64x2(wav_f64x2_as_u64x2(a), b));
}

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_b8x16_xor_i8x16(wav_b8x16_t a, wav_i8x16_t b) {
  return wav_i8x16_xor_b8x16(b, a);
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_b16x8_xor_i16x8(wav_b16x8_t a, wav_i16x8_t b) {
  return wav_i16x8_xor_b16x8(b, a);
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_b32x4_xor_i32x4(wav_b32x4_t a, wav_i32x4_t b) {
  return wav_i32x4_xor_b32x4(b, a);
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_b64x2_xor_i64x2(wav_b64x2_t a, wav_i64x2_t b) {
  return wav_i64x2_xor_b64x2(b, a);
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_b8x16_xor_u8x16(wav_b8x16_t a, wav_u8x16_t b) {
  return wav_u8x16_xor_b8x16(b, a);
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_b16x8_xor_u16x8(wav_b16x8_t a, wav_u16x8_t b) {
  return wav_u16x8_xor_b16x8(b, a);
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_b32x4_xor_u32x4(wav_b32x4_t a, wav_u32x4_t b) {
  return wav_u32x4_xor_b32x4(b, a);
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_b64x2_xor_u64x2(wav_b64x2_t a, wav_u64x2_t b) {
  return wav_u64x2_xor_b64x2(b, a);
}

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_b32x4_xor_f32x4(wav_b32x4_t a, wav_f32x4_t b) {
  return wav_f32x4_xor_b32x4(b, a);
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_b64x2_xor_f64x2(wav_b64x2_t a, wav_f64x2_t b) {
  return wav_f64x2_xor_b64x2(b, a);
}

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_xor(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_xor(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_xor(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_xor(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_xor(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_xor(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_xor(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_xor(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_xor(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_xor(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_xor(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_xor(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_xor(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_xor(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_xor(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_xor(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t wav_xor(wav_b8x16_t a, wav_b8x16_t b) { return wav_b8x16_xor(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t wav_xor(wav_b16x8_t a, wav_b16x8_t b) { return wav_b16x8_xor(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_xor(wav_b32x4_t a, wav_b32x4_t b) { return wav_b32x4_xor(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_xor(wav_b64x2_t a, wav_b64x2_t b) { return wav_b64x2_xor(a, b); }

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_xor(wav_i8x16_t a, wav_b8x16_t b) { return wav_i8x16_xor_b8x16(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_xor(wav_i16x8_t a, wav_b16x8_t b) { return wav_i16x8_xor_b16x8(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_xor(wav_i32x4_t a, wav_b32x4_t b) { return wav_i32x4_xor_b32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_xor(wav_i64x2_t a, wav_b64x2_t b) { return wav_i64x2_xor_b64x2(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_xor(wav_u8x16_t a, wav_b8x16_t b) { return wav_u8x16_xor_b8x16(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_xor(wav_u16x8_t a, wav_b16x8_t b) { return wav_u16x8_xor_b16x8(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_xor(wav_u32x4_t a, wav_b32x4_t b) { return wav_u32x4_xor_b32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_xor(wav_u64x2_t a, wav_b64x2_t b) { return wav_u64x2_xor_b64x2(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_xor(wav_f32x4_t a, wav_b32x4_t b) { return wav_f32x4_xor_b32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_xor(wav_f64x2_t a, wav_b64x2_t b) { return wav_f64x2_xor_b64x2(a, b); }

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_xor(wav_b8x16_t a, wav_i8x16_t b) { return wav_b8x16_xor_i8x16(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_xor(wav_b16x8_t a, wav_i16x8_t b) { return wav_b16x8_xor_i16x8(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_xor(wav_b32x4_t a, wav_i32x4_t b) { return wav_b32x4_xor_i32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_xor(wav_b64x2_t a, wav_i64x2_t b) { return wav_b64x2_xor_i64x2(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_xor(wav_b8x16_t a, wav_u8x16_t b) { return wav_b8x16_xor_u8x16(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_xor(wav_b16x8_t a, wav_u16x8_t b) { return wav_b16x8_xor_u16x8(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_xor(wav_b32x4_t a, wav_u32x4_t b) { return wav_b32x4_xor_u32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_xor(wav_b64x2_t a, wav_u64x2_t b) { return wav_b64x2_xor_u64x2(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_xor(wav_b32x4_t a, wav_f32x4_t b) { return wav_b32x4_xor_f32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_xor(wav_b64x2_t a, wav_f64x2_t b) { return wav_b64x2_xor_f64x2(a, b); }

#if WAV_OPERATOR_OVERLOADING
WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t operator^(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_xor(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t operator^(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_xor(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t operator^(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_xor(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t operator^(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_xor(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t operator^(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_xor(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t operator^(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_xor(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t operator^(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_xor(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t operator^(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_xor(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t operator^(wav_b8x16_t a, wav_b8x16_t b) { return wav_b8x16_xor(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t operator^(wav_b16x8_t a, wav_b16x8_t b) { return wav_b16x8_xor(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t operator^(wav_b32x4_t a, wav_b32x4_t b) { return wav_b32x4_xor(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t operator^(wav_b64x2_t a, wav_b64x2_t b) { return wav_b64x2_xor(a, b); }

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t operator^(wav_i8x16_t a, wav_b8x16_t b) { return wav_i8x16_xor_b8x16(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t operator^(wav_i16x8_t a, wav_b16x8_t b) { return wav_i16x8_xor_b16x8(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t operator^(wav_i32x4_t a, wav_b32x4_t b) { return wav_i32x4_xor_b32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t operator^(wav_i64x2_t a, wav_b64x2_t b) { return wav_i64x2_xor_b64x2(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t operator^(wav_u8x16_t a, wav_b8x16_t b) { return wav_u8x16_xor_b8x16(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t operator^(wav_u16x8_t a, wav_b16x8_t b) { return wav_u16x8_xor_b16x8(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t operator^(wav_u32x4_t a, wav_b32x4_t b) { return wav_u32x4_xor_b32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t operator^(wav_u64x2_t a, wav_b64x2_t b) { return wav_u64x2_xor_b64x2(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t operator^(wav_f32x4_t a, wav_b32x4_t b) { return wav_f32x4_xor_b32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t operator^(wav_f64x2_t a, wav_b64x2_t b) { return wav_f64x2_xor_b64x2(a, b); }

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t operator^(wav_b8x16_t a, wav_i8x16_t b) { return wav_b8x16_xor_i8x16(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t operator^(wav_b16x8_t a, wav_i16x8_t b) { return wav_b16x8_xor_i16x8(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t operator^(wav_b32x4_t a, wav_i32x4_t b) { return wav_b32x4_xor_i32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t operator^(wav_b64x2_t a, wav_i64x2_t b) { return wav_b64x2_xor_i64x2(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t operator^(wav_b8x16_t a, wav_u8x16_t b) { return wav_b8x16_xor_u8x16(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t operator^(wav_b16x8_t a, wav_u16x8_t b) { return wav_b16x8_xor_u16x8(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t operator^(wav_b32x4_t a, wav_u32x4_t b) { return wav_b32x4_xor_u32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t operator^(wav_b64x2_t a, wav_u64x2_t b) { return wav_b64x2_xor_u64x2(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t operator^(wav_b32x4_t a, wav_f32x4_t b) { return wav_b32x4_xor_f32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t operator^(wav_b64x2_t a, wav_f64x2_t b) { return wav_b64x2_xor_f64x2(a, b); }
#endif

/* not -- Bitwise NOT */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_not(wav_i8x16_t v) {
  return (wav_i8x16_t) { ~v.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_not(wav_i16x8_t v) {
  return (wav_i16x8_t) { ~v.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_not(wav_i32x4_t v) {
  return (wav_i32x4_t) { ~v.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_not(wav_i64x2_t v) {
  return (wav_i64x2_t) { ~v.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_not(wav_u8x16_t v) {
  return (wav_u8x16_t) { ~v.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_not(wav_u16x8_t v) {
  return (wav_u16x8_t) { ~v.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_not(wav_u32x4_t v) {
  return (wav_u32x4_t) { ~v.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_not(wav_u64x2_t v) {
  return (wav_u64x2_t) { ~v.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_b8x16_not(wav_b8x16_t v) {
  return (wav_b8x16_t) { ~v.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_b16x8_not(wav_b16x8_t v) {
  return (wav_b16x8_t) { ~v.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_b32x4_not(wav_b32x4_t v) {
  return (wav_b32x4_t) { ~v.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_b64x2_not(wav_b64x2_t v) {
  return (wav_b64x2_t) { ~v.values };
}

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_not(wav_i8x16_t v) { return wav_i8x16_not(v); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_not(wav_i16x8_t v) { return wav_i16x8_not(v); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_not(wav_i32x4_t v) { return wav_i32x4_not(v); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_not(wav_i64x2_t v) { return wav_i64x2_not(v); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_not(wav_u8x16_t v) { return wav_u8x16_not(v); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_not(wav_u16x8_t v) { return wav_u16x8_not(v); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_not(wav_u32x4_t v) { return wav_u32x4_not(v); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_not(wav_u64x2_t v) { return wav_u64x2_not(v); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t wav_not(wav_b8x16_t v) { return wav_b8x16_not(v); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t wav_not(wav_b16x8_t v) { return wav_b16x8_not(v); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_not(wav_b32x4_t v) { return wav_b32x4_not(v); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_not(wav_b64x2_t v) { return wav_b64x2_not(v); }

#if WAV_OPERATOR_OVERLOADING
WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t operator~(wav_i8x16_t v) { return wav_i8x16_not(v); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t operator~(wav_i16x8_t v) { return wav_i16x8_not(v); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t operator~(wav_i32x4_t v) { return wav_i32x4_not(v); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t operator~(wav_i64x2_t v) { return wav_i64x2_not(v); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t operator~(wav_u8x16_t v) { return wav_u8x16_not(v); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t operator~(wav_u16x8_t v) { return wav_u16x8_not(v); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t operator~(wav_u32x4_t v) { return wav_u32x4_not(v); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t operator~(wav_u64x2_t v) { return wav_u64x2_not(v); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t operator~(wav_b8x16_t v) { return wav_b8x16_not(v); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t operator~(wav_b16x8_t v) { return wav_b16x8_not(v); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t operator~(wav_b32x4_t v) { return wav_b32x4_not(v); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t operator~(wav_b64x2_t v) { return wav_b64x2_not(v); }
#endif

/* andnot -- Bitwise AND-NOT */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_andnot(wav_i8x16_t a, wav_i8x16_t b) {
  return (wav_i8x16_t) { a.values & ~b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_andnot(wav_i16x8_t a, wav_i16x8_t b) {
  return (wav_i16x8_t) { a.values & ~b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_andnot(wav_i32x4_t a, wav_i32x4_t b) {
  return (wav_i32x4_t) { a.values & ~b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_andnot(wav_i64x2_t a, wav_i64x2_t b) {
  return (wav_i64x2_t) { a.values & ~b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_andnot(wav_u8x16_t a, wav_u8x16_t b) {
  return (wav_u8x16_t) { a.values & ~b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_andnot(wav_u16x8_t a, wav_u16x8_t b) {
  return (wav_u16x8_t) { a.values & ~b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_andnot(wav_u32x4_t a, wav_u32x4_t b) {
  return (wav_u32x4_t) { a.values & ~b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_andnot(wav_u64x2_t a, wav_u64x2_t b) {
  return (wav_u64x2_t) { a.values & ~b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_b8x16_andnot(wav_b8x16_t a, wav_b8x16_t b) {
  return (wav_b8x16_t) { a.values & ~b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_b16x8_andnot(wav_b16x8_t a, wav_b16x8_t b) {
  return (wav_b16x8_t) { a.values & ~b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_b32x4_andnot(wav_b32x4_t a, wav_b32x4_t b) {
  return (wav_b32x4_t) { a.values & ~b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_b64x2_andnot(wav_b64x2_t a, wav_b64x2_t b) {
  return (wav_b64x2_t) { a.values & ~b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_b8x16_andnot_i8x16(wav_b8x16_t a, wav_i8x16_t b) {
  return (wav_i8x16_t) { a.values & ~b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_b16x8_andnot_i16x8(wav_b16x8_t a, wav_i16x8_t b) {
  return (wav_i16x8_t) { a.values & ~b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_b32x4_andnot_i32x4(wav_b32x4_t a, wav_i32x4_t b) {
  return (wav_i32x4_t) { a.values & ~b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_b64x2_andnot_i64x2(wav_b64x2_t a, wav_i64x2_t b) {
  return (wav_i64x2_t) { a.values & ~b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_b8x16_andnot_u8x16(wav_b8x16_t a, wav_u8x16_t b) {
  return (wav_u8x16_t) { a.values & ~b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_b16x8_andnot_u16x8(wav_b16x8_t a, wav_u16x8_t b) {
  return (wav_u16x8_t) { a.values & ~b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_b32x4_andnot_u32x4(wav_b32x4_t a, wav_u32x4_t b) {
  return (wav_u32x4_t) { a.values & ~b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_b64x2_andnot_u64x2(wav_b64x2_t a, wav_u64x2_t b) {
  return (wav_u64x2_t) { a.values & ~b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_andnot_b8x16(wav_i8x16_t a, wav_b8x16_t b) {
  return (wav_i8x16_t) { a.values & ~b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_andnot_b16x8(wav_i16x8_t a, wav_b16x8_t b) {
  return (wav_i16x8_t) { a.values & ~b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_andnot_b32x4(wav_i32x4_t a, wav_b32x4_t b) {
  return (wav_i32x4_t) { a.values & ~b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_andnot_b64x2(wav_i64x2_t a, wav_b64x2_t b) {
  return (wav_i64x2_t) { a.values & ~b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_andnot_b8x16(wav_u8x16_t a, wav_b8x16_t b) {
  return (wav_u8x16_t) { a.values & ~b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_andnot_b16x8(wav_u16x8_t a, wav_b16x8_t b) {
  return (wav_u16x8_t) { a.values & ~b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_andnot_b32x4(wav_u32x4_t a, wav_b32x4_t b) {
  return (wav_u32x4_t) { a.values & ~b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_andnot_b64x2(wav_u64x2_t a, wav_b64x2_t b) {
  return (wav_u64x2_t) { a.values & ~b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_andnot_b32x4(wav_f32x4_t a, wav_b32x4_t b) {
  return wav_u32x4_as_f32x4(wav_u32x4_andnot_b32x4(wav_f32x4_as_u32x4(a), b));
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_andnot_b64x2(wav_f64x2_t a, wav_b64x2_t b) {
  return wav_u64x2_as_f64x2(wav_u64x2_andnot_b64x2(wav_f64x2_as_u64x2(a), b));
}

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_andnot(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_andnot(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_andnot(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_andnot(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_andnot(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_andnot(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_andnot(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_andnot(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_andnot(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_andnot(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_andnot(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_andnot(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_andnot(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_andnot(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_andnot(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_andnot(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t wav_andnot(wav_b8x16_t a, wav_b8x16_t b) { return wav_b8x16_andnot(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t wav_andnot(wav_b16x8_t a, wav_b16x8_t b) { return wav_b16x8_andnot(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_andnot(wav_b32x4_t a, wav_b32x4_t b) { return wav_b32x4_andnot(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_andnot(wav_b64x2_t a, wav_b64x2_t b) { return wav_b64x2_andnot(a, b); }

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_andnot(wav_i8x16_t a, wav_b8x16_t b) { return wav_i8x16_andnot_b8x16(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_andnot(wav_i16x8_t a, wav_b16x8_t b) { return wav_i16x8_andnot_b16x8(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_andnot(wav_i32x4_t a, wav_b32x4_t b) { return wav_i32x4_andnot_b32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_andnot(wav_i64x2_t a, wav_b64x2_t b) { return wav_i64x2_andnot_b64x2(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_andnot(wav_u8x16_t a, wav_b8x16_t b) { return wav_u8x16_andnot_b8x16(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_andnot(wav_u16x8_t a, wav_b16x8_t b) { return wav_u16x8_andnot_b16x8(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_andnot(wav_u32x4_t a, wav_b32x4_t b) { return wav_u32x4_andnot_b32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_andnot(wav_u64x2_t a, wav_b64x2_t b) { return wav_u64x2_andnot_b64x2(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_andnot(wav_f32x4_t a, wav_b32x4_t b) { return wav_f32x4_andnot_b32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_andnot(wav_f64x2_t a, wav_b64x2_t b) { return wav_f64x2_andnot_b64x2(a, b); }

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_andnot(wav_b8x16_t a, wav_i8x16_t b) { return wav_b8x16_andnot_i8x16(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_andnot(wav_b16x8_t a, wav_i16x8_t b) { return wav_b16x8_andnot_i16x8(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_andnot(wav_b32x4_t a, wav_i32x4_t b) { return wav_b32x4_andnot_i32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_andnot(wav_b64x2_t a, wav_i64x2_t b) { return wav_b64x2_andnot_i64x2(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_andnot(wav_b8x16_t a, wav_u8x16_t b) { return wav_b8x16_andnot_u8x16(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_andnot(wav_b16x8_t a, wav_u16x8_t b) { return wav_b16x8_andnot_u16x8(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_andnot(wav_b32x4_t a, wav_u32x4_t b) { return wav_b32x4_andnot_u32x4(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_andnot(wav_b64x2_t a, wav_u64x2_t b) { return wav_b64x2_andnot_u64x2(a, b); }

/* eq
 *
 * Compares operands, returning all ones if the elements are equal, or
 * all zeros if inequal.
 */

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_i8x16_eq(wav_i8x16_t lhs, wav_i8x16_t rhs) {
  wav_b8x16_t r;
  r.values = (__typeof__(r.values)) (lhs.values == rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_i16x8_eq(wav_i16x8_t lhs, wav_i16x8_t rhs) {
  wav_b16x8_t r;
  r.values = (__typeof__(r.values)) (lhs.values == rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_i32x4_eq(wav_i32x4_t lhs, wav_i32x4_t rhs) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (lhs.values == rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_i64x2_eq(wav_i64x2_t lhs, wav_i64x2_t rhs) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (lhs.values == rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_u8x16_eq(wav_u8x16_t lhs, wav_u8x16_t rhs) {
  wav_b8x16_t r;
  r.values = (__typeof__(r.values)) (lhs.values == rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_u16x8_eq(wav_u16x8_t lhs, wav_u16x8_t rhs) {
  wav_b16x8_t r;
  r.values = (__typeof__(r.values)) (lhs.values == rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_u32x4_eq(wav_u32x4_t lhs, wav_u32x4_t rhs) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (lhs.values == rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_u64x2_eq(wav_u64x2_t lhs, wav_u64x2_t rhs) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (lhs.values == rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_b8x16_eq(wav_b8x16_t lhs, wav_b8x16_t rhs) {
  wav_b8x16_t r;
  r.values = (__typeof__(r.values)) (lhs.values == rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_b16x8_eq(wav_b16x8_t lhs, wav_b16x8_t rhs) {
  wav_b16x8_t r;
  r.values = (__typeof__(r.values)) (lhs.values == rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_b32x4_eq(wav_b32x4_t lhs, wav_b32x4_t rhs) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (lhs.values == rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_b64x2_eq(wav_b64x2_t lhs, wav_b64x2_t rhs) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (lhs.values == rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_f32x4_eq(wav_f32x4_t lhs, wav_f32x4_t rhs) {
  wav_b32x4_t r;
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wfloat-equal"
  r.values = (__typeof__(r.values)) (lhs.values == rhs.values);
  #pragma clang diagnostic pop
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_f64x2_eq(wav_f64x2_t lhs, wav_f64x2_t rhs) {
  wav_b64x2_t r;
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wfloat-equal"
  r.values = (__typeof__(r.values)) (lhs.values == rhs.values);
  #pragma clang diagnostic pop
  return r;
}

WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t wav_eq(wav_i8x16_t lhs, wav_i8x16_t rhs) { return wav_i8x16_eq(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t wav_eq(wav_i16x8_t lhs, wav_i16x8_t rhs) { return wav_i16x8_eq(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_eq(wav_i32x4_t lhs, wav_i32x4_t rhs) { return wav_i32x4_eq(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_eq(wav_i64x2_t lhs, wav_i64x2_t rhs) { return wav_i64x2_eq(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t wav_eq(wav_u8x16_t lhs, wav_u8x16_t rhs) { return wav_u8x16_eq(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t wav_eq(wav_u16x8_t lhs, wav_u16x8_t rhs) { return wav_u16x8_eq(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_eq(wav_u32x4_t lhs, wav_u32x4_t rhs) { return wav_u32x4_eq(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_eq(wav_u64x2_t lhs, wav_u64x2_t rhs) { return wav_u64x2_eq(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t wav_eq(wav_b8x16_t lhs, wav_b8x16_t rhs) { return wav_b8x16_eq(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t wav_eq(wav_b16x8_t lhs, wav_b16x8_t rhs) { return wav_b16x8_eq(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_eq(wav_b32x4_t lhs, wav_b32x4_t rhs) { return wav_b32x4_eq(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_eq(wav_b64x2_t lhs, wav_b64x2_t rhs) { return wav_b64x2_eq(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_eq(wav_f32x4_t lhs, wav_f32x4_t rhs) { return wav_f32x4_eq(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_eq(wav_f64x2_t lhs, wav_f64x2_t rhs) { return wav_f64x2_eq(lhs, rhs); }

#if WAV_OPERATOR_OVERLOADING
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t operator==(wav_i8x16_t lhs, wav_i8x16_t rhs) { return wav_i8x16_eq(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t operator==(wav_i16x8_t lhs, wav_i16x8_t rhs) { return wav_i16x8_eq(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t operator==(wav_i32x4_t lhs, wav_i32x4_t rhs) { return wav_i32x4_eq(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t operator==(wav_i64x2_t lhs, wav_i64x2_t rhs) { return wav_i64x2_eq(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t operator==(wav_u8x16_t lhs, wav_u8x16_t rhs) { return wav_u8x16_eq(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t operator==(wav_u16x8_t lhs, wav_u16x8_t rhs) { return wav_u16x8_eq(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t operator==(wav_u32x4_t lhs, wav_u32x4_t rhs) { return wav_u32x4_eq(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t operator==(wav_u64x2_t lhs, wav_u64x2_t rhs) { return wav_u64x2_eq(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t operator==(wav_b8x16_t lhs, wav_b8x16_t rhs) { return wav_b8x16_eq(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t operator==(wav_b16x8_t lhs, wav_b16x8_t rhs) { return wav_b16x8_eq(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t operator==(wav_b32x4_t lhs, wav_b32x4_t rhs) { return wav_b32x4_eq(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t operator==(wav_b64x2_t lhs, wav_b64x2_t rhs) { return wav_b64x2_eq(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t operator==(wav_f32x4_t lhs, wav_f32x4_t rhs) { return wav_f32x4_eq(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t operator==(wav_f64x2_t lhs, wav_f64x2_t rhs) { return wav_f64x2_eq(lhs, rhs); }
#endif

/* ne
 *
 * The ne operations produce the inverse of their eq counterparts.
 */

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_i8x16_ne(wav_i8x16_t lhs, wav_i8x16_t rhs) {
  wav_b8x16_t r;
  r.values = (__typeof__(r.values)) (lhs.values != rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_i16x8_ne(wav_i16x8_t lhs, wav_i16x8_t rhs) {
  wav_b16x8_t r;
  r.values = (__typeof__(r.values)) (lhs.values != rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_i32x4_ne(wav_i32x4_t lhs, wav_i32x4_t rhs) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (lhs.values != rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_i64x2_ne(wav_i64x2_t lhs, wav_i64x2_t rhs) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (lhs.values != rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_u8x16_ne(wav_u8x16_t lhs, wav_u8x16_t rhs) {
  wav_b8x16_t r;
  r.values = (__typeof__(r.values)) (lhs.values != rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_u16x8_ne(wav_u16x8_t lhs, wav_u16x8_t rhs) {
  wav_b16x8_t r;
  r.values = (__typeof__(r.values)) (lhs.values != rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_u32x4_ne(wav_u32x4_t lhs, wav_u32x4_t rhs) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (lhs.values != rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_u64x2_ne(wav_u64x2_t lhs, wav_u64x2_t rhs) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (lhs.values != rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_b8x16_ne(wav_b8x16_t lhs, wav_b8x16_t rhs) {
  wav_b8x16_t r;
  r.values = (__typeof__(r.values)) (lhs.values != rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_b16x8_ne(wav_b16x8_t lhs, wav_b16x8_t rhs) {
  wav_b16x8_t r;
  r.values = (__typeof__(r.values)) (lhs.values != rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_b32x4_ne(wav_b32x4_t lhs, wav_b32x4_t rhs) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (lhs.values != rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_b64x2_ne(wav_b64x2_t lhs, wav_b64x2_t rhs) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (lhs.values != rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_f32x4_ne(wav_f32x4_t lhs, wav_f32x4_t rhs) {
  wav_b32x4_t r;
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wfloat-equal"
  r.values = (__typeof__(r.values)) (lhs.values != rhs.values);
  #pragma clang diagnostic pop
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_f64x2_ne(wav_f64x2_t lhs, wav_f64x2_t rhs) {
  wav_b64x2_t r;
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wfloat-equal"
  r.values = (__typeof__(r.values)) (lhs.values != rhs.values);
  #pragma clang diagnostic pop
  return r;
}

WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t wav_ne(wav_i8x16_t lhs, wav_i8x16_t rhs) { return wav_i8x16_ne(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t wav_ne(wav_i16x8_t lhs, wav_i16x8_t rhs) { return wav_i16x8_ne(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_ne(wav_i32x4_t lhs, wav_i32x4_t rhs) { return wav_i32x4_ne(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_ne(wav_i64x2_t lhs, wav_i64x2_t rhs) { return wav_i64x2_ne(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t wav_ne(wav_u8x16_t lhs, wav_u8x16_t rhs) { return wav_u8x16_ne(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t wav_ne(wav_u16x8_t lhs, wav_u16x8_t rhs) { return wav_u16x8_ne(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_ne(wav_u32x4_t lhs, wav_u32x4_t rhs) { return wav_u32x4_ne(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_ne(wav_u64x2_t lhs, wav_u64x2_t rhs) { return wav_u64x2_ne(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t wav_ne(wav_b8x16_t lhs, wav_b8x16_t rhs) { return wav_b8x16_ne(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t wav_ne(wav_b16x8_t lhs, wav_b16x8_t rhs) { return wav_b16x8_ne(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_ne(wav_b32x4_t lhs, wav_b32x4_t rhs) { return wav_b32x4_ne(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_ne(wav_b64x2_t lhs, wav_b64x2_t rhs) { return wav_b64x2_ne(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_ne(wav_f32x4_t lhs, wav_f32x4_t rhs) { return wav_f32x4_ne(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_ne(wav_f64x2_t lhs, wav_f64x2_t rhs) { return wav_f64x2_ne(lhs, rhs); }

#if WAV_OPERATOR_OVERLOADING
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t operator!=(wav_i8x16_t lhs, wav_i8x16_t rhs) { return wav_i8x16_ne(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t operator!=(wav_i16x8_t lhs, wav_i16x8_t rhs) { return wav_i16x8_ne(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t operator!=(wav_i32x4_t lhs, wav_i32x4_t rhs) { return wav_i32x4_ne(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t operator!=(wav_i64x2_t lhs, wav_i64x2_t rhs) { return wav_i64x2_ne(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t operator!=(wav_u8x16_t lhs, wav_u8x16_t rhs) { return wav_u8x16_ne(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t operator!=(wav_u16x8_t lhs, wav_u16x8_t rhs) { return wav_u16x8_ne(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t operator!=(wav_u32x4_t lhs, wav_u32x4_t rhs) { return wav_u32x4_ne(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t operator!=(wav_u64x2_t lhs, wav_u64x2_t rhs) { return wav_u64x2_ne(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t operator!=(wav_b8x16_t lhs, wav_b8x16_t rhs) { return wav_b8x16_ne(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t operator!=(wav_b16x8_t lhs, wav_b16x8_t rhs) { return wav_b16x8_ne(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t operator!=(wav_b32x4_t lhs, wav_b32x4_t rhs) { return wav_b32x4_ne(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t operator!=(wav_b64x2_t lhs, wav_b64x2_t rhs) { return wav_b64x2_ne(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t operator!=(wav_f32x4_t lhs, wav_f32x4_t rhs) { return wav_f32x4_ne(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t operator!=(wav_f64x2_t lhs, wav_f64x2_t rhs) { return wav_f64x2_ne(lhs, rhs); }
#endif

/* lt */

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_i8x16_lt(wav_i8x16_t lhs, wav_i8x16_t rhs) {
  wav_b8x16_t r;
  r.values = (__typeof__(r.values)) (lhs.values < rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_i16x8_lt(wav_i16x8_t lhs, wav_i16x8_t rhs) {
  wav_b16x8_t r;
  r.values = (__typeof__(r.values)) (lhs.values < rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_i32x4_lt(wav_i32x4_t lhs, wav_i32x4_t rhs) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (lhs.values < rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_i64x2_lt(wav_i64x2_t lhs, wav_i64x2_t rhs) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (lhs.values < rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_u8x16_lt(wav_u8x16_t lhs, wav_u8x16_t rhs) {
  wav_b8x16_t r;
  r.values = (__typeof__(r.values)) (lhs.values < rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_u16x8_lt(wav_u16x8_t lhs, wav_u16x8_t rhs) {
  wav_b16x8_t r;
  r.values = (__typeof__(r.values)) (lhs.values < rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_u32x4_lt(wav_u32x4_t lhs, wav_u32x4_t rhs) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (lhs.values < rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_f32x4_lt(wav_f32x4_t lhs, wav_f32x4_t rhs) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (lhs.values < rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_f64x2_lt(wav_f64x2_t lhs, wav_f64x2_t rhs) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (lhs.values < rhs.values);
  return r;
}

WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t wav_lt(wav_i8x16_t lhs, wav_i8x16_t rhs) { return wav_i8x16_lt(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t wav_lt(wav_i16x8_t lhs, wav_i16x8_t rhs) { return wav_i16x8_lt(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_lt(wav_i32x4_t lhs, wav_i32x4_t rhs) { return wav_i32x4_lt(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_lt(wav_i64x2_t lhs, wav_i64x2_t rhs) { return wav_i64x2_lt(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t wav_lt(wav_u8x16_t lhs, wav_u8x16_t rhs) { return wav_u8x16_lt(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t wav_lt(wav_u16x8_t lhs, wav_u16x8_t rhs) { return wav_u16x8_lt(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_lt(wav_u32x4_t lhs, wav_u32x4_t rhs) { return wav_u32x4_lt(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_lt(wav_f32x4_t lhs, wav_f32x4_t rhs) { return wav_f32x4_lt(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_lt(wav_f64x2_t lhs, wav_f64x2_t rhs) { return wav_f64x2_lt(lhs, rhs); }

#if WAV_OPERATOR_OVERLOADING
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t operator<(wav_i8x16_t lhs, wav_i8x16_t rhs) { return wav_i8x16_lt(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t operator<(wav_i16x8_t lhs, wav_i16x8_t rhs) { return wav_i16x8_lt(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t operator<(wav_i32x4_t lhs, wav_i32x4_t rhs) { return wav_i32x4_lt(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t operator<(wav_i64x2_t lhs, wav_i64x2_t rhs) { return wav_i64x2_lt(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t operator<(wav_u8x16_t lhs, wav_u8x16_t rhs) { return wav_u8x16_lt(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t operator<(wav_u16x8_t lhs, wav_u16x8_t rhs) { return wav_u16x8_lt(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t operator<(wav_u32x4_t lhs, wav_u32x4_t rhs) { return wav_u32x4_lt(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t operator<(wav_f32x4_t lhs, wav_f32x4_t rhs) { return wav_f32x4_lt(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t operator<(wav_f64x2_t lhs, wav_f64x2_t rhs) { return wav_f64x2_lt(lhs, rhs); }
#endif

/* le */

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_i8x16_le(wav_i8x16_t lhs, wav_i8x16_t rhs) {
  wav_b8x16_t r;
  r.values = (__typeof__(r.values)) (lhs.values <= rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_i16x8_le(wav_i16x8_t lhs, wav_i16x8_t rhs) {
  wav_b16x8_t r;
  r.values = (__typeof__(r.values)) (lhs.values <= rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_i32x4_le(wav_i32x4_t lhs, wav_i32x4_t rhs) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (lhs.values <= rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_i64x2_le(wav_i64x2_t lhs, wav_i64x2_t rhs) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (lhs.values <= rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_u8x16_le(wav_u8x16_t lhs, wav_u8x16_t rhs) {
  wav_b8x16_t r;
  r.values = (__typeof__(r.values)) (lhs.values <= rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_u16x8_le(wav_u16x8_t lhs, wav_u16x8_t rhs) {
  wav_b16x8_t r;
  r.values = (__typeof__(r.values)) (lhs.values <= rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_u32x4_le(wav_u32x4_t lhs, wav_u32x4_t rhs) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (lhs.values <= rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_f32x4_le(wav_f32x4_t lhs, wav_f32x4_t rhs) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (lhs.values <= rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_f64x2_le(wav_f64x2_t lhs, wav_f64x2_t rhs) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (lhs.values <= rhs.values);
  return r;
}

WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t wav_le(wav_i8x16_t lhs, wav_i8x16_t rhs) { return wav_i8x16_le(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t wav_le(wav_i16x8_t lhs, wav_i16x8_t rhs) { return wav_i16x8_le(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_le(wav_i32x4_t lhs, wav_i32x4_t rhs) { return wav_i32x4_le(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_le(wav_i64x2_t lhs, wav_i64x2_t rhs) { return wav_i64x2_le(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t wav_le(wav_u8x16_t lhs, wav_u8x16_t rhs) { return wav_u8x16_le(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t wav_le(wav_u16x8_t lhs, wav_u16x8_t rhs) { return wav_u16x8_le(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_le(wav_u32x4_t lhs, wav_u32x4_t rhs) { return wav_u32x4_le(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_le(wav_f32x4_t lhs, wav_f32x4_t rhs) { return wav_f32x4_le(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_le(wav_f64x2_t lhs, wav_f64x2_t rhs) { return wav_f64x2_le(lhs, rhs); }

#if WAV_OPERATOR_OVERLOADING
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t operator<=(wav_i8x16_t lhs, wav_i8x16_t rhs) { return wav_i8x16_le(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t operator<=(wav_i16x8_t lhs, wav_i16x8_t rhs) { return wav_i16x8_le(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t operator<=(wav_i32x4_t lhs, wav_i32x4_t rhs) { return wav_i32x4_le(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t operator<=(wav_i64x2_t lhs, wav_i64x2_t rhs) { return wav_i64x2_le(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t operator<=(wav_u8x16_t lhs, wav_u8x16_t rhs) { return wav_u8x16_le(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t operator<=(wav_u16x8_t lhs, wav_u16x8_t rhs) { return wav_u16x8_le(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t operator<=(wav_u32x4_t lhs, wav_u32x4_t rhs) { return wav_u32x4_le(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t operator<=(wav_f32x4_t lhs, wav_f32x4_t rhs) { return wav_f32x4_le(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t operator<=(wav_f64x2_t lhs, wav_f64x2_t rhs) { return wav_f64x2_le(lhs, rhs); }
#endif

/* gt */

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_i8x16_gt(wav_i8x16_t lhs, wav_i8x16_t rhs) {
  wav_b8x16_t r;
  r.values = (__typeof__(r.values)) (lhs.values > rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_i16x8_gt(wav_i16x8_t lhs, wav_i16x8_t rhs) {
  wav_b16x8_t r;
  r.values = (__typeof__(r.values)) (lhs.values > rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_i32x4_gt(wav_i32x4_t lhs, wav_i32x4_t rhs) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (lhs.values > rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_i64x2_gt(wav_i64x2_t lhs, wav_i64x2_t rhs) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (lhs.values > rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_u8x16_gt(wav_u8x16_t lhs, wav_u8x16_t rhs) {
  wav_b8x16_t r;
  r.values = (__typeof__(r.values)) (lhs.values > rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_u16x8_gt(wav_u16x8_t lhs, wav_u16x8_t rhs) {
  wav_b16x8_t r;
  r.values = (__typeof__(r.values)) (lhs.values > rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_u32x4_gt(wav_u32x4_t lhs, wav_u32x4_t rhs) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (lhs.values > rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_f32x4_gt(wav_f32x4_t lhs, wav_f32x4_t rhs) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (lhs.values > rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_f64x2_gt(wav_f64x2_t lhs, wav_f64x2_t rhs) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (lhs.values > rhs.values);
  return r;
}

WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t wav_gt(wav_i8x16_t lhs, wav_i8x16_t rhs) { return wav_i8x16_gt(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t wav_gt(wav_i16x8_t lhs, wav_i16x8_t rhs) { return wav_i16x8_gt(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_gt(wav_i32x4_t lhs, wav_i32x4_t rhs) { return wav_i32x4_gt(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_gt(wav_i64x2_t lhs, wav_i64x2_t rhs) { return wav_i64x2_gt(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t wav_gt(wav_u8x16_t lhs, wav_u8x16_t rhs) { return wav_u8x16_gt(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t wav_gt(wav_u16x8_t lhs, wav_u16x8_t rhs) { return wav_u16x8_gt(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_gt(wav_u32x4_t lhs, wav_u32x4_t rhs) { return wav_u32x4_gt(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_gt(wav_f32x4_t lhs, wav_f32x4_t rhs) { return wav_f32x4_gt(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_gt(wav_f64x2_t lhs, wav_f64x2_t rhs) { return wav_f64x2_gt(lhs, rhs); }

#if WAV_OPERATOR_OVERLOADING
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t operator>(wav_i8x16_t lhs, wav_i8x16_t rhs) { return wav_i8x16_gt(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t operator>(wav_i16x8_t lhs, wav_i16x8_t rhs) { return wav_i16x8_gt(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t operator>(wav_i32x4_t lhs, wav_i32x4_t rhs) { return wav_i32x4_gt(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t operator>(wav_i64x2_t lhs, wav_i64x2_t rhs) { return wav_i64x2_gt(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t operator>(wav_u8x16_t lhs, wav_u8x16_t rhs) { return wav_u8x16_gt(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t operator>(wav_u16x8_t lhs, wav_u16x8_t rhs) { return wav_u16x8_gt(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t operator>(wav_u32x4_t lhs, wav_u32x4_t rhs) { return wav_u32x4_gt(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t operator>(wav_f32x4_t lhs, wav_f32x4_t rhs) { return wav_f32x4_gt(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t operator>(wav_f64x2_t lhs, wav_f64x2_t rhs) { return wav_f64x2_gt(lhs, rhs); }
#endif

/* ge */

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_i8x16_ge(wav_i8x16_t lhs, wav_i8x16_t rhs) {
  wav_b8x16_t r;
  r.values = (__typeof__(r.values)) (lhs.values >= rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_i16x8_ge(wav_i16x8_t lhs, wav_i16x8_t rhs) {
  wav_b16x8_t r;
  r.values = (__typeof__(r.values)) (lhs.values >= rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_i32x4_ge(wav_i32x4_t lhs, wav_i32x4_t rhs) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (lhs.values >= rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_i64x2_ge(wav_i64x2_t lhs, wav_i64x2_t rhs) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (lhs.values >= rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_u8x16_ge(wav_u8x16_t lhs, wav_u8x16_t rhs) {
  wav_b8x16_t r;
  r.values = (__typeof__(r.values)) (lhs.values >= rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_u16x8_ge(wav_u16x8_t lhs, wav_u16x8_t rhs) {
  wav_b16x8_t r;
  r.values = (__typeof__(r.values)) (lhs.values >= rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_u32x4_ge(wav_u32x4_t lhs, wav_u32x4_t rhs) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (lhs.values >= rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_u64x2_ge(wav_u64x2_t lhs, wav_u64x2_t rhs) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (lhs.values >= rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_f32x4_ge(wav_f32x4_t lhs, wav_f32x4_t rhs) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (lhs.values >= rhs.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_f64x2_ge(wav_f64x2_t lhs, wav_f64x2_t rhs) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (lhs.values >= rhs.values);
  return r;
}

WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t wav_ge(wav_i8x16_t lhs, wav_i8x16_t rhs) { return wav_i8x16_ge(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t wav_ge(wav_i16x8_t lhs, wav_i16x8_t rhs) { return wav_i16x8_ge(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_ge(wav_i32x4_t lhs, wav_i32x4_t rhs) { return wav_i32x4_ge(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_ge(wav_i64x2_t lhs, wav_i64x2_t rhs) { return wav_i64x2_ge(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t wav_ge(wav_u8x16_t lhs, wav_u8x16_t rhs) { return wav_u8x16_ge(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t wav_ge(wav_u16x8_t lhs, wav_u16x8_t rhs) { return wav_u16x8_ge(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_ge(wav_u32x4_t lhs, wav_u32x4_t rhs) { return wav_u32x4_ge(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t wav_ge(wav_f32x4_t lhs, wav_f32x4_t rhs) { return wav_f32x4_ge(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t wav_ge(wav_f64x2_t lhs, wav_f64x2_t rhs) { return wav_f64x2_ge(lhs, rhs); }

#if WAV_OPERATOR_OVERLOADING
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t operator>=(wav_i8x16_t lhs, wav_i8x16_t rhs) { return wav_i8x16_ge(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t operator>=(wav_i16x8_t lhs, wav_i16x8_t rhs) { return wav_i16x8_ge(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t operator>=(wav_i32x4_t lhs, wav_i32x4_t rhs) { return wav_i32x4_ge(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t operator>=(wav_i64x2_t lhs, wav_i64x2_t rhs) { return wav_i64x2_ge(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b8x16_t operator>=(wav_u8x16_t lhs, wav_u8x16_t rhs) { return wav_u8x16_ge(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b16x8_t operator>=(wav_u16x8_t lhs, wav_u16x8_t rhs) { return wav_u16x8_ge(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t operator>=(wav_u32x4_t lhs, wav_u32x4_t rhs) { return wav_u32x4_ge(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b32x4_t operator>=(wav_f32x4_t lhs, wav_f32x4_t rhs) { return wav_f32x4_ge(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_b64x2_t operator>=(wav_f64x2_t lhs, wav_f64x2_t rhs) { return wav_f64x2_ge(lhs, rhs); }
#endif

/* bitselect -- Bitwise select
 *
 * Use the bits in the control mask c to select the corresponding bit
 * from v1 when 1 and v2 when 0.
 */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_bitselect(wav_b8x16_t control, wav_i8x16_t when_set, wav_i8x16_t when_unset) {
  return (wav_i8x16_t) { (when_set.values & control.values) ^ (when_unset.values & ~control.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_bitselect(wav_b16x8_t control, wav_i16x8_t when_set, wav_i16x8_t when_unset) {
  return (wav_i16x8_t) { (when_set.values & control.values) ^ (when_unset.values & ~control.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_bitselect(wav_b32x4_t control, wav_i32x4_t when_set, wav_i32x4_t when_unset) {
  return (wav_i32x4_t) { (when_set.values & control.values) ^ (when_unset.values & ~control.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_bitselect(wav_b64x2_t control, wav_i64x2_t when_set, wav_i64x2_t when_unset) {
  return (wav_i64x2_t) { (when_set.values & control.values) ^ (when_unset.values & ~control.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_bitselect(wav_b8x16_t control, wav_u8x16_t when_set, wav_u8x16_t when_unset) {
  return (wav_u8x16_t) { (when_set.values & control.values) ^ (when_unset.values & ~control.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_bitselect(wav_b16x8_t control, wav_u16x8_t when_set, wav_u16x8_t when_unset) {
  return (wav_u16x8_t) { (when_set.values & control.values) ^ (when_unset.values & ~control.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_bitselect(wav_b32x4_t control, wav_u32x4_t when_set, wav_u32x4_t when_unset) {
  return (wav_u32x4_t) { (when_set.values & control.values) ^ (when_unset.values & ~control.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_bitselect(wav_b64x2_t control, wav_u64x2_t when_set, wav_u64x2_t when_unset) {
  return (wav_u64x2_t) { (when_set.values & control.values) ^ (when_unset.values & ~control.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_bitselect(wav_b32x4_t control, wav_f32x4_t when_set, wav_f32x4_t when_unset) {
  return
    wav_u32x4_as_f32x4(
      wav_u32x4_bitselect(
        control,
        wav_f32x4_as_u32x4(when_set),
        wav_f32x4_as_u32x4(when_unset)
      )
    );
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_bitselect(wav_b64x2_t control, wav_f64x2_t when_set, wav_f64x2_t when_unset) {
  return
    wav_u64x2_as_f64x2(
      wav_u64x2_bitselect(
        control,
        wav_f64x2_as_u64x2(when_set),
        wav_f64x2_as_u64x2(when_unset)
      )
    );
}

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_bitselect(wav_b8x16_t control, wav_i8x16_t when_set, wav_i8x16_t when_unset) { return wav_i8x16_bitselect(control, when_set, when_unset); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_bitselect(wav_b16x8_t control, wav_i16x8_t when_set, wav_i16x8_t when_unset) { return wav_i16x8_bitselect(control, when_set, when_unset); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_bitselect(wav_b32x4_t control, wav_i32x4_t when_set, wav_i32x4_t when_unset) { return wav_i32x4_bitselect(control, when_set, when_unset); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_bitselect(wav_b64x2_t control, wav_i64x2_t when_set, wav_i64x2_t when_unset) { return wav_i64x2_bitselect(control, when_set, when_unset); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_bitselect(wav_b8x16_t control, wav_u8x16_t when_set, wav_u8x16_t when_unset) { return wav_u8x16_bitselect(control, when_set, when_unset); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_bitselect(wav_b16x8_t control, wav_u16x8_t when_set, wav_u16x8_t when_unset) { return wav_u16x8_bitselect(control, when_set, when_unset); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_bitselect(wav_b32x4_t control, wav_u32x4_t when_set, wav_u32x4_t when_unset) { return wav_u32x4_bitselect(control, when_set, when_unset); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_bitselect(wav_b64x2_t control, wav_u64x2_t when_set, wav_u64x2_t when_unset) { return wav_u64x2_bitselect(control, when_set, when_unset); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_bitselect(wav_b32x4_t control, wav_f32x4_t when_set, wav_f32x4_t when_unset) { return wav_f32x4_bitselect(control, when_set, when_unset); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_bitselect(wav_b64x2_t control, wav_f64x2_t when_set, wav_f64x2_t when_unset) { return wav_f64x2_bitselect(control, when_set, when_unset); }

/* blend -- Lanewise select
 *
 * Use the values in c to select the corresponding element from v1 when ~0
 * and v2 when 0.
 *
 * Note that the "blend" operation is not currently part of WASM SIMD;
 * this is an alternative interface for "bitselect", but may one day be
 * implemented with something else.  There is a good discussion at
 * <https://github.com/WebAssembly/simd/issues/192> which should
 * explain why this exists.
 */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_blend(wav_b8x16_t control, wav_i8x16_t when_set, wav_i8x16_t when_unset) {
  return (wav_i8x16_t) { (when_set.values & control.values) ^ (when_unset.values & ~control.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_blend(wav_b16x8_t control, wav_i16x8_t when_set, wav_i16x8_t when_unset) {
  return (wav_i16x8_t) { (when_set.values & control.values) ^ (when_unset.values & ~control.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_blend(wav_b32x4_t control, wav_i32x4_t when_set, wav_i32x4_t when_unset) {
  return (wav_i32x4_t) { (when_set.values & control.values) ^ (when_unset.values & ~control.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_blend(wav_b64x2_t control, wav_i64x2_t when_set, wav_i64x2_t when_unset) {
  return (wav_i64x2_t) { (when_set.values & control.values) ^ (when_unset.values & ~control.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_blend(wav_b8x16_t control, wav_u8x16_t when_set, wav_u8x16_t when_unset) {
  return (wav_u8x16_t) { (when_set.values & control.values) ^ (when_unset.values & ~control.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_blend(wav_b16x8_t control, wav_u16x8_t when_set, wav_u16x8_t when_unset) {
  return (wav_u16x8_t) { (when_set.values & control.values) ^ (when_unset.values & ~control.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_blend(wav_b32x4_t control, wav_u32x4_t when_set, wav_u32x4_t when_unset) {
  return (wav_u32x4_t) { (when_set.values & control.values) ^ (when_unset.values & ~control.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_blend(wav_b64x2_t control, wav_u64x2_t when_set, wav_u64x2_t when_unset) {
  return (wav_u64x2_t) { (when_set.values & control.values) ^ (when_unset.values & ~control.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_blend(wav_b32x4_t control, wav_f32x4_t when_set, wav_f32x4_t when_unset) {
  return
    wav_u32x4_as_f32x4(
      wav_u32x4_blend(
        control,
        wav_f32x4_as_u32x4(when_set),
        wav_f32x4_as_u32x4(when_unset)
      )
    );
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_blend(wav_b64x2_t control, wav_f64x2_t when_set, wav_f64x2_t when_unset) {
  return
    wav_u64x2_as_f64x2(
      wav_u64x2_blend(
        control,
        wav_f64x2_as_u64x2(when_set),
        wav_f64x2_as_u64x2(when_unset)
      )
    );
}

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_blend(wav_b8x16_t control, wav_i8x16_t when_set, wav_i8x16_t when_unset) { return wav_i8x16_blend(control, when_set, when_unset); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_blend(wav_b16x8_t control, wav_i16x8_t when_set, wav_i16x8_t when_unset) { return wav_i16x8_blend(control, when_set, when_unset); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_blend(wav_b32x4_t control, wav_i32x4_t when_set, wav_i32x4_t when_unset) { return wav_i32x4_blend(control, when_set, when_unset); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_blend(wav_b64x2_t control, wav_i64x2_t when_set, wav_i64x2_t when_unset) { return wav_i64x2_blend(control, when_set, when_unset); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_blend(wav_b8x16_t control, wav_u8x16_t when_set, wav_u8x16_t when_unset) { return wav_u8x16_blend(control, when_set, when_unset); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_blend(wav_b16x8_t control, wav_u16x8_t when_set, wav_u16x8_t when_unset) { return wav_u16x8_blend(control, when_set, when_unset); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_blend(wav_b32x4_t control, wav_u32x4_t when_set, wav_u32x4_t when_unset) { return wav_u32x4_blend(control, when_set, when_unset); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_blend(wav_b64x2_t control, wav_u64x2_t when_set, wav_u64x2_t when_unset) { return wav_u64x2_blend(control, when_set, when_unset); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_blend(wav_b32x4_t control, wav_f32x4_t when_set, wav_f32x4_t when_unset) { return wav_f32x4_blend(control, when_set, when_unset); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_blend(wav_b64x2_t control, wav_f64x2_t when_set, wav_f64x2_t when_unset) { return wav_f64x2_blend(control, when_set, when_unset); }

/* min -- Lane-wise integer minimum
 *
 * Compares lane-wise signed/unsigned integers, and returns the minimum
 * of each pair.
 */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_min(wav_i8x16_t a, wav_i8x16_t b)  {
  return wav_i8x16_blend(wav_i8x16_lt(a, b), a, b);
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_min(wav_i16x8_t a, wav_i16x8_t b)  {
  return wav_i16x8_blend(wav_i16x8_lt(a, b), a, b);
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_min(wav_i32x4_t a, wav_i32x4_t b)  {
  return wav_i32x4_blend(wav_i32x4_lt(a, b), a, b);
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_min(wav_u8x16_t a, wav_u8x16_t b)  {
  return wav_u8x16_blend(wav_u8x16_lt(a, b), a, b);
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_min(wav_u16x8_t a, wav_u16x8_t b)  {
  return wav_u16x8_blend(wav_u16x8_lt(a, b), a, b);
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_min(wav_u32x4_t a, wav_u32x4_t b)  {
  return wav_u32x4_blend(wav_u32x4_lt(a, b), a, b);
}

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_min(wav_f32x4_t a, wav_f32x4_t b)  {
  #if WAV_PORTABLE_SLOW(TODO)
    return wav_f32x4_blend(
      wav_b32x4_or(
        wav_f32x4_lt(a, b),
        wav_b32x4_not(wav_f32x4_eq(a, a))
      ),
      a, b
    );
  #else
    return (wav_f32x4_t) { __builtin_wasm_min_f32x4(a.values, b.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_min(wav_f64x2_t a, wav_f64x2_t b)  {
  #if WAV_PORTABLE_SLOW(TODO)
    return wav_f64x2_blend(
      wav_b64x2_or(
        wav_f64x2_lt(a, b),
        wav_b64x2_not(wav_f64x2_eq(a, a))
      ),
      a, b
    );
  #else
    return (wav_f64x2_t) { __builtin_wasm_min_f64x2(a.values, b.values) };
  #endif
}

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_min(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_min(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_min(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_min(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_min(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_min(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_min(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_min(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_min(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_min(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_min(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_min(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_min(wav_f32x4_t a, wav_f32x4_t b) { return wav_f32x4_min(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_min(wav_f64x2_t a, wav_f64x2_t b) { return wav_f64x2_min(a, b); }

/* max -- Lane-wise integer maximum
 *
 * Compares lane-wise signed/unsigned integers, and returns the maximum
 * of each pair.
 */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_max(wav_i8x16_t a, wav_i8x16_t b)  {
  return wav_i8x16_blend(wav_i8x16_gt(a, b), a, b);
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_max(wav_i16x8_t a, wav_i16x8_t b)  {
  return wav_i16x8_blend(wav_i16x8_gt(a, b), a, b);
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_max(wav_i32x4_t a, wav_i32x4_t b)  {
  return wav_i32x4_blend(wav_i32x4_gt(a, b), a, b);
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_max(wav_u8x16_t a, wav_u8x16_t b)  {
  return wav_u8x16_blend(wav_u8x16_gt(a, b), a, b);
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_max(wav_u16x8_t a, wav_u16x8_t b)  {
  return wav_u16x8_blend(wav_u16x8_gt(a, b), a, b);
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_max(wav_u32x4_t a, wav_u32x4_t b)  {
  return wav_u32x4_blend(wav_u32x4_gt(a, b), a, b);
}

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_max(wav_f32x4_t a, wav_f32x4_t b)  {
  #if WAV_PORTABLE_SLOW(TODO)
    return wav_f32x4_blend(
      wav_b32x4_or(
        wav_f32x4_gt(a, b),
        wav_b32x4_not(wav_f32x4_eq(a, a))
      ),
      a, b
    );
  #else
    return (wav_f32x4_t) { __builtin_wasm_max_f32x4(a.values, b.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_max(wav_f64x2_t a, wav_f64x2_t b)  {
  #if WAV_PORTABLE_SLOW(TODO)
    return wav_f64x2_blend(
      wav_b64x2_or(
        wav_f64x2_gt(a, b),
        wav_b64x2_not(wav_f64x2_eq(a, a))
      ),
      a, b
    );
  #else
    return (wav_f64x2_t) { __builtin_wasm_max_f64x2(a.values, b.values) };
  #endif
}

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_max(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_max(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_max(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_max(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_max(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_max(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_max(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_max(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_max(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_max(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_max(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_max(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_max(wav_f32x4_t a, wav_f32x4_t b) { return wav_f32x4_max(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_max(wav_f64x2_t a, wav_f64x2_t b) { return wav_f64x2_max(a, b); }

/* shuffle -- Shuffling using immediate indices
 *
 * Returns a new vector with lanes selected from the lanes of the two
 * input vectors a and b.
 */

#define \
  wav_i8x16_shuffle( \
      a, b, \
      c0, c1,  c2,  c3,  c4,  c5,  c6,  c7, \
      c8, c9, c10, c11, c12, c13, c14, c15) \
    (__extension__ ({ \
      _Pragma("clang diagnostic push") \
      _Pragma("clang diagnostic ignored \"-Wc99-extensions\"") \
      ((wav_i8x16_t) { { __builtin_shufflevector(\
        WAV_REQUIRE_TYPE(wav_i8x16_t, a).values, \
        WAV_REQUIRE_TYPE(wav_i8x16_t, b).values, \
        c0, c1,  c2,  c3,  c4,  c5,  c6,  c7, \
        c8, c9, c10, c11, c12, c13, c14, c15) } } ); \
      _Pragma("clang diagnostic pop") \
    }))

#define \
  wav_i16x8_shuffle( \
      a, b, \
      c0, c1, c2, c3, c4, c5, c6, c7) \
    (__extension__ ({ \
      _Pragma("clang diagnostic push") \
      _Pragma("clang diagnostic ignored \"-Wc99-extensions\"") \
      ((wav_i16x8_t) { { __builtin_shufflevector(\
        WAV_REQUIRE_TYPE(wav_i16x8_t, a).values, \
        WAV_REQUIRE_TYPE(wav_i16x8_t, b).values, \
        c0, c1, c2, c3, c4, c5, c6, c7) } } ); \
      _Pragma("clang diagnostic pop") \
    }))

#define \
  wav_i32x4_shuffle( \
      a, b, \
      c0, c1, c2, c3) \
    (__extension__ ({ \
      _Pragma("clang diagnostic push") \
      _Pragma("clang diagnostic ignored \"-Wc99-extensions\"") \
      ((wav_i32x4_t) { { __builtin_shufflevector(\
        WAV_REQUIRE_TYPE(wav_i32x4_t, a).values, \
        WAV_REQUIRE_TYPE(wav_i32x4_t, b).values, \
        c0, c1, c2, c3) } } ); \
      _Pragma("clang diagnostic pop") \
    }))

#define \
  wav_i64x2_shuffle( \
      a, b, \
      c0, c1) \
    (__extension__ ({ \
      _Pragma("clang diagnostic push") \
      _Pragma("clang diagnostic ignored \"-Wc99-extensions\"") \
      ((wav_i64x2_t) { { __builtin_shufflevector(\
        WAV_REQUIRE_TYPE(wav_i64x2_t, a).values, \
        WAV_REQUIRE_TYPE(wav_i64x2_t, b).values, \
        c0, c1) } } ); \
      _Pragma("clang diagnostic pop") \
    }))

#define \
  wav_u8x16_shuffle( \
      a, b, \
      c0, c1,  c2,  c3,  c4,  c5,  c6,  c7, \
      c8, c9, c10, c11, c12, c13, c14, c15) \
    (__extension__ ({ \
      _Pragma("clang diagnostic push") \
      _Pragma("clang diagnostic ignored \"-Wc99-extensions\"") \
      ((wav_u8x16_t) { { __builtin_shufflevector(\
        WAV_REQUIRE_TYPE(wav_u8x16_t, a).values, \
        WAV_REQUIRE_TYPE(wav_u8x16_t, b).values, \
        c0, c1,  c2,  c3,  c4,  c5,  c6,  c7, \
        c8, c9, c10, c11, c12, c13, c14, c15) } } ); \
      _Pragma("clang diagnostic pop") \
    }))

#define \
  wav_u16x8_shuffle( \
      a, b, \
      c0, c1, c2, c3, c4, c5, c6, c7) \
    (__extension__ ({ \
      _Pragma("clang diagnostic push") \
      _Pragma("clang diagnostic ignored \"-Wc99-extensions\"") \
      ((wav_u16x8_t) { { __builtin_shufflevector(\
        WAV_REQUIRE_TYPE(wav_u16x8_t, a).values, \
        WAV_REQUIRE_TYPE(wav_u16x8_t, b).values, \
        c0, c1, c2, c3, c4, c5, c6, c7) } } ); \
      _Pragma("clang diagnostic pop") \
    }))

#define \
  wav_u32x4_shuffle( \
      a, b, \
      c0, c1, c2, c3) \
    (__extension__ ({ \
      _Pragma("clang diagnostic push") \
      _Pragma("clang diagnostic ignored \"-Wc99-extensions\"") \
      ((wav_u32x4_t) { { __builtin_shufflevector(\
        WAV_REQUIRE_TYPE(wav_u32x4_t, a).values, \
        WAV_REQUIRE_TYPE(wav_u32x4_t, b).values, \
        c0, c1, c2, c3) } } ); \
      _Pragma("clang diagnostic pop") \
    }))

#define \
  wav_u64x2_shuffle( \
      a, b, \
      c0, c1) \
    (__extension__ ({ \
      _Pragma("clang diagnostic push") \
      _Pragma("clang diagnostic ignored \"-Wc99-extensions\"") \
      ((wav_u64x2_t) { { __builtin_shufflevector(\
        WAV_REQUIRE_TYPE(wav_u64x2_t, a).values, \
        WAV_REQUIRE_TYPE(wav_u64x2_t, b).values, \
        c0, c1) } } ); \
      _Pragma("clang diagnostic pop") \
    }))

#define \
  wav_f32x4_shuffle( \
      a, b, \
      c0, c1, c2, c3) \
    (__extension__ ({ \
      _Pragma("clang diagnostic push") \
      _Pragma("clang diagnostic ignored \"-Wc99-extensions\"") \
      ((wav_f32x4_t) { { __builtin_shufflevector(\
        WAV_REQUIRE_TYPE(wav_f32x4_t, a).values, \
        WAV_REQUIRE_TYPE(wav_f32x4_t, b).values, \
        c0, c1, c2, c3) } } ); \
      _Pragma("clang diagnostic pop") \
    }))

#define \
  wav_f64x2_shuffle( \
      a, b, \
      c0, c1) \
    (__extension__ ({ \
      _Pragma("clang diagnostic push") \
      _Pragma("clang diagnostic ignored \"-Wc99-extensions\"") \
      ((wav_f64x2_t) { { __builtin_shufflevector(\
        WAV_REQUIRE_TYPE(wav_f64x2_t, a).values, \
        WAV_REQUIRE_TYPE(wav_f64x2_t, b).values, \
        c0, c1) } } ); \
      _Pragma("clang diagnostic pop") \
    }))

#define \
  wav_shuffle(a, b, ...) \
    ( \
      _Pragma("clang diagnostic push") \
      _Pragma("clang diagnostic ignored \"-Wc99-extensions\"") \
      (__typeof__(a)) { __builtin_shufflevector(\
        WAV_REQUIRE_TYPE(__typeof__(b), a).values, \
        WAV_REQUIRE_TYPE(__typeof__(a), b).values, \
        __VA_ARGS__) } \
      _Pragma("clang diagnostic pop") \
    )

/* swizzle -- Swizzling using variable indices
 *
 * Returns a new vector with lanes selected from the lanes of the first
 * input vector a specified in the second input vector s.
 */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_swizzle(wav_i8x16_t data, wav_i8x16_t idx) {
  #if WAV_PORTABLE_SLOW(TODO)
    wav_i8x16_t r;

    for (int i = 0 ; i < 16 ; i++) {
      r.values[i] =
        ((idx.values[i] & 15) == idx.values[i]) ?
          data.values[idx.values[i]] : 0;
    }

    #pragma clang diagnostic push /* This is a false positive */
    #pragma clang diagnostic ignored "-Wconditional-uninitialized"
    return r;
    #pragma clang diagnostic pop
  #else
    return (wav_i8x16_t) { __builtin_wasm_swizzle_i8x16(data.values, idx.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_swizzle(wav_u8x16_t a, wav_u8x16_t b) {
  return
    wav_i8x16_as_u8x16(
      wav_i8x16_swizzle(
        wav_u8x16_as_i8x16(a),
        wav_u8x16_as_i8x16(b)
      )
    );
}

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_swizzle(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_swizzle(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_swizzle(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_swizzle(a, b); }

/* add — Addition. */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_add(wav_i8x16_t a, wav_i8x16_t b) {
  return (wav_i8x16_t) { a.values + b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_add(wav_i16x8_t a, wav_i16x8_t b) {
  return (wav_i16x8_t) { a.values + b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_add(wav_i32x4_t a, wav_i32x4_t b) {
  return (wav_i32x4_t) { a.values + b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_add(wav_i64x2_t a, wav_i64x2_t b) {
  return (wav_i64x2_t) { a.values + b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_add(wav_u8x16_t a, wav_u8x16_t b) {
  return (wav_u8x16_t) { a.values + b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_add(wav_u16x8_t a, wav_u16x8_t b) {
  return (wav_u16x8_t) { a.values + b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_add(wav_u32x4_t a, wav_u32x4_t b) {
  return (wav_u32x4_t) { a.values + b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_add(wav_u64x2_t a, wav_u64x2_t b) {
  return (wav_u64x2_t) { a.values + b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_add(wav_f32x4_t a, wav_f32x4_t b) {
  return (wav_f32x4_t) { a.values + b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_add(wav_f64x2_t a, wav_f64x2_t b) {
  return (wav_f64x2_t) { a.values + b.values };
}

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_add(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_add(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_add(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_add(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_add(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_add(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_add(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_add(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_add(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_add(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_add(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_add(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_add(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_add(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_add(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_add(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_add(wav_f32x4_t a, wav_f32x4_t b) { return wav_f32x4_add(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_add(wav_f64x2_t a, wav_f64x2_t b) { return wav_f64x2_add(a, b); }

#if defined(__cplusplus)
  WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t operator+(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_add(a, b); }
  WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t operator+(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_add(a, b); }
  WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t operator+(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_add(a, b); }
  WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t operator+(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_add(a, b); }
  WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t operator+(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_add(a, b); }
  WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t operator+(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_add(a, b); }
  WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t operator+(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_add(a, b); }
  WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t operator+(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_add(a, b); }
  WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t operator+(wav_f32x4_t a, wav_f32x4_t b) { return wav_f32x4_add(a, b); }
  WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t operator+(wav_f64x2_t a, wav_f64x2_t b) { return wav_f64x2_add(a, b); }
#endif

/* sub — Subtraction. */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_sub(wav_i8x16_t lhs, wav_i8x16_t rhs) {
  return (wav_i8x16_t) { lhs.values - rhs.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_sub(wav_i16x8_t lhs, wav_i16x8_t rhs) {
  return (wav_i16x8_t) { lhs.values - rhs.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_sub(wav_i32x4_t lhs, wav_i32x4_t rhs) {
  return (wav_i32x4_t) { lhs.values - rhs.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_sub(wav_i64x2_t lhs, wav_i64x2_t rhs) {
  return (wav_i64x2_t) { lhs.values - rhs.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_sub(wav_u8x16_t lhs, wav_u8x16_t rhs) {
  return (wav_u8x16_t) { lhs.values - rhs.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_sub(wav_u16x8_t lhs, wav_u16x8_t rhs) {
  return (wav_u16x8_t) { lhs.values - rhs.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_sub(wav_u32x4_t lhs, wav_u32x4_t rhs) {
  return (wav_u32x4_t) { lhs.values - rhs.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_sub(wav_u64x2_t lhs, wav_u64x2_t rhs) {
  return (wav_u64x2_t) { lhs.values - rhs.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_sub(wav_f32x4_t lhs, wav_f32x4_t rhs) {
  return (wav_f32x4_t) { lhs.values - rhs.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_sub(wav_f64x2_t lhs, wav_f64x2_t rhs) {
  return (wav_f64x2_t) { lhs.values - rhs.values };
}

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_sub(wav_i8x16_t lhs, wav_i8x16_t rhs) { return wav_i8x16_sub(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_sub(wav_i16x8_t lhs, wav_i16x8_t rhs) { return wav_i16x8_sub(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_sub(wav_i32x4_t lhs, wav_i32x4_t rhs) { return wav_i32x4_sub(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_sub(wav_i64x2_t lhs, wav_i64x2_t rhs) { return wav_i64x2_sub(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_sub(wav_u8x16_t lhs, wav_u8x16_t rhs) { return wav_u8x16_sub(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_sub(wav_u16x8_t lhs, wav_u16x8_t rhs) { return wav_u16x8_sub(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_sub(wav_u32x4_t lhs, wav_u32x4_t rhs) { return wav_u32x4_sub(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_sub(wav_u64x2_t lhs, wav_u64x2_t rhs) { return wav_u64x2_sub(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_sub(wav_f32x4_t lhs, wav_f32x4_t rhs) { return wav_f32x4_sub(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_sub(wav_f64x2_t lhs, wav_f64x2_t rhs) { return wav_f64x2_sub(lhs, rhs); }

#if WAV_OPERATOR_OVERLOADING
  WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t operator-(wav_i8x16_t lhs, wav_i8x16_t rhs) { return wav_i8x16_sub(lhs, rhs); }
  WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t operator-(wav_i16x8_t lhs, wav_i16x8_t rhs) { return wav_i16x8_sub(lhs, rhs); }
  WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t operator-(wav_i32x4_t lhs, wav_i32x4_t rhs) { return wav_i32x4_sub(lhs, rhs); }
  WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t operator-(wav_i64x2_t lhs, wav_i64x2_t rhs) { return wav_i64x2_sub(lhs, rhs); }
  WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t operator-(wav_u8x16_t lhs, wav_u8x16_t rhs) { return wav_u8x16_sub(lhs, rhs); }
  WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t operator-(wav_u16x8_t lhs, wav_u16x8_t rhs) { return wav_u16x8_sub(lhs, rhs); }
  WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t operator-(wav_u32x4_t lhs, wav_u32x4_t rhs) { return wav_u32x4_sub(lhs, rhs); }
  WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t operator-(wav_u64x2_t lhs, wav_u64x2_t rhs) { return wav_u64x2_sub(lhs, rhs); }
  WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t operator-(wav_f32x4_t lhs, wav_f32x4_t rhs) { return wav_f32x4_sub(lhs, rhs); }
  WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t operator-(wav_f64x2_t lhs, wav_f64x2_t rhs) { return wav_f64x2_sub(lhs, rhs); }
#endif

/* mul — Multiplication. */

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_mul(wav_i16x8_t a, wav_i16x8_t b) {
  return (wav_i16x8_t) { a.values * b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_mul(wav_i32x4_t a, wav_i32x4_t b) {
  return (wav_i32x4_t) { a.values * b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_mul(wav_i64x2_t a, wav_i64x2_t b) {
  return (wav_i64x2_t) { a.values * b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_mul(wav_u16x8_t a, wav_u16x8_t b) {
  return (wav_u16x8_t) { a.values * b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_mul(wav_u32x4_t a, wav_u32x4_t b) {
  return (wav_u32x4_t) { a.values * b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_mul(wav_u64x2_t a, wav_u64x2_t b) {
  return (wav_u64x2_t) { a.values * b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_mul(wav_f32x4_t a, wav_f32x4_t b) {
  return (wav_f32x4_t) { a.values * b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_mul(wav_f64x2_t a, wav_f64x2_t b) {
  return (wav_f64x2_t) { a.values * b.values };
}

WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_mul(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_mul(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_mul(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_mul(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_mul(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_mul(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_mul(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_mul(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_mul(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_mul(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_mul(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_mul(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_mul(wav_f32x4_t a, wav_f32x4_t b) { return wav_f32x4_mul(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_mul(wav_f64x2_t a, wav_f64x2_t b) { return wav_f64x2_mul(a, b); }

#if WAV_OPERATOR_OVERLOADING
  WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t operator*(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_mul(a, b); }
  WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t operator*(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_mul(a, b); }
  WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t operator*(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_mul(a, b); }
  WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t operator*(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_mul(a, b); }
  WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t operator*(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_mul(a, b); }
  WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t operator*(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_mul(a, b); }
  WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t operator*(wav_f32x4_t a, wav_f32x4_t b) { return wav_f32x4_mul(a, b); }
  WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t operator*(wav_f64x2_t a, wav_f64x2_t b) { return wav_f64x2_mul(a, b); }
#endif

/* div — Division. */

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_div(wav_f32x4_t lhs, wav_f32x4_t rhs) {
  return (wav_f32x4_t) { lhs.values / rhs.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_div(wav_f64x2_t lhs, wav_f64x2_t rhs) {
  return (wav_f64x2_t) { lhs.values / rhs.values };
}

WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_div(wav_f32x4_t lhs, wav_f32x4_t rhs) { return wav_f32x4_div(lhs, rhs); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_div(wav_f64x2_t lhs, wav_f64x2_t rhs) { return wav_f64x2_div(lhs, rhs); }

#if WAV_OPERATOR_OVERLOADING
  WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t operator/(wav_f32x4_t lhs, wav_f32x4_t rhs) { return wav_f32x4_div(lhs, rhs); }
  WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t operator/(wav_f64x2_t lhs, wav_f64x2_t rhs) { return wav_f64x2_div(lhs, rhs); }
#endif

/* neg — Negation. */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_neg(wav_i8x16_t vec) {
  return (wav_i8x16_t) { -vec.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_neg(wav_i16x8_t vec) {
  return (wav_i16x8_t) { -vec.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_neg(wav_i32x4_t vec) {
  return (wav_i32x4_t) { -vec.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_neg(wav_i64x2_t vec) {
  return (wav_i64x2_t) { -vec.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_neg(wav_f32x4_t vec) {
  return (wav_f32x4_t) { -vec.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_neg(wav_f64x2_t vec) {
  return (wav_f64x2_t) { -vec.values };
}

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_neg(wav_i8x16_t vec) { return wav_i8x16_neg(vec); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_neg(wav_i16x8_t vec) { return wav_i16x8_neg(vec); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_neg(wav_i32x4_t vec) { return wav_i32x4_neg(vec); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_neg(wav_i64x2_t vec) { return wav_i64x2_neg(vec); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_neg(wav_f32x4_t vec) { return wav_f32x4_neg(vec); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_neg(wav_f64x2_t vec) { return wav_f64x2_neg(vec); }

#if WAV_OPERATOR_OVERLOADING
WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t operator-(wav_i8x16_t vec) { return wav_i8x16_neg(vec); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t operator-(wav_i16x8_t vec) { return wav_i16x8_neg(vec); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t operator-(wav_i32x4_t vec) { return wav_i32x4_neg(vec); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t operator-(wav_i64x2_t vec) { return wav_i64x2_neg(vec); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t operator-(wav_f32x4_t vec) { return wav_f32x4_neg(vec); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t operator-(wav_f64x2_t vec) { return wav_f64x2_neg(vec); }
#endif

/* sqrt — Square root.
 *
 * Lane-wise IEEE squareRoot.
 */

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_sqrt(wav_f32x4_t vec) {
  #if WAV_PORTABLE_SLOW(50247)
    wav_f32x4_t r;

    for (int i = 0 ; i < 4 ; i++) {
      r.values[i] = __builtin_sqrtf(vec.values[i]);
    }

    #pragma clang diagnostic push /* This is a false positive */
    #pragma clang diagnostic ignored "-Wconditional-uninitialized"
    return r;
    #pragma clang diagnostic pop
  #else
    return (wav_f32x4_t) { __builtin_wasm_sqrt_f32x4(vec.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_sqrt(wav_f64x2_t vec) {
  #if WAV_PORTABLE_SLOW(50247)
    wav_f64x2_t r;

    for (int i = 0 ; i < 2 ; i++) {
      r.values[i] = __builtin_sqrt(vec.values[i]);
    }

    #pragma clang diagnostic push /* This is a false positive */
    #pragma clang diagnostic ignored "-Wconditional-uninitialized"
    return r;
    #pragma clang diagnostic pop
  #else
    return (wav_f64x2_t) { __builtin_wasm_sqrt_f64x2(vec.values) };
  #endif
}

WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_sqrt(wav_f32x4_t vec) { return wav_f32x4_sqrt(vec); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_sqrt(wav_f64x2_t vec) { return wav_f64x2_sqrt(vec); }

/* ceil — Round to integer above (ceiling)
 *
 * Lane-wise rounding to the nearest integral value not smaller than
 * the input.
 */

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_ceil(wav_f32x4_t vec) {
  #if WAV_PORTABLE_SLOW(50247)
    wav_f32x4_t r;

    for (int i = 0 ; i < 4 ; i++) {
      r.values[i] = __builtin_ceilf(vec.values[i]);
    }

    #pragma clang diagnostic push /* This is a false positive */
    #pragma clang diagnostic ignored "-Wconditional-uninitialized"
    return r;
    #pragma clang diagnostic pop
  #else
    return (wav_f32x4_t) { __builtin_wasm_ceil_f32x4(vec.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_ceil(wav_f64x2_t vec) {
  #if WAV_PORTABLE_SLOW(50247)
    wav_f64x2_t r;

    for (int i = 0 ; i < 2 ; i++) {
      r.values[i] = __builtin_ceil(vec.values[i]);
    }

    #pragma clang diagnostic push /* This is a false positive */
    #pragma clang diagnostic ignored "-Wconditional-uninitialized"
    return r;
    #pragma clang diagnostic pop
  #else
    return (wav_f64x2_t) { __builtin_wasm_ceil_f64x2(vec.values) };
  #endif
}

WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_ceil(wav_f32x4_t vec) { return wav_f32x4_ceil(vec); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_ceil(wav_f64x2_t vec) { return wav_f64x2_ceil(vec); }

/* floor — Round to integer below (floor)
 *
 * Lane-wise rounding to the nearest integral value not greater than
 * the input.
 */

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_floor(wav_f32x4_t vec) {
  #if WAV_PORTABLE_SLOW(50247)
    wav_f32x4_t r;

    for (int i = 0 ; i < 4 ; i++) {
      r.values[i] = __builtin_floorf(vec.values[i]);
    }

    #pragma clang diagnostic push /* This is a false positive */
    #pragma clang diagnostic ignored "-Wconditional-uninitialized"
    return r;
    #pragma clang diagnostic pop
  #else
    return (wav_f32x4_t) { __builtin_wasm_floor_f32x4(vec.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_floor(wav_f64x2_t vec) {
  #if WAV_PORTABLE_SLOW(50247)
    wav_f64x2_t r;

    for (int i = 0 ; i < 2 ; i++) {
      r.values[i] = __builtin_floor(vec.values[i]);
    }

    #pragma clang diagnostic push /* This is a false positive */
    #pragma clang diagnostic ignored "-Wconditional-uninitialized"
    return r;
    #pragma clang diagnostic pop
  #else
    return (wav_f64x2_t) { __builtin_wasm_floor_f64x2(vec.values) };
  #endif
}

WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_floor(wav_f32x4_t vec) { return wav_f32x4_floor(vec); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_floor(wav_f64x2_t vec) { return wav_f64x2_floor(vec); }

/* trunc — Round to integer toward zero (truncate to integer)
 *
 * Lane-wise rounding to the nearest integral value with the magnitude
 * not larger than the input.
 */

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_trunc(wav_f32x4_t vec) {
  #if WAV_PORTABLE_SLOW(50247)
    wav_f32x4_t r;

    for (int i = 0 ; i < 4 ; i++) {
      r.values[i] = __builtin_truncf(vec.values[i]);
    }

    #pragma clang diagnostic push /* This is a false positive */
    #pragma clang diagnostic ignored "-Wconditional-uninitialized"
    return r;
    #pragma clang diagnostic pop
  #else
    return (wav_f32x4_t) { __builtin_wasm_trunc_f32x4(vec.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_trunc(wav_f64x2_t vec) {
  #if WAV_PORTABLE_SLOW(50247)
    wav_f64x2_t r;

    for (int i = 0 ; i < 2 ; i++) {
      r.values[i] = __builtin_trunc(vec.values[i]);
    }

    #pragma clang diagnostic push /* This is a false positive */
    #pragma clang diagnostic ignored "-Wconditional-uninitialized"
    return r;
    #pragma clang diagnostic pop
  #else
    return (wav_f64x2_t) { __builtin_wasm_trunc_f64x2(vec.values) };
  #endif
}

WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_trunc(wav_f32x4_t vec) { return wav_f32x4_trunc(vec); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_trunc(wav_f64x2_t vec) { return wav_f64x2_trunc(vec); }

/* nearest — Round to nearest integer, ties to even
 *
 * Lane-wise rounding to the nearest integral value; if two values are
 * equally near, rounds to the even one.
 */

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_nearest(wav_f32x4_t vec) {
  #if WAV_PORTABLE_SLOW(50247)
    wav_f32x4_t r;

    for (int i = 0 ; i < 4 ; i++) {
      r.values[i] = __builtin_roundf(vec.values[i]);
    }

    #pragma clang diagnostic push /* This is a false positive */
    #pragma clang diagnostic ignored "-Wconditional-uninitialized"
    return r;
    #pragma clang diagnostic pop
  #else
    return (wav_f32x4_t) { __builtin_wasm_nearest_f32x4(vec.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_nearest(wav_f64x2_t vec) {
  #if WAV_PORTABLE_SLOW(50247)
    wav_f64x2_t r;

    for (int i = 0 ; i < 2 ; i++) {
      r.values[i] = __builtin_round(vec.values[i]);
    }

    #pragma clang diagnostic push /* This is a false positive */
    #pragma clang diagnostic ignored "-Wconditional-uninitialized"
    return r;
    #pragma clang diagnostic pop
  #else
    return (wav_f64x2_t) { __builtin_wasm_nearest_f64x2(vec.values) };
  #endif
}

WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_nearest(wav_f32x4_t vec) { return wav_f32x4_nearest(vec); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_nearest(wav_f64x2_t vec) { return wav_f64x2_nearest(vec); }

/* extend_low -- Integer to integer extension
 *
 * Converts low half of the smaller lane vector to a larger lane
 * vector, sign extended or zero (unsigned) extended.
 */

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i8x16_extend_low(wav_i8x16_t vec) {
  wav_i16x8_t r;

  r.values =
    __builtin_convertvector(
      __builtin_shufflevector(vec.values, vec.values, 0, 1, 2, 3, 4, 5, 6, 7),
      __typeof__(r.values)
    );

  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i16x8_extend_low(wav_i16x8_t vec) {
  wav_i32x4_t r;

  r.values =
    __builtin_convertvector(
      __builtin_shufflevector(vec.values, vec.values, 0, 1, 2, 3),
      __typeof__(r.values)
    );

  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i32x4_extend_low(wav_i32x4_t vec) {
  wav_i64x2_t r;

  r.values =
    __builtin_convertvector(
      __builtin_shufflevector(vec.values, vec.values, 0, 1),
      __typeof__(r.values)
    );

  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u8x16_extend_low(wav_u8x16_t vec) {
  wav_u16x8_t r;

  r.values =
    __builtin_convertvector(
      __builtin_shufflevector(vec.values, vec.values, 0, 1, 2, 3, 4, 5, 6, 7),
      __typeof__(r.values)
    );

  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u16x8_extend_low(wav_u16x8_t vec) {
  wav_u32x4_t r;

  r.values =
    __builtin_convertvector(
      __builtin_shufflevector(vec.values, vec.values, 0, 1, 2, 3),
      __typeof__(r.values)
    );

  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u32x4_extend_low(wav_u32x4_t vec) {
  wav_u64x2_t r;

  r.values =
    __builtin_convertvector(
      __builtin_shufflevector(vec.values, vec.values, 0, 1),
      __typeof__(r.values)
    );

  return r;
}

WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_extend_low(wav_i8x16_t vec) { return wav_i8x16_extend_low(vec); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_extend_low(wav_i16x8_t vec) { return wav_i16x8_extend_low(vec); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_extend_low(wav_i32x4_t vec) { return wav_i32x4_extend_low(vec); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_extend_low(wav_u8x16_t vec) { return wav_u8x16_extend_low(vec); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_extend_low(wav_u16x8_t vec) { return wav_u16x8_extend_low(vec); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_extend_low(wav_u32x4_t vec) { return wav_u32x4_extend_low(vec); }

/* promote_low -- Single-precision floating point to double-precision
 *
 * Conversion of the two lower single-precision floating point lanes to
 * the two double-precision lanes of the result.
 */

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f32x4_promote_low(wav_f32x4_t vec) {
  #if WAV_PORTABLE_SLOW(50232)
    wav_f64x2_t r;
    r.values = __builtin_convertvector(__builtin_shufflevector(vec.values, vec.values, 0, 1), __typeof__(r.values));
    return r;
  #else
    return (wav_f64x2_t) { __builtin_wasm_promote_low_f32x4_f64x2(vec.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_promote_low(wav_f32x4_t vec) { return wav_f32x4_promote_low(vec); }

/* extend_high -- Integer to integer extension
 *
 * Converts high half of the smaller lane vector to a larger lane
 * vector, sign extended or zero (unsigned) extended.
 */

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i8x16_extend_high(wav_i8x16_t vec) {
  wav_i16x8_t r;

  r.values =
    __builtin_convertvector(
      __builtin_shufflevector(vec.values, vec.values, 8, 9, 10, 11, 12, 13, 14, 15),
      __typeof__(r.values)
    );

  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i16x8_extend_high(wav_i16x8_t vec) {
  wav_i32x4_t r;

  r.values =
    __builtin_convertvector(
      __builtin_shufflevector(vec.values, vec.values, 4, 5, 6, 7),
      __typeof__(r.values)
    );

  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i32x4_extend_high(wav_i32x4_t vec) {
  wav_i64x2_t r;

  r.values =
    __builtin_convertvector(
      __builtin_shufflevector(vec.values, vec.values, 2, 3),
      __typeof__(r.values)
    );

  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u8x16_extend_high(wav_u8x16_t vec) {
  wav_u16x8_t r;

  r.values =
    __builtin_convertvector(
      __builtin_shufflevector(vec.values, vec.values, 8, 9, 10, 11, 12, 13, 14, 15),
      __typeof__(r.values)
    );

  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u16x8_extend_high(wav_u16x8_t vec) {
  wav_u32x4_t r;

  r.values =
    __builtin_convertvector(
      __builtin_shufflevector(vec.values, vec.values, 4, 5, 6, 7),
      __typeof__(r.values)
    );

  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u32x4_extend_high(wav_u32x4_t vec) {
  wav_u64x2_t r;

  r.values =
    __builtin_convertvector(
      __builtin_shufflevector(vec.values, vec.values, 2, 3),
      __typeof__(r.values)
    );

  return r;
}

WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_extend_high(wav_i8x16_t vec) { return wav_i8x16_extend_high(vec); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_extend_high(wav_i16x8_t vec) { return wav_i16x8_extend_high(vec); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_extend_high(wav_i32x4_t vec) { return wav_i32x4_extend_high(vec); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_extend_high(wav_u8x16_t vec) { return wav_u8x16_extend_high(vec); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_extend_high(wav_u16x8_t vec) { return wav_u16x8_extend_high(vec); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_extend_high(wav_u32x4_t vec) { return wav_u32x4_extend_high(vec); }

/* narrow -- Narrowing / demotion
 *
 * Converts two input vectors into a smaller lane vector by narrowing
 * each lane, signed or unsigned. The signed narrowing operation will
 * use signed saturation to handle overflow, 0x7f or 0x80 for i8x16,
 * the unsigned narrowing operation will use unsigned saturation to
 * handle overflow, 0x00 or 0xff for i8x16.
 */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i16x8_narrow(wav_i16x8_t a, wav_i16x8_t b) {
  #if WAV_PORTABLE_SLOW(TODO)
    wav_i8x16_t r;
    a = wav_i16x8_max(wav_i16x8_min(a, wav_i16x8_splat(INT8_MAX)), wav_i16x8_splat(INT8_MIN));
    b = wav_i16x8_max(wav_i16x8_min(b, wav_i16x8_splat(INT8_MAX)), wav_i16x8_splat(INT8_MIN));
    r.values = __builtin_convertvector(__builtin_shufflevector(a.values, b.values, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15), __typeof__(r.values));
    return r;
  #else
    return (wav_i8x16_t) { __builtin_wasm_narrow_s_i8x16_i16x8(a.values, b.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i32x4_narrow(wav_i32x4_t a, wav_i32x4_t b) {
  #if WAV_PORTABLE_SLOW(TODO)
    wav_i16x8_t r;
    a = wav_i32x4_max(wav_i32x4_min(a, wav_i32x4_splat(INT16_MAX)), wav_i32x4_splat(INT16_MIN));
    b = wav_i32x4_max(wav_i32x4_min(b, wav_i32x4_splat(INT16_MAX)), wav_i32x4_splat(INT16_MIN));
    r.values = __builtin_convertvector(__builtin_shufflevector(a.values, b.values, 0, 1, 2, 3, 4, 5, 6, 7), __typeof__(r.values));
    return r;
  #else
    return (wav_i16x8_t) { __builtin_wasm_narrow_s_i16x8_i32x4(a.values, b.values) };
  #endif
}

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_narrow(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_narrow(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_narrow(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_narrow(a, b); }

/* narrow_u */

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_i16x8_narrow_u(wav_i16x8_t a, wav_i16x8_t b) {
  #if WAV_PORTABLE_SLOW(TODO)
    wav_u8x16_t r;
    a = wav_i16x8_max(wav_i16x8_min(a, wav_i16x8_splat(UINT8_MAX)), wav_i16x8_splat(0));
    b = wav_i16x8_max(wav_i16x8_min(b, wav_i16x8_splat(UINT8_MAX)), wav_i16x8_splat(0));
    r.values = __builtin_convertvector(__builtin_shufflevector(a.values, b.values, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15), __typeof__(r.values));
    return r;
  #else
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wvector-conversion"
    return (wav_u8x16_t) { __builtin_wasm_narrow_u_i8x16_i16x8(a.values, b.values) };
    #pragma clang diagnostic pop
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_i32x4_narrow_u(wav_i32x4_t a, wav_i32x4_t b) {
  #if WAV_PORTABLE_SLOW(TODO)
    wav_u16x8_t r;
    a = wav_i32x4_max(wav_i32x4_min(a, wav_i32x4_splat(UINT16_MAX)), wav_i32x4_splat(0));
    b = wav_i32x4_max(wav_i32x4_min(b, wav_i32x4_splat(UINT16_MAX)), wav_i32x4_splat(0));
    r.values = __builtin_convertvector(__builtin_shufflevector(a.values, b.values, 0, 1, 2, 3, 4, 5, 6, 7), __typeof__(r.values));
    return r;
  #else
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wvector-conversion"
    return (wav_u16x8_t) { __builtin_wasm_narrow_u_i16x8_i32x4(a.values, b.values) };
    #pragma clang diagnostic pop
  #endif
}

WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_narrow_u(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_narrow_u(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_narrow_u(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_narrow_u(a, b); }

/* demote -- Double-precision floating point to single-precision
 *
 * Conversion of the two double-precision floating point lanes to two
 * lower single-precision lanes of the result. The two higher lanes of
 * the result are initialized to zero. If the conversion result is not
 * representable as a single-precision floating point number, it is
 * rounded to the nearest-even representable number.
 */

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f64x2_demote(wav_f64x2_t a) {
  #if WAV_PORTABLE_SLOW(TODO)
    return (wav_f32x4_t) { (float) a.values[0], (float) a.values[1], 0.0f, 0.0f };
  #elif WAV_PORTABLE_SLOW(TODO)
    wav_f32x4_t r;
    __typeof__(a.values) z = { 0.0, 0.0 };
    r.values = __builtin_convertvector(__builtin_shufflevector(a.values, z, 0, 1, 2, 3), __typeof__(r.values));
    return r;
  #elif WAV_PORTABLE_SLOW(TODO)
    wav_f32x4_t r;
    float f32x2_a __attribute__((__vector_size__(8))) = __builtin_convertvector(a.values, __typeof__(f32x2_a));
    __typeof__(f32x2_a) f32x2_z = { 0.0f, 0.0f };
    r.values = __builtin_shufflevector(f32x2_a, f32x2_z, 0, 1, 2, 3);
    return r;
  #else
    return (wav_f32x4_t) { __builtin_wasm_demote_zero_f64x2_f32x4(a.values) };
  #endif
}

WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_demote(wav_f64x2_t a) { return wav_f64x2_demote(a); }

/* convert */

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_i32x4_convert_f32x4(wav_i32x4_t a) {
  wav_f32x4_t r;
  r.values = __builtin_convertvector(a.values, __typeof__(r.values));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_u32x4_convert_f32x4(wav_u32x4_t a) {
  wav_f32x4_t r;
  r.values = __builtin_convertvector(a.values, __typeof__(r.values));
  return r;
}

WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_convert_f32x4(wav_i32x4_t a) { return wav_i32x4_convert_f32x4(a); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_convert_f32x4(wav_u32x4_t a) { return wav_u32x4_convert_f32x4(a); }

/* convert_low */

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_i32x4_convert_low_f64x2(wav_i32x4_t a) {
  wav_f64x2_t r;
  r.values = __builtin_convertvector(__builtin_shufflevector(a.values, a.values, 0, 1), __typeof__(r.values));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_u32x4_convert_low_f64x2(wav_u32x4_t a) {
  wav_f64x2_t r;
  r.values = __builtin_convertvector(__builtin_shufflevector(a.values, a.values, 0, 1), __typeof__(r.values));
  return r;
}

WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_convert_low_f64x2(wav_i32x4_t a) { return wav_i32x4_convert_low_f64x2(a); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_convert_low_f64x2(wav_u32x4_t a) { return wav_u32x4_convert_low_f64x2(a); }

/* trunc_sat */

/* The implementations here are a bit different.  In WASM SIMD128, all
 * we need is the __builtin_convertvector and LLVM will generate an
 * i32x4.trunc_sat_f32x4_s for us.  However, in standard C, "if the
 * value being converted is outside the range of values that can be
 * represented, the behavior is undefined."  The WASM SIMD128 function
 * does define behavior for these cases (NaN -> 0, outside INT32_MIN ...
 * INT32_MAX saturates). */

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_f32x4_trunc_sat_i32x4(wav_f32x4_t a) {
  wav_i32x4_t r;

  #if !defined(__wasm_simd128__)
    a =
      wav_f32x4_and_b32x4(
        wav_f32x4_min(
          wav_f32x4_max(
            a,
            wav_f32x4_splat((float) INT32_MIN)
          ),
          wav_f32x4_splat((float) INT32_MAX)
        ),
        wav_f32x4_eq(a, a)
      );
  #endif

  r.values = __builtin_convertvector(a.values, __typeof__(r.values));

  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_f32x4_trunc_sat_u32x4(wav_f32x4_t a) {
  wav_u32x4_t r;

  #if !defined(__wasm_simd128__)
    a =
      wav_f32x4_and_b32x4(
        wav_f32x4_min(
          wav_f32x4_max(
            a,
            wav_f32x4_splat(0.0f)
          ),
          wav_f32x4_splat((float) UINT32_MAX)
        ),
        wav_f32x4_eq(a, a)
      );
  #endif

  r.values = __builtin_convertvector(a.values, __typeof__(r.values));

  return r;
}

WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_trunc_sat_i32x4(wav_f32x4_t a) { return wav_f32x4_trunc_sat_i32x4(a); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_trunc_sat_u32x4(wav_f32x4_t a) { return wav_f32x4_trunc_sat_u32x4(a); }

/* trunc_sat_zero */

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_f64x2_trunc_sat_zero_i32x4(wav_f64x2_t a) {
  #if WAV_PORTABLE_SLOW(TODO)
    wav_i32x4_t r;

    #if !defined(__wasm_simd128__)
      a =
        wav_f64x2_and_b64x2(
          wav_f64x2_min(
            wav_f64x2_max(
              a,
              wav_f64x2_splat((double) INT32_MIN)
            ),
            wav_f64x2_splat((double) INT32_MAX)
          ),
          wav_f64x2_eq(a, a)
        );
    #endif

    int32_t __attribute__((__vector_size__(8))) z = { 0, 0 };
    r.values =
      __builtin_shufflevector(
        __builtin_convertvector(
          a.values,
          __typeof__(z)
        ),
        z,
        0, 1, 2, 3
      );

    return r;
  #else
    return (wav_i32x4_t) { __builtin_wasm_trunc_sat_zero_s_f64x2_i32x4(a.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_f64x2_trunc_sat_zero_u32x4(wav_f64x2_t a) {
  #if WAV_PORTABLE_SLOW(TODO)
    wav_u32x4_t r;

    #if !defined(__wasm_simd128__)
      a =
        wav_f64x2_and_b64x2(
          wav_f64x2_min(
            wav_f64x2_max(
              a,
              wav_f64x2_splat(0.0)
            ),
            wav_f64x2_splat((double) UINT32_MAX)
          ),
          wav_f64x2_eq(a, a)
        );
    #endif

    uint32_t __attribute__((__vector_size__(8))) z = { 0, 0 };
    r.values =
      __builtin_shufflevector(
        __builtin_convertvector(
          a.values,
          __typeof__(z)
        ),
        z,
        0, 1, 2, 3
      );

    return r;
  #else
    return (wav_u32x4_t) { __builtin_wasm_trunc_sat_zero_u_f64x2_i32x4(a.values) };
  #endif
}

WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_trunc_sat_zero_i32x4(wav_f64x2_t a) { return wav_f64x2_trunc_sat_zero_i32x4(a); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_trunc_sat_zero_u32x4(wav_f64x2_t a) { return wav_f64x2_trunc_sat_zero_u32x4(a); }

/* extmul_low -- Extended wultiplication.
 *
 * Lane-wise integer extended multiplication producing twice wider
 * result than the inputs.
 */

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i8x16_extmul_low(wav_i8x16_t a, wav_i8x16_t b) {
  #if WAV_PORTABLE_SLOW(TODO)
    return wav_i16x8_mul(wav_i8x16_extend_low(a), wav_i8x16_extend_low(b));
  #else
    return (wav_i16x8_t) { __builtin_wasm_extmul_low_i8x16_s_i16x8(a.values, b.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i16x8_extmul_low(wav_i16x8_t a, wav_i16x8_t b) {
  #if WAV_PORTABLE_SLOW(TODO)
    return wav_i32x4_mul(wav_i16x8_extend_low(a), wav_i16x8_extend_low(b));
  #else
    return (wav_i32x4_t) { __builtin_wasm_extmul_low_i16x8_s_i32x4(a.values, b.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i32x4_extmul_low(wav_i32x4_t a, wav_i32x4_t b) {
  #if WAV_PORTABLE_SLOW(TODO)
    return wav_i64x2_mul(wav_i32x4_extend_low(a), wav_i32x4_extend_low(b));
  #else
    return (wav_i64x2_t) { __builtin_wasm_extmul_low_i32x4_s_i64x2(a.values, b.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u8x16_extmul_low(wav_u8x16_t a, wav_u8x16_t b) {
  #if WAV_PORTABLE_SLOW(TODO)
    return (wav_u16x8_t) { wav_u8x16_extend_low(a).values * wav_u8x16_extend_low(b).values };
  #else
    return (wav_u16x8_t) { __builtin_wasm_extmul_low_i8x16_u_i16x8(a.values, b.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u16x8_extmul_low(wav_u16x8_t a, wav_u16x8_t b) {
  #if WAV_PORTABLE_SLOW(TODO)
    return (wav_u32x4_t) { wav_u16x8_extend_low(a).values * wav_u16x8_extend_low(b).values };
  #else
    return (wav_u32x4_t) { __builtin_wasm_extmul_low_i16x8_u_i32x4(a.values, b.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u32x4_extmul_low(wav_u32x4_t a, wav_u32x4_t b) {
  #if WAV_PORTABLE_SLOW(TODO)
    return (wav_u64x2_t) { wav_u32x4_extend_low(a).values * wav_u32x4_extend_low(b).values };
  #else
    return (wav_u64x2_t) { __builtin_wasm_extmul_low_i32x4_u_i64x2(a.values, b.values) };
  #endif
}

WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_extmul_low(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_extmul_low(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_extmul_low(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_extmul_low(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_extmul_low(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_extmul_low(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_extmul_low(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_extmul_low(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_extmul_low(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_extmul_low(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_extmul_low(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_extmul_low(a, b); }

/* extmul_high -- Extended wultiplication.
 *
 * Lane-wise integer extended multiplication producing twice wider
 * result than the inputs.
 */

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i8x16_extmul_high(wav_i8x16_t a, wav_i8x16_t b) {
  #if WAV_PORTABLE_SLOW(TODO)
    return wav_i16x8_mul(wav_i8x16_extend_high(a), wav_i8x16_extend_high(b));
  #else
    return (wav_i16x8_t) { __builtin_wasm_extmul_high_i8x16_s_i16x8(a.values, b.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i16x8_extmul_high(wav_i16x8_t a, wav_i16x8_t b) {
  #if WAV_PORTABLE_SLOW(TODO)
    return wav_i32x4_mul(wav_i16x8_extend_high(a), wav_i16x8_extend_high(b));
  #else
    return (wav_i32x4_t) { __builtin_wasm_extmul_high_i16x8_s_i32x4(a.values, b.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i32x4_extmul_high(wav_i32x4_t a, wav_i32x4_t b) {
  #if WAV_PORTABLE_SLOW(TODO)
    return wav_i64x2_mul(wav_i32x4_extend_high(a), wav_i32x4_extend_high(b));
  #else
    return (wav_i64x2_t) { __builtin_wasm_extmul_high_i32x4_s_i64x2(a.values, b.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u8x16_extmul_high(wav_u8x16_t a, wav_u8x16_t b) {
  #if WAV_PORTABLE_SLOW(TODO)
    return (wav_u16x8_t) { wav_u8x16_extend_high(a).values * wav_u8x16_extend_high(b).values };
  #else
    return (wav_u16x8_t) { __builtin_wasm_extmul_high_i8x16_u_i16x8(a.values, b.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u16x8_extmul_high(wav_u16x8_t a, wav_u16x8_t b) {
  #if WAV_PORTABLE_SLOW(TODO)
    return (wav_u32x4_t) { wav_u16x8_extend_high(a).values * wav_u16x8_extend_high(b).values };
  #else
    return (wav_u32x4_t) { __builtin_wasm_extmul_high_i16x8_u_i32x4(a.values, b.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u32x4_extmul_high(wav_u32x4_t a, wav_u32x4_t b) {
  #if WAV_PORTABLE_SLOW(TODO)
    return (wav_u64x2_t) { wav_u32x4_extend_high(a).values * wav_u32x4_extend_high(b).values };
  #else
    return (wav_u64x2_t) { __builtin_wasm_extmul_high_i32x4_u_i64x2(a.values, b.values) };
  #endif
}

WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_extmul_high(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_extmul_high(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_extmul_high(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_extmul_high(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_extmul_high(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_extmul_high(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_extmul_high(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_extmul_high(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_extmul_high(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_extmul_high(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_extmul_high(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_extmul_high(a, b); }

/* extadd_pairwise
 *
 * Lane-wise integer extended pairwise addition producing extended
 * results (twice wider results than the inputs).
 */

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i8x16_extadd_pairwise(wav_i8x16_t v) {
  #if WAV_PORTABLE_SLOW(TODO)
    wav_i16x8_t r;
    r.values =
      __builtin_convertvector(__builtin_shufflevector(v.values, v.values, 0, 2, 4, 6, 8, 10, 12, 14), __typeof__(r.values)) +
      __builtin_convertvector(__builtin_shufflevector(v.values, v.values, 1, 3, 5, 7, 9, 11, 13, 15), __typeof__(r.values));
    return r;
  #else
    return (wav_i16x8_t) { __builtin_wasm_extadd_pairwise_i8x16_s_i16x8(v.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i16x8_extadd_pairwise(wav_i16x8_t v) {
  #if WAV_PORTABLE_SLOW(TODO)
    wav_i32x4_t r;
    r.values =
      __builtin_convertvector(__builtin_shufflevector(v.values, v.values, 0, 2, 4, 6), __typeof__(r.values)) +
      __builtin_convertvector(__builtin_shufflevector(v.values, v.values, 1, 3, 5, 7), __typeof__(r.values));
    return r;
  #else
    return (wav_i32x4_t) { __builtin_wasm_extadd_pairwise_i16x8_s_i32x4(v.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u8x16_extadd_pairwise(wav_u8x16_t v) {
  #if WAV_PORTABLE_SLOW(TODO)
    wav_u16x8_t r;
    r.values =
      __builtin_convertvector(__builtin_shufflevector(v.values, v.values, 0, 2, 4, 6, 8, 10, 12, 14), __typeof__(r.values)) +
      __builtin_convertvector(__builtin_shufflevector(v.values, v.values, 1, 3, 5, 7, 9, 11, 13, 15), __typeof__(r.values));
    return r;
  #else
    return (wav_u16x8_t) { __builtin_wasm_extadd_pairwise_i8x16_u_i16x8(v.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u16x8_extadd_pairwise(wav_u16x8_t v) {
  #if WAV_PORTABLE_SLOW(TODO)
    wav_u32x4_t r;
    r.values =
      __builtin_convertvector(__builtin_shufflevector(v.values, v.values, 0, 2, 4, 6), __typeof__(r.values)) +
      __builtin_convertvector(__builtin_shufflevector(v.values, v.values, 1, 3, 5, 7), __typeof__(r.values));
    return r;
  #else
    return (wav_u32x4_t) { __builtin_wasm_extadd_pairwise_i16x8_u_i32x4(v.values) };
  #endif
}

WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_extadd_pairwise(wav_i8x16_t v) { return wav_i8x16_extadd_pairwise(v); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_extadd_pairwise(wav_i16x8_t v) { return wav_i16x8_extadd_pairwise(v); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_extadd_pairwise(wav_u8x16_t v) { return wav_u8x16_extadd_pairwise(v); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_extadd_pairwise(wav_u16x8_t v) { return wav_u16x8_extadd_pairwise(v); }

/* dot — dot product
 *
 * Lane-wise multiply signed 16-bit integers in the two input vectors
 * and add adjacent pairs of the full 32-bit results.
 */

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i16x8_dot(wav_i16x8_t a, wav_i16x8_t b) {
  #if WAV_PORTABLE_SLOW(TODO)
    int32_t p __attribute__((__vector_size__(32))) =
      __builtin_convertvector(a.values, __typeof__(p)) *
      __builtin_convertvector(b.values, __typeof__(p));
    return (wav_i32x4_t) {
      __builtin_shufflevector(p, p, 0, 2, 4, 6) +
      __builtin_shufflevector(p, p, 1, 3, 5, 7)
    };
  #else
    return (wav_i32x4_t) { __builtin_wasm_dot_s_i32x4_i16x8(a.values, b.values) };
  #endif
}

WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_dot(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_dot(a, b); }

/* add_sat -- Saturating integer addition
 *
 * Lane-wise saturating addition.
 */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_add_sat(wav_i8x16_t a, wav_i8x16_t b) {
  #if WAV_PORTABLE_SLOW(TODO)
    wav_u8x16_t au = wav_i8x16_as_u8x16(a);
    wav_u8x16_t bu = wav_i8x16_as_u8x16(b);
    wav_u8x16_t ru = wav_u8x16_add(au, bu);

    au.values = (au.values >> 7) + INT8_MAX;

    return wav_u8x16_as_i8x16(
      wav_u8x16_blend(
        wav_i8x16_ge(
          wav_u8x16_as_i8x16(
            wav_u8x16_or(
              wav_u8x16_xor(au, bu),
              wav_u8x16_not(wav_u8x16_xor(bu, ru))
            )
          ),
          wav_i8x16_splat(0)
        ),
        au,
        ru
      )
    );
  #else
    return (wav_i8x16_t) { __builtin_wasm_add_sat_s_i8x16(a.values, b.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_add_sat(wav_i16x8_t a, wav_i16x8_t b) {
  #if WAV_PORTABLE_SLOW(TODO)
    wav_u16x8_t au = wav_i16x8_as_u16x8(a);
    wav_u16x8_t bu = wav_i16x8_as_u16x8(b);
    wav_u16x8_t ru = wav_u16x8_add(au, bu);

    au.values = (au.values >> 15) + INT16_MAX;

    return wav_u16x8_as_i16x8(
      wav_u16x8_blend(
        wav_i16x8_ge(
          wav_u16x8_as_i16x8(
            wav_u16x8_or(
              wav_u16x8_xor(au, bu),
              wav_u16x8_not(wav_u16x8_xor(bu, ru))
            )
          ),
          wav_i16x8_splat(0)
        ),
        au,
        ru
      )
    );
  #else
    return (wav_i16x8_t) { __builtin_wasm_add_sat_s_i16x8(a.values, b.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_add_sat(wav_u8x16_t a, wav_u8x16_t b) {
  #if WAV_PORTABLE_SLOW(TODO)
    wav_u8x16_t r = wav_u8x16_add(a, b);
    r.values |= r.values < a.values;
    return r;
  #else
    return (wav_u8x16_t) { __builtin_wasm_add_sat_u_i8x16(a.values, b.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_add_sat(wav_u16x8_t a, wav_u16x8_t b) {
  #if WAV_PORTABLE_SLOW(TODO)
    wav_u16x8_t r = wav_u16x8_add(a, b);
    r.values |= r.values < a.values;
    return r;
  #else
    return (wav_u16x8_t) { __builtin_wasm_add_sat_u_i16x8(a.values, b.values) };
  #endif
}

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_add_sat(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_add_sat(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_add_sat(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_add_sat(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_add_sat(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_add_sat(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_add_sat(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_add_sat(a, b); }

/* sub_sat -- Saturating integer subtraction
 *
 * Lane-wise saturating subtraction.
 */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_sub_sat(wav_i8x16_t a, wav_i8x16_t b) {
  #if WAV_PORTABLE_SLOW(TODO)
    wav_u8x16_t au = wav_i8x16_as_u8x16(a);
    wav_u8x16_t bu = wav_i8x16_as_u8x16(b);
    wav_u8x16_t ru = wav_u8x16_sub(au, bu);

    au.values = (au.values >> 7) + INT8_MAX;

    return wav_u8x16_as_i8x16(
      wav_u8x16_blend(
        wav_i8x16_lt(
          wav_u8x16_as_i8x16(
            wav_u8x16_and(
              wav_u8x16_xor(au, bu),
              wav_u8x16_xor(au, ru)
            )
          ),
          wav_i8x16_splat(0)
        ),
        au,
        ru
      )
    );
  #else
    return (wav_i8x16_t) { __builtin_wasm_sub_sat_s_i8x16(a.values, b.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_sub_sat(wav_i16x8_t a, wav_i16x8_t b) {
  #if WAV_PORTABLE_SLOW(TODO)
    wav_u16x8_t au = wav_i16x8_as_u16x8(a);
    wav_u16x8_t bu = wav_i16x8_as_u16x8(b);
    wav_u16x8_t ru = wav_u16x8_sub(au, bu);

    au.values = (au.values >> 15) + INT16_MAX;

    return wav_u16x8_as_i16x8(
      wav_u16x8_blend(
        wav_i16x8_lt(
          wav_u16x8_as_i16x8(
            wav_u16x8_and(
              wav_u16x8_xor(au, bu),
              wav_u16x8_xor(au, ru)
            )
          ),
          wav_i16x8_splat(0)
        ),
        au,
        ru
      )
    );
  #else
    return (wav_i16x8_t) { __builtin_wasm_sub_sat_s_i16x8(a.values, b.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_sub_sat(wav_u8x16_t a, wav_u8x16_t b) {
  #if WAV_PORTABLE_SLOW(TODO)
    wav_u8x16_t r = wav_u8x16_sub(a, b);
    r.values &= r.values <= a.values;
    return r;
  #else
    return (wav_u8x16_t) { __builtin_wasm_sub_sat_u_i8x16(a.values, b.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_sub_sat(wav_u16x8_t a, wav_u16x8_t b) {
  #if WAV_PORTABLE_SLOW(TODO)
    wav_u16x8_t r = wav_u16x8_sub(a, b);
    r.values &= r.values <= a.values;
    return r;
  #else
    return (wav_u16x8_t) { __builtin_wasm_sub_sat_u_i16x8(a.values, b.values) };
  #endif
}

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_sub_sat(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_sub_sat(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_sub_sat(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_sub_sat(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_sub_sat(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_sub_sat(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_sub_sat(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_sub_sat(a, b); }

/* q15mulr_sat -- Saturating integer Q-format rounding multiplication
 *
 * Lane-wise saturating rounding multiplication in Q15 format
 */

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_q15mulr_sat(wav_i16x8_t a, wav_i16x8_t b) {
  #if WAV_PORTABLE_SLOW(TODO)
    wav_i16x8_t r;
    int32_t __attribute__((__vector_size__(32))) tmp;

    tmp =
      __builtin_convertvector(a.values, __typeof__(tmp)) *
      __builtin_convertvector(b.values, __typeof__(tmp));
    tmp += INT32_C(0x4000);
    tmp >>= 15;

    const __typeof__(tmp) min = {
      INT16_MIN, INT16_MIN, INT16_MIN, INT16_MIN,
      INT16_MIN, INT16_MIN, INT16_MIN, INT16_MIN };
    const __typeof__(tmp) max = {
      INT16_MAX, INT16_MAX, INT16_MAX, INT16_MAX,
      INT16_MAX, INT16_MAX, INT16_MAX, INT16_MAX };

    __typeof__(tmp) mask = (__typeof__(tmp)) (tmp < min);
    tmp = (tmp & ~mask) | (min & mask);
    mask = (__typeof__(tmp)) (tmp > max);
    tmp = (tmp & ~mask) | (max & mask);

    r.values = __builtin_convertvector(tmp, __typeof__(r.values));

    return r;
  #else
    return (wav_i16x8_t) { __builtin_wasm_q15mulr_sat_s_i16x8(a.values, b.values) };
  #endif
}

WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_q15mulr_sat(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_q15mulr_sat(a, b); }

/* pmin -- Pseudo-minimum
 *
 * Lane-wise minimum value, defined as b < a ? b : a.
 */

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_pmin(wav_f32x4_t a, wav_f32x4_t b)  {
  #if WAV_PORTABLE_SLOW(TODO)
    return wav_f32x4_blend(wav_f32x4_lt(b, a), b, a);
  #else
    return (wav_f32x4_t) { __builtin_wasm_pmin_f32x4(a.values, b.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_pmin(wav_f64x2_t a, wav_f64x2_t b)  {
  #if WAV_PORTABLE_SLOW(TODO)
    return wav_f64x2_blend(wav_f64x2_lt(b, a), b, a);
  #else
    return (wav_f64x2_t) { __builtin_wasm_pmin_f64x2(a.values, b.values) };
  #endif
}

WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_pmin(wav_f32x4_t a, wav_f32x4_t b) { return wav_f32x4_pmin(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_pmin(wav_f64x2_t a, wav_f64x2_t b) { return wav_f64x2_pmin(a, b); }

/* pmax -- Pseudo-maximum
 *
 * Lane-wise maximum value, defined as a < b ? b : a.
 */

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_pmax(wav_f32x4_t a, wav_f32x4_t b)  {
  #if WAV_PORTABLE_SLOW(TODO)
    return wav_f32x4_blend(wav_f32x4_lt(a, b), b, a);
  #else
    return (wav_f32x4_t) { __builtin_wasm_pmax_f32x4(a.values, b.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_pmax(wav_f64x2_t a, wav_f64x2_t b)  {
  #if WAV_PORTABLE_SLOW(TODO)
    return wav_f64x2_blend(wav_f64x2_lt(a, b), b, a);
  #else
    return (wav_f64x2_t) { __builtin_wasm_pmax_f64x2(a.values, b.values) };
  #endif
}

WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_pmax(wav_f32x4_t a, wav_f32x4_t b) { return wav_f32x4_pmax(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_pmax(wav_f64x2_t a, wav_f64x2_t b) { return wav_f64x2_pmax(a, b); }

/* avgr -- Lane-wise integer rounding average
 *
 * Lane-wise rounding average.
 */

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_avgr(wav_u8x16_t a, wav_u8x16_t b)  {
  #if WAV_PORTABLE_SLOW(TODO)
    wav_u8x16_t r;
    r.values = (a.values + b.values + 1) >> 1;
    return r;
  #else
    return (wav_u8x16_t) { __builtin_wasm_avgr_u_i8x16(a.values, b.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_avgr(wav_u16x8_t a, wav_u16x8_t b)  {
  #if WAV_PORTABLE_SLOW(TODO)
    wav_u16x8_t r;
    r.values = (a.values + b.values + 1) >> 1;
    return r;
  #else
    return (wav_u16x8_t) { __builtin_wasm_avgr_u_i16x8(a.values, b.values) };
  #endif
}

WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_avgr(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_avgr(a, b); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_avgr(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_avgr(a, b); }

/* abs -- Lane-wise integer absolute value
 *
 * Lane-wise wrapping absolute value.
 */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_abs(wav_i8x16_t a) {
  wav_i8x16_t m;
  m.values = (__typeof__(m.values)) (a.values < ((__typeof__(m.values)) { 0, }));
  return (wav_i8x16_t) { (-a.values & m.values) | (a.values & ~m.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_abs(wav_i16x8_t a) {
  wav_i16x8_t m;
  m.values = (__typeof__(m.values)) (a.values < ((__typeof__(m.values)) { 0, }));
  return (wav_i16x8_t) { (-a.values & m.values) | (a.values & ~m.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_abs(wav_i32x4_t a) {
  wav_i32x4_t m;
  m.values = (__typeof__(m.values)) (a.values < ((__typeof__(m.values)) { 0, }));
  return (wav_i32x4_t) { (-a.values & m.values) | (a.values & ~m.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_abs(wav_i64x2_t a) {
  #if WAV_PORTABLE_SLOW(TODO)
    wav_i64x2_t m;
    m.values = (__typeof__(m.values)) (a.values < ((__typeof__(m.values)) { 0, }));
    return (wav_i64x2_t) { (-a.values & m.values) | (a.values & ~m.values) };
  #else
    return (wav_i64x2_t) { __builtin_wasm_abs_i64x2(a.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_abs(wav_f32x4_t a) {
  #if WAV_PORTABLE_SLOW(TODO)
    wav_i32x4_t m;
    m.values = (__typeof__(m.values)) (a.values < ((__typeof__(a.values)) { 0.0f, 0.0f, 0.0f, 0.0f }));
    return (wav_f32x4_t) { (((__typeof__(m.values)) (-a.values)) & m.values) | (((__typeof__(m.values)) (a.values)) & ~m.values) };
  #else
    return (wav_f32x4_t) { __builtin_wasm_abs_f32x4(a.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_abs(wav_f64x2_t a) {
  #if WAV_PORTABLE_SLOW(TODO)
    wav_i64x2_t m;
    m.values = (__typeof__(m.values)) (a.values < ((__typeof__(a.values)) { 0.0, 0.0 }));
    return (wav_f64x2_t) { (((__typeof__(m.values)) (-a.values)) & m.values) | (((__typeof__(m.values)) (a.values)) & ~m.values) };
  #else
    return (wav_f64x2_t) { __builtin_wasm_abs_f64x2(a.values) };
  #endif
}

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_abs(wav_i8x16_t a) { return wav_i8x16_abs(a); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_abs(wav_i16x8_t a) { return wav_i16x8_abs(a); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_abs(wav_i32x4_t a) { return wav_i32x4_abs(a); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_abs(wav_i64x2_t a) { return wav_i64x2_abs(a); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_abs(wav_f32x4_t a) { return wav_f32x4_abs(a); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_abs(wav_f64x2_t a) { return wav_f64x2_abs(a); }

/* shl -- Left shift by scalar
 *
 * Shift the bits in each lane to the left by the same amount. The
 * shift count is taken modulo lane width.
 */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_shl(wav_i8x16_t a, int count) {
  return (wav_i8x16_t) { a.values << count };
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_shl(wav_i16x8_t a, int count) {
  return (wav_i16x8_t) { a.values << count };
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_shl(wav_i32x4_t a, int count) {
  return (wav_i32x4_t) { a.values << count };
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_shl(wav_i64x2_t a, int count) {
  return (wav_i64x2_t) { a.values << ((int64_t) count) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_shl(wav_u8x16_t a, int count) {
  return (wav_u8x16_t) { a.values << count };
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_shl(wav_u16x8_t a, int count) {
  return (wav_u16x8_t) { a.values << count };
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_shl(wav_u32x4_t a, int count) {
  return (wav_u32x4_t) { a.values << count };
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_shl(wav_u64x2_t a, int count) {
  return (wav_u64x2_t) { a.values << ((int64_t) count) };
}

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_shl(wav_i8x16_t a, int count) { return wav_i8x16_shl(a, count); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_shl(wav_i16x8_t a, int count) { return wav_i16x8_shl(a, count); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_shl(wav_i32x4_t a, int count) { return wav_i32x4_shl(a, count); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_shl(wav_i64x2_t a, int count) { return wav_i64x2_shl(a, count); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_shl(wav_u8x16_t a, int count) { return wav_u8x16_shl(a, count); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_shl(wav_u16x8_t a, int count) { return wav_u16x8_shl(a, count); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_shl(wav_u32x4_t a, int count) { return wav_u32x4_shl(a, count); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_shl(wav_u64x2_t a, int count) { return wav_u64x2_shl(a, count); }

#if WAV_OPERATOR_OVERLOADING
WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t operator<<(wav_i8x16_t a, int count) { return wav_i8x16_shl(a, count); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t operator<<(wav_i16x8_t a, int count) { return wav_i16x8_shl(a, count); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t operator<<(wav_i32x4_t a, int count) { return wav_i32x4_shl(a, count); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t operator<<(wav_i64x2_t a, int count) { return wav_i64x2_shl(a, count); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t operator<<(wav_u8x16_t a, int count) { return wav_u8x16_shl(a, count); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t operator<<(wav_u16x8_t a, int count) { return wav_u16x8_shl(a, count); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t operator<<(wav_u32x4_t a, int count) { return wav_u32x4_shl(a, count); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t operator<<(wav_u64x2_t a, int count) { return wav_u64x2_shl(a, count); }
#endif

/* shr -- Right shift by scalar
 *
 * Shift the bits in each lane to the right by the same amount. The
 * shift count is taken modulo lane width. This is an arithmetic right
 * shift for the signed variants and a logical right shift for the
 * unsigned variants.
 */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_shr(wav_i8x16_t a, int count) {
  return (wav_i8x16_t) { a.values >> count };
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_shr(wav_i16x8_t a, int count) {
  return (wav_i16x8_t) { a.values >> count };
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_shr(wav_i32x4_t a, int count) {
  return (wav_i32x4_t) { a.values >> count };
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_shr(wav_i64x2_t a, int count) {
  return (wav_i64x2_t) { a.values >> ((int64_t) count) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_shr(wav_u8x16_t a, int count) {
  return (wav_u8x16_t) { a.values >> count };
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_shr(wav_u16x8_t a, int count) {
  return (wav_u16x8_t) { a.values >> count };
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_shr(wav_u32x4_t a, int count) {
  return (wav_u32x4_t) { a.values >> count };
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_shr(wav_u64x2_t a, int count) {
  return (wav_u64x2_t) { a.values >> ((int64_t) count) };
}

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_shr(wav_i8x16_t a, int count) { return wav_i8x16_shr(a, count); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_shr(wav_i16x8_t a, int count) { return wav_i16x8_shr(a, count); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_shr(wav_i32x4_t a, int count) { return wav_i32x4_shr(a, count); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_shr(wav_i64x2_t a, int count) { return wav_i64x2_shr(a, count); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_shr(wav_u8x16_t a, int count) { return wav_u8x16_shr(a, count); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_shr(wav_u16x8_t a, int count) { return wav_u16x8_shr(a, count); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_shr(wav_u32x4_t a, int count) { return wav_u32x4_shr(a, count); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_shr(wav_u64x2_t a, int count) { return wav_u64x2_shr(a, count); }

#if WAV_OPERATOR_OVERLOADING
WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t operator>>(wav_i8x16_t a, int count) { return wav_i8x16_shr(a, count); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t operator>>(wav_i16x8_t a, int count) { return wav_i16x8_shr(a, count); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t operator>>(wav_i32x4_t a, int count) { return wav_i32x4_shr(a, count); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t operator>>(wav_i64x2_t a, int count) { return wav_i64x2_shr(a, count); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t operator>>(wav_u8x16_t a, int count) { return wav_u8x16_shr(a, count); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t operator>>(wav_u16x8_t a, int count) { return wav_u16x8_shr(a, count); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t operator>>(wav_u32x4_t a, int count) { return wav_u32x4_shr(a, count); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t operator>>(wav_u64x2_t a, int count) { return wav_u64x2_shr(a, count); }
#endif

/* popcnt -- Lane-wise Population Count
 *
 * Count the number of bits set to one within each lane.
 */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_popcnt(wav_i8x16_t a) {
  #if WAV_PORTABLE_SLOW(TODO)
    wav_i8x16_t r;

    for (int i = 0 ; i < 16 ; i++) {
      r.values[i] = (int8_t) __builtin_popcount((unsigned int) a.values[i]);
    }

    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wconditional-uninitialized" /* ??? */
    return r;
    #pragma clang diagnostic pop
  #else
    return (wav_i8x16_t) { __builtin_wasm_popcnt_i8x16(a.values) };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_popcnt(wav_u8x16_t a) {
  return wav_i8x16_as_u8x16(wav_i8x16_popcnt(wav_u8x16_as_i8x16(a)));
}

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_popcnt(wav_i8x16_t a) { return wav_i8x16_popcnt(a); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_popcnt(wav_u8x16_t a) { return wav_u8x16_popcnt(a); }

/* any_true -- Any bit true
 *
 * These functions return 1 if any bit in a is non-zero, 0 otherwise.
 */

WAV_FUNCTION_ATTRIBUTES
bool
wav_i64x2_any_true(wav_i64x2_t value) {
  #if WAV_PORTABLE_SLOW(TODO)
    int32_t r = 0;
    for (int i = 0 ; i < 2 ; i++) {
      r |= value.values[i];
    }
    return !!r;
  #else
    return __builtin_wasm_any_true_v128((int8_t __attribute__((__vector_size__(16)))) value.values);
  #endif
}

WAV_FUNCTION_ATTRIBUTES
bool
wav_i32x4_any_true(wav_i32x4_t value) {
  return wav_i64x2_any_true(wav_i32x4_as_i64x2(value));
}

WAV_FUNCTION_ATTRIBUTES
bool
wav_i16x8_any_true(wav_i16x8_t value) {
  return wav_i64x2_any_true(wav_i16x8_as_i64x2(value));
}

WAV_FUNCTION_ATTRIBUTES
bool
wav_i8x16_any_true(wav_i8x16_t value) {
  return wav_i64x2_any_true(wav_i8x16_as_i64x2(value));
}

WAV_FUNCTION_ATTRIBUTES
bool
wav_u64x2_any_true(wav_u64x2_t value) {
  return wav_i64x2_any_true(wav_u64x2_as_i64x2(value));
}

WAV_FUNCTION_ATTRIBUTES
bool
wav_u32x4_any_true(wav_u32x4_t value) {
  return wav_i64x2_any_true(wav_u32x4_as_i64x2(value));
}

WAV_FUNCTION_ATTRIBUTES
bool
wav_u16x8_any_true(wav_u16x8_t value) {
  return wav_i64x2_any_true(wav_u16x8_as_i64x2(value));
}

WAV_FUNCTION_ATTRIBUTES
bool
wav_u8x16_any_true(wav_u8x16_t value) {
  return wav_i64x2_any_true(wav_u8x16_as_i64x2(value));
}

WAV_FUNCTION_ATTRIBUTES
bool
wav_b64x2_any_true(wav_b64x2_t value) {
  return wav_i64x2_any_true(wav_b64x2_as_i64x2(value));
}

WAV_FUNCTION_ATTRIBUTES
bool
wav_b32x4_any_true(wav_b32x4_t value) {
  return wav_i64x2_any_true(wav_b32x4_as_i64x2(value));
}

WAV_FUNCTION_ATTRIBUTES
bool
wav_b16x8_any_true(wav_b16x8_t value) {
  return wav_i64x2_any_true(wav_b16x8_as_i64x2(value));
}

WAV_FUNCTION_ATTRIBUTES
bool
wav_b8x16_any_true(wav_b8x16_t value) {
  return wav_i64x2_any_true(wav_b8x16_as_i64x2(value));
}

WAV_OVERLOAD_ATTRIBUTES bool wav_any_true(wav_i8x16_t a) { return wav_i8x16_any_true(a); }
WAV_OVERLOAD_ATTRIBUTES bool wav_any_true(wav_i16x8_t a) { return wav_i16x8_any_true(a); }
WAV_OVERLOAD_ATTRIBUTES bool wav_any_true(wav_i32x4_t a) { return wav_i32x4_any_true(a); }
WAV_OVERLOAD_ATTRIBUTES bool wav_any_true(wav_i64x2_t a) { return wav_i64x2_any_true(a); }
WAV_OVERLOAD_ATTRIBUTES bool wav_any_true(wav_u8x16_t a) { return wav_u8x16_any_true(a); }
WAV_OVERLOAD_ATTRIBUTES bool wav_any_true(wav_u16x8_t a) { return wav_u16x8_any_true(a); }
WAV_OVERLOAD_ATTRIBUTES bool wav_any_true(wav_u32x4_t a) { return wav_u32x4_any_true(a); }
WAV_OVERLOAD_ATTRIBUTES bool wav_any_true(wav_u64x2_t a) { return wav_u64x2_any_true(a); }
WAV_OVERLOAD_ATTRIBUTES bool wav_any_true(wav_b8x16_t a) { return wav_b8x16_any_true(a); }
WAV_OVERLOAD_ATTRIBUTES bool wav_any_true(wav_b16x8_t a) { return wav_b16x8_any_true(a); }
WAV_OVERLOAD_ATTRIBUTES bool wav_any_true(wav_b32x4_t a) { return wav_b32x4_any_true(a); }
WAV_OVERLOAD_ATTRIBUTES bool wav_any_true(wav_b64x2_t a) { return wav_b64x2_any_true(a); }

/* all_true -- Any bit true
 *
 * These functions return 1 if all bit in a is non-zero, 0 otherwise.
 */

WAV_FUNCTION_ATTRIBUTES
bool
wav_i8x16_all_true(wav_i8x16_t value) {
  #if WAV_PORTABLE_SLOW(TODO)
    bool r = 1;
    for (int i = 0 ; i < 16 ; i++)
      r &= !!(value.values[i]);
    return r;
  #else
    return __builtin_wasm_all_true_i8x16(value.values);
  #endif
}

WAV_FUNCTION_ATTRIBUTES
bool
wav_i16x8_all_true(wav_i16x8_t value) {
  #if WAV_PORTABLE_SLOW(TODO)
    bool r = 1;
    for (int i = 0 ; i < 8 ; i++)
      r &= !!(value.values[i]);
    return r;
  #else
    return __builtin_wasm_all_true_i16x8(value.values);
  #endif
}

WAV_FUNCTION_ATTRIBUTES
bool
wav_i32x4_all_true(wav_i32x4_t value) {
  #if WAV_PORTABLE_SLOW(TODO)
    bool r = 1;
    for (int i = 0 ; i < 4 ; i++)
      r &= !!(value.values[i]);
    return r;
  #else
    return __builtin_wasm_all_true_i32x4(value.values);
  #endif
}

WAV_FUNCTION_ATTRIBUTES
bool
wav_i64x2_all_true(wav_i64x2_t value) {
  #if WAV_PORTABLE_SLOW(TODO)
    bool r = 1;
    for (int i = 0 ; i < 2 ; i++)
      r &= !!(value.values[i]);
    return r;
  #else
    return __builtin_wasm_all_true_i64x2(value.values);
  #endif
}

WAV_FUNCTION_ATTRIBUTES
bool
wav_u8x16_all_true(wav_u8x16_t value) {
  return wav_i8x16_all_true(wav_u8x16_as_i8x16(value));
}

WAV_FUNCTION_ATTRIBUTES
bool
wav_u16x8_all_true(wav_u16x8_t value) {
  return wav_i16x8_all_true(wav_u16x8_as_i16x8(value));
}

WAV_FUNCTION_ATTRIBUTES
bool
wav_u32x4_all_true(wav_u32x4_t value) {
  return wav_i32x4_all_true(wav_u32x4_as_i32x4(value));
}

WAV_FUNCTION_ATTRIBUTES
bool
wav_u64x2_all_true(wav_u64x2_t value) {
  return wav_i64x2_all_true(wav_u64x2_as_i64x2(value));
}

WAV_FUNCTION_ATTRIBUTES
bool
wav_b8x16_all_true(wav_b8x16_t value) {
  return wav_i8x16_all_true(wav_b8x16_as_i8x16(value));
}

WAV_FUNCTION_ATTRIBUTES
bool
wav_b16x8_all_true(wav_b16x8_t value) {
  return wav_i16x8_all_true(wav_b16x8_as_i16x8(value));
}

WAV_FUNCTION_ATTRIBUTES
bool
wav_b32x4_all_true(wav_b32x4_t value) {
  return wav_i32x4_all_true(wav_b32x4_as_i32x4(value));
}

WAV_FUNCTION_ATTRIBUTES
bool
wav_b64x2_all_true(wav_b64x2_t value) {
  return wav_i64x2_all_true(wav_b64x2_as_i64x2(value));
}

WAV_OVERLOAD_ATTRIBUTES bool wav_all_true(wav_i8x16_t a) { return wav_i8x16_all_true(a); }
WAV_OVERLOAD_ATTRIBUTES bool wav_all_true(wav_i16x8_t a) { return wav_i16x8_all_true(a); }
WAV_OVERLOAD_ATTRIBUTES bool wav_all_true(wav_i32x4_t a) { return wav_i32x4_all_true(a); }
WAV_OVERLOAD_ATTRIBUTES bool wav_all_true(wav_i64x2_t a) { return wav_i64x2_all_true(a); }
WAV_OVERLOAD_ATTRIBUTES bool wav_all_true(wav_u8x16_t a) { return wav_u8x16_all_true(a); }
WAV_OVERLOAD_ATTRIBUTES bool wav_all_true(wav_u16x8_t a) { return wav_u16x8_all_true(a); }
WAV_OVERLOAD_ATTRIBUTES bool wav_all_true(wav_u32x4_t a) { return wav_u32x4_all_true(a); }
WAV_OVERLOAD_ATTRIBUTES bool wav_all_true(wav_u64x2_t a) { return wav_u64x2_all_true(a); }
WAV_OVERLOAD_ATTRIBUTES bool wav_all_true(wav_b8x16_t a) { return wav_b8x16_all_true(a); }
WAV_OVERLOAD_ATTRIBUTES bool wav_all_true(wav_b16x8_t a) { return wav_b16x8_all_true(a); }
WAV_OVERLOAD_ATTRIBUTES bool wav_all_true(wav_b32x4_t a) { return wav_b32x4_all_true(a); }
WAV_OVERLOAD_ATTRIBUTES bool wav_all_true(wav_b64x2_t a) { return wav_b64x2_all_true(a); }

/* load_zero -- Load and Zero-Pad
 *
 * Load a single 32-bit or 64-bit element into the lowest bits of a
 * vector, and initialize all other bits of the vector to zero.
 */

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_load_zero(const int32_t * a) {
  #if WAV_PORTABLE_SLOW(TODO)
    return (wav_i32x4_t) { { *a, 0, 0, 0 } };
  #elif WAV_PORTABLE_SLOW(TODO)
    return (wav_i32x4_t) { __builtin_wasm_load32_zero(a) };
  #else
    return (wav_i32x4_t) {
      #pragma clang diagnostic push
      #pragma clang diagnostic ignored "-Wcast-qual"
      __builtin_wasm_load32_zero((int32_t*) a)
      #pragma clang diagnostic pop
    };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_load_zero(const int64_t * a) {
  #if WAV_PORTABLE_SLOW(TODO)
    return (wav_i64x2_t) { { *a, 0 } };
  #elif WAV_PORTABLE_SLOW(TODO)
    return (wav_i64x2_t) { __builtin_wasm_load64_zero(a) };
  #else
    return (wav_i64x2_t) {
      #pragma clang diagnostic push
      #pragma clang diagnostic ignored "-Wcast-qual"
      __builtin_wasm_load64_zero((int64_t*) a)
      #pragma clang diagnostic pop
    };
  #endif
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_load_zero(const uint32_t * a) {
  return wav_i32x4_as_u32x4(wav_i32x4_load_zero((const int32_t*) a));
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_load_zero(const uint64_t * a) {
  return wav_i64x2_as_u64x2(wav_i64x2_load_zero((const int64_t*) a));
}

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_load_zero(const float * a) {
  return wav_i32x4_as_f32x4(wav_i32x4_load_zero((const int32_t*) a));
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_load_zero(const double * a) {
  return wav_i64x2_as_f64x2(wav_i64x2_load_zero((const int64_t*) a));
}

WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_load_zero(const  int32_t * src) { return wav_i32x4_load_zero(src); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_load_zero(const  int64_t * src) { return wav_i64x2_load_zero(src); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_load_zero(const uint32_t * src) { return wav_u32x4_load_zero(src); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_load_zero(const uint64_t * src) { return wav_u64x2_load_zero(src); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_load_zero(const    float * src) { return wav_f32x4_load_zero(src); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_load_zero(const   double * src) { return wav_f64x2_load_zero(src); }

/* load_splat -- Load and Splat
 *
 * Load a single element and splat to all lanes of a vector. The
 * natural alignment is the size of the element loaded.
 */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_load_splat(const int8_t * a) {
  return wav_i8x16_splat(*a);
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_load_splat(const int16_t * a) {
  return wav_i16x8_splat(*a);
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_load_splat(const int32_t * a) {
  return wav_i32x4_splat(*a);
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_load_splat(const int64_t * a) {
  return wav_i64x2_splat(*a);
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_load_splat(const uint8_t * a) {
  return wav_u8x16_splat(*a);
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_load_splat(const uint16_t * a) {
  return wav_u16x8_splat(*a);
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_load_splat(const uint32_t * a) {
  return wav_u32x4_splat(*a);
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_load_splat(const uint64_t * a) {
  return wav_u64x2_splat(*a);
}

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_load_splat(const float * a) {
  return wav_f32x4_splat(*a);
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_load_splat(const double * a) {
  return wav_f64x2_splat(*a);
}

WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_load_splat(const   int8_t* a) { return wav_i8x16_load_splat(a); }
WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_load_splat(const  int16_t* a) { return wav_i16x8_load_splat(a); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_load_splat(const  int32_t* a) { return wav_i32x4_load_splat(a); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_load_splat(const  int64_t* a) { return wav_i64x2_load_splat(a); }
WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_load_splat(const  uint8_t* a) { return wav_u8x16_load_splat(a); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_load_splat(const uint16_t* a) { return wav_u16x8_load_splat(a); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_load_splat(const uint32_t* a) { return wav_u32x4_load_splat(a); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_load_splat(const uint64_t* a) { return wav_u64x2_load_splat(a); }
WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_load_splat(const    float* a) { return wav_f32x4_load_splat(a); }
WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_load_splat(const   double* a) { return wav_f64x2_load_splat(a); }

/* loadu_splat -- Load and Splat
 *
 * Load a single unaligned element and splat to all lanes of a vector.
 */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_loadu_splat(const void * a) {
  int8_t v;
  __builtin_memcpy(&v, a, sizeof(v));
  return wav_i8x16_splat(v);
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_loadu_splat(const void * a) {
  int16_t v;
  __builtin_memcpy(&v, a, sizeof(v));
  return wav_i16x8_splat(v);
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_loadu_splat(const void * a) {
  int32_t v;
  __builtin_memcpy(&v, a, sizeof(v));
  return wav_i32x4_splat(v);
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_loadu_splat(const void * a) {
  int64_t v;
  __builtin_memcpy(&v, a, sizeof(v));
  return wav_i64x2_splat(v);
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_loadu_splat(const void * a) {
  uint8_t v;
  __builtin_memcpy(&v, a, sizeof(v));
  return wav_u8x16_splat(v);
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_loadu_splat(const void * a) {
  uint16_t v;
  __builtin_memcpy(&v, a, sizeof(v));
  return wav_u16x8_splat(v);
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_loadu_splat(const void * a) {
  uint32_t v;
  __builtin_memcpy(&v, a, sizeof(v));
  return wav_u32x4_splat(v);
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_loadu_splat(const void * a) {
  uint64_t v;
  __builtin_memcpy(&v, a, sizeof(v));
  return wav_u64x2_splat(v);
}

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_loadu_splat(const void * a) {
  float v;
  __builtin_memcpy(&v, a, sizeof(v));
  return wav_f32x4_splat(v);
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_loadu_splat(const void * a) {
  double v;
  __builtin_memcpy(&v, a, sizeof(v));
  return wav_f64x2_splat(v);
}

/* load_lane -- Load Lane
 *
 * Load a single element from m into the lane of x specified in the
 * immediate mode operand imm. The values of all other lanes of x are
 * bypassed as is.
 */

/* The overloaded version is not implemented yet since the intrinsic
 * is not usable for it; lane must be an ICE, so this has to be
 * implemented as a macro.  */

#if WAV_PORTABLE_SLOW(TODO)
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_i8x16_load_lane(wav_i8x16_t dest, const int lane, const   int8_t * src) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0, 15) { dest.values[lane] = *src; return v; }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_i16x8_load_lane(wav_i16x8_t dest, const int lane, const  int16_t * src) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  7) { dest.values[lane] = *src; return v; }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_i32x4_load_lane(wav_i32x4_t dest, const int lane, const  int32_t * src) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  3) { dest.values[lane] = *src; return v; }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_i64x2_load_lane(wav_i64x2_t dest, const int lane, const  int64_t * src) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  1) { dest.values[lane] = *src; return v; }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_u8x16_load_lane(wav_u8x16_t dest, const int lane, const  uint8_t * src) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0, 15) { dest.values[lane] = *src; return v; }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_u16x8_load_lane(wav_u16x8_t dest, const int lane, const uint16_t * src) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  7) { dest.values[lane] = *src; return v; }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_u32x4_load_lane(wav_u32x4_t dest, const int lane, const uint32_t * src) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  3) { dest.values[lane] = *src; return v; }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_u64x2_load_lane(wav_u64x2_t dest, const int lane, const uint64_t * src) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  1) { dest.values[lane] = *src; return v; }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_f32x4_load_lane(wav_f32x4_t dest, const int lane, const    float * src) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  3) { dest.values[lane] = *src; return v; }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_f64x2_load_lane(wav_f64x2_t dest, const int lane, const   double * src) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  1) { dest.values[lane] = *src; return v; }

  WAV_OVERLOAD_ATTRIBUTES wav_i8x16_t wav_load_lane(wav_i8x16_t dest, const int lane, const   int8_t * src) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0, 15) { dest.values[lane] = *src; return v; }
  WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_load_lane(wav_i16x8_t dest, const int lane, const  int16_t * src) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  7) { dest.values[lane] = *src; return v; }
  WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_load_lane(wav_i32x4_t dest, const int lane, const  int32_t * src) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  3) { dest.values[lane] = *src; return v; }
  WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_load_lane(wav_i64x2_t dest, const int lane, const  int64_t * src) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  1) { dest.values[lane] = *src; return v; }
  WAV_OVERLOAD_ATTRIBUTES wav_u8x16_t wav_load_lane(wav_u8x16_t dest, const int lane, const  uint8_t * src) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0, 15) { dest.values[lane] = *src; return v; }
  WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_load_lane(wav_u16x8_t dest, const int lane, const uint16_t * src) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  7) { dest.values[lane] = *src; return v; }
  WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_load_lane(wav_u32x4_t dest, const int lane, const uint32_t * src) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  3) { dest.values[lane] = *src; return v; }
  WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_load_lane(wav_u64x2_t dest, const int lane, const uint64_t * src) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  1) { dest.values[lane] = *src; return v; }
  WAV_OVERLOAD_ATTRIBUTES wav_f32x4_t wav_load_lane(wav_f32x4_t dest, const int lane, const    float * src) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  3) { dest.values[lane] = *src; return v; }
  WAV_OVERLOAD_ATTRIBUTES wav_f64x2_t wav_load_lane(wav_f64x2_t dest, const int lane, const   double * src) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  1) { dest.values[lane] = *src; return v; }
#else
  #define wav_i8x16_load_lane(dest, lane, src) (__extension__({ \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Wold-style-cast\"") \
    _Pragma("clang diagnostic ignored \"-Wcast-qual\"") \
    (wav_i8x16_t) { __builtin_wasm_load8_lane ((int8_t *) (src), (dest).values, (lane)) }; \
    _Pragma("clang diagnostic pop") \
  }))
  #define wav_i16x8_load_lane(dest, lane, src) (__extension__({ \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Wold-style-cast\"") \
    _Pragma("clang diagnostic ignored \"-Wcast-qual\"") \
    (wav_i16x8_t) { __builtin_wasm_load16_lane((int16_t *) (src), (dest).values, (lane)) }; \
    _Pragma("clang diagnostic pop") \
  }))
  #define wav_i32x4_load_lane(dest, lane, src) (__extension__({ \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Wold-style-cast\"") \
    _Pragma("clang diagnostic ignored \"-Wcast-qual\"") \
    (wav_i32x4_t) { __builtin_wasm_load32_lane((int32_t *) (src), (dest).values, (lane)) }; \
    _Pragma("clang diagnostic pop") \
  }))
  #define wav_i64x2_load_lane(dest, lane, src) (__extension__({ \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Wold-style-cast\"") \
    _Pragma("clang diagnostic ignored \"-Wcast-qual\"") \
    (wav_i64x2_t) { __builtin_wasm_load64_lane((int64_t *) (src), (dest).values, (lane)) }; \
    _Pragma("clang diagnostic pop") \
  }))
  #define wav_u8x16_load_lane(dest, lane, src) (__extension__({ \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Wold-style-cast\"") \
    _Pragma("clang diagnostic ignored \"-Wcast-qual\"") \
    (__typeof__(dest)) { \
      (__typeof__((dest).values)) __builtin_wasm_load8_lane( \
        (int8_t *) (value), \
        (int8_t __attribute__((__vector_size__(16)))) ((dest).values), \
        (lane) \
      ) \
    }; \
    _Pragma("clang diagnostic pop") \
  }))
  #define wav_u16x8_load_lane(dest, lane, src) (__extension__({ \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Wold-style-cast\"") \
    _Pragma("clang diagnostic ignored \"-Wcast-qual\"") \
    (__typeof__(dest)) { \
      (__typeof__((dest).values)) __builtin_wasm_load16_lane( \
        (int16_t *) (src), \
        (int16_t __attribute__((__vector_size__(16)))) ((dest).values), \
        (lane) \
      ) \
    }; \
    _Pragma("clang diagnostic pop") \
  }))
  #define wav_u32x4_load_lane(dest, lane, src) (__extension__({ \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Wold-style-cast\"") \
    _Pragma("clang diagnostic ignored \"-Wcast-qual\"") \
    (__typeof__(dest)) { \
      (__typeof__((dest).values)) __builtin_wasm_load32_lane( \
        (int32_t *) (src), \
        (int32_t __attribute__((__vector_size__(16)))) ((dest).values), \
        (lane) \
      ) \
    }; \
    _Pragma("clang diagnostic pop") \
  }))
  #define wav_u64x2_load_lane(dest, lane, src) (__extension__({ \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Wold-style-cast\"") \
    _Pragma("clang diagnostic ignored \"-Wcast-qual\"") \
    (__typeof__(dest)) { \
      (__typeof__((dest).values)) __builtin_wasm_load64_lane( \
        (int64_t *) (src), \
        (int64_t __attribute__((__vector_size__(16)))) ((dest).values), \
        (lane) \
      ) \
    }; \
    _Pragma("clang diagnostic pop") \
  }))
  #define wav_f32x4_load_lane(dest, lane, src) (__extension__({ \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Wold-style-cast\"") \
    _Pragma("clang diagnostic ignored \"-Wcast-qual\"") \
    (__typeof__(dest)) { \
      (__typeof__((dest).values)) __builtin_wasm_load32_lane( \
        (int32_t *) (src), \
        (int32_t __attribute__((__vector_size__(16)))) ((dest).values), \
        (lane) \
      ) \
    }; \
    _Pragma("clang diagnostic pop") \
  }))
  #define wav_f64x2_load_lane(dest, lane, src) (__extension__({ \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Wold-style-cast\"") \
    _Pragma("clang diagnostic ignored \"-Wcast-qual\"") \
    (__typeof__(dest)) { \
      (__typeof__((dest).values)) __builtin_wasm_load64_lane( \
        (int64_t *) (src), \
        (int64_t __attribute__((__vector_size__(16)))) ((dest).values), \
        (lane) \
      ) \
    }; \
    _Pragma("clang diagnostic pop") \
  }))
#endif

/* store_lane -- Load Lane
 *
 * Store into m the lane of data specified.
 */

/* The overloaded version is not implemented yet since the intrinsic
 * is not usable for it; lane must be an ICE, so this has to be
 * implemented as a macro.  */

#if WAV_PORTABLE_SLOW(TODO)
  WAV_FUNCTION_ATTRIBUTES void wav_i8x16_store_lane(  int8_t * dest, wav_i8x16_t src, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0, 15) { *dest = src.values[lane]; }
  WAV_FUNCTION_ATTRIBUTES void wav_i16x8_store_lane( int16_t * dest, wav_i16x8_t src, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  7) { *dest = src.values[lane]; }
  WAV_FUNCTION_ATTRIBUTES void wav_i32x4_store_lane( int32_t * dest, wav_i32x4_t src, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  3) { *dest = src.values[lane]; }
  WAV_FUNCTION_ATTRIBUTES void wav_i64x2_store_lane( int64_t * dest, wav_i64x2_t src, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  1) { *dest = src.values[lane]; }
  WAV_FUNCTION_ATTRIBUTES void wav_u8x16_store_lane( uint8_t * dest, wav_u8x16_t src, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0, 15) { *dest = src.values[lane]; }
  WAV_FUNCTION_ATTRIBUTES void wav_u16x8_store_lane(uint16_t * dest, wav_u16x8_t src, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  7) { *dest = src.values[lane]; }
  WAV_FUNCTION_ATTRIBUTES void wav_u32x4_store_lane(uint32_t * dest, wav_u32x4_t src, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  3) { *dest = src.values[lane]; }
  WAV_FUNCTION_ATTRIBUTES void wav_u64x2_store_lane(uint64_t * dest, wav_u64x2_t src, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  1) { *dest = src.values[lane]; }
  WAV_FUNCTION_ATTRIBUTES void wav_f32x4_store_lane(   float * dest, wav_f32x4_t src, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  3) { *dest = src.values[lane]; }
  WAV_FUNCTION_ATTRIBUTES void wav_f64x2_store_lane(  double * dest, wav_f64x2_t src, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  1) { *dest = src.values[lane]; }

  WAV_OVERLOAD_ATTRIBUTES void wav_store_lane(  int8_t * dest, wav_i8x16_t src, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0, 15) { *dest = src.values[lane]; }
  WAV_OVERLOAD_ATTRIBUTES void wav_store_lane( int16_t * dest, wav_i16x8_t src, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  7) { *dest = src.values[lane]; }
  WAV_OVERLOAD_ATTRIBUTES void wav_store_lane( int32_t * dest, wav_i32x4_t src, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  3) { *dest = src.values[lane]; }
  WAV_OVERLOAD_ATTRIBUTES void wav_store_lane( int64_t * dest, wav_i64x2_t src, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  1) { *dest = src.values[lane]; }
  WAV_OVERLOAD_ATTRIBUTES void wav_store_lane( uint8_t * dest, wav_u8x16_t src, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0, 15) { *dest = src.values[lane]; }
  WAV_OVERLOAD_ATTRIBUTES void wav_store_lane(uint16_t * dest, wav_u16x8_t src, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  7) { *dest = src.values[lane]; }
  WAV_OVERLOAD_ATTRIBUTES void wav_store_lane(uint32_t * dest, wav_u32x4_t src, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  3) { *dest = src.values[lane]; }
  WAV_OVERLOAD_ATTRIBUTES void wav_store_lane(uint64_t * dest, wav_u64x2_t src, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  1) { *dest = src.values[lane]; }
  WAV_OVERLOAD_ATTRIBUTES void wav_store_lane(   float * dest, wav_f32x4_t src, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  3) { *dest = src.values[lane]; }
  WAV_OVERLOAD_ATTRIBUTES void wav_store_lane(  double * dest, wav_f64x2_t src, const int lane) WAV_REQUIRE_CONSTANT_RANGE_PARAM(lane, 0,  1) { *dest = src.values[lane]; }
#else
  #define wav_i8x16_store_lane(dest, src, lane) (__extension__({ __builtin_wasm_store8_lane ((dest), (src).values, (lane)); }))
  #define wav_i16x8_store_lane(dest, src, lane) (__extension__({ __builtin_wasm_store16_lane((dest), (src).values, (lane)); }))
  #define wav_i32x4_store_lane(dest, src, lane) (__extension__({ __builtin_wasm_store32_lane((dest), (src).values, (lane)); }))
  #define wav_i64x2_store_lane(dest, src, lane) (__extension__({ __builtin_wasm_store64_lane((dest), (src).values, (lane)); }))
  #define wav_u8x16_store_lane(dest, src, lane) (__extension__({ \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Wold-style-cast\"") \
    __builtin_wasm_store8_lane( \
      (int8_t *) (dest), \
      (int8_t __attribute__((__vector_size__(16)))) ((src).values), \
      (lane) \
    ); \
    _Pragma("clang diagnostic pop") \
  }))
  #define wav_u16x8_store_lane(dest, src, lane) (__extension__({ \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Wold-style-cast\"") \
    __builtin_wasm_store16_lane( \
      (int16_t *) (dest), \
      (int16_t __attribute__((__vector_size__(16)))) ((src).values), \
      (lane) \
    ); \
    _Pragma("clang diagnostic pop") \
  }))
  #define wav_u32x4_store_lane(dest, src, lane) (__extension__({ \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Wold-style-cast\"") \
    __builtin_wasm_store32_lane( \
      (int32_t *) (dest), \
      (int32_t __attribute__((__vector_size__(16)))) ((src).values), \
      (lane) \
    ); \
    _Pragma("clang diagnostic pop") \
  }))
  #define wav_u64x2_store_lane(dest, src, lane) (__extension__({ \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Wold-style-cast\"") \
    __builtin_wasm_store64_lane( \
      (int64_t *) (dest), \
      (int64_t __attribute__((__vector_size__(16)))) ((src).values), \
      (lane) \
    ); \
    _Pragma("clang diagnostic pop") \
  }))
  #define wav_f32x4_store_lane(dest, src, lane) (__extension__({ \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Wold-style-cast\"") \
    __builtin_wasm_store32_lane( \
      (int32_t *) (dest), \
      (int32_t __attribute__((__vector_size__(16)))) ((src).values), \
      (lane) \
    ); \
    _Pragma("clang diagnostic pop") \
  }))
  #define wav_f64x2_store_lane(dest, src, lane) (__extension__({ \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Wold-style-cast\"") \
    __builtin_wasm_store64_lane( \
      (int64_t *) (dest), \
      (int64_t __attribute__((__vector_size__(16)))) ((src).values), \
      (lane) \
    ); \
    _Pragma("clang diagnostic pop") \
  }))
#endif

/* load_extend -- Load and Extend
 *
 * Fetch consecutive integers up to 32-bit wide and produce a vector
 * with lanes up to 64 bits. The natural alignment is 8 bytes.
 */

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_load_extend(WAV_ALIGNED(8) int8_t values[WAV_ARRAY_LENGTH(8)]) {
  wav_i16x8_t r;
  int8_t vec __attribute__((__vector_size__(8)));
  __builtin_memcpy(&vec, values, sizeof(vec));
  r.values = __builtin_convertvector(vec, __typeof__(r.values));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_load_extend(WAV_ALIGNED(8) int16_t values[WAV_ARRAY_LENGTH(4)]) {
  wav_i32x4_t r;
  int16_t vec __attribute__((__vector_size__(8)));
  __builtin_memcpy(&vec, values, sizeof(vec));
  r.values = __builtin_convertvector(vec, __typeof__(r.values));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_load_extend(WAV_ALIGNED(8) int32_t values[WAV_ARRAY_LENGTH(2)]) {
  wav_i64x2_t r;
  int32_t vec __attribute__((__vector_size__(8)));
  __builtin_memcpy(&vec, values, sizeof(vec));
  r.values = __builtin_convertvector(vec, __typeof__(r.values));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_load_extend(WAV_ALIGNED(8) uint8_t values[WAV_ARRAY_LENGTH(8)]) {
  wav_u16x8_t r;
  int8_t vec __attribute__((__vector_size__(8)));
  __builtin_memcpy(&vec, values, sizeof(vec));
  r.values = __builtin_convertvector(vec, __typeof__(r.values));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_load_extend(WAV_ALIGNED(8) uint16_t values[WAV_ARRAY_LENGTH(4)]) {
  wav_u32x4_t r;
  int16_t vec __attribute__((__vector_size__(8)));
  __builtin_memcpy(&vec, values, sizeof(vec));
  r.values = __builtin_convertvector(vec, __typeof__(r.values));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_load_extend(WAV_ALIGNED(8) uint32_t values[WAV_ARRAY_LENGTH(2)]) {
  wav_u64x2_t r;
  int32_t vec __attribute__((__vector_size__(8)));
  __builtin_memcpy(&vec, values, sizeof(vec));
  r.values = __builtin_convertvector(vec, __typeof__(r.values));
  return r;
}

WAV_OVERLOAD_ATTRIBUTES wav_i16x8_t wav_load_extend(WAV_ALIGNED(8)   int8_t a[WAV_ARRAY_LENGTH(8)]) { return wav_i16x8_load_extend(a); }
WAV_OVERLOAD_ATTRIBUTES wav_i32x4_t wav_load_extend(WAV_ALIGNED(8)  int16_t a[WAV_ARRAY_LENGTH(4)]) { return wav_i32x4_load_extend(a); }
WAV_OVERLOAD_ATTRIBUTES wav_i64x2_t wav_load_extend(WAV_ALIGNED(8)  int32_t a[WAV_ARRAY_LENGTH(2)]) { return wav_i64x2_load_extend(a); }
WAV_OVERLOAD_ATTRIBUTES wav_u16x8_t wav_load_extend(WAV_ALIGNED(8)  uint8_t a[WAV_ARRAY_LENGTH(8)]) { return wav_u16x8_load_extend(a); }
WAV_OVERLOAD_ATTRIBUTES wav_u32x4_t wav_load_extend(WAV_ALIGNED(8) uint16_t a[WAV_ARRAY_LENGTH(4)]) { return wav_u32x4_load_extend(a); }
WAV_OVERLOAD_ATTRIBUTES wav_u64x2_t wav_load_extend(WAV_ALIGNED(8) uint32_t a[WAV_ARRAY_LENGTH(2)]) { return wav_u64x2_load_extend(a); }

/* bitmask -- Bitmask extraction
 *
 * These operations extract the high bit for each lane in a and produce
 * a scalar mask with all bits concatenated.
 */

WAV_FUNCTION_ATTRIBUTES
unsigned int
wav_i8x16_bitmask(wav_i8x16_t a) {
  #if WAV_PORTABLE_SLOW(TODO)
    unsigned int r = 0;

    for (int i = 0 ; i < 16 ; i++) {
      r |= ((unsigned int) (a.values[i] < 0)) << i;
    }

    return r;
  #else
    return (unsigned int) __builtin_wasm_bitmask_i8x16(a.values);
  #endif
}

WAV_FUNCTION_ATTRIBUTES
unsigned int
wav_i16x8_bitmask(wav_i16x8_t a) {
  #if WAV_PORTABLE_SLOW(TODO)
    unsigned int r = 0;

    for (int i = 0 ; i < 8 ; i++) {
      r |= ((unsigned int) (a.values[i] < 0)) << i;
    }

    return r;
  #else
    return (unsigned int) __builtin_wasm_bitmask_i16x8(a.values);
  #endif
}

WAV_FUNCTION_ATTRIBUTES
unsigned int
wav_i32x4_bitmask(wav_i32x4_t a) {
  #if WAV_PORTABLE_SLOW(TODO)
    unsigned int r = 0;

    for (int i = 0 ; i < 4 ; i++) {
      r |= ((unsigned int) (a.values[i] < 0)) << i;
    }

    return r;
  #else
    return (unsigned int) __builtin_wasm_bitmask_i32x4(a.values);
  #endif
}

WAV_FUNCTION_ATTRIBUTES
unsigned int
wav_i64x2_bitmask(wav_i64x2_t a) {
  #if WAV_PORTABLE_SLOW(TODO)
    unsigned int r = 0;

    for (int i = 0 ; i < 2 ; i++) {
      r |= ((unsigned int) (a.values[i] < 0)) << i;
    }

    return r;
  #else
    return (unsigned int) __builtin_wasm_bitmask_i64x2(a.values);
  #endif
}

WAV_FUNCTION_ATTRIBUTES
unsigned int
wav_u8x16_bitmask(wav_u8x16_t a) {
  return wav_i8x16_bitmask(wav_u8x16_as_i8x16(a));
}

WAV_FUNCTION_ATTRIBUTES
unsigned int
wav_u16x8_bitmask(wav_u16x8_t a) {
  return wav_i16x8_bitmask(wav_u16x8_as_i16x8(a));
}

WAV_FUNCTION_ATTRIBUTES
unsigned int
wav_u32x4_bitmask(wav_u32x4_t a) {
  return wav_i32x4_bitmask(wav_u32x4_as_i32x4(a));
}

WAV_FUNCTION_ATTRIBUTES
unsigned int
wav_u64x2_bitmask(wav_u64x2_t a) {
  return wav_i64x2_bitmask(wav_u64x2_as_i64x2(a));
}

WAV_FUNCTION_ATTRIBUTES
unsigned int
wav_b8x16_bitmask(wav_b8x16_t a) {
  return wav_i8x16_bitmask(wav_b8x16_as_i8x16(a));
}

WAV_FUNCTION_ATTRIBUTES
unsigned int
wav_b16x8_bitmask(wav_b16x8_t a) {
  return wav_i16x8_bitmask(wav_b16x8_as_i16x8(a));
}

WAV_FUNCTION_ATTRIBUTES
unsigned int
wav_b32x4_bitmask(wav_b32x4_t a) {
  return wav_i32x4_bitmask(wav_b32x4_as_i32x4(a));
}

WAV_FUNCTION_ATTRIBUTES
unsigned int
wav_b64x2_bitmask(wav_b64x2_t a) {
  return wav_i64x2_bitmask(wav_b64x2_as_i64x2(a));
}

WAV_OVERLOAD_ATTRIBUTES unsigned int wav_bitmask(wav_i8x16_t a) { return wav_i8x16_bitmask(a); }
WAV_OVERLOAD_ATTRIBUTES unsigned int wav_bitmask(wav_i16x8_t a) { return wav_i16x8_bitmask(a); }
WAV_OVERLOAD_ATTRIBUTES unsigned int wav_bitmask(wav_i32x4_t a) { return wav_i32x4_bitmask(a); }
WAV_OVERLOAD_ATTRIBUTES unsigned int wav_bitmask(wav_i64x2_t a) { return wav_i64x2_bitmask(a); }
WAV_OVERLOAD_ATTRIBUTES unsigned int wav_bitmask(wav_u8x16_t a) { return wav_u8x16_bitmask(a); }
WAV_OVERLOAD_ATTRIBUTES unsigned int wav_bitmask(wav_u16x8_t a) { return wav_u16x8_bitmask(a); }
WAV_OVERLOAD_ATTRIBUTES unsigned int wav_bitmask(wav_u32x4_t a) { return wav_u32x4_bitmask(a); }
WAV_OVERLOAD_ATTRIBUTES unsigned int wav_bitmask(wav_u64x2_t a) { return wav_u64x2_bitmask(a); }
WAV_OVERLOAD_ATTRIBUTES unsigned int wav_bitmask(wav_b8x16_t a) { return wav_b8x16_bitmask(a); }
WAV_OVERLOAD_ATTRIBUTES unsigned int wav_bitmask(wav_b16x8_t a) { return wav_b16x8_bitmask(a); }
WAV_OVERLOAD_ATTRIBUTES unsigned int wav_bitmask(wav_b32x4_t a) { return wav_b32x4_bitmask(a); }
WAV_OVERLOAD_ATTRIBUTES unsigned int wav_bitmask(wav_b64x2_t a) { return wav_b64x2_bitmask(a); }

#pragma clang diagnostic pop

#endif /* !defined(WAV_H) */
