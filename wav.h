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

/* We don't actually use wasm_simd128.h internally, but we want the
 * v128_t type so we can generate API to convert between it and our
 * types. */
#include <wasm_simd128.h>

/* Some builtins exist, but don't do anything (i.e., if you look
 * at the disassembly nothing is emitted). */
#define WAV_BUILTIN_IS_NOOP 0

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc99-extensions"
#pragma clang diagnostic ignored "-Wc11-extensions"
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#pragma clang diagnostic ignored "-Wold-style-cast"

/*
 * Configuration
 */

/* This determines whether we should use C11 generic selections or C++
 * overloaded functions to create the un-typed functions, or whether
 * we should skip them altogether. */

#define WAV_OVERLOADS_NONE 0
#define WAV_OVERLOADS_C11  1
#define WAV_OVERLOADS_CXX  2

#if !defined(WAV_OVERLOADS)
  #if defined(__cplusplus)
    #define WAV_OVERLOADS WAV_OVERLOADS_CXX
  #else
    /* clang allows generic selections in older versions of C, but
     * emits the -Wc11-extensions diagnostic (which we suppress). */
    #define WAV_OVERLOADS WAV_OVERLOADS_C11
  #endif
#endif

/* We use structs to make sure types are
 * incompatible.  If we just used raw vectors the boolean types would
 * be compatible with the unsigned types. */

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

#define WAV_FUNCTION_ATTRIBUTES static inline __attribute__((__artificial__,__always_inline__))
#if defined(__wasm_unimplemented_simd128__)
  #define WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
#else
  #define WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE __attribute__((__unavailable__("requires -munimplemented-simd128")))
#endif

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
  #define WAV_ARRAY_PARAM(Length) Length
#else
  #define WAV_ARRAY_PARAM(Length)
#endif

#define WAV_REQUIRE_CONSTANT(value) \
  _Static_assert(__builtin_constant_p(value), #value " is not constant")

#define WAV_REQUIRE_CONSTANT_RANGE(value, min, max) (__extension__ ({ \
    WAV_REQUIRE_CONSTANT(value); \
    _Static_assert((value <= max) && (value >= min), #value " is not between " #min " and " #max "."); \
  }))

#define WAV_REQUIRE_TYPE(Type, Value) (__extension__ ({ \
    const Type wav_require_type_value_ = Value; \
    wav_require_type_value_; \
  }))

#define WAV_DEFINE_REINTERPRET_FUNCTION_(Type_To, Type_From, Function_Name) \
  WAV_FUNCTION_ATTRIBUTES \
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
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i8x16_t,        v128_t, wav_v128_as_i8x16)
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
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i16x8_t,        v128_t, wav_v128_as_i16x8)
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
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i32x4_t,        v128_t, wav_v128_as_i32x4)
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
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_i64x2_t,        v128_t, wav_v128_as_i64x2)
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
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u8x16_t,        v128_t, wav_v128_as_u8x16)
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
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u16x8_t,        v128_t, wav_v128_as_u16x8)
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
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u32x4_t,        v128_t, wav_v128_as_u32x4)
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
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_u64x2_t,        v128_t, wav_v128_as_u64x2)
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
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_f32x4_t,        v128_t, wav_v128_as_f32x4)
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
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_f64x2_t,        v128_t, wav_v128_as_f64x2)
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
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b8x16_t,        v128_t, wav_v128_as_b8x16)
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
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b16x8_t,        v128_t, wav_v128_as_b16x8)
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
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b32x4_t,        v128_t, wav_v128_as_b32x4)
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
WAV_DEFINE_REINTERPRET_FUNCTION_(wav_b64x2_t,        v128_t, wav_v128_as_b64x2)
WAV_DEFINE_REINTERPRET_FUNCTION_(       v128_t, wav_i8x16_t, wav_i8x16_as_v128)
WAV_DEFINE_REINTERPRET_FUNCTION_(       v128_t, wav_i16x8_t, wav_i16x8_as_v128)
WAV_DEFINE_REINTERPRET_FUNCTION_(       v128_t, wav_i32x4_t, wav_i32x4_as_v128)
WAV_DEFINE_REINTERPRET_FUNCTION_(       v128_t, wav_i64x2_t, wav_i64x2_as_v128)
WAV_DEFINE_REINTERPRET_FUNCTION_(       v128_t, wav_u8x16_t, wav_u8x16_as_v128)
WAV_DEFINE_REINTERPRET_FUNCTION_(       v128_t, wav_u16x8_t, wav_u16x8_as_v128)
WAV_DEFINE_REINTERPRET_FUNCTION_(       v128_t, wav_u32x4_t, wav_u32x4_as_v128)
WAV_DEFINE_REINTERPRET_FUNCTION_(       v128_t, wav_u64x2_t, wav_u64x2_as_v128)
WAV_DEFINE_REINTERPRET_FUNCTION_(       v128_t, wav_f32x4_t, wav_f32x4_as_v128)
WAV_DEFINE_REINTERPRET_FUNCTION_(       v128_t, wav_f64x2_t, wav_f64x2_as_v128)
WAV_DEFINE_REINTERPRET_FUNCTION_(       v128_t, wav_b8x16_t, wav_b8x16_as_v128)
WAV_DEFINE_REINTERPRET_FUNCTION_(       v128_t, wav_b16x8_t, wav_b16x8_as_v128)
WAV_DEFINE_REINTERPRET_FUNCTION_(       v128_t, wav_b32x4_t, wav_b32x4_as_v128)
WAV_DEFINE_REINTERPRET_FUNCTION_(       v128_t, wav_b64x2_t, wav_b64x2_as_v128)

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_as_i8x16(v) \
    (_Generic( \
      wav_i16x8_t: wav_i16x8_as_i8x16, \
      wav_i32x4_t: wav_i32x4_as_i8x16, \
      wav_i64x2_t: wav_i64x2_as_i8x16, \
      wav_u8x16_t: wav_u8x16_as_i8x16, \
      wav_u16x8_t: wav_u16x8_as_i8x16, \
      wav_u32x4_t: wav_u32x4_as_i8x16, \
      wav_u64x2_t: wav_u64x2_as_i8x16, \
      wav_f32x4_t: wav_f32x4_as_i8x16, \
      wav_f64x2_t: wav_f64x2_as_i8x16, \
      wav_b8x16_t: wav_b8x16_as_i8x16, \
      wav_b16x8_t: wav_b16x8_as_i8x16, \
      wav_b32x4_t: wav_b32x4_as_i8x16, \
      wav_b64x2_t: wav_b64x2_as_i8x16, \
             v128_t: wav_v128_as_i8x16) (v))
  #define wav_as_i16x8(v) \
    (_Generic( \
      wav_i8x16_t: wav_i8x16_as_i16x8, \
      wav_i32x4_t: wav_i32x4_as_i16x8, \
      wav_i64x2_t: wav_i64x2_as_i16x8, \
      wav_u8x16_t: wav_u8x16_as_i16x8, \
      wav_u16x8_t: wav_u16x8_as_i16x8, \
      wav_u32x4_t: wav_u32x4_as_i16x8, \
      wav_u64x2_t: wav_u64x2_as_i16x8, \
      wav_f32x4_t: wav_f32x4_as_i16x8, \
      wav_f64x2_t: wav_f64x2_as_i16x8, \
      wav_b8x16_t: wav_b8x16_as_i16x8, \
      wav_b16x8_t: wav_b16x8_as_i16x8, \
      wav_b32x4_t: wav_b32x4_as_i16x8, \
      wav_b64x2_t: wav_b64x2_as_i16x8, \
             v128_t: wav_v128_as_i16x8) (v))
  #define wav_as_i32x4(v) \
    (_Generic( \
      wav_i8x16_t: wav_i8x16_as_i32x4, \
      wav_i16x8_t: wav_i16x8_as_i32x4, \
      wav_i64x2_t: wav_i64x2_as_i32x4, \
      wav_u8x16_t: wav_u8x16_as_i32x4, \
      wav_u16x8_t: wav_u16x8_as_i32x4, \
      wav_u32x4_t: wav_u32x4_as_i32x4, \
      wav_u64x2_t: wav_u64x2_as_i32x4, \
      wav_f32x4_t: wav_f32x4_as_i32x4, \
      wav_f64x2_t: wav_f64x2_as_i32x4, \
      wav_b8x16_t: wav_b8x16_as_i32x4, \
      wav_b16x8_t: wav_b16x8_as_i32x4, \
      wav_b32x4_t: wav_b32x4_as_i32x4, \
      wav_b64x2_t: wav_b64x2_as_i32x4, \
             v128_t: wav_v128_as_i32x4) (v))
  #define wav_as_i64x2(v) \
    (_Generic( \
      wav_i8x16_t: wav_i8x16_as_i64x2, \
      wav_i16x8_t: wav_i16x8_as_i64x2, \
      wav_i32x4_t: wav_i32x4_as_i64x2, \
      wav_u8x16_t: wav_u8x16_as_i64x2, \
      wav_u16x8_t: wav_u16x8_as_i64x2, \
      wav_u32x4_t: wav_u32x4_as_i64x2, \
      wav_u64x2_t: wav_u64x2_as_i64x2, \
      wav_f32x4_t: wav_f32x4_as_i64x2, \
      wav_f64x2_t: wav_f64x2_as_i64x2, \
      wav_b8x16_t: wav_b8x16_as_i64x2, \
      wav_b16x8_t: wav_b16x8_as_i64x2, \
      wav_b32x4_t: wav_b32x4_as_i64x2, \
      wav_b64x2_t: wav_b64x2_as_i64x2, \
             v128_t: wav_v128_as_i64x2) (v))
  #define wav_as_u8x16(v) \
    (_Generic( \
      wav_i8x16_t: wav_i8x16_as_u8x16, \
      wav_i16x8_t: wav_i16x8_as_u8x16, \
      wav_i32x4_t: wav_i32x4_as_u8x16, \
      wav_i64x2_t: wav_i64x2_as_u8x16, \
      wav_u16x8_t: wav_u16x8_as_u8x16, \
      wav_u32x4_t: wav_u32x4_as_u8x16, \
      wav_u64x2_t: wav_u64x2_as_u8x16, \
      wav_f32x4_t: wav_f32x4_as_u8x16, \
      wav_f64x2_t: wav_f64x2_as_u8x16, \
      wav_b8x16_t: wav_b8x16_as_u8x16, \
      wav_b16x8_t: wav_b16x8_as_u8x16, \
      wav_b32x4_t: wav_b32x4_as_u8x16, \
      wav_b64x2_t: wav_b64x2_as_u8x16, \
             v128_t: wav_v128_as_u8x16) (v))
  #define wav_as_u16x8(v) \
    (_Generic( \
      wav_i8x16_t: wav_i8x16_as_u16x8, \
      wav_i16x8_t: wav_i16x8_as_u16x8, \
      wav_i32x4_t: wav_i32x4_as_u16x8, \
      wav_i64x2_t: wav_i64x2_as_u16x8, \
      wav_u8x16_t: wav_u8x16_as_u16x8, \
      wav_u32x4_t: wav_u32x4_as_u16x8, \
      wav_u64x2_t: wav_u64x2_as_u16x8, \
      wav_f32x4_t: wav_f32x4_as_u16x8, \
      wav_f64x2_t: wav_f64x2_as_u16x8, \
      wav_b8x16_t: wav_b8x16_as_u16x8, \
      wav_b16x8_t: wav_b16x8_as_u16x8, \
      wav_b32x4_t: wav_b32x4_as_u16x8, \
      wav_b64x2_t: wav_b64x2_as_u16x8, \
             v128_t: wav_v128_as_u16x8) (v))
  #define wav_as_u32x4(v) \
    (_Generic( \
      wav_i8x16_t: wav_i8x16_as_u32x4, \
      wav_i16x8_t: wav_i16x8_as_u32x4, \
      wav_i32x4_t: wav_i32x4_as_u32x4, \
      wav_i64x2_t: wav_i64x2_as_u32x4, \
      wav_u8x16_t: wav_u8x16_as_u32x4, \
      wav_u16x8_t: wav_u16x8_as_u32x4, \
      wav_u64x2_t: wav_u64x2_as_u32x4, \
      wav_f32x4_t: wav_f32x4_as_u32x4, \
      wav_f64x2_t: wav_f64x2_as_u32x4, \
      wav_b8x16_t: wav_b8x16_as_u32x4, \
      wav_b16x8_t: wav_b16x8_as_u32x4, \
      wav_b32x4_t: wav_b32x4_as_u32x4, \
      wav_b64x2_t: wav_b64x2_as_u32x4, \
             v128_t: wav_v128_as_u32x4) (v))
  #define wav_as_u64x2(v) \
    (_Generic( \
      wav_i8x16_t: wav_i8x16_as_u64x2, \
      wav_i16x8_t: wav_i16x8_as_u64x2, \
      wav_i32x4_t: wav_i32x4_as_u64x2, \
      wav_i64x2_t: wav_i64x2_as_u64x2, \
      wav_u8x16_t: wav_u8x16_as_u64x2, \
      wav_u16x8_t: wav_u16x8_as_u64x2, \
      wav_u32x4_t: wav_u32x4_as_u64x2, \
      wav_f32x4_t: wav_f32x4_as_u64x2, \
      wav_f64x2_t: wav_f64x2_as_u64x2, \
      wav_b8x16_t: wav_b8x16_as_u64x2, \
      wav_b16x8_t: wav_b16x8_as_u64x2, \
      wav_b32x4_t: wav_b32x4_as_u64x2, \
      wav_b64x2_t: wav_b64x2_as_u64x2, \
             v128_t: wav_v128_as_u64x2) (v))
  #define wav_as_f32x4(v) \
    (_Generic( \
      wav_i8x16_t: wav_i8x16_as_f32x4, \
      wav_i16x8_t: wav_i16x8_as_f32x4, \
      wav_i32x4_t: wav_i32x4_as_f32x4, \
      wav_i64x2_t: wav_i64x2_as_f32x4, \
      wav_u8x16_t: wav_u8x16_as_f32x4, \
      wav_u16x8_t: wav_u16x8_as_f32x4, \
      wav_u32x4_t: wav_u32x4_as_f32x4, \
      wav_u64x2_t: wav_u64x2_as_f32x4, \
      wav_f64x2_t: wav_f64x2_as_f32x4, \
      wav_b8x16_t: wav_b8x16_as_f32x4, \
      wav_b16x8_t: wav_b16x8_as_f32x4, \
      wav_b32x4_t: wav_b32x4_as_f32x4, \
      wav_b64x2_t: wav_b64x2_as_f32x4, \
             v128_t: wav_v128_as_f32x4) (v))
  #define wav_as_f64x2(v) \
    (_Generic( \
      wav_i8x16_t: wav_i8x16_as_f64x2, \
      wav_i16x8_t: wav_i16x8_as_f64x2, \
      wav_i32x4_t: wav_i32x4_as_f64x2, \
      wav_i64x2_t: wav_i64x2_as_f64x2, \
      wav_u8x16_t: wav_u8x16_as_f64x2, \
      wav_u16x8_t: wav_u16x8_as_f64x2, \
      wav_u32x4_t: wav_u32x4_as_f64x2, \
      wav_u64x2_t: wav_u64x2_as_f64x2, \
      wav_f32x4_t: wav_f32x4_as_f64x2, \
      wav_b8x16_t: wav_b8x16_as_f64x2, \
      wav_b16x8_t: wav_b16x8_as_f64x2, \
      wav_b32x4_t: wav_b32x4_as_f64x2, \
      wav_b64x2_t: wav_b64x2_as_f64x2, \
             v128_t: wav_v128_as_f64x2) (v))
  #define wav_as_b8x16(v) \
    (_Generic( \
      wav_i8x16_t: wav_i8x16_as_b8x16, \
      wav_i16x8_t: wav_i16x8_as_b8x16, \
      wav_i32x4_t: wav_i32x4_as_b8x16, \
      wav_i64x2_t: wav_i64x2_as_b8x16, \
      wav_u8x16_t: wav_u8x16_as_b8x16, \
      wav_u16x8_t: wav_u16x8_as_b8x16, \
      wav_u32x4_t: wav_u32x4_as_b8x16, \
      wav_u64x2_t: wav_u64x2_as_b8x16, \
      wav_f32x4_t: wav_f32x4_as_b8x16, \
      wav_f64x2_t: wav_f64x2_as_b8x16, \
      wav_b16x8_t: wav_b16x8_as_b8x16, \
      wav_b32x4_t: wav_b32x4_as_b8x16, \
      wav_b64x2_t: wav_b64x2_as_b8x16, \
             v128_t: wav_v128_as_b8x16) (v))
  #define wav_as_b16x8(v) \
    (_Generic( \
      wav_i8x16_t: wav_i8x16_as_b16x8, \
      wav_i16x8_t: wav_i16x8_as_b16x8, \
      wav_i32x4_t: wav_i32x4_as_b16x8, \
      wav_i64x2_t: wav_i64x2_as_b16x8, \
      wav_u8x16_t: wav_u8x16_as_b16x8, \
      wav_u16x8_t: wav_u16x8_as_b16x8, \
      wav_u32x4_t: wav_u32x4_as_b16x8, \
      wav_u64x2_t: wav_u64x2_as_b16x8, \
      wav_f32x4_t: wav_f32x4_as_b16x8, \
      wav_f64x2_t: wav_f64x2_as_b16x8, \
      wav_b8x16_t: wav_b8x16_as_b16x8, \
      wav_b32x4_t: wav_b32x4_as_b16x8, \
      wav_b64x2_t: wav_b64x2_as_b16x8, \
             v128_t: wav_v128_as_b16x8) (v))
  #define wav_as_b32x4(v) \
    (_Generic( \
      wav_i8x16_t: wav_i8x16_as_b32x4, \
      wav_i16x8_t: wav_i16x8_as_b32x4, \
      wav_i32x4_t: wav_i32x4_as_b32x4, \
      wav_i64x2_t: wav_i64x2_as_b32x4, \
      wav_u8x16_t: wav_u8x16_as_b32x4, \
      wav_u16x8_t: wav_u16x8_as_b32x4, \
      wav_u32x4_t: wav_u32x4_as_b32x4, \
      wav_u64x2_t: wav_u64x2_as_b32x4, \
      wav_f32x4_t: wav_f32x4_as_b32x4, \
      wav_f64x2_t: wav_f64x2_as_b32x4, \
      wav_b8x16_t: wav_b8x16_as_b32x4, \
      wav_b16x8_t: wav_b16x8_as_b32x4, \
      wav_b64x2_t: wav_b64x2_as_b32x4, \
             v128_t: wav_v128_as_b32x4) (v))
  #define wav_as_b64x2(v) \
    (_Generic( \
      wav_i8x16_t: wav_i8x16_as_b64x2, \
      wav_i16x8_t: wav_i16x8_as_b64x2, \
      wav_i32x4_t: wav_i32x4_as_b64x2, \
      wav_i64x2_t: wav_i64x2_as_b64x2, \
      wav_u8x16_t: wav_u8x16_as_b64x2, \
      wav_u16x8_t: wav_u16x8_as_b64x2, \
      wav_u32x4_t: wav_u32x4_as_b64x2, \
      wav_u64x2_t: wav_u64x2_as_b64x2, \
      wav_f32x4_t: wav_f32x4_as_b64x2, \
      wav_f64x2_t: wav_f64x2_as_b64x2, \
      wav_b8x16_t: wav_b8x16_as_b64x2, \
      wav_b16x8_t: wav_b16x8_as_b64x2, \
      wav_b32x4_t: wav_b32x4_as_b64x2, \
             v128_t: wav_v128_as_b64x2) (v))
  #define wav_as_v128(v) \
    (_Generic( \
      wav_i8x16_t: wav_i8x16_as_v128, \
      wav_i16x8_t: wav_i16x8_as_v128, \
      wav_i32x4_t: wav_i32x4_as_v128, \
      wav_i64x2_t: wav_i64x2_as_v128, \
      wav_u8x16_t: wav_u8x16_as_v128, \
      wav_u16x8_t: wav_u16x8_as_v128, \
      wav_u32x4_t: wav_u32x4_as_v128, \
      wav_u64x2_t: wav_u64x2_as_v128, \
      wav_f32x4_t: wav_f32x4_as_v128, \
      wav_f64x2_t: wav_f64x2_as_v128, \
      wav_b8x16_t: wav_b8x16_as_v128, \
      wav_b16x8_t: wav_b16x8_as_v128, \
      wav_b32x4_t: wav_b32x4_as_v128, \
      wav_b64x2_t: wav_b64x2_as_v128) (v))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_as_i8x16(wav_i16x8_t value) { return wav_i16x8_as_i8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_as_i8x16(wav_i32x4_t value) { return wav_i32x4_as_i8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_as_i8x16(wav_i64x2_t value) { return wav_i64x2_as_i8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_as_i8x16(wav_u8x16_t value) { return wav_u8x16_as_i8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_as_i8x16(wav_u16x8_t value) { return wav_u16x8_as_i8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_as_i8x16(wav_u32x4_t value) { return wav_u32x4_as_i8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_as_i8x16(wav_u64x2_t value) { return wav_u64x2_as_i8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_as_i8x16(wav_f32x4_t value) { return wav_f32x4_as_i8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_as_i8x16(wav_f64x2_t value) { return wav_f64x2_as_i8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_as_i8x16(wav_b8x16_t value) { return wav_b8x16_as_i8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_as_i8x16(wav_b16x8_t value) { return wav_b16x8_as_i8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_as_i8x16(wav_b32x4_t value) { return wav_b32x4_as_i8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_as_i8x16(wav_b64x2_t value) { return wav_b64x2_as_i8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_as_i8x16(       v128_t value) { return wav_v128_as_i8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_as_i16x8(wav_i8x16_t value) { return wav_i8x16_as_i16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_as_i16x8(wav_i32x4_t value) { return wav_i32x4_as_i16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_as_i16x8(wav_i64x2_t value) { return wav_i64x2_as_i16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_as_i16x8(wav_u8x16_t value) { return wav_u8x16_as_i16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_as_i16x8(wav_u16x8_t value) { return wav_u16x8_as_i16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_as_i16x8(wav_u32x4_t value) { return wav_u32x4_as_i16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_as_i16x8(wav_u64x2_t value) { return wav_u64x2_as_i16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_as_i16x8(wav_f32x4_t value) { return wav_f32x4_as_i16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_as_i16x8(wav_f64x2_t value) { return wav_f64x2_as_i16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_as_i16x8(wav_b8x16_t value) { return wav_b8x16_as_i16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_as_i16x8(wav_b16x8_t value) { return wav_b16x8_as_i16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_as_i16x8(wav_b32x4_t value) { return wav_b32x4_as_i16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_as_i16x8(wav_b64x2_t value) { return wav_b64x2_as_i16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_as_i16x8(       v128_t value) { return wav_v128_as_i16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_as_i32x4(wav_i8x16_t value) { return wav_i8x16_as_i32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_as_i32x4(wav_i16x8_t value) { return wav_i16x8_as_i32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_as_i32x4(wav_i64x2_t value) { return wav_i64x2_as_i32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_as_i32x4(wav_u8x16_t value) { return wav_u8x16_as_i32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_as_i32x4(wav_u16x8_t value) { return wav_u16x8_as_i32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_as_i32x4(wav_u32x4_t value) { return wav_u32x4_as_i32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_as_i32x4(wav_u64x2_t value) { return wav_u64x2_as_i32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_as_i32x4(wav_f32x4_t value) { return wav_f32x4_as_i32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_as_i32x4(wav_f64x2_t value) { return wav_f64x2_as_i32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_as_i32x4(wav_b8x16_t value) { return wav_b8x16_as_i32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_as_i32x4(wav_b16x8_t value) { return wav_b16x8_as_i32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_as_i32x4(wav_b32x4_t value) { return wav_b32x4_as_i32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_as_i32x4(wav_b64x2_t value) { return wav_b64x2_as_i32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_as_i32x4(       v128_t value) { return wav_v128_as_i32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_as_i64x2(wav_i8x16_t value) { return wav_i8x16_as_i64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_as_i64x2(wav_i16x8_t value) { return wav_i16x8_as_i64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_as_i64x2(wav_i32x4_t value) { return wav_i32x4_as_i64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_as_i64x2(wav_u8x16_t value) { return wav_u8x16_as_i64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_as_i64x2(wav_u16x8_t value) { return wav_u16x8_as_i64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_as_i64x2(wav_u32x4_t value) { return wav_u32x4_as_i64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_as_i64x2(wav_u64x2_t value) { return wav_u64x2_as_i64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_as_i64x2(wav_f32x4_t value) { return wav_f32x4_as_i64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_as_i64x2(wav_f64x2_t value) { return wav_f64x2_as_i64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_as_i64x2(wav_b8x16_t value) { return wav_b8x16_as_i64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_as_i64x2(wav_b16x8_t value) { return wav_b16x8_as_i64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_as_i64x2(wav_b32x4_t value) { return wav_b32x4_as_i64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_as_i64x2(wav_b64x2_t value) { return wav_b64x2_as_i64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_as_i64x2(       v128_t value) { return wav_v128_as_i64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_as_u8x16(wav_i8x16_t value) { return wav_i8x16_as_u8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_as_u8x16(wav_i16x8_t value) { return wav_i16x8_as_u8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_as_u8x16(wav_i32x4_t value) { return wav_i32x4_as_u8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_as_u8x16(wav_i64x2_t value) { return wav_i64x2_as_u8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_as_u8x16(wav_u16x8_t value) { return wav_u16x8_as_u8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_as_u8x16(wav_u32x4_t value) { return wav_u32x4_as_u8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_as_u8x16(wav_u64x2_t value) { return wav_u64x2_as_u8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_as_u8x16(wav_f32x4_t value) { return wav_f32x4_as_u8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_as_u8x16(wav_f64x2_t value) { return wav_f64x2_as_u8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_as_u8x16(wav_b8x16_t value) { return wav_b8x16_as_u8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_as_u8x16(wav_b16x8_t value) { return wav_b16x8_as_u8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_as_u8x16(wav_b32x4_t value) { return wav_b32x4_as_u8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_as_u8x16(wav_b64x2_t value) { return wav_b64x2_as_u8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_as_u8x16(       v128_t value) { return wav_v128_as_u8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_as_u16x8(wav_i8x16_t value) { return wav_i8x16_as_u16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_as_u16x8(wav_i16x8_t value) { return wav_i16x8_as_u16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_as_u16x8(wav_i32x4_t value) { return wav_i32x4_as_u16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_as_u16x8(wav_i64x2_t value) { return wav_i64x2_as_u16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_as_u16x8(wav_u8x16_t value) { return wav_u8x16_as_u16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_as_u16x8(wav_u32x4_t value) { return wav_u32x4_as_u16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_as_u16x8(wav_u64x2_t value) { return wav_u64x2_as_u16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_as_u16x8(wav_f32x4_t value) { return wav_f32x4_as_u16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_as_u16x8(wav_f64x2_t value) { return wav_f64x2_as_u16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_as_u16x8(wav_b8x16_t value) { return wav_b8x16_as_u16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_as_u16x8(wav_b16x8_t value) { return wav_b16x8_as_u16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_as_u16x8(wav_b32x4_t value) { return wav_b32x4_as_u16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_as_u16x8(wav_b64x2_t value) { return wav_b64x2_as_u16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_as_u16x8(       v128_t value) { return wav_v128_as_u16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_as_u32x4(wav_i8x16_t value) { return wav_i8x16_as_u32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_as_u32x4(wav_i16x8_t value) { return wav_i16x8_as_u32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_as_u32x4(wav_i32x4_t value) { return wav_i32x4_as_u32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_as_u32x4(wav_i64x2_t value) { return wav_i64x2_as_u32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_as_u32x4(wav_u8x16_t value) { return wav_u8x16_as_u32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_as_u32x4(wav_u16x8_t value) { return wav_u16x8_as_u32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_as_u32x4(wav_u64x2_t value) { return wav_u64x2_as_u32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_as_u32x4(wav_f32x4_t value) { return wav_f32x4_as_u32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_as_u32x4(wav_f64x2_t value) { return wav_f64x2_as_u32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_as_u32x4(wav_b8x16_t value) { return wav_b8x16_as_u32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_as_u32x4(wav_b16x8_t value) { return wav_b16x8_as_u32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_as_u32x4(wav_b32x4_t value) { return wav_b32x4_as_u32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_as_u32x4(wav_b64x2_t value) { return wav_b64x2_as_u32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_as_u32x4(       v128_t value) { return wav_v128_as_u32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_as_u64x2(wav_i8x16_t value) { return wav_i8x16_as_u64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_as_u64x2(wav_i16x8_t value) { return wav_i16x8_as_u64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_as_u64x2(wav_i32x4_t value) { return wav_i32x4_as_u64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_as_u64x2(wav_i64x2_t value) { return wav_i64x2_as_u64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_as_u64x2(wav_u8x16_t value) { return wav_u8x16_as_u64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_as_u64x2(wav_u16x8_t value) { return wav_u16x8_as_u64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_as_u64x2(wav_u32x4_t value) { return wav_u32x4_as_u64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_as_u64x2(wav_f32x4_t value) { return wav_f32x4_as_u64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_as_u64x2(wav_f64x2_t value) { return wav_f64x2_as_u64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_as_u64x2(wav_b8x16_t value) { return wav_b8x16_as_u64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_as_u64x2(wav_b16x8_t value) { return wav_b16x8_as_u64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_as_u64x2(wav_b32x4_t value) { return wav_b32x4_as_u64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_as_u64x2(wav_b64x2_t value) { return wav_b64x2_as_u64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_as_u64x2(       v128_t value) { return wav_v128_as_u64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_as_f32x4(wav_i8x16_t value) { return wav_i8x16_as_f32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_as_f32x4(wav_i16x8_t value) { return wav_i16x8_as_f32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_as_f32x4(wav_i32x4_t value) { return wav_i32x4_as_f32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_as_f32x4(wav_i64x2_t value) { return wav_i64x2_as_f32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_as_f32x4(wav_u8x16_t value) { return wav_u8x16_as_f32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_as_f32x4(wav_u16x8_t value) { return wav_u16x8_as_f32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_as_f32x4(wav_u32x4_t value) { return wav_u32x4_as_f32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_as_f32x4(wav_u64x2_t value) { return wav_u64x2_as_f32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_as_f32x4(wav_f64x2_t value) { return wav_f64x2_as_f32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_as_f32x4(wav_b8x16_t value) { return wav_b8x16_as_f32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_as_f32x4(wav_b16x8_t value) { return wav_b16x8_as_f32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_as_f32x4(wav_b32x4_t value) { return wav_b32x4_as_f32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_as_f32x4(wav_b64x2_t value) { return wav_b64x2_as_f32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_as_f32x4(       v128_t value) { return wav_v128_as_f32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_as_f64x2(wav_i8x16_t value) { return wav_i8x16_as_f64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_as_f64x2(wav_i16x8_t value) { return wav_i16x8_as_f64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_as_f64x2(wav_i32x4_t value) { return wav_i32x4_as_f64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_as_f64x2(wav_i64x2_t value) { return wav_i64x2_as_f64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_as_f64x2(wav_u8x16_t value) { return wav_u8x16_as_f64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_as_f64x2(wav_u16x8_t value) { return wav_u16x8_as_f64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_as_f64x2(wav_u32x4_t value) { return wav_u32x4_as_f64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_as_f64x2(wav_u64x2_t value) { return wav_u64x2_as_f64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_as_f64x2(wav_f32x4_t value) { return wav_f32x4_as_f64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_as_f64x2(wav_b8x16_t value) { return wav_b8x16_as_f64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_as_f64x2(wav_b16x8_t value) { return wav_b16x8_as_f64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_as_f64x2(wav_b32x4_t value) { return wav_b32x4_as_f64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_as_f64x2(wav_b64x2_t value) { return wav_b64x2_as_f64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_as_f64x2(       v128_t value) { return wav_v128_as_f64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_as_b8x16(wav_i8x16_t value) { return wav_i8x16_as_b8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_as_b8x16(wav_i16x8_t value) { return wav_i16x8_as_b8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_as_b8x16(wav_i32x4_t value) { return wav_i32x4_as_b8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_as_b8x16(wav_i64x2_t value) { return wav_i64x2_as_b8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_as_b8x16(wav_u8x16_t value) { return wav_u8x16_as_b8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_as_b8x16(wav_u16x8_t value) { return wav_u16x8_as_b8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_as_b8x16(wav_u32x4_t value) { return wav_u32x4_as_b8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_as_b8x16(wav_u64x2_t value) { return wav_u64x2_as_b8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_as_b8x16(wav_f32x4_t value) { return wav_f32x4_as_b8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_as_b8x16(wav_f64x2_t value) { return wav_f64x2_as_b8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_as_b8x16(wav_b16x8_t value) { return wav_b16x8_as_b8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_as_b8x16(wav_b32x4_t value) { return wav_b32x4_as_b8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_as_b8x16(wav_b64x2_t value) { return wav_b64x2_as_b8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_as_b8x16(       v128_t value) { return wav_v128_as_b8x16(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_as_b16x8(wav_i8x16_t value) { return wav_i8x16_as_b16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_as_b16x8(wav_i16x8_t value) { return wav_i16x8_as_b16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_as_b16x8(wav_i32x4_t value) { return wav_i32x4_as_b16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_as_b16x8(wav_i64x2_t value) { return wav_i64x2_as_b16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_as_b16x8(wav_u8x16_t value) { return wav_u8x16_as_b16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_as_b16x8(wav_u16x8_t value) { return wav_u16x8_as_b16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_as_b16x8(wav_u32x4_t value) { return wav_u32x4_as_b16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_as_b16x8(wav_u64x2_t value) { return wav_u64x2_as_b16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_as_b16x8(wav_f32x4_t value) { return wav_f32x4_as_b16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_as_b16x8(wav_f64x2_t value) { return wav_f64x2_as_b16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_as_b16x8(wav_b8x16_t value) { return wav_b8x16_as_b16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_as_b16x8(wav_b32x4_t value) { return wav_b32x4_as_b16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_as_b16x8(wav_b64x2_t value) { return wav_b64x2_as_b16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_as_b16x8(       v128_t value) { return wav_v128_as_b16x8(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_as_b32x4(wav_i8x16_t value) { return wav_i8x16_as_b32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_as_b32x4(wav_i16x8_t value) { return wav_i16x8_as_b32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_as_b32x4(wav_i32x4_t value) { return wav_i32x4_as_b32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_as_b32x4(wav_i64x2_t value) { return wav_i64x2_as_b32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_as_b32x4(wav_u8x16_t value) { return wav_u8x16_as_b32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_as_b32x4(wav_u16x8_t value) { return wav_u16x8_as_b32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_as_b32x4(wav_u32x4_t value) { return wav_u32x4_as_b32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_as_b32x4(wav_u64x2_t value) { return wav_u64x2_as_b32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_as_b32x4(wav_f32x4_t value) { return wav_f32x4_as_b32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_as_b32x4(wav_f64x2_t value) { return wav_f64x2_as_b32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_as_b32x4(wav_b8x16_t value) { return wav_b8x16_as_b32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_as_b32x4(wav_b16x8_t value) { return wav_b16x8_as_b32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_as_b32x4(wav_b64x2_t value) { return wav_b64x2_as_b32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_as_b32x4(       v128_t value) { return wav_v128_as_b32x4(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_as_b64x2(wav_i8x16_t value) { return wav_i8x16_as_b64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_as_b64x2(wav_i16x8_t value) { return wav_i16x8_as_b64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_as_b64x2(wav_i32x4_t value) { return wav_i32x4_as_b64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_as_b64x2(wav_i64x2_t value) { return wav_i64x2_as_b64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_as_b64x2(wav_u8x16_t value) { return wav_u8x16_as_b64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_as_b64x2(wav_u16x8_t value) { return wav_u16x8_as_b64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_as_b64x2(wav_u32x4_t value) { return wav_u32x4_as_b64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_as_b64x2(wav_u64x2_t value) { return wav_u64x2_as_b64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_as_b64x2(wav_f32x4_t value) { return wav_f32x4_as_b64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_as_b64x2(wav_f64x2_t value) { return wav_f64x2_as_b64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_as_b64x2(wav_b8x16_t value) { return wav_b8x16_as_b64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_as_b64x2(wav_b16x8_t value) { return wav_b16x8_as_b64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_as_b64x2(wav_b32x4_t value) { return wav_b32x4_as_b64x2(value); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_as_b64x2(       v128_t value) { return wav_v128_as_b64x2(value); }
  WAV_FUNCTION_ATTRIBUTES        v128_t  wav_as_v128(wav_i8x16_t value) { return wav_i8x16_as_v128(value); }
  WAV_FUNCTION_ATTRIBUTES        v128_t  wav_as_v128(wav_i16x8_t value) { return wav_i16x8_as_v128(value); }
  WAV_FUNCTION_ATTRIBUTES        v128_t  wav_as_v128(wav_i32x4_t value) { return wav_i32x4_as_v128(value); }
  WAV_FUNCTION_ATTRIBUTES        v128_t  wav_as_v128(wav_i64x2_t value) { return wav_i64x2_as_v128(value); }
  WAV_FUNCTION_ATTRIBUTES        v128_t  wav_as_v128(wav_u8x16_t value) { return wav_u8x16_as_v128(value); }
  WAV_FUNCTION_ATTRIBUTES        v128_t  wav_as_v128(wav_u16x8_t value) { return wav_u16x8_as_v128(value); }
  WAV_FUNCTION_ATTRIBUTES        v128_t  wav_as_v128(wav_u32x4_t value) { return wav_u32x4_as_v128(value); }
  WAV_FUNCTION_ATTRIBUTES        v128_t  wav_as_v128(wav_u64x2_t value) { return wav_u64x2_as_v128(value); }
  WAV_FUNCTION_ATTRIBUTES        v128_t  wav_as_v128(wav_f32x4_t value) { return wav_f32x4_as_v128(value); }
  WAV_FUNCTION_ATTRIBUTES        v128_t  wav_as_v128(wav_f64x2_t value) { return wav_f64x2_as_v128(value); }
  WAV_FUNCTION_ATTRIBUTES        v128_t  wav_as_v128(wav_b8x16_t value) { return wav_b8x16_as_v128(value); }
  WAV_FUNCTION_ATTRIBUTES        v128_t  wav_as_v128(wav_b16x8_t value) { return wav_b16x8_as_v128(value); }
  WAV_FUNCTION_ATTRIBUTES        v128_t  wav_as_v128(wav_b32x4_t value) { return wav_b32x4_as_v128(value); }
  WAV_FUNCTION_ATTRIBUTES        v128_t  wav_as_v128(wav_b64x2_t value) { return wav_b64x2_as_v128(value); }
#endif

/* load
 *
 * Load data.
 *
 * WASM can load and store unaligned data, so these are really only
 * here to provide a type safe API. */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_load(const int8_t src[WAV_ARRAY_PARAM(16)]) {
  wav_i8x16_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_load(const int16_t src[WAV_ARRAY_PARAM(8)]) {
  wav_i16x8_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_load(const int32_t src[WAV_ARRAY_PARAM(4)]) {
  wav_i32x4_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_load(const int64_t src[WAV_ARRAY_PARAM(2)]) {
  wav_i64x2_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_load(const uint8_t src[WAV_ARRAY_PARAM(16)]) {
  wav_u8x16_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_load(const uint16_t src[WAV_ARRAY_PARAM(8)]) {
  wav_u16x8_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_load(const uint32_t src[WAV_ARRAY_PARAM(4)]) {
  wav_u32x4_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_load(const uint64_t src[WAV_ARRAY_PARAM(2)]) {
  wav_u64x2_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_load(const float src[WAV_ARRAY_PARAM(4)]) {
  wav_f32x4_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_load(const double src[WAV_ARRAY_PARAM(2)]) {
  wav_f64x2_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_load(src) \
    (_Generic((src), \
        int8_t*: wav_i8x16_load, \
       int16_t*: wav_i16x8_load, \
       int32_t*: wav_i32x4_load, \
       int64_t*: wav_i64x2_load, \
       uint8_t*: wav_u8x16_load, \
      uint16_t*: wav_u16x8_load, \
      uint32_t*: wav_u32x4_load, \
      uint64_t*: wav_u64x2_load, \
         float*: wav_f32x4_load, \
        double*: wav_f64x2_load) (src))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_load(const   int8_t src[WAV_ARRAY_PARAM(16)]) { return wav_i8x16_load(src); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_load(const  int16_t src[WAV_ARRAY_PARAM( 8)]) { return wav_i16x8_load(src); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_load(const  int32_t src[WAV_ARRAY_PARAM( 4)]) { return wav_i32x4_load(src); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_load(const  int64_t src[WAV_ARRAY_PARAM( 2)]) { return wav_i64x2_load(src); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_load(const  uint8_t src[WAV_ARRAY_PARAM(16)]) { return wav_u8x16_load(src); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_load(const uint16_t src[WAV_ARRAY_PARAM( 8)]) { return wav_u16x8_load(src); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_load(const uint32_t src[WAV_ARRAY_PARAM( 4)]) { return wav_u32x4_load(src); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_load(const uint64_t src[WAV_ARRAY_PARAM( 2)]) { return wav_u64x2_load(src); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_load(const    float src[WAV_ARRAY_PARAM( 4)]) { return wav_f32x4_load(src); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_load(const   double src[WAV_ARRAY_PARAM( 2)]) { return wav_f64x2_load(src); }
#endif

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

/* store
 *
 * Store data. */

WAV_FUNCTION_ATTRIBUTES
void
wav_i8x16_store(int8_t dest[WAV_ARRAY_PARAM(16)], wav_i8x16_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_i16x8_store(int16_t dest[WAV_ARRAY_PARAM(16)], wav_i16x8_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_i32x4_store(int32_t dest[WAV_ARRAY_PARAM(16)], wav_i32x4_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_i64x2_store(int64_t dest[WAV_ARRAY_PARAM(16)], wav_i64x2_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_u8x16_store(uint8_t dest[WAV_ARRAY_PARAM(16)], wav_u8x16_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_u16x8_store(uint16_t dest[WAV_ARRAY_PARAM(16)], wav_u16x8_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_u32x4_store(uint32_t dest[WAV_ARRAY_PARAM(16)], wav_u32x4_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_u64x2_store(uint64_t dest[WAV_ARRAY_PARAM(16)], wav_u64x2_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_f32x4_store(float dest[WAV_ARRAY_PARAM(16)], wav_f32x4_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WAV_FUNCTION_ATTRIBUTES
void
wav_f64x2_store(double dest[WAV_ARRAY_PARAM(16)], wav_f64x2_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_store(dest, src) \
    (_Generic((src), \
      wav_i8x16_t: wav_i8x16_store, \
      wav_i16x8_t: wav_i16x8_store, \
      wav_i32x4_t: wav_i32x4_store, \
      wav_i64x2_t: wav_i64x2_store, \
      wav_u8x16_t: wav_u8x16_store, \
      wav_u16x8_t: wav_u16x8_store, \
      wav_u32x4_t: wav_u32x4_store, \
      wav_u64x2_t: wav_u64x2_store, \
      wav_f32x4_t: wav_f32x4_store, \
      wav_f64x2_t: wav_f64x2_store) (dest, src))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES void wav_store(  int8_t dest[WAV_ARRAY_PARAM(16)], wav_i8x16_t src) { wav_i8x16_store(dest, src); }
  WAV_FUNCTION_ATTRIBUTES void wav_store( int16_t dest[WAV_ARRAY_PARAM( 8)], wav_i16x8_t src) { wav_i16x8_store(dest, src); }
  WAV_FUNCTION_ATTRIBUTES void wav_store( int32_t dest[WAV_ARRAY_PARAM( 4)], wav_i32x4_t src) { wav_i32x4_store(dest, src); }
  WAV_FUNCTION_ATTRIBUTES void wav_store( int64_t dest[WAV_ARRAY_PARAM( 2)], wav_i64x2_t src) { wav_i64x2_store(dest, src); }
  WAV_FUNCTION_ATTRIBUTES void wav_store( uint8_t dest[WAV_ARRAY_PARAM(16)], wav_u8x16_t src) { wav_u8x16_store(dest, src); }
  WAV_FUNCTION_ATTRIBUTES void wav_store(uint16_t dest[WAV_ARRAY_PARAM( 8)], wav_u16x8_t src) { wav_u16x8_store(dest, src); }
  WAV_FUNCTION_ATTRIBUTES void wav_store(uint32_t dest[WAV_ARRAY_PARAM( 4)], wav_u32x4_t src) { wav_u32x4_store(dest, src); }
  WAV_FUNCTION_ATTRIBUTES void wav_store(uint64_t dest[WAV_ARRAY_PARAM( 2)], wav_u64x2_t src) { wav_u64x2_store(dest, src); }
  WAV_FUNCTION_ATTRIBUTES void wav_store(   float dest[WAV_ARRAY_PARAM( 4)], wav_f32x4_t src) { wav_f32x4_store(dest, src); }
  WAV_FUNCTION_ATTRIBUTES void wav_store(  double dest[WAV_ARRAY_PARAM( 2)], wav_f64x2_t src) { wav_f64x2_store(dest, src); }
#endif

/* store
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

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_storeu(dest, src) \
    (_Generic((src), \
      wav_i8x16_t: wav_i8x16_storeu, \
      wav_i16x8_t: wav_i16x8_storeu, \
      wav_i32x4_t: wav_i32x4_storeu, \
      wav_i64x2_t: wav_i64x2_storeu, \
      wav_u8x16_t: wav_u8x16_storeu, \
      wav_u16x8_t: wav_u16x8_storeu, \
      wav_u32x4_t: wav_u32x4_storeu, \
      wav_u64x2_t: wav_u64x2_storeu, \
      wav_f32x4_t: wav_f32x4_storeu, \
      wav_f64x2_t: wav_f64x2_storeu) (dest, src))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES void wav_storeu(void * dest, wav_i8x16_t src) { wav_i8x16_storeu(dest, src); }
  WAV_FUNCTION_ATTRIBUTES void wav_storeu(void * dest, wav_i16x8_t src) { wav_i16x8_storeu(dest, src); }
  WAV_FUNCTION_ATTRIBUTES void wav_storeu(void * dest, wav_i32x4_t src) { wav_i32x4_storeu(dest, src); }
  WAV_FUNCTION_ATTRIBUTES void wav_storeu(void * dest, wav_i64x2_t src) { wav_i64x2_storeu(dest, src); }
  WAV_FUNCTION_ATTRIBUTES void wav_storeu(void * dest, wav_u8x16_t src) { wav_u8x16_storeu(dest, src); }
  WAV_FUNCTION_ATTRIBUTES void wav_storeu(void * dest, wav_u16x8_t src) { wav_u16x8_storeu(dest, src); }
  WAV_FUNCTION_ATTRIBUTES void wav_storeu(void * dest, wav_u32x4_t src) { wav_u32x4_storeu(dest, src); }
  WAV_FUNCTION_ATTRIBUTES void wav_storeu(void * dest, wav_u64x2_t src) { wav_u64x2_storeu(dest, src); }
  WAV_FUNCTION_ATTRIBUTES void wav_storeu(void * dest, wav_f32x4_t src) { wav_f32x4_storeu(dest, src); }
  WAV_FUNCTION_ATTRIBUTES void wav_storeu(void * dest, wav_f64x2_t src) { wav_f64x2_storeu(dest, src); }
#endif

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

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_splat(value) \
    (_Generic((value), \
        int8_t: wav_i8x16_splat, \
       int16_t: wav_i16x8_splat, \
       int32_t: wav_i32x4_splat, \
       int64_t: wav_i64x2_splat, \
       uint8_t: wav_u8x16_splat, \
      uint16_t: wav_u16x8_splat, \
      uint32_t: wav_u32x4_splat, \
      uint64_t: wav_u64x2_splat, \
         float: wav_f32x4_splat, \
        double: wav_f64x2_splat) (value))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_splat(  int8_t value) { return wav_i8x16_splat(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_splat( int16_t value) { return wav_i16x8_splat(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_splat( int32_t value) { return wav_i32x4_splat(value); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_splat( int64_t value) { return wav_i64x2_splat(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_splat( uint8_t value) { return wav_u8x16_splat(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_splat(uint16_t value) { return wav_u16x8_splat(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_splat(uint32_t value) { return wav_u32x4_splat(value); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_splat(uint64_t value) { return wav_u64x2_splat(value); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_splat(   float value) { return wav_f32x4_splat(value); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_splat(  double value) { return wav_f64x2_splat(value); }
#endif

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

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_make(c0, ...) \
    (_Generic((c0), \
        int8_t: wav_i8x16_make, \
       int16_t: wav_i16x8_make, \
       int32_t: wav_i32x4_make, \
       int64_t: wav_i64x2_make, \
       uint8_t: wav_u8x16_make, \
      uint16_t: wav_u16x8_make, \
      uint32_t: wav_u32x4_make, \
      uint64_t: wav_u64x2_make, \
         float: wav_f32x4_make, \
        double: wav_f64x2_make) (c0, __VA_ARGS__))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_make(
      int8_t c0, int8_t c1, int8_t  c2, int8_t  c3, int8_t  c4, int8_t  c5, int8_t  c6, int8_t  c7,
      int8_t c8, int8_t c9, int8_t c10, int8_t c11, int8_t c12, int8_t c13, int8_t c14, int8_t c15) {
    return wav_i8x16_make(
      c0, c1,  c2,  c3,  c4,  c5,  c6,  c7,
      c8, c9, c10, c11, c12, c13, c14, c15);
  }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_make(
      int16_t c0, int16_t c1, int16_t c2, int16_t c3, int16_t c4, int16_t c5, int16_t c6, int16_t c7) {
    return wav_i16x8_make(c0, c1, c2, c3, c4, c5, c6, c7);
  }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_make(int32_t c0, int32_t c1, int32_t c2, int32_t c3) {
    return wav_i32x4_make(c0, c1, c2, c3);
  }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_make(int64_t c0, int64_t c1) {
    return wav_i64x2_make(c0, c1);
  }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_make(
      uint8_t c0, uint8_t c1, uint8_t  c2, uint8_t  c3, uint8_t  c4, uint8_t  c5, uint8_t  c6, uint8_t  c7,
      uint8_t c8, uint8_t c9, uint8_t c10, uint8_t c11, uint8_t c12, uint8_t c13, uint8_t c14, uint8_t c15) {
    return wav_u8x16_make(
      c0, c1,  c2,  c3,  c4,  c5,  c6,  c7,
      c8, c9, c10, c11, c12, c13, c14, c15);
  }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_make(
      uint16_t c0, uint16_t c1, uint16_t c2, uint16_t c3, uint16_t c4, uint16_t c5, uint16_t c6, uint16_t c7) {
    return wav_u16x8_make(c0, c1, c2, c3, c4, c5, c6, c7);
  }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_make(uint32_t c0, uint32_t c1, uint32_t c2, uint32_t c3) {
    return wav_u32x4_make(c0, c1, c2, c3);
  }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_make(uint64_t c0, uint64_t c1) {
    return wav_u64x2_make(c0, c1);
  }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_make(float c0, float c1, float c2, float c3) {
    return wav_f32x4_make(c0, c1, c2, c3);
  }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_make(double c0, double c1) {
    return wav_f64x2_make(c0, c1);
  }
#endif

/* const
 *
 * Create a vector with all lanes are known at compile time.
 *
 * Slow; splat + 15 replace_lanes
 *
 * Anyone have any idea how to make a generic macro for this in C?
 */

#define \
  wav_i8x16_const( \
      c0, c1,  c2,  c3,  c4,  c5,  c6,  c7, \
      c8, c9, c10, c11, c12, c13, c14, c15) (__extension__ ({ \
    WAV_REQUIRE_CONSTANT(c0); \
    WAV_REQUIRE_CONSTANT(c1); \
    WAV_REQUIRE_CONSTANT(c2); \
    WAV_REQUIRE_CONSTANT(c3); \
    WAV_REQUIRE_CONSTANT(c4); \
    WAV_REQUIRE_CONSTANT(c5); \
    WAV_REQUIRE_CONSTANT(c6); \
    WAV_REQUIRE_CONSTANT(c7); \
    WAV_REQUIRE_CONSTANT(c8); \
    WAV_REQUIRE_CONSTANT(c9); \
    WAV_REQUIRE_CONSTANT(c10); \
    WAV_REQUIRE_CONSTANT(c11); \
    WAV_REQUIRE_CONSTANT(c12); \
    WAV_REQUIRE_CONSTANT(c13); \
    WAV_REQUIRE_CONSTANT(c14); \
    WAV_REQUIRE_CONSTANT(c15); \
    \
    (wav_i8x16_t) { \
      c0, c1,  c2,  c3,  c4,  c5,  c6,  c7, \
      c8, c9, c10, c11, c12, c13, c14, c15 }; \
  }))

#define \
  wav_i16x8_const(c0, c1,  c2,  c3,  c4,  c5,  c6,  c7) (__extension__ ({ \
    WAV_REQUIRE_CONSTANT(c0); \
    WAV_REQUIRE_CONSTANT(c1); \
    WAV_REQUIRE_CONSTANT(c2); \
    WAV_REQUIRE_CONSTANT(c3); \
    WAV_REQUIRE_CONSTANT(c4); \
    WAV_REQUIRE_CONSTANT(c5); \
    WAV_REQUIRE_CONSTANT(c6); \
    WAV_REQUIRE_CONSTANT(c7); \
    \
    (wav_i16x8_t) { c0, c1, c2, c3, c4, c5, c6, c7 }; \
  }))

#define \
  wav_i32x4_const(c0, c1,  c2,  c3) (__extension__ ({ \
    WAV_REQUIRE_CONSTANT(c0); \
    WAV_REQUIRE_CONSTANT(c1); \
    WAV_REQUIRE_CONSTANT(c2); \
    WAV_REQUIRE_CONSTANT(c3); \
    \
    (wav_i32x4_t) { c0, c1, c2, c3 }; \
  }))

/* extract_lane -- Extract lane as a scalar
 *
 * Extract the scalar value of lane specified in the immediate mode
 * operand lane in v.
 */

#define \
  wav_i8x16_extract_lane(v, lane) (__extension__ ({ \
    WAV_REQUIRE_CONSTANT_RANGE(lane, 0, 15); \
    WAV_REQUIRE_TYPE(wav_i8x16_t, v).values[lane]; \
  }))

#define \
  wav_i16x8_extract_lane(v, lane) (__extension__ ({ \
    WAV_REQUIRE_CONSTANT_RANGE(lane, 0, 7); \
    WAV_REQUIRE_TYPE(wav_i16x8_t, v).values[lane]; \
  }))

#define \
  wav_i32x4_extract_lane(v, lane) (__extension__ ({ \
    WAV_REQUIRE_CONSTANT_RANGE(lane, 0, 3); \
    WAV_REQUIRE_TYPE(wav_i32x4_t, v).values[lane]; \
  }))

#define \
  wav_i64x2_extract_lane(v, lane) (__extension__ ({ \
    WAV_REQUIRE_CONSTANT_RANGE(lane, 0, 1); \
    WAV_REQUIRE_TYPE(wav_i64x2_t, v).values[lane]; \
  }))

#define \
  wav_u8x16_extract_lane(v, lane) (__extension__ ({ \
    WAV_REQUIRE_CONSTANT_RANGE(lane, 0, 15); \
    WAV_REQUIRE_TYPE(wav_u8x16_t, v).values[lane]; \
  }))

#define \
  wav_u16x8_extract_lane(v, lane) (__extension__ ({ \
    WAV_REQUIRE_CONSTANT_RANGE(lane, 0, 7); \
    WAV_REQUIRE_TYPE(wav_u16x8_t, v).values[lane]; \
  }))

#define \
  wav_u32x4_extract_lane(v, lane) (__extension__ ({ \
    WAV_REQUIRE_CONSTANT_RANGE(lane, 0, 3); \
    WAV_REQUIRE_TYPE(wav_u32x4_t, v).values[lane]; \
  }))

#define \
  wav_u64x2_extract_lane(v, lane) (__extension__ ({ \
    WAV_REQUIRE_CONSTANT_RANGE(lane, 0, 1); \
    WAV_REQUIRE_TYPE(wav_u64x2_t, v).values[lane]; \
  }))

#define \
  wav_f32x4_extract_lane(v, lane) (__extension__ ({ \
    WAV_REQUIRE_CONSTANT_RANGE(lane, 0, 3); \
    WAV_REQUIRE_TYPE(wav_f32x4_t, v).values[lane]; \
  }))

#define \
  wav_f64x2_extract_lane(v, lane) (__extension__ ({ \
    WAV_REQUIRE_CONSTANT_RANGE(lane, 0, 1); \
    WAV_REQUIRE_TYPE(wav_f64x2_t, v).values[lane]; \
  }))

#if WAV_OVERLOADS != WAV_OVERLOADS_NONE
  #define wav_extract_lane(v, lane) \
    (__extension__ ({ \
      WAV_REQUIRE_CONSTANT_RANGE(lane, 0, (sizeof(v.values) / sizeof(v.values[0])) - 1); \
      v.values[lane]; \
    }))
#endif

/* replace_lane -- Replace lane value
 *
 * Return a new vector with lanes identical to v, except for the lane
 * specified in the immediate mode operand lane which has the value
 * value.
 */

#define \
  wav_i8x16_replace_lane(v, lane, value) (__extension__ ({ \
    WAV_REQUIRE_CONSTANT_RANGE(lane, 0, 15); \
    wav_i8x16_t v_ = v; \
    v_.values[lane] = value; \
    v_; \
  }))

#define \
  wav_i16x8_replace_lane(v, lane, value) (__extension__ ({ \
    WAV_REQUIRE_CONSTANT_RANGE(lane, 0, 15); \
    wav_i16x8_t v_ = v; \
    v_.values[lane] = value; \
    v_; \
  }))

#define \
  wav_i32x4_replace_lane(v, lane, value) (__extension__ ({ \
    WAV_REQUIRE_CONSTANT_RANGE(lane, 0, 15); \
    wav_i32x4_t v_ = v; \
    v_.values[lane] = value; \
    v_; \
  }))

#define \
  wav_i64x2_replace_lane(v, lane, value) (__extension__ ({ \
    WAV_REQUIRE_CONSTANT_RANGE(lane, 0, 15); \
    wav_i64x2_t v_ = v; \
    v_.values[lane] = value; \
    v_; \
  }))

#define \
  wav_u8x16_replace_lane(v, lane, value) (__extension__ ({ \
    WAV_REQUIRE_CONSTANT_RANGE(lane, 0, 15); \
    wav_u8x16_t v_ = v; \
    v_.values[lane] = value; \
    v_; \
  }))

#define \
  wav_u16x8_replace_lane(v, lane, value) (__extension__ ({ \
    WAV_REQUIRE_CONSTANT_RANGE(lane, 0, 15); \
    wav_u16x8_t v_ = v; \
    v_.values[lane] = value; \
    v_; \
  }))

#define \
  wav_u32x4_replace_lane(v, lane, value) (__extension__ ({ \
    WAV_REQUIRE_CONSTANT_RANGE(lane, 0, 15); \
    wav_u32x4_t v_ = v; \
    v_.values[lane] = value; \
    v_; \
  }))

#define \
  wav_u64x2_replace_lane(v, lane, value) (__extension__ ({ \
    WAV_REQUIRE_CONSTANT_RANGE(lane, 0, 15); \
    wav_u64x2_t v_ = v; \
    v_.values[lane] = value; \
    v_; \
  }))

#define \
  wav_f32x4_replace_lane(v, lane, value) (__extension__ ({ \
    WAV_REQUIRE_CONSTANT_RANGE(lane, 0, 15); \
    wav_f32x4_t v_ = v; \
    v_.values[lane] = value; \
    v_; \
  }))

#define \
  wav_f64x2_replace_lane(v, lane, value) (__extension__ ({ \
    WAV_REQUIRE_CONSTANT_RANGE(lane, 0, 15); \
    wav_f64x2_t v_ = v; \
    v_.values[lane] = value; \
    v_; \
  }))

#if WAV_OVERLOADS != WAV_OVERLOADS_NONE
  #define wav_replace_lane(v, lane, value) \
    (__extension__ ({ \
      WAV_REQUIRE_CONSTANT_RANGE(lane, 0, (sizeof(v.values) / sizeof(v.values[0])) - 1); \
      v.values[lane] = (value); \
    }))
#endif

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
    ((wav_i8x16_t) { __builtin_shufflevector(\
        WAV_REQUIRE_TYPE(wav_i8x16_t, a).values, \
        WAV_REQUIRE_TYPE(wav_i8x16_t, b).values, \
        c0, c1,  c2,  c3,  c4,  c5,  c6,  c7, \
        c8, c9, c10, c11, c12, c13, c14, c15) } )

#define \
  wav_i16x8_shuffle( \
      a, b, c0, c1, c2, c3, c4, c5, c6, c7) \
    ((wav_i16x8_t) { __builtin_shufflevector(\
        WAV_REQUIRE_TYPE(wav_i16x8_t, a).values, \
        WAV_REQUIRE_TYPE(wav_i16x8_t, b).values, \
        c0, c1,  c2,  c3,  c4,  c5,  c6,  c7) } )

#define \
  wav_i32x4_shuffle(a, b, c0, c1, c2, c3) \
    ((wav_i32x4_t) { __builtin_shufflevector(\
        WAV_REQUIRE_TYPE(wav_i32x4_t, a).values, \
        WAV_REQUIRE_TYPE(wav_i32x4_t, b).values, \
        c0, c1, c2, c3) } )

#define \
  wav_i64x2_shuffle(a, b, c0, c1) \
    ((wav_i64x2_t) { __builtin_shufflevector(\
        WAV_REQUIRE_TYPE(wav_i64x2_t, a).values, \
        WAV_REQUIRE_TYPE(wav_i64x2_t, b).values, \
        c0, c1) } )

#define \
  wav_u8x16_shuffle( \
      a, b, \
      c0, c1,  c2,  c3,  c4,  c5,  c6,  c7, \
      c8, c9, c10, c11, c12, c13, c14, c15) \
    ((wav_u8x16_t) { __builtin_shufflevector(\
        WAV_REQUIRE_TYPE(wav_u8x16_t, a).values, \
        WAV_REQUIRE_TYPE(wav_u8x16_t, b).values, \
        c0, c1,  c2,  c3,  c4,  c5,  c6,  c7, \
        c8, c9, c10, c11, c12, c13, c14, c15) } )

#define \
  wav_u16x8_shuffle( \
      a, b, c0, c1, c2, c3, c4, c5, c6, c7) \
    ((wav_u16x8_t) { __builtin_shufflevector(\
        WAV_REQUIRE_TYPE(wav_u16x8_t, a).values, \
        WAV_REQUIRE_TYPE(wav_u16x8_t, b).values, \
        c0, c1,  c2,  c3,  c4,  c5,  c6,  c7) } )

#define \
  wav_u32x4_shuffle(a, b, c0, c1, c2, c3) \
    ((wav_u32x4_t) { __builtin_shufflevector(\
        WAV_REQUIRE_TYPE(wav_u32x4_t, a).values, \
        WAV_REQUIRE_TYPE(wav_u32x4_t, b).values, \
        c0, c1, c2, c3) } )

#define \
  wav_u64x2_shuffle(a, b, c0, c1) \
    ((wav_u64x2_t) { __builtin_shufflevector(\
        WAV_REQUIRE_TYPE(wav_u64x2_t, a).values, \
        WAV_REQUIRE_TYPE(wav_u64x2_t, b).values, \
        c0, c1) } )

#define \
  wav_f32x4_shuffle(a, b, c0, c1, c2, c3) \
    ((wav_f32x4_t) { __builtin_shufflevector(\
        WAV_REQUIRE_TYPE(wav_f32x4_t, a).values, \
        WAV_REQUIRE_TYPE(wav_f32x4_t, b).values, \
        c0, c1, c2, c3) } )

#define \
  wav_f64x2_shuffle(a, b, c0, c1) \
    ((wav_f64x2_t) { __builtin_shufflevector(\
        WAV_REQUIRE_TYPE(wav_f64x2_t, a).values, \
        WAV_REQUIRE_TYPE(wav_f64x2_t, b).values, \
        c0, c1) } )

#if WAV_OVERLOADS != WAV_OVERLOADS_NONE
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
#endif

/* swizzle -- Swizzling using variable indices
 *
 * Returns a new vector with lanes selected from the lanes of the first
 * input vector a specified in the second input vector s.
 */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_swizzle(wav_i8x16_t a, wav_i8x16_t b) {
  return (wav_i8x16_t) { __builtin_wasm_swizzle_v8x16(a.values, b.values) };
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

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_swizzle(a, b) \
    (_Generic((a), \
      wav_i8x16_t: wav_i8x16_swizzle, \
      wav_u8x16_t: wav_u8x16_swizzle) (a, b))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_swizzle(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_swizzle(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_swizzle(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_swizzle(a, b); }
#endif

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

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_add(a, b) \
    (_Generic((a), \
      wav_i8x16_t: wav_i8x16_add, \
      wav_i16x8_t: wav_i16x8_add, \
      wav_i32x4_t: wav_i32x4_add, \
      wav_i64x2_t: wav_i64x2_add, \
      wav_u8x16_t: wav_u8x16_add, \
      wav_u16x8_t: wav_u16x8_add, \
      wav_u32x4_t: wav_u32x4_add, \
      wav_u64x2_t: wav_u64x2_add, \
      wav_f32x4_t: wav_f32x4_add, \
      wav_f64x2_t: wav_f64x2_add) (a, b))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_add(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_add(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_add(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_add(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_add(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_add(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_add(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_add(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_add(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_add(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_add(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_add(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_add(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_add(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_add(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_add(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_add(wav_f32x4_t a, wav_f32x4_t b) { return wav_f32x4_add(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_add(wav_f64x2_t a, wav_f64x2_t b) { return wav_f64x2_add(a, b); }

  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t operator+(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_add(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t operator+(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_add(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t operator+(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_add(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t operator+(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_add(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t operator+(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_add(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t operator+(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_add(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t operator+(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_add(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t operator+(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_add(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t operator+(wav_f32x4_t a, wav_f32x4_t b) { return wav_f32x4_add(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t operator+(wav_f64x2_t a, wav_f64x2_t b) { return wav_f64x2_add(a, b); }
#endif

/* sub — Subtraction. */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_sub(wav_i8x16_t a, wav_i8x16_t b) {
  return (wav_i8x16_t) { a.values - b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_sub(wav_i16x8_t a, wav_i16x8_t b) {
  return (wav_i16x8_t) { a.values - b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_sub(wav_i32x4_t a, wav_i32x4_t b) {
  return (wav_i32x4_t) { a.values - b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_sub(wav_i64x2_t a, wav_i64x2_t b) {
  return (wav_i64x2_t) { a.values - b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_sub(wav_u8x16_t a, wav_u8x16_t b) {
  return (wav_u8x16_t) { a.values - b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_sub(wav_u16x8_t a, wav_u16x8_t b) {
  return (wav_u16x8_t) { a.values - b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_sub(wav_u32x4_t a, wav_u32x4_t b) {
  return (wav_u32x4_t) { a.values - b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_sub(wav_u64x2_t a, wav_u64x2_t b) {
  return (wav_u64x2_t) { a.values - b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_sub(wav_f32x4_t a, wav_f32x4_t b) {
  return (wav_f32x4_t) { a.values - b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_sub(wav_f64x2_t a, wav_f64x2_t b) {
  return (wav_f64x2_t) { a.values - b.values };
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_sub(a, b) \
    (_Generic((a), \
      wav_i8x16_t: wav_i8x16_sub, \
      wav_i16x8_t: wav_i16x8_sub, \
      wav_i32x4_t: wav_i32x4_sub, \
      wav_i64x2_t: wav_i64x2_sub, \
      wav_u8x16_t: wav_u8x16_sub, \
      wav_u16x8_t: wav_u16x8_sub, \
      wav_u32x4_t: wav_u32x4_sub, \
      wav_u64x2_t: wav_u64x2_sub, \
      wav_f32x4_t: wav_f32x4_sub, \
      wav_f64x2_t: wav_f64x2_sub) (a, b))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_sub(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_sub(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_sub(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_sub(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_sub(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_sub(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_sub(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_sub(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_sub(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_sub(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_sub(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_sub(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_sub(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_sub(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_sub(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_sub(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_sub(wav_f32x4_t a, wav_f32x4_t b) { return wav_f32x4_sub(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_sub(wav_f64x2_t a, wav_f64x2_t b) { return wav_f64x2_sub(a, b); }

  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t operator-(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_sub(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t operator-(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_sub(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t operator-(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_sub(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t operator-(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_sub(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t operator-(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_sub(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t operator-(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_sub(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t operator-(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_sub(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t operator-(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_sub(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t operator-(wav_f32x4_t a, wav_f32x4_t b) { return wav_f32x4_sub(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t operator-(wav_f64x2_t a, wav_f64x2_t b) { return wav_f64x2_sub(a, b); }
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
wav_f32x4_t
wav_f32x4_mul(wav_f32x4_t a, wav_f32x4_t b) {
  return (wav_f32x4_t) { a.values * b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_mul(wav_f64x2_t a, wav_f64x2_t b) {
  return (wav_f64x2_t) { a.values * b.values };
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_mul(a, b) \
    (_Generic((a), \
      wav_i16x8_t: wav_i16x8_mul, \
      wav_i32x4_t: wav_i32x4_mul, \
      wav_i64x2_t: wav_i64x2_mul, \
      wav_f32x4_t: wav_f32x4_mul, \
      wav_f64x2_t: wav_f64x2_mul) (a, b))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_mul(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_mul(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_mul(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_mul(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_mul(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_mul(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_mul(wav_f32x4_t a, wav_f32x4_t b) { return wav_f32x4_mul(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_mul(wav_f64x2_t a, wav_f64x2_t b) { return wav_f64x2_mul(a, b); }

  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t operator*(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_mul(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t operator*(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_mul(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t operator*(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_mul(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t operator*(wav_f32x4_t a, wav_f32x4_t b) { return wav_f32x4_mul(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t operator*(wav_f64x2_t a, wav_f64x2_t b) { return wav_f64x2_mul(a, b); }
#endif

/* div — Division. */

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_div(wav_f32x4_t a, wav_f32x4_t b) {
  return (wav_f32x4_t) { a.values * b.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_div(wav_f64x2_t a, wav_f64x2_t b) {
  return (wav_f64x2_t) { a.values * b.values };
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_div(a, b) \
    (_Generic((a), \
      wav_f32x4_t: wav_f32x4_div, \
      wav_f64x2_t: wav_f64x2_div) (a, b))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_div(wav_f32x4_t a, wav_f32x4_t b) { return wav_f32x4_div(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_div(wav_f64x2_t a, wav_f64x2_t b) { return wav_f64x2_div(a, b); }

  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t operator/(wav_f32x4_t a, wav_f32x4_t b) { return wav_f32x4_div(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t operator/(wav_f64x2_t a, wav_f64x2_t b) { return wav_f64x2_div(a, b); }
#endif

/* neg — Negation. */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_neg(wav_i8x16_t a) {
  return (wav_i8x16_t) { -a.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_neg(wav_i16x8_t a) {
  return (wav_i16x8_t) { -a.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_neg(wav_i32x4_t a) {
  return (wav_i32x4_t) { -a.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_neg(wav_i64x2_t a) {
  return (wav_i64x2_t) { -a.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_neg(wav_f32x4_t a) {
  return (wav_f32x4_t) { -a.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_neg(wav_f64x2_t a) {
  return (wav_f64x2_t) { -a.values };
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_neg(a) \
    (_Generic((a), \
      wav_i8x16_t: wav_i8x16_neg, \
      wav_i16x8_t: wav_i16x8_neg, \
      wav_i32x4_t: wav_i32x4_neg, \
      wav_i64x2_t: wav_i64x2_neg, \
      wav_f32x4_t: wav_f32x4_neg, \
      wav_f64x2_t: wav_f64x2_neg) (a))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_neg(wav_i8x16_t a) { return wav_i8x16_neg(a); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_neg(wav_i16x8_t a) { return wav_i16x8_neg(a); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_neg(wav_i32x4_t a) { return wav_i32x4_neg(a); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_neg(wav_i64x2_t a) { return wav_i64x2_neg(a); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_neg(wav_f32x4_t a) { return wav_f32x4_neg(a); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_neg(wav_f64x2_t a) { return wav_f64x2_neg(a); }

  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t operator-(wav_i8x16_t a) { return wav_i8x16_neg(a); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t operator-(wav_i16x8_t a) { return wav_i16x8_neg(a); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t operator-(wav_i32x4_t a) { return wav_i32x4_neg(a); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t operator-(wav_i64x2_t a) { return wav_i64x2_neg(a); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t operator-(wav_f32x4_t a) { return wav_f32x4_neg(a); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t operator-(wav_f64x2_t a) { return wav_f64x2_neg(a); }
#endif

/* sqrt — Square root.
 *
 * Lane-wise IEEE squareRoot.
 */

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_sqrt(wav_f32x4_t a) {
  return (wav_f32x4_t) { __builtin_wasm_sqrt_f32x4(a.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_sqrt(wav_f64x2_t a) {
  return (wav_f64x2_t) { __builtin_wasm_sqrt_f64x2(a.values) };
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_sqrt(a) \
    (_Generic((a), \
      wav_f32x4_t: wav_f32x4_sqrt, \
      wav_f64x2_t: wav_f64x2_sqrt) (a))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_sqrt(wav_f32x4_t a) { return wav_f32x4_sqrt(a); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_sqrt(wav_f64x2_t a) { return wav_f64x2_sqrt(a); }
#endif

/* ceil — Round to integer above (ceiling)
 *
 * Lane-wise rounding to the nearest integral value not smaller than
 * the input.
 */

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_ceil(wav_f32x4_t a) {
  return (wav_f32x4_t) { __builtin_wasm_ceil_f32x4(a.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_ceil(wav_f64x2_t a) {
  return (wav_f64x2_t) { __builtin_wasm_ceil_f64x2(a.values) };
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_ceil(a) \
    (_Generic((a), \
      wav_f32x4_t: wav_f32x4_ceil, \
      wav_f64x2_t: wav_f64x2_ceil) (a))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_ceil(wav_f32x4_t a) { return wav_f32x4_ceil(a); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_ceil(wav_f64x2_t a) { return wav_f64x2_ceil(a); }
#endif

/* floor — Round to integer below (floor)
 *
 * Lane-wise rounding to the nearest integral value not greater than
 * the input.
 */

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_floor(wav_f32x4_t a) {
  return (wav_f32x4_t) { __builtin_wasm_floor_f32x4(a.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_floor(wav_f64x2_t a) {
  return (wav_f64x2_t) { __builtin_wasm_floor_f64x2(a.values) };
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_floor(a) \
    (_Generic((a), \
      wav_f32x4_t: wav_f32x4_floor, \
      wav_f64x2_t: wav_f64x2_floor) (a))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_floor(wav_f32x4_t a) { return wav_f32x4_floor(a); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_floor(wav_f64x2_t a) { return wav_f64x2_floor(a); }
#endif

/* trunc — Round to integer toward zero (truncate to integer)
 *
 * Lane-wise rounding to the nearest integral value with the magnitude
 * not larger than the input.
 */

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_trunc(wav_f32x4_t a) {
  return (wav_f32x4_t) { __builtin_wasm_trunc_f32x4(a.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_trunc(wav_f64x2_t a) {
  return (wav_f64x2_t) { __builtin_wasm_trunc_f64x2(a.values) };
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_trunc(a) \
    (_Generic((a), \
      wav_f32x4_t: wav_f32x4_trunc, \
      wav_f64x2_t: wav_f64x2_trunc) (a))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_trunc(wav_f32x4_t a) { return wav_f32x4_trunc(a); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_trunc(wav_f64x2_t a) { return wav_f64x2_trunc(a); }
#endif

/* nearest — Round to nearest integer, ties to even
 *
 * Lane-wise rounding to the nearest integral value; if two values are
 * equally near, rounds to the even one.
 */

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_nearest(wav_f32x4_t a) {
  return (wav_f32x4_t) { __builtin_wasm_nearest_f32x4(a.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_nearest(wav_f64x2_t a) {
  return (wav_f64x2_t) { __builtin_wasm_nearest_f64x2(a.values) };
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_nearest(a) \
    (_Generic((a), \
      wav_f32x4_t: wav_f32x4_nearest, \
      wav_f64x2_t: wav_f64x2_nearest) (a))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_nearest(wav_f32x4_t a) { return wav_f32x4_nearest(a); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_nearest(wav_f64x2_t a) { return wav_f64x2_nearest(a); }
#endif

/* extend_low -- Integer to integer extension
 *
 * Converts low half of the smaller lane vector to a larger lane
 * vector, sign extended or zero (unsigned) extended.
 */

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i8x16_extend_low(wav_i8x16_t a) {
  wav_i16x8_t r;
  const int8_t low __attribute__((__vector_size__(8))) = {
    a.values[0], a.values[1], a.values[2], a.values[3],
    a.values[4], a.values[5], a.values[6], a.values[7],
  };

  r.values = __builtin_convertvector(low, __typeof__(r.values));

  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i16x8_extend_low(wav_i16x8_t a) {
  wav_i32x4_t r;
  const int16_t low __attribute__((__vector_size__(8))) = {
    a.values[0], a.values[1], a.values[2], a.values[3],
  };

  r.values = __builtin_convertvector(low, __typeof__(r.values));

  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i32x4_extend_low(wav_i32x4_t a) {
  wav_i64x2_t r;
  const int32_t low __attribute__((__vector_size__(8))) = {
    a.values[0], a.values[1],
  };

  r.values = __builtin_convertvector(low, __typeof__(r.values));

  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u8x16_extend_low(wav_u8x16_t a) {
  wav_u16x8_t r;
  const uint8_t low __attribute__((__vector_size__(8))) = {
    a.values[0], a.values[1], a.values[2], a.values[3],
    a.values[4], a.values[5], a.values[6], a.values[7],
  };

  r.values = __builtin_convertvector(low, __typeof__(r.values));

  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u16x8_extend_low(wav_u16x8_t a) {
  wav_u32x4_t r;
  const uint16_t low __attribute__((__vector_size__(8))) = {
    a.values[0], a.values[1], a.values[2], a.values[3],
  };

  r.values = __builtin_convertvector(low, __typeof__(r.values));

  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u32x4_extend_low(wav_u32x4_t a) {
  wav_u64x2_t r;
  const uint32_t low __attribute__((__vector_size__(8))) = {
    a.values[0], a.values[1],
  };

  r.values = __builtin_convertvector(low, __typeof__(r.values));

  return r;
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_extend_low(a) \
    (_Generic((a), \
      wav_i8x16_t: wav_i8x16_extend_low, \
      wav_i16x8_t: wav_i16x8_extend_low, \
      wav_i32x4_t: wav_i32x4_extend_low, \
      wav_u8x16_t: wav_u8x16_extend_low, \
      wav_u16x8_t: wav_u16x8_extend_low, \
      wav_u32x4_t: wav_u32x4_extend_low) (a))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_extend_low(wav_i8x16_t a) { return wav_i8x16_extend_low(a); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_extend_low(wav_i16x8_t a) { return wav_i16x8_extend_low(a); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_extend_low(wav_i32x4_t a) { return wav_i32x4_extend_low(a); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_extend_low(wav_u8x16_t a) { return wav_u8x16_extend_low(a); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_extend_low(wav_u16x8_t a) { return wav_u16x8_extend_low(a); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_extend_low(wav_u32x4_t a) { return wav_u32x4_extend_low(a); }
#endif

/* promote_low -- Single-precision floating point to double-precision
 *
 * Conversion of the two lower single-precision floating point lanes to
 * the two double-precision lanes of the result.
 */

WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f32x4_promote_low(wav_f32x4_t a) {
  #if WAV_BUILTIN_IS_NOOP
    return (wav_f64x2_t) { __builtin_wasm_promote_low_f32x4_f64x2(a.values) };
  #else
    wav_f64x2_t r;
    const float low __attribute__((__vector_size__(8))) = {
      a.values[0], a.values[1],
    };

    r.values = __builtin_convertvector(low, __typeof__(r.values));

    return r;
  #endif
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_promote_low(a) \
    (_Generic((a), \
      wav_f32x4_t: wav_f32x4_promote_low) (a))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_promote_low(wav_f32x4_t a) { return wav_f32x4_promote_low(a); }
#endif

/* extend_high -- Integer to integer extension
 *
 * Converts high half of the smaller lane vector to a larger lane
 * vector, sign extended or zero (unsigned) extended.
 */

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i8x16_extend_high(wav_i8x16_t a) {
  wav_i8x16_t high;
  high.values = __builtin_shufflevector(a.values, a.values, 8, 9, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1);
  return wav_i8x16_extend_low(high);
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i16x8_extend_high(wav_i16x8_t a) {
  wav_i16x8_t high;
  high.values = __builtin_shufflevector(a.values, a.values, 4, 5, 6, 7, -1, -1, -1, -1);
  return wav_i16x8_extend_low(high);
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i32x4_extend_high(wav_i32x4_t a) {
  wav_i32x4_t high;
  high.values = __builtin_shufflevector(a.values, a.values, 2, 3, -1, -1);
  return wav_i32x4_extend_low(high);
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u8x16_extend_high(wav_u8x16_t a) {
  wav_u8x16_t high;
  high.values = __builtin_shufflevector(a.values, a.values, 8, 9, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1);
  return wav_u8x16_extend_low(high);
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u16x8_extend_high(wav_u16x8_t a) {
  wav_u16x8_t high;
  high.values = __builtin_shufflevector(a.values, a.values, 4, 5, 6, 7, -1, -1, -1, -1);
  return wav_u16x8_extend_low(high);
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u32x4_extend_high(wav_u32x4_t a) {
  wav_u32x4_t high;
  high.values = __builtin_shufflevector(a.values, a.values, 2, 3, -1, -1);
  return wav_u32x4_extend_low(high);
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_extend_high(a) \
    (_Generic((a), \
      wav_i8x16_t: wav_i8x16_extend_high, \
      wav_i16x8_t: wav_i16x8_extend_high, \
      wav_i32x4_t: wav_i32x4_extend_high, \
      wav_u8x16_t: wav_u8x16_extend_high, \
      wav_u16x8_t: wav_u16x8_extend_high, \
      wav_u32x4_t: wav_u32x4_extend_high) (a))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_extend_high(wav_i8x16_t a) { return wav_i8x16_extend_high(a); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_extend_high(wav_i16x8_t a) { return wav_i16x8_extend_high(a); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_extend_high(wav_i32x4_t a) { return wav_i32x4_extend_high(a); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_extend_high(wav_u8x16_t a) { return wav_u8x16_extend_high(a); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_extend_high(wav_u16x8_t a) { return wav_u16x8_extend_high(a); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_extend_high(wav_u32x4_t a) { return wav_u32x4_extend_high(a); }
#endif

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
  return (wav_i8x16_t) { __builtin_wasm_narrow_s_i8x16_i16x8(a.values, b.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i32x4_narrow(wav_i32x4_t a, wav_i32x4_t b) {
  return (wav_i16x8_t) { __builtin_wasm_narrow_s_i16x8_i32x4(a.values, b.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u16x8_narrow(wav_u16x8_t a, wav_u16x8_t b) {
  return (wav_u8x16_t) { __builtin_wasm_narrow_u_i8x16_i16x8(a.values, b.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u32x4_narrow(wav_u32x4_t a, wav_u32x4_t b) {
  return (wav_u16x8_t) { __builtin_wasm_narrow_u_i16x8_i32x4(a.values, b.values) };
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_narrow(a, b) \
    (_Generic((a), \
      wav_i16x8_t: wav_i16x8_narrow, \
      wav_i32x4_t: wav_i32x4_narrow, \
      wav_u16x8_t: wav_u16x8_narrow, \
      wav_u32x4_t: wav_u32x4_narrow) (a, b))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_narrow(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_narrow(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_narrow(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_narrow(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_narrow(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_narrow(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_narrow(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_narrow(a, b); }
#endif

/* demote -- Double-precision floating point to single-precision
 *
 * Conversion of the two double-precision floating point lanes to two
 * lower single-precision lanes of the result. The two higher lanes of
 * the result are initialized to zero. If the conversion result is not
 * representable as a single-precision floating point number, it is
 * rounded to the nearest-even representable number.
 */

WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f64x2_demote(wav_f64x2_t a) {
  #if WAV_BUILTIN_IS_NOOP
    return (wav_f32x4_t) { __builtin_wasm_demote_zero_f64x2_f32x4(a.values) };
  #else
    return (wav_f32x4_t) {
      (float) a.values[0], (float) a.values[1], 0.0f, 0.0f
    };
  #endif
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_demote(a) \
    (_Generic((a), \
      wav_f64x2_t: wav_f64x2_demote) (a))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_demote(wav_f64x2_t a) { return wav_f64x2_demote(a); }
#endif

/* extmul_low -- Extended wultiplication.
 *
 * Lane-wise integer extended multiplication producing twice wider
 * result than the inputs.
 */

WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i8x16_extmul_low(wav_i8x16_t a, wav_i8x16_t b) {
  #if WAV_BUILTIN_IS_NOOP
    return (wav_i16x8_t) { __builtin_wasm_extmul_low_i8x16_s_i16x8(a.values, b.values) };
  #else
    return wav_i16x8_mul(wav_i8x16_extend_low(a), wav_i8x16_extend_low(b));
  #endif
}

WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i16x8_extmul_low(wav_i16x8_t a, wav_i16x8_t b) {
  #if WAV_BUILTIN_IS_NOOP
    return (wav_i16x8_t) { __builtin_wasm_extmul_low_i16x8_s_i32x4(a.values, b.values) };
  #else
    return wav_i32x4_mul(wav_i16x8_extend_low(a), wav_i16x8_extend_low(b));
  #endif
}

WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i32x4_extmul_low(wav_i32x4_t a, wav_i32x4_t b) {
  #if WAV_BUILTIN_IS_NOOP
    return (wav_i16x8_t) { __builtin_wasm_extmul_low_i32x4_s_i64x2(a.values, b.values) };
  #else
    return wav_i64x2_mul(wav_i32x4_extend_low(a), wav_i32x4_extend_low(b));
  #endif
}

WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u8x16_extmul_low(wav_u8x16_t a, wav_u8x16_t b) {
  #if WAV_BUILTIN_IS_NOOP
    return (wav_i16x8_t) { __builtin_wasm_extmul_low_i8x16_u_i16x8(a.values, b.values) };
  #else
    return (wav_u16x8_t) { wav_u8x16_extend_low(a).values * wav_u8x16_extend_low(b).values };
  #endif
}

WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u16x8_extmul_low(wav_u16x8_t a, wav_u16x8_t b) {
  #if WAV_BUILTIN_IS_NOOP
    return (wav_i16x8_t) { __builtin_wasm_extmul_low_i16x8_u_i32x4(a.values, b.values) };
  #else
    return (wav_u32x4_t) { wav_u16x8_extend_low(a).values * wav_u16x8_extend_low(b).values };
  #endif
}

WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u32x4_extmul_low(wav_u32x4_t a, wav_u32x4_t b) {
  #if WAV_BUILTIN_IS_NOOP
    return (wav_i16x8_t) { __builtin_wasm_extmul_low_i32x4_u_i64x2(a.values, b.values) };
  #else
    return (wav_u64x2_t) { wav_u32x4_extend_low(a).values * wav_u32x4_extend_low(b).values };
  #endif
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_extmul_low(a, b) \
    (_Generic((a), \
      wav_i8x16_t: wav_i8x16_extmul_low, \
      wav_i16x8_t: wav_i16x8_extmul_low, \
      wav_i32x4_t: wav_i32x4_extmul_low, \
      wav_u8x16_t: wav_u8x16_extmul_low, \
      wav_u16x8_t: wav_u16x8_extmul_low, \
      wav_u32x4_t: wav_u32x4_extmul_low) (a, b))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_extmul_low(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_extmul_low(a, b); }
  WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_extmul_low(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_extmul_low(a, b); }
  WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_extmul_low(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_extmul_low(a, b); }
  WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_extmul_low(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_extmul_low(a, b); }
  WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_extmul_low(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_extmul_low(a, b); }
  WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_extmul_low(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_extmul_low(a, b); }
#endif

/* extmul_high -- Extended wultiplication.
 *
 * Lane-wise integer extended multiplication producing twice wider
 * result than the inputs.
 */

WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i8x16_extmul_high(wav_i8x16_t a, wav_i8x16_t b) {
  return wav_i16x8_mul(wav_i8x16_extend_high(a), wav_i8x16_extend_high(b));
}

WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i16x8_extmul_high(wav_i16x8_t a, wav_i16x8_t b) {
  return wav_i32x4_mul(wav_i16x8_extend_high(a), wav_i16x8_extend_high(b));
}

WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i32x4_extmul_high(wav_i32x4_t a, wav_i32x4_t b) {
  return wav_i64x2_mul(wav_i32x4_extend_high(a), wav_i32x4_extend_high(b));
}

WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u8x16_extmul_high(wav_u8x16_t a, wav_u8x16_t b) {
  return (wav_u16x8_t) { wav_u8x16_extend_high(a).values * wav_u8x16_extend_high(b).values };
}

WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u16x8_extmul_high(wav_u16x8_t a, wav_u16x8_t b) {
  return (wav_u32x4_t) { wav_u16x8_extend_high(a).values * wav_u16x8_extend_high(b).values };
}

WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u32x4_extmul_high(wav_u32x4_t a, wav_u32x4_t b) {
  return (wav_u64x2_t) { wav_u32x4_extend_high(a).values * wav_u32x4_extend_high(b).values };
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_extmul_high(a, b) \
    (_Generic((a), \
      wav_i8x16_t: wav_i8x16_extmul_high, \
      wav_i16x8_t: wav_i16x8_extmul_high, \
      wav_i32x4_t: wav_i32x4_extmul_high, \
      wav_u8x16_t: wav_u8x16_extmul_high, \
      wav_u16x8_t: wav_u16x8_extmul_high, \
      wav_u32x4_t: wav_u32x4_extmul_high) (a, b))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_extmul_high(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_extmul_high(a, b); }
  WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_extmul_high(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_extmul_high(a, b); }
  WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_extmul_high(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_extmul_high(a, b); }
  WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_extmul_high(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_extmul_high(a, b); }
  WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_extmul_high(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_extmul_high(a, b); }
  WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_extmul_high(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_extmul_high(a, b); }
#endif

/* extadd_pairwise
 *
 * Lane-wise integer extended pairwise addition producing extended
 * results (twice wider results than the inputs).
 */

WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i8x16_extadd_pairwise(wav_i8x16_t v) {
  #if WAV_BUILTIN_IS_NOOP
    return (wav_i16x8_t) { __builtin_wasm_extadd_pairwise_i8x16_s_i16x8(v.values) };
  #else
    wav_i16x8_t r;
    const int8_t even __attribute__((__vector_size__(8))) = {
      v.values[0], v.values[2], v.values[4], v.values[6]
    };
    const int8_t odd __attribute__((__vector_size__(8))) = {
      v.values[1], v.values[3], v.values[5], v.values[7]
    };

    r.values =
      __builtin_convertvector(even, __typeof__(r.values)) +
      __builtin_convertvector( odd, __typeof__(r.values));

    return r;
  #endif
}

WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i16x8_extadd_pairwise(wav_i16x8_t v) {
  #if WAV_BUILTIN_IS_NOOP
    return (wav_i32x4_t) { __builtin_wasm_extadd_pairwise_i16x8_s_i32x4(v.values) };
  #else
    wav_i32x4_t r;
    const int16_t even __attribute__((__vector_size__(8))) = {
      v.values[0], v.values[2]
    };
    const int16_t odd __attribute__((__vector_size__(8))) = {
      v.values[1], v.values[3]
    };

    r.values =
      __builtin_convertvector(even, __typeof__(r.values)) +
      __builtin_convertvector( odd, __typeof__(r.values));

    return r;
  #endif
}

WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u8x16_extadd_pairwise(wav_u8x16_t v) {
  #if WAV_BUILTIN_IS_NOOP
    return (wav_i32x4_t) { __builtin_wasm_extadd_pairwise_i8x16_u_i16x8(v.values) };
  #else
    wav_u16x8_t r;
    const uint8_t even __attribute__((__vector_size__(8))) = {
      v.values[0], v.values[2], v.values[4], v.values[6]
    };
    const uint8_t odd __attribute__((__vector_size__(8))) = {
      v.values[1], v.values[3], v.values[5], v.values[7]
    };

    r.values =
      __builtin_convertvector(even, __typeof__(r.values)) +
      __builtin_convertvector( odd, __typeof__(r.values));

    return r;
  #endif
}

WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u16x8_extadd_pairwise(wav_u16x8_t v) {
  #if WAV_BUILTIN_IS_NOOP
    return (wav_i32x4_t) { __builtin_wasm_extadd_pairwise_i8x16_u_i16x8(v.values) };
  #else
    wav_u32x4_t r;
    const uint16_t even __attribute__((__vector_size__(8))) = {
      v.values[0], v.values[2]
    };
    const uint16_t odd __attribute__((__vector_size__(8))) = {
      v.values[1], v.values[3]
    };

    r.values =
      __builtin_convertvector(even, __typeof__(r.values)) +
      __builtin_convertvector( odd, __typeof__(r.values));

    return r;
  #endif
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_extadd_pairwise(a) \
    (_Generic((a), \
      wav_i8x16_t: wav_i8x16_extadd_pairwise, \
      wav_i16x8_t: wav_i16x8_extadd_pairwise, \
      wav_u8x16_t: wav_u8x16_extadd_pairwise, \
      wav_u16x8_t: wav_u16x8_extadd_pairwise) (a))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_extadd_pairwise(wav_i8x16_t v) { return wav_i8x16_extadd_pairwise(v); }
  WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_extadd_pairwise(wav_i16x8_t v) { return wav_i16x8_extadd_pairwise(v); }
  WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_extadd_pairwise(wav_u8x16_t v) { return wav_u8x16_extadd_pairwise(v); }
  WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_extadd_pairwise(wav_u16x8_t v) { return wav_u16x8_extadd_pairwise(v); }
#endif

/* dot — dot product
 *
 * Lane-wise multiply signed 16-bit integers in the two input vectors
 * and add adjacent pairs of the full 32-bit results.
 */

WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i16x8_dot(wav_i16x8_t a, wav_i16x8_t b) {
  #if WAV_BUILTIN_IS_NOOP
    return (wav_i32x4_t) { __builtin_wasm_dot_s_i32x4_i16x8(a.values, b.values) };
  #else
    return wav_i16x8_extadd_pairwise(wav_i16x8_mul(a, b));
  #endif
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_dot(a, b) \
    (_Generic((a) \
      wav_i16x8_t: wav_i16x8_dot) (a, b))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_dot(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_dot(a, b); }
#endif

/* add_saturate -- Saturating integer addition
 *
 * Lane-wise saturating addition.
 */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_add_saturate(wav_i8x16_t a, wav_i8x16_t b) {
  return (wav_i8x16_t) { __builtin_wasm_add_saturate_s_i8x16(a.values, b.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_add_saturate(wav_i16x8_t a, wav_i16x8_t b) {
  return (wav_i16x8_t) { __builtin_wasm_add_saturate_s_i16x8(a.values, b.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_add_saturate(wav_u8x16_t a, wav_u8x16_t b) {
  return (wav_u8x16_t) { __builtin_wasm_add_saturate_u_i8x16(a.values, b.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_add_saturate(wav_u16x8_t a, wav_u16x8_t b) {
  return (wav_u16x8_t) { __builtin_wasm_add_saturate_u_i16x8(a.values, b.values) };
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_add_saturate(a, b) \
    (_Generic((a), \
      wav_i8x16_t: wav_i8x16_add_saturate, \
      wav_i16x8_t: wav_i16x8_add_saturate, \
      wav_u8x16_t: wav_u8x16_add_saturate, \
      wav_u16x8_t: wav_u16x8_add_saturate) (a, b))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_add_saturate(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_add_saturate(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_add_saturate(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_add_saturate(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_add_saturate(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_add_saturate(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_add_saturate(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_add_saturate(a, b); }
#endif

/* sub_saturate -- Saturating integer subtraction
 *
 * Lane-wise saturating subtraction.
 */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_sub_saturate(wav_i8x16_t a, wav_i8x16_t b) {
  return (wav_i8x16_t) { __builtin_wasm_sub_saturate_s_i8x16(a.values, b.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_sub_saturate(wav_i16x8_t a, wav_i16x8_t b) {
  return (wav_i16x8_t) { __builtin_wasm_sub_saturate_s_i16x8(a.values, b.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_sub_saturate(wav_u8x16_t a, wav_u8x16_t b) {
  return (wav_u8x16_t) { __builtin_wasm_sub_saturate_u_i8x16(a.values, b.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_sub_saturate(wav_u16x8_t a, wav_u16x8_t b) {
  return (wav_u16x8_t) { __builtin_wasm_sub_saturate_u_i16x8(a.values, b.values) };
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_sub_saturate(a, b) \
    (_Generic((a), \
      wav_i8x16_t: wav_i8x16_sub_saturate, \
      wav_i16x8_t: wav_i16x8_sub_saturate, \
      wav_u8x16_t: wav_u8x16_sub_saturate, \
      wav_u16x8_t: wav_u16x8_sub_saturate) (a, b))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_sub_saturate(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_sub_saturate(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_sub_saturate(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_sub_saturate(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_sub_saturate(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_sub_saturate(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_sub_saturate(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_sub_saturate(a, b); }
#endif

/* q15mulr_saturate -- Saturating integer Q-format rounding multiplication
 *
 * Lane-wise saturating rounding multiplication in Q15 format
 */

#if WAV_BUILTIN_IS_NOOP

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_q15mulr_saturate(wav_i16x8_t a, wav_i16x8_t b) {
  return (wav_i16x8_t) { __builtin_wasm_q15mulr_saturate_s_i16x8(a.values, b.values) };
}

#endif

/* min -- Lane-wise integer minimum
 *
 * Compares lane-wise signed/unsigned integers, and returns the minimum
 * of each pair.
 */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_min(wav_i8x16_t a, wav_i8x16_t b)  {
  return (wav_i8x16_t) { __builtin_wasm_min_s_i8x16(a.values, b.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_min(wav_i16x8_t a, wav_i16x8_t b)  {
  return (wav_i16x8_t) { __builtin_wasm_min_s_i16x8(a.values, b.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_min(wav_i32x4_t a, wav_i32x4_t b)  {
  return (wav_i32x4_t) { __builtin_wasm_min_s_i32x4(a.values, b.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_min(wav_u8x16_t a, wav_u8x16_t b)  {
  return (wav_u8x16_t) { __builtin_wasm_min_u_i8x16(a.values, b.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_min(wav_u16x8_t a, wav_u16x8_t b)  {
  return (wav_u16x8_t) { __builtin_wasm_min_u_i16x8(a.values, b.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_min(wav_u32x4_t a, wav_u32x4_t b)  {
  return (wav_u32x4_t) { __builtin_wasm_min_u_i32x4(a.values, b.values) };
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_min(a, b) \
    (_Generic((a), \
      wav_i8x16_t: wav_i8x16_min, \
      wav_i16x8_t: wav_i16x8_min, \
      wav_i32x4_t: wav_i32x4_min, \
      wav_u8x16_t: wav_u8x16_min, \
      wav_u16x8_t: wav_u16x8_min, \
      wav_u32x4_t: wav_u32x4_min) (a, b))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_min(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_min(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_min(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_min(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_min(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_min(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_min(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_min(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_min(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_min(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_min(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_min(a, b); }
#endif

/* max -- Lane-wise integer maximum
 *
 * Compares lane-wise signed/unsigned integers, and returns the maximum
 * of each pair.
 */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_max(wav_i8x16_t a, wav_i8x16_t b)  {
  return (wav_i8x16_t) { __builtin_wasm_max_s_i8x16(a.values, b.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_max(wav_i16x8_t a, wav_i16x8_t b)  {
  return (wav_i16x8_t) { __builtin_wasm_max_s_i16x8(a.values, b.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_max(wav_i32x4_t a, wav_i32x4_t b)  {
  return (wav_i32x4_t) { __builtin_wasm_max_s_i32x4(a.values, b.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_max(wav_u8x16_t a, wav_u8x16_t b)  {
  return (wav_u8x16_t) { __builtin_wasm_max_u_i8x16(a.values, b.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_max(wav_u16x8_t a, wav_u16x8_t b)  {
  return (wav_u16x8_t) { __builtin_wasm_max_u_i16x8(a.values, b.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_max(wav_u32x4_t a, wav_u32x4_t b)  {
  return (wav_u32x4_t) { __builtin_wasm_max_u_i32x4(a.values, b.values) };
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_max(a, b) \
    (_Generic((a), \
      wav_i8x16_t: wav_i8x16_max, \
      wav_i16x8_t: wav_i16x8_max, \
      wav_i32x4_t: wav_i32x4_max, \
      wav_u8x16_t: wav_u8x16_max, \
      wav_u16x8_t: wav_u16x8_max, \
      wav_u32x4_t: wav_u32x4_max) (a, b))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_max(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_max(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_max(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_max(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_max(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_max(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_max(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_max(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_max(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_max(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_max(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_max(a, b); }
#endif

/* avgr -- Lane-wise integer rounding average
 *
 * Lane-wise rounding average.
 */

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_avgr(wav_u8x16_t a, wav_u8x16_t b)  {
  return (wav_u8x16_t) { __builtin_wasm_avgr_u_i8x16(a.values, b.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_avgr(wav_u16x8_t a, wav_u16x8_t b)  {
  return (wav_u16x8_t) { __builtin_wasm_avgr_u_i16x8(a.values, b.values) };
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_avgr(a, b) \
    (_Generic((a), \
      wav_u8x16_t: wav_u8x16_avgr, \
      wav_u16x8_t: wav_u16x8_avgr) (a, b))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_avgr(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_avgr(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_avgr(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_avgr(a, b); }
#endif

/* abs -- Lane-wise integer absolute value
 *
 * Lane-wise wrapping absolute value.
 */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_abs(wav_i8x16_t a) {
  return (wav_i8x16_t) { -a.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_abs(wav_i16x8_t a) {
  return (wav_i16x8_t) { -a.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_abs(wav_i32x4_t a) {
  return (wav_i32x4_t) { -a.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_abs(wav_i64x2_t a) {
  return (wav_i64x2_t) { -a.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_abs(wav_u8x16_t a) {
  return (wav_u8x16_t) { -a.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_abs(wav_f32x4_t a) {
  return (wav_f32x4_t) { -a.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_abs(wav_f64x2_t a) {
  return (wav_f64x2_t) { -a.values };
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_abs(a) \
    (_Generic((a), \
      wav_i8x16_t: wav_i8x16_abs, \
      wav_i16x8_t: wav_i16x8_abs, \
      wav_i32x4_t: wav_i32x4_abs, \
      wav_i64x2_t: wav_i64x2_abs, \
      wav_f32x4_t: wav_f32x4_abs, \
      wav_f64x2_t: wav_f64x2_abs) (a))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_abs(wav_i8x16_t a) { return wav_i8x16_abs(a); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_abs(wav_i16x8_t a) { return wav_i16x8_abs(a); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_abs(wav_i32x4_t a) { return wav_i32x4_abs(a); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_abs(wav_i64x2_t a) { return wav_i64x2_abs(a); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_abs(wav_f32x4_t a) { return wav_f32x4_abs(a); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_abs(wav_f64x2_t a) { return wav_f64x2_abs(a); }
#endif

/* shl -- Left shift by scalar
 *
 * Shift the bits in each lane to the left by the same amount. The
 * shift count is taken modulo lane width.
 */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_shl(wav_i8x16_t a, int count) {
  return (wav_i8x16_t) { a.values << (count & 15) };
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_shl(wav_i16x8_t a, int count) {
  return (wav_i16x8_t) { a.values << (count & 15) };
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_shl(wav_i32x4_t a, int count) {
  return (wav_i32x4_t) { a.values << (count & 15) };
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_shl(wav_i64x2_t a, int count) {
  return (wav_i64x2_t) { a.values << (count & 15) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_shl(wav_u8x16_t a, int count) {
  return (wav_u8x16_t) { a.values << (count & 15) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_shl(wav_u16x8_t a, int count) {
  return (wav_u16x8_t) { a.values << (count & 15) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_shl(wav_u32x4_t a, int count) {
  return (wav_u32x4_t) { a.values << (count & 15) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_shl(wav_u64x2_t a, int count) {
  return (wav_u64x2_t) { a.values << (count & 15) };
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_shl(a, count) \
    (_Generic((a), \
      wav_i8x16_t: wav_i8x16_shl, \
      wav_i16x8_t: wav_i16x8_shl, \
      wav_i32x4_t: wav_i32x4_shl, \
      wav_i64x2_t: wav_i64x2_shl, \
      wav_u8x16_t: wav_u8x16_shl, \
      wav_u16x8_t: wav_u16x8_shl, \
      wav_u32x4_t: wav_u32x4_shl, \
      wav_u64x2_t: wav_u64x2_shl) (a, count))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_shl(wav_i8x16_t a, int count) { return wav_i8x16_shl(a, count); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_shl(wav_i16x8_t a, int count) { return wav_i16x8_shl(a, count); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_shl(wav_i32x4_t a, int count) { return wav_i32x4_shl(a, count); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_shl(wav_i64x2_t a, int count) { return wav_i64x2_shl(a, count); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_shl(wav_u8x16_t a, int count) { return wav_u8x16_shl(a, count); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_shl(wav_u16x8_t a, int count) { return wav_u16x8_shl(a, count); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_shl(wav_u32x4_t a, int count) { return wav_u32x4_shl(a, count); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_shl(wav_u64x2_t a, int count) { return wav_u64x2_shl(a, count); }

  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t operator<<(wav_i8x16_t a, int count) { return wav_i8x16_shl(a, count); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t operator<<(wav_i16x8_t a, int count) { return wav_i16x8_shl(a, count); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t operator<<(wav_i32x4_t a, int count) { return wav_i32x4_shl(a, count); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t operator<<(wav_i64x2_t a, int count) { return wav_i64x2_shl(a, count); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t operator<<(wav_u8x16_t a, int count) { return wav_u8x16_shl(a, count); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t operator<<(wav_u16x8_t a, int count) { return wav_u16x8_shl(a, count); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t operator<<(wav_u32x4_t a, int count) { return wav_u32x4_shl(a, count); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t operator<<(wav_u64x2_t a, int count) { return wav_u64x2_shl(a, count); }
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
  return (wav_i8x16_t) { a.values << (count & 15) };
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_shr(wav_i16x8_t a, int count) {
  return (wav_i16x8_t) { a.values << (count & 15) };
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_shr(wav_i32x4_t a, int count) {
  return (wav_i32x4_t) { a.values << (count & 15) };
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_shr(wav_i64x2_t a, int count) {
  return (wav_i64x2_t) { a.values << (count & 15) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_shr(wav_u8x16_t a, int count) {
  return (wav_u8x16_t) { a.values << (count & 15) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_shr(wav_u16x8_t a, int count) {
  return (wav_u16x8_t) { a.values << (count & 15) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_shr(wav_u32x4_t a, int count) {
  return (wav_u32x4_t) { a.values << (count & 15) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_shr(wav_u64x2_t a, int count) {
  return (wav_u64x2_t) { a.values << (count & 15) };
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_shr(a, count) \
    (_Generic((a), \
      wav_i8x16_t: wav_i8x16_shr, \
      wav_i16x8_t: wav_i16x8_shr, \
      wav_i32x4_t: wav_i32x4_shr, \
      wav_i64x2_t: wav_i64x2_shr, \
      wav_u8x16_t: wav_u8x16_shr, \
      wav_u16x8_t: wav_u16x8_shr, \
      wav_u32x4_t: wav_u32x4_shr, \
      wav_u64x2_t: wav_u64x2_shr) (a, count))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_shr(wav_i8x16_t a, int count) { return wav_i8x16_shr(a, count); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_shr(wav_i16x8_t a, int count) { return wav_i16x8_shr(a, count); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_shr(wav_i32x4_t a, int count) { return wav_i32x4_shr(a, count); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_shr(wav_i64x2_t a, int count) { return wav_i64x2_shr(a, count); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_shr(wav_u8x16_t a, int count) { return wav_u8x16_shr(a, count); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_shr(wav_u16x8_t a, int count) { return wav_u16x8_shr(a, count); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_shr(wav_u32x4_t a, int count) { return wav_u32x4_shr(a, count); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_shr(wav_u64x2_t a, int count) { return wav_u64x2_shr(a, count); }

  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t operator>>(wav_i8x16_t a, int count) { return wav_i8x16_shr(a, count); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t operator>>(wav_i16x8_t a, int count) { return wav_i16x8_shr(a, count); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t operator>>(wav_i32x4_t a, int count) { return wav_i32x4_shr(a, count); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t operator>>(wav_i64x2_t a, int count) { return wav_i64x2_shr(a, count); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t operator>>(wav_u8x16_t a, int count) { return wav_u8x16_shr(a, count); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t operator>>(wav_u16x8_t a, int count) { return wav_u16x8_shr(a, count); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t operator>>(wav_u32x4_t a, int count) { return wav_u32x4_shr(a, count); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t operator>>(wav_u64x2_t a, int count) { return wav_u64x2_shr(a, count); }
#endif

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

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_and(a, b) \
    (_Generic((a), \
      wav_i8x16_t: (_Generic((b), \
        wav_b8x16_t: wav_i8x16_and_b8x16, \
        default /* wav_i8x16_t */: wav_i8x16_and)), \
      wav_i16x8_t: (_Generic((b), \
        wav_b16x8_t: wav_i16x8_and_b16x8, \
        default /* wav_i16x8_t */: wav_i16x8_and)), \
      wav_i32x4_t: (_Generic((b), \
        wav_b32x4_t: wav_i32x4_and_b32x4, \
        default /* wav_i32x4_t */: wav_i32x4_and)), \
      wav_i64x2_t: (_Generic((b), \
        wav_b64x2_t: wav_i64x2_and_b64x2, \
        default /* wav_i64x2_t */: wav_i64x2_and)), \
      wav_u8x16_t: (_Generic((b), \
        wav_b8x16_t: wav_u8x16_and_b8x16, \
        default /* wav_u8x16_t */: wav_u8x16_and)), \
      wav_u16x8_t: (_Generic((b), \
        wav_b16x8_t: wav_u16x8_and_b16x8, \
        default /* wav_u16x8_t */: wav_u16x8_and)), \
      wav_u32x4_t: (_Generic((b), \
        wav_b32x4_t: wav_u32x4_and_b32x4, \
        default /* wav_u32x4_t */: wav_u32x4_and)), \
      wav_u64x2_t: (_Generic((b), \
        wav_b64x2_t: wav_u64x2_and_b64x2, \
        default /* wav_u64x2_t */: wav_u64x2_and)), \
      wav_f32x4_t: wav_f32x4_and_b32x4, \
      wav_f64x2_t: wav_f64x2_and_b64x2, \
      wav_b8x16_t: (_Generic((b), \
        wav_i8x16_t: wav_b8x16_and_i8x16, \
        wav_u8x16_t: wav_b8x16_and_u8x16, \
        default /* wav_b8x16_t */: wav_b8x16_and)), \
      wav_b16x8_t: (_Generic((b), \
        wav_i16x8_t: wav_b16x8_and_i16x8, \
        wav_u16x8_t: wav_b16x8_and_u16x8, \
        default /* wav_b16x8_t */: wav_b16x8_and)), \
      wav_b32x4_t: (_Generic((b), \
        wav_i32x4_t: wav_b32x4_and_i32x4, \
        wav_u32x4_t: wav_b32x4_and_u32x4, \
        wav_f32x4_t: wav_b32x4_and_f32x4, \
        default /* wav_b32x4_t */: wav_b32x4_and)), \
      wav_b64x2_t: (_Generic((b), \
        wav_i64x2_t: wav_b64x2_and_i64x2, \
        wav_u64x2_t: wav_b64x2_and_u64x2, \
        wav_f64x2_t: wav_b64x2_and_f64x2, \
        default /* wav_b64x2_t */: wav_b64x2_and))) (a, b))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_and(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_and(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_and(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_and(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_and(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_and(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_and(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_and(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_and(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_and(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_and(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_and(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_and(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_and(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_and(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_and(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_and(wav_b8x16_t a, wav_b8x16_t b) { return wav_b8x16_and(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_and(wav_b16x8_t a, wav_b16x8_t b) { return wav_b16x8_and(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_and(wav_b32x4_t a, wav_b32x4_t b) { return wav_b32x4_and(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_and(wav_b64x2_t a, wav_b64x2_t b) { return wav_b64x2_and(a, b); }

  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_and(wav_i8x16_t a, wav_b8x16_t b) { return wav_i8x16_and_b8x16(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_and(wav_i16x8_t a, wav_b16x8_t b) { return wav_i16x8_and_b16x8(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_and(wav_i32x4_t a, wav_b32x4_t b) { return wav_i32x4_and_b32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_and(wav_i64x2_t a, wav_b64x2_t b) { return wav_i64x2_and_b64x2(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_and(wav_u8x16_t a, wav_b8x16_t b) { return wav_u8x16_and_b8x16(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_and(wav_u16x8_t a, wav_b16x8_t b) { return wav_u16x8_and_b16x8(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_and(wav_u32x4_t a, wav_b32x4_t b) { return wav_u32x4_and_b32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_and(wav_u64x2_t a, wav_b64x2_t b) { return wav_u64x2_and_b64x2(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_and(wav_f32x4_t a, wav_b32x4_t b) { return wav_f32x4_and_b32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_and(wav_f64x2_t a, wav_b64x2_t b) { return wav_f64x2_and_b64x2(a, b); }

  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_and(wav_b8x16_t a, wav_i8x16_t b) { return wav_b8x16_and_i8x16(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_and(wav_b16x8_t a, wav_i16x8_t b) { return wav_b16x8_and_i16x8(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_and(wav_b32x4_t a, wav_i32x4_t b) { return wav_b32x4_and_i32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_and(wav_b64x2_t a, wav_i64x2_t b) { return wav_b64x2_and_i64x2(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_and(wav_b8x16_t a, wav_u8x16_t b) { return wav_b8x16_and_u8x16(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_and(wav_b16x8_t a, wav_u16x8_t b) { return wav_b16x8_and_u16x8(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_and(wav_b32x4_t a, wav_u32x4_t b) { return wav_b32x4_and_u32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_and(wav_b64x2_t a, wav_u64x2_t b) { return wav_b64x2_and_u64x2(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_and(wav_b32x4_t a, wav_f32x4_t b) { return wav_b32x4_and_f32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_and(wav_b64x2_t a, wav_f64x2_t b) { return wav_b64x2_and_f64x2(a, b); }

  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t operator&(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_and(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t operator&(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_and(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t operator&(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_and(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t operator&(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_and(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t operator&(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_and(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t operator&(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_and(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t operator&(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_and(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t operator&(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_and(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t operator&(wav_b8x16_t a, wav_b8x16_t b) { return wav_b8x16_and(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t operator&(wav_b16x8_t a, wav_b16x8_t b) { return wav_b16x8_and(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t operator&(wav_b32x4_t a, wav_b32x4_t b) { return wav_b32x4_and(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t operator&(wav_b64x2_t a, wav_b64x2_t b) { return wav_b64x2_and(a, b); }

  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t operator&(wav_i8x16_t a, wav_b8x16_t b) { return wav_i8x16_and_b8x16(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t operator&(wav_i16x8_t a, wav_b16x8_t b) { return wav_i16x8_and_b16x8(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t operator&(wav_i32x4_t a, wav_b32x4_t b) { return wav_i32x4_and_b32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t operator&(wav_i64x2_t a, wav_b64x2_t b) { return wav_i64x2_and_b64x2(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t operator&(wav_u8x16_t a, wav_b8x16_t b) { return wav_u8x16_and_b8x16(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t operator&(wav_u16x8_t a, wav_b16x8_t b) { return wav_u16x8_and_b16x8(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t operator&(wav_u32x4_t a, wav_b32x4_t b) { return wav_u32x4_and_b32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t operator&(wav_u64x2_t a, wav_b64x2_t b) { return wav_u64x2_and_b64x2(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t operator&(wav_f32x4_t a, wav_b32x4_t b) { return wav_f32x4_and_b32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t operator&(wav_f64x2_t a, wav_b64x2_t b) { return wav_f64x2_and_b64x2(a, b); }

  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t operator&(wav_b8x16_t a, wav_i8x16_t b) { return wav_b8x16_and_i8x16(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t operator&(wav_b16x8_t a, wav_i16x8_t b) { return wav_b16x8_and_i16x8(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t operator&(wav_b32x4_t a, wav_i32x4_t b) { return wav_b32x4_and_i32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t operator&(wav_b64x2_t a, wav_i64x2_t b) { return wav_b64x2_and_i64x2(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t operator&(wav_b8x16_t a, wav_u8x16_t b) { return wav_b8x16_and_u8x16(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t operator&(wav_b16x8_t a, wav_u16x8_t b) { return wav_b16x8_and_u16x8(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t operator&(wav_b32x4_t a, wav_u32x4_t b) { return wav_b32x4_and_u32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t operator&(wav_b64x2_t a, wav_u64x2_t b) { return wav_b64x2_and_u64x2(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t operator&(wav_b32x4_t a, wav_f32x4_t b) { return wav_b32x4_and_f32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t operator&(wav_b64x2_t a, wav_f64x2_t b) { return wav_b64x2_and_f64x2(a, b); }
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

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_or(a, b) \
    (_Generic((a), \
      wav_i8x16_t: (_Generic((b), \
        wav_b8x16_t: wav_i8x16_or_b8x16, \
        default /* wav_i8x16_t */: wav_i8x16_or)), \
      wav_i16x8_t: (_Generic((b), \
        wav_b16x8_t: wav_i16x8_or_b16x8, \
        default /* wav_i16x8_t */: wav_i16x8_or)), \
      wav_i32x4_t: (_Generic((b), \
        wav_b32x4_t: wav_i32x4_or_b32x4, \
        default /* wav_i32x4_t */: wav_i32x4_or)), \
      wav_i64x2_t: (_Generic((b), \
        wav_b64x2_t: wav_i64x2_or_b64x2, \
        default /* wav_i64x2_t */: wav_i64x2_or)), \
      wav_u8x16_t: (_Generic((b), \
        wav_b8x16_t: wav_u8x16_or_b8x16, \
        default /* wav_u8x16_t */: wav_u8x16_or)), \
      wav_u16x8_t: (_Generic((b), \
        wav_b16x8_t: wav_u16x8_or_b16x8, \
        default /* wav_u16x8_t */: wav_u16x8_or)), \
      wav_u32x4_t: (_Generic((b), \
        wav_b32x4_t: wav_u32x4_or_b32x4, \
        default /* wav_u32x4_t */: wav_u32x4_or)), \
      wav_u64x2_t: (_Generic((b), \
        wav_b64x2_t: wav_u64x2_or_b64x2, \
        default /* wav_u64x2_t */: wav_u64x2_or)), \
      wav_f32x4_t: wav_f32x4_or_b32x4, \
      wav_f64x2_t: wav_f64x2_or_b64x2, \
      wav_b8x16_t: (_Generic((b), \
        wav_i8x16_t: wav_b8x16_or_i8x16, \
        wav_u8x16_t: wav_b8x16_or_u8x16, \
        default /* wav_b8x16_t */: wav_b8x16_or)), \
      wav_b16x8_t: (_Generic((b), \
        wav_i16x8_t: wav_b16x8_or_i16x8, \
        wav_u16x8_t: wav_b16x8_or_u16x8, \
        default /* wav_b16x8_t */: wav_b16x8_or)), \
      wav_b32x4_t: (_Generic((b), \
        wav_i32x4_t: wav_b32x4_or_i32x4, \
        wav_u32x4_t: wav_b32x4_or_u32x4, \
        wav_f32x4_t: wav_b32x4_or_f32x4, \
        default /* wav_b32x4_t */: wav_b32x4_or)), \
      wav_b64x2_t: (_Generic((b), \
        wav_i64x2_t: wav_b64x2_or_i64x2, \
        wav_u64x2_t: wav_b64x2_or_u64x2, \
        wav_f64x2_t: wav_b64x2_or_f64x2, \
        default /* wav_b64x2_t */: wav_b64x2_or))) (a, b))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_or(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_or(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_or(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_or(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_or(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_or(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_or(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_or(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_or(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_or(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_or(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_or(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_or(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_or(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_or(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_or(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_or(wav_b8x16_t a, wav_b8x16_t b) { return wav_b8x16_or(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_or(wav_b16x8_t a, wav_b16x8_t b) { return wav_b16x8_or(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_or(wav_b32x4_t a, wav_b32x4_t b) { return wav_b32x4_or(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_or(wav_b64x2_t a, wav_b64x2_t b) { return wav_b64x2_or(a, b); }

  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_or(wav_i8x16_t a, wav_b8x16_t b) { return wav_i8x16_or_b8x16(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_or(wav_i16x8_t a, wav_b16x8_t b) { return wav_i16x8_or_b16x8(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_or(wav_i32x4_t a, wav_b32x4_t b) { return wav_i32x4_or_b32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_or(wav_i64x2_t a, wav_b64x2_t b) { return wav_i64x2_or_b64x2(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_or(wav_u8x16_t a, wav_b8x16_t b) { return wav_u8x16_or_b8x16(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_or(wav_u16x8_t a, wav_b16x8_t b) { return wav_u16x8_or_b16x8(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_or(wav_u32x4_t a, wav_b32x4_t b) { return wav_u32x4_or_b32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_or(wav_u64x2_t a, wav_b64x2_t b) { return wav_u64x2_or_b64x2(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_or(wav_f32x4_t a, wav_b32x4_t b) { return wav_f32x4_or_b32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_or(wav_f64x2_t a, wav_b64x2_t b) { return wav_f64x2_or_b64x2(a, b); }

  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_or(wav_b8x16_t a, wav_i8x16_t b) { return wav_b8x16_or_i8x16(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_or(wav_b16x8_t a, wav_i16x8_t b) { return wav_b16x8_or_i16x8(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_or(wav_b32x4_t a, wav_i32x4_t b) { return wav_b32x4_or_i32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_or(wav_b64x2_t a, wav_i64x2_t b) { return wav_b64x2_or_i64x2(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_or(wav_b8x16_t a, wav_u8x16_t b) { return wav_b8x16_or_u8x16(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_or(wav_b16x8_t a, wav_u16x8_t b) { return wav_b16x8_or_u16x8(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_or(wav_b32x4_t a, wav_u32x4_t b) { return wav_b32x4_or_u32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_or(wav_b64x2_t a, wav_u64x2_t b) { return wav_b64x2_or_u64x2(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_or(wav_b32x4_t a, wav_f32x4_t b) { return wav_b32x4_or_f32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_or(wav_b64x2_t a, wav_f64x2_t b) { return wav_b64x2_or_f64x2(a, b); }

  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t operator|(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_or(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t operator|(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_or(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t operator|(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_or(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t operator|(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_or(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t operator|(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_or(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t operator|(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_or(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t operator|(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_or(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t operator|(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_or(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t operator|(wav_b8x16_t a, wav_b8x16_t b) { return wav_b8x16_or(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t operator|(wav_b16x8_t a, wav_b16x8_t b) { return wav_b16x8_or(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t operator|(wav_b32x4_t a, wav_b32x4_t b) { return wav_b32x4_or(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t operator|(wav_b64x2_t a, wav_b64x2_t b) { return wav_b64x2_or(a, b); }

  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t operator|(wav_i8x16_t a, wav_b8x16_t b) { return wav_i8x16_or_b8x16(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t operator|(wav_i16x8_t a, wav_b16x8_t b) { return wav_i16x8_or_b16x8(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t operator|(wav_i32x4_t a, wav_b32x4_t b) { return wav_i32x4_or_b32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t operator|(wav_i64x2_t a, wav_b64x2_t b) { return wav_i64x2_or_b64x2(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t operator|(wav_u8x16_t a, wav_b8x16_t b) { return wav_u8x16_or_b8x16(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t operator|(wav_u16x8_t a, wav_b16x8_t b) { return wav_u16x8_or_b16x8(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t operator|(wav_u32x4_t a, wav_b32x4_t b) { return wav_u32x4_or_b32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t operator|(wav_u64x2_t a, wav_b64x2_t b) { return wav_u64x2_or_b64x2(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t operator|(wav_f32x4_t a, wav_b32x4_t b) { return wav_f32x4_or_b32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t operator|(wav_f64x2_t a, wav_b64x2_t b) { return wav_f64x2_or_b64x2(a, b); }

  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t operator|(wav_b8x16_t a, wav_i8x16_t b) { return wav_b8x16_or_i8x16(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t operator|(wav_b16x8_t a, wav_i16x8_t b) { return wav_b16x8_or_i16x8(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t operator|(wav_b32x4_t a, wav_i32x4_t b) { return wav_b32x4_or_i32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t operator|(wav_b64x2_t a, wav_i64x2_t b) { return wav_b64x2_or_i64x2(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t operator|(wav_b8x16_t a, wav_u8x16_t b) { return wav_b8x16_or_u8x16(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t operator|(wav_b16x8_t a, wav_u16x8_t b) { return wav_b16x8_or_u16x8(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t operator|(wav_b32x4_t a, wav_u32x4_t b) { return wav_b32x4_or_u32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t operator|(wav_b64x2_t a, wav_u64x2_t b) { return wav_b64x2_or_u64x2(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t operator|(wav_b32x4_t a, wav_f32x4_t b) { return wav_b32x4_or_f32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t operator|(wav_b64x2_t a, wav_f64x2_t b) { return wav_b64x2_or_f64x2(a, b); }
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

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_xor(a, b) \
    (_Generic((a), \
      wav_i8x16_t: (_Generic((b), \
        wav_b8x16_t: wav_i8x16_xor_b8x16, \
        default /* wav_i8x16_t */: wav_i8x16_xor)), \
      wav_i16x8_t: (_Generic((b), \
        wav_b16x8_t: wav_i16x8_xor_b16x8, \
        default /* wav_i16x8_t */: wav_i16x8_xor)), \
      wav_i32x4_t: (_Generic((b), \
        wav_b32x4_t: wav_i32x4_xor_b32x4, \
        default /* wav_i32x4_t */: wav_i32x4_xor)), \
      wav_i64x2_t: (_Generic((b), \
        wav_b64x2_t: wav_i64x2_xor_b64x2, \
        default /* wav_i64x2_t */: wav_i64x2_xor)), \
      wav_u8x16_t: (_Generic((b), \
        wav_b8x16_t: wav_u8x16_xor_b8x16, \
        default /* wav_u8x16_t */: wav_u8x16_xor)), \
      wav_u16x8_t: (_Generic((b), \
        wav_b16x8_t: wav_u16x8_xor_b16x8, \
        default /* wav_u16x8_t */: wav_u16x8_xor)), \
      wav_u32x4_t: (_Generic((b), \
        wav_b32x4_t: wav_u32x4_xor_b32x4, \
        default /* wav_u32x4_t */: wav_u32x4_xor)), \
      wav_u64x2_t: (_Generic((b), \
        wav_b64x2_t: wav_u64x2_xor_b64x2, \
        default /* wav_u64x2_t */: wav_u64x2_xor)), \
      wav_f32x4_t: wav_f32x4_xor_b32x4, \
      wav_f64x2_t: wav_f64x2_xor_b64x2, \
      wav_b8x16_t: (_Generic((b), \
        wav_i8x16_t: wav_b8x16_xor_i8x16, \
        wav_u8x16_t: wav_b8x16_xor_u8x16, \
        default /* wav_b8x16_t */: wav_b8x16_xor)), \
      wav_b16x8_t: (_Generic((b), \
        wav_i16x8_t: wav_b16x8_xor_i16x8, \
        wav_u16x8_t: wav_b16x8_xor_u16x8, \
        default /* wav_b16x8_t */: wav_b16x8_xor)), \
      wav_b32x4_t: (_Generic((b), \
        wav_i32x4_t: wav_b32x4_xor_i32x4, \
        wav_u32x4_t: wav_b32x4_xor_u32x4, \
        wav_f32x4_t: wav_b32x4_xor_f32x4, \
        default /* wav_b32x4_t */: wav_b32x4_xor)), \
      wav_b64x2_t: (_Generic((b), \
        wav_i64x2_t: wav_b64x2_xor_i64x2, \
        wav_u64x2_t: wav_b64x2_xor_u64x2, \
        wav_f64x2_t: wav_b64x2_xor_f64x2, \
        default /* wav_b64x2_t */: wav_b64x2_xor))) (a, b))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_xor(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_xor(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_xor(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_xor(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_xor(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_xor(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_xor(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_xor(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_xor(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_xor(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_xor(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_xor(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_xor(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_xor(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_xor(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_xor(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_xor(wav_b8x16_t a, wav_b8x16_t b) { return wav_b8x16_xor(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_xor(wav_b16x8_t a, wav_b16x8_t b) { return wav_b16x8_xor(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_xor(wav_b32x4_t a, wav_b32x4_t b) { return wav_b32x4_xor(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_xor(wav_b64x2_t a, wav_b64x2_t b) { return wav_b64x2_xor(a, b); }

  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_xor(wav_i8x16_t a, wav_b8x16_t b) { return wav_i8x16_xor_b8x16(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_xor(wav_i16x8_t a, wav_b16x8_t b) { return wav_i16x8_xor_b16x8(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_xor(wav_i32x4_t a, wav_b32x4_t b) { return wav_i32x4_xor_b32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_xor(wav_i64x2_t a, wav_b64x2_t b) { return wav_i64x2_xor_b64x2(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_xor(wav_u8x16_t a, wav_b8x16_t b) { return wav_u8x16_xor_b8x16(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_xor(wav_u16x8_t a, wav_b16x8_t b) { return wav_u16x8_xor_b16x8(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_xor(wav_u32x4_t a, wav_b32x4_t b) { return wav_u32x4_xor_b32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_xor(wav_u64x2_t a, wav_b64x2_t b) { return wav_u64x2_xor_b64x2(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_xor(wav_f32x4_t a, wav_b32x4_t b) { return wav_f32x4_xor_b32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_xor(wav_f64x2_t a, wav_b64x2_t b) { return wav_f64x2_xor_b64x2(a, b); }

  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_xor(wav_b8x16_t a, wav_i8x16_t b) { return wav_b8x16_xor_i8x16(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_xor(wav_b16x8_t a, wav_i16x8_t b) { return wav_b16x8_xor_i16x8(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_xor(wav_b32x4_t a, wav_i32x4_t b) { return wav_b32x4_xor_i32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_xor(wav_b64x2_t a, wav_i64x2_t b) { return wav_b64x2_xor_i64x2(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_xor(wav_b8x16_t a, wav_u8x16_t b) { return wav_b8x16_xor_u8x16(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_xor(wav_b16x8_t a, wav_u16x8_t b) { return wav_b16x8_xor_u16x8(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_xor(wav_b32x4_t a, wav_u32x4_t b) { return wav_b32x4_xor_u32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_xor(wav_b64x2_t a, wav_u64x2_t b) { return wav_b64x2_xor_u64x2(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_xor(wav_b32x4_t a, wav_f32x4_t b) { return wav_b32x4_xor_f32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_xor(wav_b64x2_t a, wav_f64x2_t b) { return wav_b64x2_xor_f64x2(a, b); }

  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t operator^(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_xor(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t operator^(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_xor(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t operator^(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_xor(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t operator^(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_xor(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t operator^(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_xor(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t operator^(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_xor(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t operator^(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_xor(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t operator^(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_xor(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t operator^(wav_b8x16_t a, wav_b8x16_t b) { return wav_b8x16_xor(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t operator^(wav_b16x8_t a, wav_b16x8_t b) { return wav_b16x8_xor(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t operator^(wav_b32x4_t a, wav_b32x4_t b) { return wav_b32x4_xor(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t operator^(wav_b64x2_t a, wav_b64x2_t b) { return wav_b64x2_xor(a, b); }

  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t operator^(wav_i8x16_t a, wav_b8x16_t b) { return wav_i8x16_xor_b8x16(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t operator^(wav_i16x8_t a, wav_b16x8_t b) { return wav_i16x8_xor_b16x8(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t operator^(wav_i32x4_t a, wav_b32x4_t b) { return wav_i32x4_xor_b32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t operator^(wav_i64x2_t a, wav_b64x2_t b) { return wav_i64x2_xor_b64x2(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t operator^(wav_u8x16_t a, wav_b8x16_t b) { return wav_u8x16_xor_b8x16(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t operator^(wav_u16x8_t a, wav_b16x8_t b) { return wav_u16x8_xor_b16x8(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t operator^(wav_u32x4_t a, wav_b32x4_t b) { return wav_u32x4_xor_b32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t operator^(wav_u64x2_t a, wav_b64x2_t b) { return wav_u64x2_xor_b64x2(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t operator^(wav_f32x4_t a, wav_b32x4_t b) { return wav_f32x4_xor_b32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t operator^(wav_f64x2_t a, wav_b64x2_t b) { return wav_f64x2_xor_b64x2(a, b); }

  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t operator^(wav_b8x16_t a, wav_i8x16_t b) { return wav_b8x16_xor_i8x16(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t operator^(wav_b16x8_t a, wav_i16x8_t b) { return wav_b16x8_xor_i16x8(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t operator^(wav_b32x4_t a, wav_i32x4_t b) { return wav_b32x4_xor_i32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t operator^(wav_b64x2_t a, wav_i64x2_t b) { return wav_b64x2_xor_i64x2(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t operator^(wav_b8x16_t a, wav_u8x16_t b) { return wav_b8x16_xor_u8x16(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t operator^(wav_b16x8_t a, wav_u16x8_t b) { return wav_b16x8_xor_u16x8(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t operator^(wav_b32x4_t a, wav_u32x4_t b) { return wav_b32x4_xor_u32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t operator^(wav_b64x2_t a, wav_u64x2_t b) { return wav_b64x2_xor_u64x2(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t operator^(wav_b32x4_t a, wav_f32x4_t b) { return wav_b32x4_xor_f32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t operator^(wav_b64x2_t a, wav_f64x2_t b) { return wav_b64x2_xor_f64x2(a, b); }
#endif

/* not -- Bitwise NOT */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_not(wav_i8x16_t a) {
  return (wav_i8x16_t) { ~a.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_not(wav_i16x8_t a) {
  return (wav_i16x8_t) { ~a.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_not(wav_i32x4_t a) {
  return (wav_i32x4_t) { ~a.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_not(wav_i64x2_t a) {
  return (wav_i64x2_t) { ~a.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_not(wav_u8x16_t a) {
  return (wav_u8x16_t) { ~a.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_not(wav_u16x8_t a) {
  return (wav_u16x8_t) { ~a.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_not(wav_u32x4_t a) {
  return (wav_u32x4_t) { ~a.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_not(wav_u64x2_t a) {
  return (wav_u64x2_t) { ~a.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_b8x16_not(wav_b8x16_t a) {
  return (wav_b8x16_t) { ~a.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_b16x8_not(wav_b16x8_t a) {
  return (wav_b16x8_t) { ~a.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_b32x4_not(wav_b32x4_t a) {
  return (wav_b32x4_t) { ~a.values };
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_b64x2_not(wav_b64x2_t a) {
  return (wav_b64x2_t) { ~a.values };
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_not(a) \
    (_Generic((a), \
      wav_i8x16_t: wav_i8x16_not, \
      wav_i16x8_t: wav_i16x8_not, \
      wav_i32x4_t: wav_i32x4_not, \
      wav_i64x2_t: wav_i64x2_not, \
      wav_u8x16_t: wav_u8x16_not, \
      wav_u16x8_t: wav_u16x8_not, \
      wav_u32x4_t: wav_u32x4_not, \
      wav_u64x2_t: wav_u64x2_not, \
      wav_b8x16_t: wav_b8x16_not, \
      wav_b16x8_t: wav_b16x8_not, \
      wav_b32x4_t: wav_b32x4_not, \
      wav_b64x2_t: wav_b64x2_not) (a))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_not(wav_i8x16_t a) { return wav_i8x16_not(a); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_not(wav_i16x8_t a) { return wav_i16x8_not(a); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_not(wav_i32x4_t a) { return wav_i32x4_not(a); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_not(wav_i64x2_t a) { return wav_i64x2_not(a); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_not(wav_u8x16_t a) { return wav_u8x16_not(a); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_not(wav_u16x8_t a) { return wav_u16x8_not(a); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_not(wav_u32x4_t a) { return wav_u32x4_not(a); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_not(wav_u64x2_t a) { return wav_u64x2_not(a); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_not(wav_b8x16_t a) { return wav_b8x16_not(a); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_not(wav_b16x8_t a) { return wav_b16x8_not(a); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_not(wav_b32x4_t a) { return wav_b32x4_not(a); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_not(wav_b64x2_t a) { return wav_b64x2_not(a); }

  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t operator~(wav_i8x16_t a) { return wav_i8x16_not(a); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t operator~(wav_i16x8_t a) { return wav_i16x8_not(a); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t operator~(wav_i32x4_t a) { return wav_i32x4_not(a); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t operator~(wav_i64x2_t a) { return wav_i64x2_not(a); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t operator~(wav_u8x16_t a) { return wav_u8x16_not(a); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t operator~(wav_u16x8_t a) { return wav_u16x8_not(a); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t operator~(wav_u32x4_t a) { return wav_u32x4_not(a); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t operator~(wav_u64x2_t a) { return wav_u64x2_not(a); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t operator~(wav_b8x16_t a) { return wav_b8x16_not(a); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t operator~(wav_b16x8_t a) { return wav_b16x8_not(a); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t operator~(wav_b32x4_t a) { return wav_b32x4_not(a); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t operator~(wav_b64x2_t a) { return wav_b64x2_not(a); }
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

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_andnot(a, b) \
    (_Generic((a), \
      wav_i8x16_t: (_Generic((b), \
        wav_b8x16_t: wav_i8x16_andnot_b8x16, \
        default /* wav_i8x16_t */: wav_i8x16_andnot)), \
      wav_i16x8_t: (_Generic((b), \
        wav_b16x8_t: wav_i16x8_andnot_b16x8, \
        default /* wav_i16x8_t */: wav_i16x8_andnot)), \
      wav_i32x4_t: (_Generic((b), \
        wav_b32x4_t: wav_i32x4_andnot_b32x4, \
        default /* wav_i32x4_t */: wav_i32x4_andnot)), \
      wav_i64x2_t: (_Generic((b), \
        wav_b64x2_t: wav_i64x2_andnot_b64x2, \
        default /* wav_i64x2_t */: wav_i64x2_andnot)), \
      wav_u8x16_t: (_Generic((b), \
        wav_b8x16_t: wav_u8x16_andnot_b8x16, \
        default /* wav_u8x16_t */: wav_u8x16_andnot)), \
      wav_u16x8_t: (_Generic((b), \
        wav_b16x8_t: wav_u16x8_andnot_b16x8, \
        default /* wav_u16x8_t */: wav_u16x8_andnot)), \
      wav_u32x4_t: (_Generic((b), \
        wav_b32x4_t: wav_u32x4_andnot_b32x4, \
        default /* wav_u32x4_t */: wav_u32x4_andnot)), \
      wav_u64x2_t: (_Generic((b), \
        wav_b64x2_t: wav_u64x2_andnot_b64x2, \
        default /* wav_u64x2_t */: wav_u64x2_andnot)), \
      wav_f32x4_t: wav_f32x4_andnot_b32x4, \
      wav_f64x2_t: wav_f64x2_andnot_b64x2, \
      wav_b8x16_t: (_Generic((b), \
        wav_i8x16_t: wav_b8x16_andnot_i8x16, \
        wav_u8x16_t: wav_b8x16_andnot_u8x16, \
        default /* wav_b8x16_t */: wav_b8x16_andnot)), \
      wav_b16x8_t: (_Generic((b), \
        wav_i16x8_t: wav_b16x8_andnot_i16x8, \
        wav_u16x8_t: wav_b16x8_andnot_u16x8, \
        default /* wav_b16x8_t */: wav_b16x8_andnot)), \
      wav_b32x4_t: (_Generic((b), \
        wav_i32x4_t: wav_b32x4_andnot_i32x4, \
        wav_u32x4_t: wav_b32x4_andnot_u32x4, \
        default /* wav_b32x4_t */: wav_b32x4_andnot)), \
      wav_b64x2_t: (_Generic((b), \
        wav_i64x2_t: wav_b64x2_andnot_i64x2, \
        wav_u64x2_t: wav_b64x2_andnot_u64x2, \
        default /* wav_b64x2_t */: wav_b64x2_andnot))) (a, b))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_andnot(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_andnot(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_andnot(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_andnot(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_andnot(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_andnot(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_andnot(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_andnot(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_andnot(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_andnot(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_andnot(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_andnot(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_andnot(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_andnot(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_andnot(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_andnot(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_andnot(wav_b8x16_t a, wav_b8x16_t b) { return wav_b8x16_andnot(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_andnot(wav_b16x8_t a, wav_b16x8_t b) { return wav_b16x8_andnot(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_andnot(wav_b32x4_t a, wav_b32x4_t b) { return wav_b32x4_andnot(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_andnot(wav_b64x2_t a, wav_b64x2_t b) { return wav_b64x2_andnot(a, b); }

  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_andnot(wav_i8x16_t a, wav_b8x16_t b) { return wav_i8x16_andnot_b8x16(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_andnot(wav_i16x8_t a, wav_b16x8_t b) { return wav_i16x8_andnot_b16x8(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_andnot(wav_i32x4_t a, wav_b32x4_t b) { return wav_i32x4_andnot_b32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_andnot(wav_i64x2_t a, wav_b64x2_t b) { return wav_i64x2_andnot_b64x2(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_andnot(wav_u8x16_t a, wav_b8x16_t b) { return wav_u8x16_andnot_b8x16(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_andnot(wav_u16x8_t a, wav_b16x8_t b) { return wav_u16x8_andnot_b16x8(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_andnot(wav_u32x4_t a, wav_b32x4_t b) { return wav_u32x4_andnot_b32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_andnot(wav_u64x2_t a, wav_b64x2_t b) { return wav_u64x2_andnot_b64x2(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_andnot(wav_f32x4_t a, wav_b32x4_t b) { return wav_f32x4_andnot_b32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_andnot(wav_f64x2_t a, wav_b64x2_t b) { return wav_f64x2_andnot_b64x2(a, b); }

  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_andnot(wav_b8x16_t a, wav_i8x16_t b) { return wav_b8x16_andnot_i8x16(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_andnot(wav_b16x8_t a, wav_i16x8_t b) { return wav_b16x8_andnot_i16x8(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_andnot(wav_b32x4_t a, wav_i32x4_t b) { return wav_b32x4_andnot_i32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_andnot(wav_b64x2_t a, wav_i64x2_t b) { return wav_b64x2_andnot_i64x2(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_andnot(wav_b8x16_t a, wav_u8x16_t b) { return wav_b8x16_andnot_u8x16(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_andnot(wav_b16x8_t a, wav_u16x8_t b) { return wav_b16x8_andnot_u16x8(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_andnot(wav_b32x4_t a, wav_u32x4_t b) { return wav_b32x4_andnot_u32x4(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_andnot(wav_b64x2_t a, wav_u64x2_t b) { return wav_b64x2_andnot_u64x2(a, b); }
#endif

/* bitselect -- Bitwise select
 *
 * Use the bits in the control mask c to select the corresponding bit
 * from v1 when 1 and v2 when 0.
 */

WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_bitselect(wav_i8x16_t a, wav_i8x16_t b, wav_i8x16_t c) {
  return (wav_i8x16_t) { (a.values & c.values) ^ (b.values & ~c.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_bitselect(wav_i16x8_t a, wav_i16x8_t b, wav_i16x8_t c) {
  return (wav_i16x8_t) { (a.values & c.values) ^ (b.values & ~c.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_bitselect(wav_i32x4_t a, wav_i32x4_t b, wav_i32x4_t c) {
  return (wav_i32x4_t) { (a.values & c.values) ^ (b.values & ~c.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_bitselect(wav_i64x2_t a, wav_i64x2_t b, wav_i64x2_t c) {
  return (wav_i64x2_t) { (a.values & c.values) ^ (b.values & ~c.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_bitselect(wav_u8x16_t a, wav_u8x16_t b, wav_u8x16_t c) {
  return (wav_u8x16_t) { (a.values & c.values) ^ (b.values & ~c.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_bitselect(wav_u16x8_t a, wav_u16x8_t b, wav_u16x8_t c) {
  return (wav_u16x8_t) { (a.values & c.values) ^ (b.values & ~c.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_bitselect(wav_u32x4_t a, wav_u32x4_t b, wav_u32x4_t c) {
  return (wav_u32x4_t) { (a.values & c.values) ^ (b.values & ~c.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_bitselect(wav_u64x2_t a, wav_u64x2_t b, wav_u64x2_t c) {
  return (wav_u64x2_t) { (a.values & c.values) ^ (b.values & ~c.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_bitselect(wav_f32x4_t a, wav_f32x4_t b, wav_u32x4_t c) {
  return
    wav_u32x4_as_f32x4(
      wav_u32x4_bitselect(
        wav_f32x4_as_u32x4(a),
        wav_f32x4_as_u32x4(b),
        c
      )
    );
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_bitselect(wav_f64x2_t a, wav_f64x2_t b, wav_u64x2_t c) {
  return
    wav_u64x2_as_f64x2(
      wav_u64x2_bitselect(
        wav_f64x2_as_u64x2(a),
        wav_f64x2_as_u64x2(b),
        c
      )
    );
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_bitselect(a, b, c) \
    (_Generic((a), \
      wav_i8x16_t: wav_i8x16_bitselect, \
      wav_i16x8_t: wav_i16x8_bitselect, \
      wav_i32x4_t: wav_i32x4_bitselect, \
      wav_i64x2_t: wav_i64x2_bitselect, \
      wav_u8x16_t: wav_u8x16_bitselect, \
      wav_u16x8_t: wav_u16x8_bitselect, \
      wav_u32x4_t: wav_u32x4_bitselect, \
      wav_u64x2_t: wav_u64x2_bitselect, \
      wav_f32x4_t: wav_f32x4_bitselect, \
      wav_f64x2_t: wav_f64x2_bitselect) (a, b, c))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_bitselect(wav_i8x16_t a, wav_i8x16_t b, wav_i8x16_t c) { return wav_i8x16_bitselect(a, b, c); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_bitselect(wav_i16x8_t a, wav_i16x8_t b, wav_i16x8_t c) { return wav_i16x8_bitselect(a, b, c); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_bitselect(wav_i32x4_t a, wav_i32x4_t b, wav_i32x4_t c) { return wav_i32x4_bitselect(a, b, c); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_bitselect(wav_i64x2_t a, wav_i64x2_t b, wav_i64x2_t c) { return wav_i64x2_bitselect(a, b, c); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_bitselect(wav_u8x16_t a, wav_u8x16_t b, wav_u8x16_t c) { return wav_u8x16_bitselect(a, b, c); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_bitselect(wav_u16x8_t a, wav_u16x8_t b, wav_u16x8_t c) { return wav_u16x8_bitselect(a, b, c); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_bitselect(wav_u32x4_t a, wav_u32x4_t b, wav_u32x4_t c) { return wav_u32x4_bitselect(a, b, c); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_bitselect(wav_u64x2_t a, wav_u64x2_t b, wav_u64x2_t c) { return wav_u64x2_bitselect(a, b, c); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_bitselect(wav_f32x4_t a, wav_f32x4_t b, wav_u32x4_t c) { return wav_f32x4_bitselect(a, b, c); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_bitselect(wav_f64x2_t a, wav_f64x2_t b, wav_u64x2_t c) { return wav_f64x2_bitselect(a, b, c); }
#endif

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
wav_i8x16_blend(wav_i8x16_t a, wav_i8x16_t b, wav_b8x16_t c) {
  return (wav_i8x16_t) { (a.values & c.values) ^ (b.values & ~c.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i16x8_blend(wav_i16x8_t a, wav_i16x8_t b, wav_b16x8_t c) {
  return (wav_i16x8_t) { (a.values & c.values) ^ (b.values & ~c.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_blend(wav_i32x4_t a, wav_i32x4_t b, wav_b32x4_t c) {
  return (wav_i32x4_t) { (a.values & c.values) ^ (b.values & ~c.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_blend(wav_i64x2_t a, wav_i64x2_t b, wav_b64x2_t c) {
  return (wav_i64x2_t) { (a.values & c.values) ^ (b.values & ~c.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_blend(wav_u8x16_t a, wav_u8x16_t b, wav_b8x16_t c) {
  return (wav_u8x16_t) { (a.values & c.values) ^ (b.values & ~c.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u16x8_blend(wav_u16x8_t a, wav_u16x8_t b, wav_b16x8_t c) {
  return (wav_u16x8_t) { (a.values & c.values) ^ (b.values & ~c.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_blend(wav_u32x4_t a, wav_u32x4_t b, wav_b32x4_t c) {
  return (wav_u32x4_t) { (a.values & c.values) ^ (b.values & ~c.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_blend(wav_u64x2_t a, wav_u64x2_t b, wav_b64x2_t c) {
  return (wav_u64x2_t) { (a.values & c.values) ^ (b.values & ~c.values) };
}

WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_blend(wav_f32x4_t a, wav_f32x4_t b, wav_b32x4_t c) {
  return
    wav_u32x4_as_f32x4(
      wav_u32x4_blend(
        wav_f32x4_as_u32x4(a),
        wav_f32x4_as_u32x4(b),
        c
      )
    );
}

WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_blend(wav_f64x2_t a, wav_f64x2_t b, wav_b64x2_t c) {
  return
    wav_u64x2_as_f64x2(
      wav_u64x2_blend(
        wav_f64x2_as_u64x2(a),
        wav_f64x2_as_u64x2(b),
        c
      )
    );
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_blend(a, b, c) \
    (_Generic((a), \
      wav_i8x16_t: wav_i8x16_blend, \
      wav_i16x8_t: wav_i16x8_blend, \
      wav_i32x4_t: wav_i32x4_blend, \
      wav_i64x2_t: wav_i64x2_blend, \
      wav_u8x16_t: wav_u8x16_blend, \
      wav_u16x8_t: wav_u16x8_blend, \
      wav_u32x4_t: wav_u32x4_blend, \
      wav_u64x2_t: wav_u64x2_blend, \
      wav_f32x4_t: wav_f32x4_blend, \
      wav_f64x2_t: wav_f64x2_blend) (a, b, c))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_blend(wav_i8x16_t a, wav_i8x16_t b, wav_b8x16_t c) { return wav_i8x16_blend(a, b, c); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_blend(wav_i16x8_t a, wav_i16x8_t b, wav_b16x8_t c) { return wav_i16x8_blend(a, b, c); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_blend(wav_i32x4_t a, wav_i32x4_t b, wav_b32x4_t c) { return wav_i32x4_blend(a, b, c); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_blend(wav_i64x2_t a, wav_i64x2_t b, wav_b64x2_t c) { return wav_i64x2_blend(a, b, c); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_blend(wav_u8x16_t a, wav_u8x16_t b, wav_b8x16_t c) { return wav_u8x16_blend(a, b, c); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_blend(wav_u16x8_t a, wav_u16x8_t b, wav_b16x8_t c) { return wav_u16x8_blend(a, b, c); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_blend(wav_u32x4_t a, wav_u32x4_t b, wav_b32x4_t c) { return wav_u32x4_blend(a, b, c); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_blend(wav_u64x2_t a, wav_u64x2_t b, wav_b64x2_t c) { return wav_u64x2_blend(a, b, c); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_blend(wav_f32x4_t a, wav_f32x4_t b, wav_b32x4_t c) { return wav_f32x4_blend(a, b, c); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_blend(wav_f64x2_t a, wav_f64x2_t b, wav_b64x2_t c) { return wav_f64x2_blend(a, b, c); }
#endif

/* popcnt -- Lane-wise Population Count
 *
 * Count the number of bits set to one within each lane.
 */

WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WAV_FUNCTION_ATTRIBUTES
wav_i8x16_t
wav_i8x16_popcount(wav_i8x16_t a) {
  wav_i8x16_t r;

  for (__SIZE_TYPE__ i = 0 ; i < (sizeof(r.values) / sizeof(r.values[i])) ; i++) {
    r.values[i] = (int8_t) __builtin_popcount((unsigned int) a.values[i]);
  }

  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wconditional-uninitialized" /* ??? */
  return r;
  #pragma clang diagnostic pop
}

WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WAV_FUNCTION_ATTRIBUTES
wav_u8x16_t
wav_u8x16_popcount(wav_u8x16_t a) {
  wav_u8x16_t r;

  for (__SIZE_TYPE__ i = 0 ; i < (sizeof(r.values) / sizeof(r.values[i])) ; i++) {
    r.values[i] = (uint8_t) __builtin_popcount((unsigned int) a.values[i]);
  }

  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wconditional-uninitialized" /* ??? */
  return r;
  #pragma clang diagnostic pop
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_popcount(a) \
    (_Generic((a), \
      wav_i8x16_t: wav_i8x16_popcount, \
      wav_u8x16_t: wav_u8x16_popcount) (a))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_popcount(wav_i8x16_t a) { return wav_i8x16_popcount(a); }
  WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_popcount(wav_u8x16_t a) { return wav_u8x16_popcount(a); }
#endif

/* cmpeq
 *
 * Compares operands, returning all ones if the elements are equal, or
 * all zeros if inequal.
 */

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_i8x16_cmpeq(wav_i8x16_t a, wav_i8x16_t b) {
  wav_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values == b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_i16x8_cmpeq(wav_i16x8_t a, wav_i16x8_t b) {
  wav_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values == b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_i32x4_cmpeq(wav_i32x4_t a, wav_i32x4_t b) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values == b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_i64x2_cmpeq(wav_i64x2_t a, wav_i64x2_t b) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values == b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_u8x16_cmpeq(wav_u8x16_t a, wav_u8x16_t b) {
  wav_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values == b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_u16x8_cmpeq(wav_u16x8_t a, wav_u16x8_t b) {
  wav_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values == b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_u32x4_cmpeq(wav_u32x4_t a, wav_u32x4_t b) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values == b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_u64x2_cmpeq(wav_u64x2_t a, wav_u64x2_t b) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values == b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_b8x16_cmpeq(wav_b8x16_t a, wav_b8x16_t b) {
  wav_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values == b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_b16x8_cmpeq(wav_b16x8_t a, wav_b16x8_t b) {
  wav_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values == b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_b32x4_cmpeq(wav_b32x4_t a, wav_b32x4_t b) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values == b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_b64x2_cmpeq(wav_b64x2_t a, wav_b64x2_t b) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values == b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_f32x4_cmpeq(wav_f32x4_t a, wav_f32x4_t b) {
  wav_b32x4_t r;
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wfloat-equal"
  r.values = (__typeof__(r.values)) (a.values == b.values);
  #pragma clang diagnostic pop
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_f64x2_cmpeq(wav_f64x2_t a, wav_f64x2_t b) {
  wav_b64x2_t r;
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wfloat-equal"
  r.values = (__typeof__(r.values)) (a.values == b.values);
  #pragma clang diagnostic pop
  return r;
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_cmpeq(a, b) \
    (_Generic((a), \
      wav_i8x16_t: wav_i8x16_cmpeq, \
      wav_i16x8_t: wav_i16x8_cmpeq, \
      wav_i32x4_t: wav_i32x4_cmpeq, \
      wav_i64x2_t: wav_i64x2_cmpeq, \
      wav_u8x16_t: wav_u8x16_cmpeq, \
      wav_u16x8_t: wav_u16x8_cmpeq, \
      wav_u32x4_t: wav_u32x4_cmpeq, \
      wav_u64x2_t: wav_u64x2_cmpeq, \
      wav_b8x16_t: wav_b8x16_cmpeq, \
      wav_b16x8_t: wav_b16x8_cmpeq, \
      wav_b32x4_t: wav_b32x4_cmpeq, \
      wav_b64x2_t: wav_b64x2_cmpeq, \
      wav_f32x4_t: wav_f32x4_cmpeq, \
      wav_f64x2_t: wav_f64x2_cmpeq) (a, b))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_cmpeq(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_cmpeq(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_cmpeq(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_cmpeq(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_cmpeq(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_cmpeq(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_cmpeq(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_cmpeq(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_cmpeq(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_cmpeq(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_cmpeq(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_cmpeq(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_cmpeq(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_cmpeq(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_cmpeq(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_cmpeq(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_cmpeq(wav_b8x16_t a, wav_b8x16_t b) { return wav_b8x16_cmpeq(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_cmpeq(wav_b16x8_t a, wav_b16x8_t b) { return wav_b16x8_cmpeq(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_cmpeq(wav_b32x4_t a, wav_b32x4_t b) { return wav_b32x4_cmpeq(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_cmpeq(wav_b64x2_t a, wav_b64x2_t b) { return wav_b64x2_cmpeq(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_cmpeq(wav_f32x4_t a, wav_f32x4_t b) { return wav_f32x4_cmpeq(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_cmpeq(wav_f64x2_t a, wav_f64x2_t b) { return wav_f64x2_cmpeq(a, b); }

  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t operator==(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_cmpeq(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t operator==(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_cmpeq(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t operator==(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_cmpeq(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t operator==(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_cmpeq(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t operator==(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_cmpeq(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t operator==(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_cmpeq(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t operator==(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_cmpeq(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t operator==(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_cmpeq(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t operator==(wav_b8x16_t a, wav_b8x16_t b) { return wav_b8x16_cmpeq(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t operator==(wav_b16x8_t a, wav_b16x8_t b) { return wav_b16x8_cmpeq(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t operator==(wav_b32x4_t a, wav_b32x4_t b) { return wav_b32x4_cmpeq(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t operator==(wav_b64x2_t a, wav_b64x2_t b) { return wav_b64x2_cmpeq(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t operator==(wav_f32x4_t a, wav_f32x4_t b) { return wav_f32x4_cmpeq(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t operator==(wav_f64x2_t a, wav_f64x2_t b) { return wav_f64x2_cmpeq(a, b); }
#endif

/* cmpne
 *
 * The ne operations produce the inverse of their eq counterparts.
 */

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_i8x16_cmpne(wav_i8x16_t a, wav_i8x16_t b) {
  wav_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values != b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_i16x8_cmpne(wav_i16x8_t a, wav_i16x8_t b) {
  wav_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values != b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_i32x4_cmpne(wav_i32x4_t a, wav_i32x4_t b) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values != b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_i64x2_cmpne(wav_i64x2_t a, wav_i64x2_t b) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values != b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_u8x16_cmpne(wav_u8x16_t a, wav_u8x16_t b) {
  wav_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values != b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_u16x8_cmpne(wav_u16x8_t a, wav_u16x8_t b) {
  wav_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values != b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_u32x4_cmpne(wav_u32x4_t a, wav_u32x4_t b) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values != b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_u64x2_cmpne(wav_u64x2_t a, wav_u64x2_t b) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values != b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_b8x16_cmpne(wav_b8x16_t a, wav_b8x16_t b) {
  wav_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values != b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_b16x8_cmpne(wav_b16x8_t a, wav_b16x8_t b) {
  wav_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values != b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_b32x4_cmpne(wav_b32x4_t a, wav_b32x4_t b) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values != b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_b64x2_cmpne(wav_b64x2_t a, wav_b64x2_t b) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values != b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_f32x4_cmpne(wav_f32x4_t a, wav_f32x4_t b) {
  wav_b32x4_t r;
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wfloat-equal"
  r.values = (__typeof__(r.values)) (a.values != b.values);
  #pragma clang diagnostic pop
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_f64x2_cmpne(wav_f64x2_t a, wav_f64x2_t b) {
  wav_b64x2_t r;
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wfloat-equal"
  r.values = (__typeof__(r.values)) (a.values != b.values);
  #pragma clang diagnostic pop
  return r;
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_cmpne(a, b) \
    (_Generic((a), \
      wav_i8x16_t: wav_i8x16_cmpne, \
      wav_i16x8_t: wav_i16x8_cmpne, \
      wav_i32x4_t: wav_i32x4_cmpne, \
      wav_i64x2_t: wav_i64x2_cmpne, \
      wav_u8x16_t: wav_u8x16_cmpne, \
      wav_u16x8_t: wav_u16x8_cmpne, \
      wav_u32x4_t: wav_u32x4_cmpne, \
      wav_u64x2_t: wav_u64x2_cmpne, \
      wav_b8x16_t: wav_b8x16_cmpne, \
      wav_b16x8_t: wav_b16x8_cmpne, \
      wav_b32x4_t: wav_b32x4_cmpne, \
      wav_b64x2_t: wav_b64x2_cmpne, \
      wav_f32x4_t: wav_f32x4_cmpne, \
      wav_f64x2_t: wav_f64x2_cmpne) (a, b))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_cmpne(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_cmpne(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_cmpne(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_cmpne(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_cmpne(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_cmpne(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_cmpne(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_cmpne(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_cmpne(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_cmpne(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_cmpne(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_cmpne(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_cmpne(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_cmpne(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_cmpne(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_cmpne(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_cmpne(wav_b8x16_t a, wav_b8x16_t b) { return wav_b8x16_cmpne(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_cmpne(wav_b16x8_t a, wav_b16x8_t b) { return wav_b16x8_cmpne(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_cmpne(wav_b32x4_t a, wav_b32x4_t b) { return wav_b32x4_cmpne(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_cmpne(wav_b64x2_t a, wav_b64x2_t b) { return wav_b64x2_cmpne(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_cmpne(wav_f32x4_t a, wav_f32x4_t b) { return wav_f32x4_cmpne(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_cmpne(wav_f64x2_t a, wav_f64x2_t b) { return wav_f64x2_cmpne(a, b); }

  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t operator!=(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_cmpne(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t operator!=(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_cmpne(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t operator!=(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_cmpne(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t operator!=(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_cmpne(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t operator!=(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_cmpne(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t operator!=(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_cmpne(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t operator!=(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_cmpne(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t operator!=(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_cmpne(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t operator!=(wav_b8x16_t a, wav_b8x16_t b) { return wav_b8x16_cmpne(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t operator!=(wav_b16x8_t a, wav_b16x8_t b) { return wav_b16x8_cmpne(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t operator!=(wav_b32x4_t a, wav_b32x4_t b) { return wav_b32x4_cmpne(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t operator!=(wav_b64x2_t a, wav_b64x2_t b) { return wav_b64x2_cmpne(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t operator!=(wav_f32x4_t a, wav_f32x4_t b) { return wav_f32x4_cmpne(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t operator!=(wav_f64x2_t a, wav_f64x2_t b) { return wav_f64x2_cmpne(a, b); }
#endif

/* cmplt */

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_i8x16_cmplt(wav_i8x16_t a, wav_i8x16_t b) {
  wav_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values < b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_i16x8_cmplt(wav_i16x8_t a, wav_i16x8_t b) {
  wav_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values < b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_i32x4_cmplt(wav_i32x4_t a, wav_i32x4_t b) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values < b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_i64x2_cmplt(wav_i64x2_t a, wav_i64x2_t b) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values < b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_u8x16_cmplt(wav_u8x16_t a, wav_u8x16_t b) {
  wav_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values < b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_u16x8_cmplt(wav_u16x8_t a, wav_u16x8_t b) {
  wav_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values < b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_u32x4_cmplt(wav_u32x4_t a, wav_u32x4_t b) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values < b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_u64x2_cmplt(wav_u64x2_t a, wav_u64x2_t b) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values < b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_b8x16_cmplt(wav_b8x16_t a, wav_b8x16_t b) {
  wav_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values < b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_b16x8_cmplt(wav_b16x8_t a, wav_b16x8_t b) {
  wav_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values < b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_b32x4_cmplt(wav_b32x4_t a, wav_b32x4_t b) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values < b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_b64x2_cmplt(wav_b64x2_t a, wav_b64x2_t b) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values < b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_f32x4_cmplt(wav_f32x4_t a, wav_f32x4_t b) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values < b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_f64x2_cmplt(wav_f64x2_t a, wav_f64x2_t b) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values < b.values);
  return r;
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_cmplt(a, b) \
    (_Generic((a), \
      wav_i8x16_t: wav_i8x16_cmplt, \
      wav_i16x8_t: wav_i16x8_cmplt, \
      wav_i32x4_t: wav_i32x4_cmplt, \
      wav_i64x2_t: wav_i64x2_cmplt, \
      wav_u8x16_t: wav_u8x16_cmplt, \
      wav_u16x8_t: wav_u16x8_cmplt, \
      wav_u32x4_t: wav_u32x4_cmplt, \
      wav_u64x2_t: wav_u64x2_cmplt, \
      wav_b8x16_t: wav_b8x16_cmplt, \
      wav_b16x8_t: wav_b16x8_cmplt, \
      wav_b32x4_t: wav_b32x4_cmplt, \
      wav_b64x2_t: wav_b64x2_cmplt, \
      wav_f32x4_t: wav_f32x4_cmplt, \
      wav_f64x2_t: wav_f64x2_cmplt) (a, b))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_cmplt(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_cmplt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_cmplt(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_cmplt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_cmplt(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_cmplt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_cmplt(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_cmplt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_cmplt(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_cmplt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_cmplt(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_cmplt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_cmplt(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_cmplt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_cmplt(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_cmplt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_cmplt(wav_b8x16_t a, wav_b8x16_t b) { return wav_b8x16_cmplt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_cmplt(wav_b16x8_t a, wav_b16x8_t b) { return wav_b16x8_cmplt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_cmplt(wav_b32x4_t a, wav_b32x4_t b) { return wav_b32x4_cmplt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_cmplt(wav_b64x2_t a, wav_b64x2_t b) { return wav_b64x2_cmplt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_cmplt(wav_f32x4_t a, wav_f32x4_t b) { return wav_f32x4_cmplt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_cmplt(wav_f64x2_t a, wav_f64x2_t b) { return wav_f64x2_cmplt(a, b); }

  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t operator<(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_cmplt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t operator<(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_cmplt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t operator<(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_cmplt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t operator<(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_cmplt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t operator<(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_cmplt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t operator<(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_cmplt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t operator<(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_cmplt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t operator<(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_cmplt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t operator<(wav_b8x16_t a, wav_b8x16_t b) { return wav_b8x16_cmplt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t operator<(wav_b16x8_t a, wav_b16x8_t b) { return wav_b16x8_cmplt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t operator<(wav_b32x4_t a, wav_b32x4_t b) { return wav_b32x4_cmplt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t operator<(wav_b64x2_t a, wav_b64x2_t b) { return wav_b64x2_cmplt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t operator<(wav_f32x4_t a, wav_f32x4_t b) { return wav_f32x4_cmplt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t operator<(wav_f64x2_t a, wav_f64x2_t b) { return wav_f64x2_cmplt(a, b); }
#endif

/* cmple */

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_i8x16_cmple(wav_i8x16_t a, wav_i8x16_t b) {
  wav_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values <= b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_i16x8_cmple(wav_i16x8_t a, wav_i16x8_t b) {
  wav_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values <= b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_i32x4_cmple(wav_i32x4_t a, wav_i32x4_t b) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values <= b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_i64x2_cmple(wav_i64x2_t a, wav_i64x2_t b) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values <= b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_u8x16_cmple(wav_u8x16_t a, wav_u8x16_t b) {
  wav_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values <= b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_u16x8_cmple(wav_u16x8_t a, wav_u16x8_t b) {
  wav_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values <= b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_u32x4_cmple(wav_u32x4_t a, wav_u32x4_t b) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values <= b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_u64x2_cmple(wav_u64x2_t a, wav_u64x2_t b) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values <= b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_b8x16_cmple(wav_b8x16_t a, wav_b8x16_t b) {
  wav_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values <= b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_b16x8_cmple(wav_b16x8_t a, wav_b16x8_t b) {
  wav_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values <= b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_b32x4_cmple(wav_b32x4_t a, wav_b32x4_t b) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values <= b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_b64x2_cmple(wav_b64x2_t a, wav_b64x2_t b) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values <= b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_f32x4_cmple(wav_f32x4_t a, wav_f32x4_t b) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values <= b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_f64x2_cmple(wav_f64x2_t a, wav_f64x2_t b) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values <= b.values);
  return r;
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_cmple(a, b) \
    (_Generic((a), \
      wav_i8x16_t: wav_i8x16_cmple, \
      wav_i16x8_t: wav_i16x8_cmple, \
      wav_i32x4_t: wav_i32x4_cmple, \
      wav_i64x2_t: wav_i64x2_cmple, \
      wav_u8x16_t: wav_u8x16_cmple, \
      wav_u16x8_t: wav_u16x8_cmple, \
      wav_u32x4_t: wav_u32x4_cmple, \
      wav_u64x2_t: wav_u64x2_cmple, \
      wav_b8x16_t: wav_b8x16_cmple, \
      wav_b16x8_t: wav_b16x8_cmple, \
      wav_b32x4_t: wav_b32x4_cmple, \
      wav_b64x2_t: wav_b64x2_cmple, \
      wav_f32x4_t: wav_f32x4_cmple, \
      wav_f64x2_t: wav_f64x2_cmple) (a, b))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_cmple(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_cmple(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_cmple(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_cmple(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_cmple(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_cmple(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_cmple(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_cmple(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_cmple(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_cmple(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_cmple(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_cmple(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_cmple(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_cmple(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_cmple(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_cmple(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_cmple(wav_b8x16_t a, wav_b8x16_t b) { return wav_b8x16_cmple(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_cmple(wav_b16x8_t a, wav_b16x8_t b) { return wav_b16x8_cmple(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_cmple(wav_b32x4_t a, wav_b32x4_t b) { return wav_b32x4_cmple(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_cmple(wav_b64x2_t a, wav_b64x2_t b) { return wav_b64x2_cmple(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_cmple(wav_f32x4_t a, wav_f32x4_t b) { return wav_f32x4_cmple(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_cmple(wav_f64x2_t a, wav_f64x2_t b) { return wav_f64x2_cmple(a, b); }

  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t operator<=(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_cmple(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t operator<=(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_cmple(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t operator<=(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_cmple(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t operator<=(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_cmple(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t operator<=(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_cmple(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t operator<=(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_cmple(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t operator<=(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_cmple(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t operator<=(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_cmple(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t operator<=(wav_b8x16_t a, wav_b8x16_t b) { return wav_b8x16_cmple(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t operator<=(wav_b16x8_t a, wav_b16x8_t b) { return wav_b16x8_cmple(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t operator<=(wav_b32x4_t a, wav_b32x4_t b) { return wav_b32x4_cmple(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t operator<=(wav_b64x2_t a, wav_b64x2_t b) { return wav_b64x2_cmple(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t operator<=(wav_f32x4_t a, wav_f32x4_t b) { return wav_f32x4_cmple(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t operator<=(wav_f64x2_t a, wav_f64x2_t b) { return wav_f64x2_cmple(a, b); }
#endif

/* cmpgt */

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_i8x16_cmpgt(wav_i8x16_t a, wav_i8x16_t b) {
  wav_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values > b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_i16x8_cmpgt(wav_i16x8_t a, wav_i16x8_t b) {
  wav_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values > b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_i32x4_cmpgt(wav_i32x4_t a, wav_i32x4_t b) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values > b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_i64x2_cmpgt(wav_i64x2_t a, wav_i64x2_t b) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values > b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_u8x16_cmpgt(wav_u8x16_t a, wav_u8x16_t b) {
  wav_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values > b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_u16x8_cmpgt(wav_u16x8_t a, wav_u16x8_t b) {
  wav_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values > b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_u32x4_cmpgt(wav_u32x4_t a, wav_u32x4_t b) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values > b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_u64x2_cmpgt(wav_u64x2_t a, wav_u64x2_t b) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values > b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_b8x16_cmpgt(wav_b8x16_t a, wav_b8x16_t b) {
  wav_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values > b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_b16x8_cmpgt(wav_b16x8_t a, wav_b16x8_t b) {
  wav_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values > b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_b32x4_cmpgt(wav_b32x4_t a, wav_b32x4_t b) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values > b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_b64x2_cmpgt(wav_b64x2_t a, wav_b64x2_t b) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values > b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_f32x4_cmpgt(wav_f32x4_t a, wav_f32x4_t b) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values > b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_f64x2_cmpgt(wav_f64x2_t a, wav_f64x2_t b) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values > b.values);
  return r;
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_cmpgt(a, b) \
    (_Generic((a), \
      wav_i8x16_t: wav_i8x16_cmpgt, \
      wav_i16x8_t: wav_i16x8_cmpgt, \
      wav_i32x4_t: wav_i32x4_cmpgt, \
      wav_i64x2_t: wav_i64x2_cmpgt, \
      wav_u8x16_t: wav_u8x16_cmpgt, \
      wav_u16x8_t: wav_u16x8_cmpgt, \
      wav_u32x4_t: wav_u32x4_cmpgt, \
      wav_u64x2_t: wav_u64x2_cmpgt, \
      wav_b8x16_t: wav_b8x16_cmpgt, \
      wav_b16x8_t: wav_b16x8_cmpgt, \
      wav_b32x4_t: wav_b32x4_cmpgt, \
      wav_b64x2_t: wav_b64x2_cmpgt, \
      wav_f32x4_t: wav_f32x4_cmpgt, \
      wav_f64x2_t: wav_f64x2_cmpgt) (a, b))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_cmpgt(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_cmpgt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_cmpgt(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_cmpgt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_cmpgt(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_cmpgt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_cmpgt(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_cmpgt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_cmpgt(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_cmpgt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_cmpgt(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_cmpgt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_cmpgt(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_cmpgt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_cmpgt(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_cmpgt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_cmpgt(wav_b8x16_t a, wav_b8x16_t b) { return wav_b8x16_cmpgt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_cmpgt(wav_b16x8_t a, wav_b16x8_t b) { return wav_b16x8_cmpgt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_cmpgt(wav_b32x4_t a, wav_b32x4_t b) { return wav_b32x4_cmpgt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_cmpgt(wav_b64x2_t a, wav_b64x2_t b) { return wav_b64x2_cmpgt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_cmpgt(wav_f32x4_t a, wav_f32x4_t b) { return wav_f32x4_cmpgt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_cmpgt(wav_f64x2_t a, wav_f64x2_t b) { return wav_f64x2_cmpgt(a, b); }

  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t operator>(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_cmpgt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t operator>(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_cmpgt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t operator>(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_cmpgt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t operator>(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_cmpgt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t operator>(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_cmpgt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t operator>(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_cmpgt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t operator>(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_cmpgt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t operator>(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_cmpgt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t operator>(wav_b8x16_t a, wav_b8x16_t b) { return wav_b8x16_cmpgt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t operator>(wav_b16x8_t a, wav_b16x8_t b) { return wav_b16x8_cmpgt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t operator>(wav_b32x4_t a, wav_b32x4_t b) { return wav_b32x4_cmpgt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t operator>(wav_b64x2_t a, wav_b64x2_t b) { return wav_b64x2_cmpgt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t operator>(wav_f32x4_t a, wav_f32x4_t b) { return wav_f32x4_cmpgt(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t operator>(wav_f64x2_t a, wav_f64x2_t b) { return wav_f64x2_cmpgt(a, b); }
#endif

/* cmpge */

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_i8x16_cmpge(wav_i8x16_t a, wav_i8x16_t b) {
  wav_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values >= b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_i16x8_cmpge(wav_i16x8_t a, wav_i16x8_t b) {
  wav_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values >= b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_i32x4_cmpge(wav_i32x4_t a, wav_i32x4_t b) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values >= b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_i64x2_cmpge(wav_i64x2_t a, wav_i64x2_t b) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values >= b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_u8x16_cmpge(wav_u8x16_t a, wav_u8x16_t b) {
  wav_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values >= b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_u16x8_cmpge(wav_u16x8_t a, wav_u16x8_t b) {
  wav_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values >= b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_u32x4_cmpge(wav_u32x4_t a, wav_u32x4_t b) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values >= b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_u64x2_cmpge(wav_u64x2_t a, wav_u64x2_t b) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values >= b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b8x16_t
wav_b8x16_cmpge(wav_b8x16_t a, wav_b8x16_t b) {
  wav_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values >= b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b16x8_t
wav_b16x8_cmpge(wav_b16x8_t a, wav_b16x8_t b) {
  wav_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values >= b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_b32x4_cmpge(wav_b32x4_t a, wav_b32x4_t b) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values >= b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_b64x2_cmpge(wav_b64x2_t a, wav_b64x2_t b) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values >= b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b32x4_t
wav_f32x4_cmpge(wav_f32x4_t a, wav_f32x4_t b) {
  wav_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values >= b.values);
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_b64x2_t
wav_f64x2_cmpge(wav_f64x2_t a, wav_f64x2_t b) {
  wav_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values >= b.values);
  return r;
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_cmpge(a, b) \
    (_Generic((a), \
      wav_i8x16_t: wav_i8x16_cmpge, \
      wav_i16x8_t: wav_i16x8_cmpge, \
      wav_i32x4_t: wav_i32x4_cmpge, \
      wav_i64x2_t: wav_i64x2_cmpge, \
      wav_u8x16_t: wav_u8x16_cmpge, \
      wav_u16x8_t: wav_u16x8_cmpge, \
      wav_u32x4_t: wav_u32x4_cmpge, \
      wav_u64x2_t: wav_u64x2_cmpge, \
      wav_b8x16_t: wav_b8x16_cmpge, \
      wav_b16x8_t: wav_b16x8_cmpge, \
      wav_b32x4_t: wav_b32x4_cmpge, \
      wav_b64x2_t: wav_b64x2_cmpge, \
      wav_f32x4_t: wav_f32x4_cmpge, \
      wav_f64x2_t: wav_f64x2_cmpge) (a, b))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_cmpge(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_cmpge(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_cmpge(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_cmpge(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_cmpge(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_cmpge(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_cmpge(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_cmpge(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_cmpge(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_cmpge(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_cmpge(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_cmpge(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_cmpge(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_cmpge(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_cmpge(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_cmpge(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t wav_cmpge(wav_b8x16_t a, wav_b8x16_t b) { return wav_b8x16_cmpge(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t wav_cmpge(wav_b16x8_t a, wav_b16x8_t b) { return wav_b16x8_cmpge(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_cmpge(wav_b32x4_t a, wav_b32x4_t b) { return wav_b32x4_cmpge(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_cmpge(wav_b64x2_t a, wav_b64x2_t b) { return wav_b64x2_cmpge(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t wav_cmpge(wav_f32x4_t a, wav_f32x4_t b) { return wav_f32x4_cmpge(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t wav_cmpge(wav_f64x2_t a, wav_f64x2_t b) { return wav_f64x2_cmpge(a, b); }

  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t operator>=(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_cmpge(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t operator>=(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_cmpge(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t operator>=(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_cmpge(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t operator>=(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_cmpge(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t operator>=(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_cmpge(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t operator>=(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_cmpge(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t operator>=(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_cmpge(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t operator>=(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_cmpge(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b8x16_t operator>=(wav_b8x16_t a, wav_b8x16_t b) { return wav_b8x16_cmpge(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b16x8_t operator>=(wav_b16x8_t a, wav_b16x8_t b) { return wav_b16x8_cmpge(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t operator>=(wav_b32x4_t a, wav_b32x4_t b) { return wav_b32x4_cmpge(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t operator>=(wav_b64x2_t a, wav_b64x2_t b) { return wav_b64x2_cmpge(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b32x4_t operator>=(wav_f32x4_t a, wav_f32x4_t b) { return wav_f32x4_cmpge(a, b); }
  WAV_FUNCTION_ATTRIBUTES wav_b64x2_t operator>=(wav_f64x2_t a, wav_f64x2_t b) { return wav_f64x2_cmpge(a, b); }
#endif

/* load_zero -- Load and Zero-Pad
 *
 * Load a single 32-bit or 64-bit element into the lowest bits of a
 * vector, and initialize all other bits of the vector to zero.
 */

WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_load_zero(const int32_t * a) {
  return (wav_i32x4_t) { { *a, 0, 0, 0 } };
}

WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_load_zero(const int64_t * a) {
  return (wav_i64x2_t) { { *a, 0 } };
}

WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_load_zero(const uint32_t * a) {
  return (wav_u32x4_t) { { *a, 0, 0, 0 } };
}

WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_load_zero(const uint64_t * a) {
  return (wav_u64x2_t) { { *a, 0  } };
}

WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_load_zero(const float * a) {
  return (wav_f32x4_t) { { *a, 0, 0, 0 } };
}

WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_load_zero(const double * a) {
  return (wav_f64x2_t) { { *a, 0  } };
}

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_load_zero(a) \
    (_Generic((a), \
       int32_t*: wav_i32x4_load_zero, \
       int64_t*: wav_i64x2_load_zero, \
      uint32_t*: wav_u32x4_load_zero, \
      uint64_t*: wav_u64x2_load_zero, \
         float*: wav_f32x4_load_zero, \
        double*: wav_f64x2_load_zero) (a))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_load_zero( int32_t* a) { return wav_i32x4_load_zero(a); }
  WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_load_zero( int64_t* a) { return wav_i64x2_load_zero(a); }
  WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_load_zero(uint32_t* a) { return wav_u32x4_load_zero(a); }
  WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_load_zero(uint64_t* a) { return wav_u64x2_load_zero(a); }
  WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_load_zero(   float* a) { return wav_f32x4_load_zero(a); }
  WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_load_zero(  double* a) { return wav_f64x2_load_zero(a); }
#endif

/* loadu_zero -- Unaligned Load and Zero-Pad
 *
 * Load a single 32-bit or 64-bit element into the lowest bits of a
 * vector, and initialize all other bits of the vector to zero.
 */

WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i32x4_loadu_zero(const void * a) {
  int32_t v;
  __builtin_memcpy(&v, a, sizeof(v));
  return (wav_i32x4_t) { { v, 0, 0, 0 } };
}

WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i64x2_loadu_zero(const void * a) {
  int64_t v;
  __builtin_memcpy(&v, a, sizeof(v));
  return (wav_i64x2_t) { { v, 0 } };
}

WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u32x4_loadu_zero(const void * a) {
  uint32_t v;
  __builtin_memcpy(&v, a, sizeof(v));
  return (wav_u32x4_t) { { v, 0, 0, 0 } };
}

WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u64x2_loadu_zero(const void * a) {
  uint64_t v;
  __builtin_memcpy(&v, a, sizeof(v));
  return (wav_u64x2_t) { { v, 0  } };
}

WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WAV_FUNCTION_ATTRIBUTES
wav_f32x4_t
wav_f32x4_loadu_zero(const void * a) {
  float v;
  __builtin_memcpy(&v, a, sizeof(v));
  return (wav_f32x4_t) { { v, 0, 0, 0 } };
}

WAV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WAV_FUNCTION_ATTRIBUTES
wav_f64x2_t
wav_f64x2_loadu_zero(const void * a) {
  double v;
  __builtin_memcpy(&v, a, sizeof(v));
  return (wav_f64x2_t) { { v, 0  } };
}

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

#if WAV_OVERLOADS == WAV_OVERLOADS_C11
  #define wav_load_splat(a) \
    (_Generic((a), \
        int8_t*: wav_i8x16_load_splat, \
       int16_t*: wav_i16x8_load_splat, \
       int32_t*: wav_i32x4_load_splat, \
       int64_t*: wav_i64x2_load_splat, \
      uint32_t*: wav_u32x4_load_splat, \
      uint64_t*: wav_u64x2_load_splat, \
         float*: wav_f32x4_load_splat, \
        double*: wav_f64x2_load_splat) (a))
#elif WAV_OVERLOADS == WAV_OVERLOADS_CXX
  WAV_FUNCTION_ATTRIBUTES wav_i8x16_t wav_load_splat(  int8_t* a) { return wav_i8x16_load_splat(a); }
  WAV_FUNCTION_ATTRIBUTES wav_i16x8_t wav_load_splat( int16_t* a) { return wav_i16x8_load_splat(a); }
  WAV_FUNCTION_ATTRIBUTES wav_i32x4_t wav_load_splat( int32_t* a) { return wav_i32x4_load_splat(a); }
  WAV_FUNCTION_ATTRIBUTES wav_i64x2_t wav_load_splat( int64_t* a) { return wav_i64x2_load_splat(a); }
  WAV_FUNCTION_ATTRIBUTES wav_u8x16_t wav_load_splat( uint8_t* a) { return wav_u8x16_load_splat(a); }
  WAV_FUNCTION_ATTRIBUTES wav_u16x8_t wav_load_splat(uint16_t* a) { return wav_u16x8_load_splat(a); }
  WAV_FUNCTION_ATTRIBUTES wav_u32x4_t wav_load_splat(uint32_t* a) { return wav_u32x4_load_splat(a); }
  WAV_FUNCTION_ATTRIBUTES wav_u64x2_t wav_load_splat(uint64_t* a) { return wav_u64x2_load_splat(a); }
  WAV_FUNCTION_ATTRIBUTES wav_f32x4_t wav_load_splat(   float* a) { return wav_f32x4_load_splat(a); }
  WAV_FUNCTION_ATTRIBUTES wav_f64x2_t wav_load_splat(  double* a) { return wav_f64x2_load_splat(a); }
#endif

/* loadu_splat -- Load and Splat
 *
 * Load a single element and splat to all lanes of a vector. The
 * natural alignment is the size of the element loaded.
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
 *
 * TODO
 */

/* load_extend -- Load and Extend
 *
 * Fetch consecutive integers up to 32-bit wide and produce a vector
 * with lanes up to 64 bits. The natural alignment is 8 bytes.
 */

WAV_FUNCTION_ATTRIBUTES
wav_i16x8_t
wav_i8x16_load_extend(int8_t values[WAV_ARRAY_PARAM(8)]) {
  wav_i16x8_t r;
  int8_t vec __attribute__((__vector_size__(8)));
  __builtin_memcpy(&vec, values, sizeof(vec));
  r.values = __builtin_convertvector(vec, __typeof__(r.values));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_i32x4_t
wav_i16x8_load_extend(int16_t values[WAV_ARRAY_PARAM(4)]) {
  wav_i32x4_t r;
  int16_t vec __attribute__((__vector_size__(8)));
  __builtin_memcpy(&vec, values, sizeof(vec));
  r.values = __builtin_convertvector(vec, __typeof__(r.values));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_i64x2_t
wav_i32x4_load_extend(int32_t values[WAV_ARRAY_PARAM(2)]) {
  wav_i64x2_t r;
  int32_t vec __attribute__((__vector_size__(8)));
  __builtin_memcpy(&vec, values, sizeof(vec));
  r.values = __builtin_convertvector(vec, __typeof__(r.values));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_u16x8_t
wav_u8x16_load_extend(int8_t values[WAV_ARRAY_PARAM(8)]) {
  wav_u16x8_t r;
  int8_t vec __attribute__((__vector_size__(8)));
  __builtin_memcpy(&vec, values, sizeof(vec));
  r.values = __builtin_convertvector(vec, __typeof__(r.values));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_u32x4_t
wav_u16x8_load_extend(int16_t values[WAV_ARRAY_PARAM(4)]) {
  wav_u32x4_t r;
  int16_t vec __attribute__((__vector_size__(8)));
  __builtin_memcpy(&vec, values, sizeof(vec));
  r.values = __builtin_convertvector(vec, __typeof__(r.values));
  return r;
}

WAV_FUNCTION_ATTRIBUTES
wav_u64x2_t
wav_u32x4_load_extend(int32_t values[WAV_ARRAY_PARAM(2)]) {
  wav_u64x2_t r;
  int32_t vec __attribute__((__vector_size__(8)));
  __builtin_memcpy(&vec, values, sizeof(vec));
  r.values = __builtin_convertvector(vec, __typeof__(r.values));
  return r;
}

#pragma clang diagnostic pop

#endif /* !defined(WAV_H) */
