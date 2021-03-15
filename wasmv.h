/* WASMV <https://github.com/nemequ/wasmv>
 * 
 * An alternative to the wasm_simd128.h API.
 *
 * The primary goal is type safety, and a lot of decision flow from
 * this focus.
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

#include <stdint.h>

/* We don't actually use wasm_simd128.h internally, but we want the
 * v128_t type so we can generate API to convert between it and our
 * types. */
#include <wasm_simd128.h>

/* Some builtins exist, but don't do anything (i.e., if you look
 * at the disassembly nothing is emitted). */
#define WASMV_BUILTIN_IS_NOOP 0

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

#define WASMV_OVERLOADS_NONE 0
#define WASMV_OVERLOADS_C11  1
#define WASMV_OVERLOADS_CXX  2

#if !defined(WASMV_OVERLOADS)
  #if defined(__cplusplus)
    #define WASMV_OVERLOADS WASMV_OVERLOADS_CXX
  #else
    /* clang allows generic selections in older versions of C, but
     * emits the -Wc11-extensions diagnostic (which we suppress). */
    #define WASMV_OVERLOADS WASMV_OVERLOADS_C11
  #endif
#endif

/* We use structs to make sure types are
 * incompatible.  If we just used raw vectors the boolean types would
 * be compatible with the unsigned types. */

typedef struct {   int8_t values __attribute__((__vector_size__(16),__aligned__(16))); } wasmv_i8x16_t;
typedef struct {  int16_t values __attribute__((__vector_size__(16),__aligned__(16))); } wasmv_i16x8_t;
typedef struct {  int32_t values __attribute__((__vector_size__(16),__aligned__(16))); } wasmv_i32x4_t;
typedef struct {  int64_t values __attribute__((__vector_size__(16),__aligned__(16))); } wasmv_i64x2_t;

typedef struct {  uint8_t values __attribute__((__vector_size__(16),__aligned__(16))); } wasmv_u8x16_t;
typedef struct { uint16_t values __attribute__((__vector_size__(16),__aligned__(16))); } wasmv_u16x8_t;
typedef struct { uint32_t values __attribute__((__vector_size__(16),__aligned__(16))); } wasmv_u32x4_t;
typedef struct { uint64_t values __attribute__((__vector_size__(16),__aligned__(16))); } wasmv_u64x2_t;

typedef struct {  uint8_t values __attribute__((__vector_size__(16),__aligned__(16))); } wasmv_b8x16_t;
typedef struct { uint16_t values __attribute__((__vector_size__(16),__aligned__(16))); } wasmv_b16x8_t;
typedef struct { uint32_t values __attribute__((__vector_size__(16),__aligned__(16))); } wasmv_b32x4_t;
typedef struct { uint64_t values __attribute__((__vector_size__(16),__aligned__(16))); } wasmv_b64x2_t;

typedef struct {    float values __attribute__((__vector_size__(16),__aligned__(16))); } wasmv_f32x4_t;
typedef struct {   double values __attribute__((__vector_size__(16),__aligned__(16))); } wasmv_f64x2_t;

#define WASMV_FUNCTION_ATTRIBUTES static inline __attribute__((__artificial__,__always_inline__))
#if defined(__wasm_unimplemented_simd128__)
  #define WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
#else
  #define WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE __attribute__((__unavailable__("requires -munimplemented-simd128")))
#endif

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
  #define WASMV_ARRAY_PARAM(Length) Length
#else
  #define WASMV_ARRAY_PARAM(Length)
#endif

#define WASMV_REQUIRE_CONSTANT(value) \
  _Static_assert(__builtin_constant_p(value), #value " is not constant")

#define WASMV_REQUIRE_CONSTANT_RANGE(value, min, max) (__extension__ ({ \
    WASMV_REQUIRE_CONSTANT(value); \
    _Static_assert((value <= max) && (value >= min), #value " is not between " #min " and " #max "."); \
  }))

#define WASMV_REQUIRE_TYPE(Type, Value) (__extension__ ({ \
    const Type wasmv_require_type_value_ = Value; \
    wasmv_require_type_value_; \
  }))

#define WASMV_DEFINE_REINTERPRET_FUNCTION_(Type_To, Type_From, Function_Name) \
  WASMV_FUNCTION_ATTRIBUTES \
  Type_To \
  Function_Name(Type_From value) { \
    _Static_assert(sizeof(Type_To) == sizeof(Type_From), #Type_To " and " #Type_From " must be the same size."); \
    Type_To r; \
    __builtin_memcpy(&r, &value, sizeof(value)); \
    return r; \
  }

WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i8x16_t, wasmv_i16x8_t, wasmv_i16x8_as_i8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i8x16_t, wasmv_i32x4_t, wasmv_i32x4_as_i8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i8x16_t, wasmv_i64x2_t, wasmv_i64x2_as_i8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i8x16_t, wasmv_u8x16_t, wasmv_u8x16_as_i8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i8x16_t, wasmv_u16x8_t, wasmv_u16x8_as_i8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i8x16_t, wasmv_u32x4_t, wasmv_u32x4_as_i8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i8x16_t, wasmv_u64x2_t, wasmv_u64x2_as_i8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i8x16_t, wasmv_f32x4_t, wasmv_f32x4_as_i8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i8x16_t, wasmv_f64x2_t, wasmv_f64x2_as_i8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i8x16_t, wasmv_b8x16_t, wasmv_b8x16_as_i8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i8x16_t, wasmv_b16x8_t, wasmv_b16x8_as_i8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i8x16_t, wasmv_b32x4_t, wasmv_b32x4_as_i8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i8x16_t, wasmv_b64x2_t, wasmv_b64x2_as_i8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i8x16_t,        v128_t, wasmv_v128_as_i8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i16x8_t, wasmv_i8x16_t, wasmv_i8x16_as_i16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i16x8_t, wasmv_i32x4_t, wasmv_i32x4_as_i16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i16x8_t, wasmv_i64x2_t, wasmv_i64x2_as_i16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i16x8_t, wasmv_u8x16_t, wasmv_u8x16_as_i16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i16x8_t, wasmv_u16x8_t, wasmv_u16x8_as_i16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i16x8_t, wasmv_u32x4_t, wasmv_u32x4_as_i16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i16x8_t, wasmv_u64x2_t, wasmv_u64x2_as_i16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i16x8_t, wasmv_f32x4_t, wasmv_f32x4_as_i16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i16x8_t, wasmv_f64x2_t, wasmv_f64x2_as_i16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i16x8_t, wasmv_b8x16_t, wasmv_b8x16_as_i16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i16x8_t, wasmv_b16x8_t, wasmv_b16x8_as_i16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i16x8_t, wasmv_b32x4_t, wasmv_b32x4_as_i16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i16x8_t, wasmv_b64x2_t, wasmv_b64x2_as_i16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i16x8_t,        v128_t, wasmv_v128_as_i16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i32x4_t, wasmv_i8x16_t, wasmv_i8x16_as_i32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i32x4_t, wasmv_i16x8_t, wasmv_i16x8_as_i32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i32x4_t, wasmv_i64x2_t, wasmv_i64x2_as_i32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i32x4_t, wasmv_u8x16_t, wasmv_u8x16_as_i32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i32x4_t, wasmv_u16x8_t, wasmv_u16x8_as_i32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i32x4_t, wasmv_u32x4_t, wasmv_u32x4_as_i32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i32x4_t, wasmv_u64x2_t, wasmv_u64x2_as_i32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i32x4_t, wasmv_f32x4_t, wasmv_f32x4_as_i32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i32x4_t, wasmv_f64x2_t, wasmv_f64x2_as_i32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i32x4_t, wasmv_b8x16_t, wasmv_b8x16_as_i32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i32x4_t, wasmv_b16x8_t, wasmv_b16x8_as_i32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i32x4_t, wasmv_b32x4_t, wasmv_b32x4_as_i32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i32x4_t, wasmv_b64x2_t, wasmv_b64x2_as_i32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i32x4_t,        v128_t, wasmv_v128_as_i32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i64x2_t, wasmv_i8x16_t, wasmv_i8x16_as_i64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i64x2_t, wasmv_i16x8_t, wasmv_i16x8_as_i64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i64x2_t, wasmv_i32x4_t, wasmv_i32x4_as_i64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i64x2_t, wasmv_u8x16_t, wasmv_u8x16_as_i64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i64x2_t, wasmv_u16x8_t, wasmv_u16x8_as_i64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i64x2_t, wasmv_u32x4_t, wasmv_u32x4_as_i64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i64x2_t, wasmv_u64x2_t, wasmv_u64x2_as_i64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i64x2_t, wasmv_f32x4_t, wasmv_f32x4_as_i64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i64x2_t, wasmv_f64x2_t, wasmv_f64x2_as_i64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i64x2_t, wasmv_b8x16_t, wasmv_b8x16_as_i64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i64x2_t, wasmv_b16x8_t, wasmv_b16x8_as_i64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i64x2_t, wasmv_b32x4_t, wasmv_b32x4_as_i64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i64x2_t, wasmv_b64x2_t, wasmv_b64x2_as_i64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_i64x2_t,        v128_t, wasmv_v128_as_i64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u8x16_t, wasmv_i8x16_t, wasmv_i8x16_as_u8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u8x16_t, wasmv_i16x8_t, wasmv_i16x8_as_u8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u8x16_t, wasmv_i32x4_t, wasmv_i32x4_as_u8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u8x16_t, wasmv_i64x2_t, wasmv_i64x2_as_u8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u8x16_t, wasmv_u16x8_t, wasmv_u16x8_as_u8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u8x16_t, wasmv_u32x4_t, wasmv_u32x4_as_u8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u8x16_t, wasmv_u64x2_t, wasmv_u64x2_as_u8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u8x16_t, wasmv_f32x4_t, wasmv_f32x4_as_u8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u8x16_t, wasmv_f64x2_t, wasmv_f64x2_as_u8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u8x16_t, wasmv_b8x16_t, wasmv_b8x16_as_u8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u8x16_t, wasmv_b16x8_t, wasmv_b16x8_as_u8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u8x16_t, wasmv_b32x4_t, wasmv_b32x4_as_u8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u8x16_t, wasmv_b64x2_t, wasmv_b64x2_as_u8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u8x16_t,        v128_t, wasmv_v128_as_u8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u16x8_t, wasmv_i8x16_t, wasmv_i8x16_as_u16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u16x8_t, wasmv_i16x8_t, wasmv_i16x8_as_u16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u16x8_t, wasmv_i32x4_t, wasmv_i32x4_as_u16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u16x8_t, wasmv_i64x2_t, wasmv_i64x2_as_u16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u16x8_t, wasmv_u8x16_t, wasmv_u8x16_as_u16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u16x8_t, wasmv_u32x4_t, wasmv_u32x4_as_u16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u16x8_t, wasmv_u64x2_t, wasmv_u64x2_as_u16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u16x8_t, wasmv_f32x4_t, wasmv_f32x4_as_u16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u16x8_t, wasmv_f64x2_t, wasmv_f64x2_as_u16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u16x8_t, wasmv_b8x16_t, wasmv_b8x16_as_u16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u16x8_t, wasmv_b16x8_t, wasmv_b16x8_as_u16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u16x8_t, wasmv_b32x4_t, wasmv_b32x4_as_u16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u16x8_t, wasmv_b64x2_t, wasmv_b64x2_as_u16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u16x8_t,        v128_t, wasmv_v128_as_u16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u32x4_t, wasmv_i8x16_t, wasmv_i8x16_as_u32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u32x4_t, wasmv_i16x8_t, wasmv_i16x8_as_u32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u32x4_t, wasmv_i32x4_t, wasmv_i32x4_as_u32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u32x4_t, wasmv_i64x2_t, wasmv_i64x2_as_u32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u32x4_t, wasmv_u8x16_t, wasmv_u8x16_as_u32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u32x4_t, wasmv_u16x8_t, wasmv_u16x8_as_u32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u32x4_t, wasmv_u64x2_t, wasmv_u64x2_as_u32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u32x4_t, wasmv_f32x4_t, wasmv_f32x4_as_u32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u32x4_t, wasmv_f64x2_t, wasmv_f64x2_as_u32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u32x4_t, wasmv_b8x16_t, wasmv_b8x16_as_u32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u32x4_t, wasmv_b16x8_t, wasmv_b16x8_as_u32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u32x4_t, wasmv_b32x4_t, wasmv_b32x4_as_u32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u32x4_t, wasmv_b64x2_t, wasmv_b64x2_as_u32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u32x4_t,        v128_t, wasmv_v128_as_u32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u64x2_t, wasmv_i8x16_t, wasmv_i8x16_as_u64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u64x2_t, wasmv_i16x8_t, wasmv_i16x8_as_u64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u64x2_t, wasmv_i32x4_t, wasmv_i32x4_as_u64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u64x2_t, wasmv_i64x2_t, wasmv_i64x2_as_u64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u64x2_t, wasmv_u8x16_t, wasmv_u8x16_as_u64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u64x2_t, wasmv_u16x8_t, wasmv_u16x8_as_u64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u64x2_t, wasmv_u32x4_t, wasmv_u32x4_as_u64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u64x2_t, wasmv_f32x4_t, wasmv_f32x4_as_u64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u64x2_t, wasmv_f64x2_t, wasmv_f64x2_as_u64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u64x2_t, wasmv_b8x16_t, wasmv_b8x16_as_u64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u64x2_t, wasmv_b16x8_t, wasmv_b16x8_as_u64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u64x2_t, wasmv_b32x4_t, wasmv_b32x4_as_u64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u64x2_t, wasmv_b64x2_t, wasmv_b64x2_as_u64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_u64x2_t,        v128_t, wasmv_v128_as_u64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_f32x4_t, wasmv_i8x16_t, wasmv_i8x16_as_f32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_f32x4_t, wasmv_i16x8_t, wasmv_i16x8_as_f32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_f32x4_t, wasmv_i32x4_t, wasmv_i32x4_as_f32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_f32x4_t, wasmv_i64x2_t, wasmv_i64x2_as_f32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_f32x4_t, wasmv_u8x16_t, wasmv_u8x16_as_f32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_f32x4_t, wasmv_u16x8_t, wasmv_u16x8_as_f32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_f32x4_t, wasmv_u32x4_t, wasmv_u32x4_as_f32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_f32x4_t, wasmv_u64x2_t, wasmv_u64x2_as_f32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_f32x4_t, wasmv_f64x2_t, wasmv_f64x2_as_f32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_f32x4_t, wasmv_b8x16_t, wasmv_b8x16_as_f32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_f32x4_t, wasmv_b16x8_t, wasmv_b16x8_as_f32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_f32x4_t, wasmv_b32x4_t, wasmv_b32x4_as_f32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_f32x4_t, wasmv_b64x2_t, wasmv_b64x2_as_f32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_f32x4_t,        v128_t, wasmv_v128_as_f32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_f64x2_t, wasmv_i8x16_t, wasmv_i8x16_as_f64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_f64x2_t, wasmv_i16x8_t, wasmv_i16x8_as_f64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_f64x2_t, wasmv_i32x4_t, wasmv_i32x4_as_f64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_f64x2_t, wasmv_i64x2_t, wasmv_i64x2_as_f64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_f64x2_t, wasmv_u8x16_t, wasmv_u8x16_as_f64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_f64x2_t, wasmv_u16x8_t, wasmv_u16x8_as_f64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_f64x2_t, wasmv_u32x4_t, wasmv_u32x4_as_f64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_f64x2_t, wasmv_u64x2_t, wasmv_u64x2_as_f64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_f64x2_t, wasmv_f32x4_t, wasmv_f32x4_as_f64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_f64x2_t, wasmv_b8x16_t, wasmv_b8x16_as_f64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_f64x2_t, wasmv_b16x8_t, wasmv_b16x8_as_f64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_f64x2_t, wasmv_b32x4_t, wasmv_b32x4_as_f64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_f64x2_t, wasmv_b64x2_t, wasmv_b64x2_as_f64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_f64x2_t,        v128_t, wasmv_v128_as_f64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b8x16_t, wasmv_i8x16_t, wasmv_i8x16_as_b8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b8x16_t, wasmv_i16x8_t, wasmv_i16x8_as_b8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b8x16_t, wasmv_i32x4_t, wasmv_i32x4_as_b8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b8x16_t, wasmv_i64x2_t, wasmv_i64x2_as_b8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b8x16_t, wasmv_u8x16_t, wasmv_u8x16_as_b8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b8x16_t, wasmv_u16x8_t, wasmv_u16x8_as_b8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b8x16_t, wasmv_u32x4_t, wasmv_u32x4_as_b8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b8x16_t, wasmv_u64x2_t, wasmv_u64x2_as_b8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b8x16_t, wasmv_f32x4_t, wasmv_f32x4_as_b8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b8x16_t, wasmv_f64x2_t, wasmv_f64x2_as_b8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b8x16_t, wasmv_b16x8_t, wasmv_b16x8_as_b8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b8x16_t, wasmv_b32x4_t, wasmv_b32x4_as_b8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b8x16_t, wasmv_b64x2_t, wasmv_b64x2_as_b8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b8x16_t,        v128_t, wasmv_v128_as_b8x16)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b16x8_t, wasmv_i8x16_t, wasmv_i8x16_as_b16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b16x8_t, wasmv_i16x8_t, wasmv_i16x8_as_b16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b16x8_t, wasmv_i32x4_t, wasmv_i32x4_as_b16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b16x8_t, wasmv_i64x2_t, wasmv_i64x2_as_b16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b16x8_t, wasmv_u8x16_t, wasmv_u8x16_as_b16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b16x8_t, wasmv_u16x8_t, wasmv_u16x8_as_b16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b16x8_t, wasmv_u32x4_t, wasmv_u32x4_as_b16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b16x8_t, wasmv_u64x2_t, wasmv_u64x2_as_b16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b16x8_t, wasmv_f32x4_t, wasmv_f32x4_as_b16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b16x8_t, wasmv_f64x2_t, wasmv_f64x2_as_b16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b16x8_t, wasmv_b8x16_t, wasmv_b8x16_as_b16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b16x8_t, wasmv_b32x4_t, wasmv_b32x4_as_b16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b16x8_t, wasmv_b64x2_t, wasmv_b64x2_as_b16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b16x8_t,        v128_t, wasmv_v128_as_b16x8)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b32x4_t, wasmv_i8x16_t, wasmv_i8x16_as_b32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b32x4_t, wasmv_i16x8_t, wasmv_i16x8_as_b32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b32x4_t, wasmv_i32x4_t, wasmv_i32x4_as_b32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b32x4_t, wasmv_i64x2_t, wasmv_i64x2_as_b32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b32x4_t, wasmv_u8x16_t, wasmv_u8x16_as_b32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b32x4_t, wasmv_u16x8_t, wasmv_u16x8_as_b32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b32x4_t, wasmv_u32x4_t, wasmv_u32x4_as_b32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b32x4_t, wasmv_u64x2_t, wasmv_u64x2_as_b32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b32x4_t, wasmv_f32x4_t, wasmv_f32x4_as_b32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b32x4_t, wasmv_f64x2_t, wasmv_f64x2_as_b32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b32x4_t, wasmv_b8x16_t, wasmv_b8x16_as_b32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b32x4_t, wasmv_b16x8_t, wasmv_b16x8_as_b32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b32x4_t, wasmv_b64x2_t, wasmv_b64x2_as_b32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b32x4_t,        v128_t, wasmv_v128_as_b32x4)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b64x2_t, wasmv_i8x16_t, wasmv_i8x16_as_b64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b64x2_t, wasmv_i16x8_t, wasmv_i16x8_as_b64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b64x2_t, wasmv_i32x4_t, wasmv_i32x4_as_b64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b64x2_t, wasmv_i64x2_t, wasmv_i64x2_as_b64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b64x2_t, wasmv_u8x16_t, wasmv_u8x16_as_b64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b64x2_t, wasmv_u16x8_t, wasmv_u16x8_as_b64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b64x2_t, wasmv_u32x4_t, wasmv_u32x4_as_b64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b64x2_t, wasmv_u64x2_t, wasmv_u64x2_as_b64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b64x2_t, wasmv_f32x4_t, wasmv_f32x4_as_b64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b64x2_t, wasmv_f64x2_t, wasmv_f64x2_as_b64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b64x2_t, wasmv_b8x16_t, wasmv_b8x16_as_b64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b64x2_t, wasmv_b16x8_t, wasmv_b16x8_as_b64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b64x2_t, wasmv_b32x4_t, wasmv_b32x4_as_b64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(wasmv_b64x2_t,        v128_t, wasmv_v128_as_b64x2)
WASMV_DEFINE_REINTERPRET_FUNCTION_(       v128_t, wasmv_i8x16_t, wasmv_i8x16_as_v128)
WASMV_DEFINE_REINTERPRET_FUNCTION_(       v128_t, wasmv_i16x8_t, wasmv_i16x8_as_v128)
WASMV_DEFINE_REINTERPRET_FUNCTION_(       v128_t, wasmv_i32x4_t, wasmv_i32x4_as_v128)
WASMV_DEFINE_REINTERPRET_FUNCTION_(       v128_t, wasmv_i64x2_t, wasmv_i64x2_as_v128)
WASMV_DEFINE_REINTERPRET_FUNCTION_(       v128_t, wasmv_u8x16_t, wasmv_u8x16_as_v128)
WASMV_DEFINE_REINTERPRET_FUNCTION_(       v128_t, wasmv_u16x8_t, wasmv_u16x8_as_v128)
WASMV_DEFINE_REINTERPRET_FUNCTION_(       v128_t, wasmv_u32x4_t, wasmv_u32x4_as_v128)
WASMV_DEFINE_REINTERPRET_FUNCTION_(       v128_t, wasmv_u64x2_t, wasmv_u64x2_as_v128)
WASMV_DEFINE_REINTERPRET_FUNCTION_(       v128_t, wasmv_f32x4_t, wasmv_f32x4_as_v128)
WASMV_DEFINE_REINTERPRET_FUNCTION_(       v128_t, wasmv_f64x2_t, wasmv_f64x2_as_v128)
WASMV_DEFINE_REINTERPRET_FUNCTION_(       v128_t, wasmv_b8x16_t, wasmv_b8x16_as_v128)
WASMV_DEFINE_REINTERPRET_FUNCTION_(       v128_t, wasmv_b16x8_t, wasmv_b16x8_as_v128)
WASMV_DEFINE_REINTERPRET_FUNCTION_(       v128_t, wasmv_b32x4_t, wasmv_b32x4_as_v128)
WASMV_DEFINE_REINTERPRET_FUNCTION_(       v128_t, wasmv_b64x2_t, wasmv_b64x2_as_v128)

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_as_i8x16(v) \
    (_Generic( \
      wasmv_i16x8_t: wasmv_i16x8_as_i8x16, \
      wasmv_i32x4_t: wasmv_i32x4_as_i8x16, \
      wasmv_i64x2_t: wasmv_i64x2_as_i8x16, \
      wasmv_u8x16_t: wasmv_u8x16_as_i8x16, \
      wasmv_u16x8_t: wasmv_u16x8_as_i8x16, \
      wasmv_u32x4_t: wasmv_u32x4_as_i8x16, \
      wasmv_u64x2_t: wasmv_u64x2_as_i8x16, \
      wasmv_f32x4_t: wasmv_f32x4_as_i8x16, \
      wasmv_f64x2_t: wasmv_f64x2_as_i8x16, \
      wasmv_b8x16_t: wasmv_b8x16_as_i8x16, \
      wasmv_b16x8_t: wasmv_b16x8_as_i8x16, \
      wasmv_b32x4_t: wasmv_b32x4_as_i8x16, \
      wasmv_b64x2_t: wasmv_b64x2_as_i8x16, \
             v128_t: wasmv_v128_as_i8x16) (v))
  #define wasmv_as_i16x8(v) \
    (_Generic( \
      wasmv_i8x16_t: wasmv_i8x16_as_i16x8, \
      wasmv_i32x4_t: wasmv_i32x4_as_i16x8, \
      wasmv_i64x2_t: wasmv_i64x2_as_i16x8, \
      wasmv_u8x16_t: wasmv_u8x16_as_i16x8, \
      wasmv_u16x8_t: wasmv_u16x8_as_i16x8, \
      wasmv_u32x4_t: wasmv_u32x4_as_i16x8, \
      wasmv_u64x2_t: wasmv_u64x2_as_i16x8, \
      wasmv_f32x4_t: wasmv_f32x4_as_i16x8, \
      wasmv_f64x2_t: wasmv_f64x2_as_i16x8, \
      wasmv_b8x16_t: wasmv_b8x16_as_i16x8, \
      wasmv_b16x8_t: wasmv_b16x8_as_i16x8, \
      wasmv_b32x4_t: wasmv_b32x4_as_i16x8, \
      wasmv_b64x2_t: wasmv_b64x2_as_i16x8, \
             v128_t: wasmv_v128_as_i16x8) (v))
  #define wasmv_as_i32x4(v) \
    (_Generic( \
      wasmv_i8x16_t: wasmv_i8x16_as_i32x4, \
      wasmv_i16x8_t: wasmv_i16x8_as_i32x4, \
      wasmv_i64x2_t: wasmv_i64x2_as_i32x4, \
      wasmv_u8x16_t: wasmv_u8x16_as_i32x4, \
      wasmv_u16x8_t: wasmv_u16x8_as_i32x4, \
      wasmv_u32x4_t: wasmv_u32x4_as_i32x4, \
      wasmv_u64x2_t: wasmv_u64x2_as_i32x4, \
      wasmv_f32x4_t: wasmv_f32x4_as_i32x4, \
      wasmv_f64x2_t: wasmv_f64x2_as_i32x4, \
      wasmv_b8x16_t: wasmv_b8x16_as_i32x4, \
      wasmv_b16x8_t: wasmv_b16x8_as_i32x4, \
      wasmv_b32x4_t: wasmv_b32x4_as_i32x4, \
      wasmv_b64x2_t: wasmv_b64x2_as_i32x4, \
             v128_t: wasmv_v128_as_i32x4) (v))
  #define wasmv_as_i64x2(v) \
    (_Generic( \
      wasmv_i8x16_t: wasmv_i8x16_as_i64x2, \
      wasmv_i16x8_t: wasmv_i16x8_as_i64x2, \
      wasmv_i32x4_t: wasmv_i32x4_as_i64x2, \
      wasmv_u8x16_t: wasmv_u8x16_as_i64x2, \
      wasmv_u16x8_t: wasmv_u16x8_as_i64x2, \
      wasmv_u32x4_t: wasmv_u32x4_as_i64x2, \
      wasmv_u64x2_t: wasmv_u64x2_as_i64x2, \
      wasmv_f32x4_t: wasmv_f32x4_as_i64x2, \
      wasmv_f64x2_t: wasmv_f64x2_as_i64x2, \
      wasmv_b8x16_t: wasmv_b8x16_as_i64x2, \
      wasmv_b16x8_t: wasmv_b16x8_as_i64x2, \
      wasmv_b32x4_t: wasmv_b32x4_as_i64x2, \
      wasmv_b64x2_t: wasmv_b64x2_as_i64x2, \
             v128_t: wasmv_v128_as_i64x2) (v))
  #define wasmv_as_u8x16(v) \
    (_Generic( \
      wasmv_i8x16_t: wasmv_i8x16_as_u8x16, \
      wasmv_i16x8_t: wasmv_i16x8_as_u8x16, \
      wasmv_i32x4_t: wasmv_i32x4_as_u8x16, \
      wasmv_i64x2_t: wasmv_i64x2_as_u8x16, \
      wasmv_u16x8_t: wasmv_u16x8_as_u8x16, \
      wasmv_u32x4_t: wasmv_u32x4_as_u8x16, \
      wasmv_u64x2_t: wasmv_u64x2_as_u8x16, \
      wasmv_f32x4_t: wasmv_f32x4_as_u8x16, \
      wasmv_f64x2_t: wasmv_f64x2_as_u8x16, \
      wasmv_b8x16_t: wasmv_b8x16_as_u8x16, \
      wasmv_b16x8_t: wasmv_b16x8_as_u8x16, \
      wasmv_b32x4_t: wasmv_b32x4_as_u8x16, \
      wasmv_b64x2_t: wasmv_b64x2_as_u8x16, \
             v128_t: wasmv_v128_as_u8x16) (v))
  #define wasmv_as_u16x8(v) \
    (_Generic( \
      wasmv_i8x16_t: wasmv_i8x16_as_u16x8, \
      wasmv_i16x8_t: wasmv_i16x8_as_u16x8, \
      wasmv_i32x4_t: wasmv_i32x4_as_u16x8, \
      wasmv_i64x2_t: wasmv_i64x2_as_u16x8, \
      wasmv_u8x16_t: wasmv_u8x16_as_u16x8, \
      wasmv_u32x4_t: wasmv_u32x4_as_u16x8, \
      wasmv_u64x2_t: wasmv_u64x2_as_u16x8, \
      wasmv_f32x4_t: wasmv_f32x4_as_u16x8, \
      wasmv_f64x2_t: wasmv_f64x2_as_u16x8, \
      wasmv_b8x16_t: wasmv_b8x16_as_u16x8, \
      wasmv_b16x8_t: wasmv_b16x8_as_u16x8, \
      wasmv_b32x4_t: wasmv_b32x4_as_u16x8, \
      wasmv_b64x2_t: wasmv_b64x2_as_u16x8, \
             v128_t: wasmv_v128_as_u16x8) (v))
  #define wasmv_as_u32x4(v) \
    (_Generic( \
      wasmv_i8x16_t: wasmv_i8x16_as_u32x4, \
      wasmv_i16x8_t: wasmv_i16x8_as_u32x4, \
      wasmv_i32x4_t: wasmv_i32x4_as_u32x4, \
      wasmv_i64x2_t: wasmv_i64x2_as_u32x4, \
      wasmv_u8x16_t: wasmv_u8x16_as_u32x4, \
      wasmv_u16x8_t: wasmv_u16x8_as_u32x4, \
      wasmv_u64x2_t: wasmv_u64x2_as_u32x4, \
      wasmv_f32x4_t: wasmv_f32x4_as_u32x4, \
      wasmv_f64x2_t: wasmv_f64x2_as_u32x4, \
      wasmv_b8x16_t: wasmv_b8x16_as_u32x4, \
      wasmv_b16x8_t: wasmv_b16x8_as_u32x4, \
      wasmv_b32x4_t: wasmv_b32x4_as_u32x4, \
      wasmv_b64x2_t: wasmv_b64x2_as_u32x4, \
             v128_t: wasmv_v128_as_u32x4) (v))
  #define wasmv_as_u64x2(v) \
    (_Generic( \
      wasmv_i8x16_t: wasmv_i8x16_as_u64x2, \
      wasmv_i16x8_t: wasmv_i16x8_as_u64x2, \
      wasmv_i32x4_t: wasmv_i32x4_as_u64x2, \
      wasmv_i64x2_t: wasmv_i64x2_as_u64x2, \
      wasmv_u8x16_t: wasmv_u8x16_as_u64x2, \
      wasmv_u16x8_t: wasmv_u16x8_as_u64x2, \
      wasmv_u32x4_t: wasmv_u32x4_as_u64x2, \
      wasmv_f32x4_t: wasmv_f32x4_as_u64x2, \
      wasmv_f64x2_t: wasmv_f64x2_as_u64x2, \
      wasmv_b8x16_t: wasmv_b8x16_as_u64x2, \
      wasmv_b16x8_t: wasmv_b16x8_as_u64x2, \
      wasmv_b32x4_t: wasmv_b32x4_as_u64x2, \
      wasmv_b64x2_t: wasmv_b64x2_as_u64x2, \
             v128_t: wasmv_v128_as_u64x2) (v))
  #define wasmv_as_f32x4(v) \
    (_Generic( \
      wasmv_i8x16_t: wasmv_i8x16_as_f32x4, \
      wasmv_i16x8_t: wasmv_i16x8_as_f32x4, \
      wasmv_i32x4_t: wasmv_i32x4_as_f32x4, \
      wasmv_i64x2_t: wasmv_i64x2_as_f32x4, \
      wasmv_u8x16_t: wasmv_u8x16_as_f32x4, \
      wasmv_u16x8_t: wasmv_u16x8_as_f32x4, \
      wasmv_u32x4_t: wasmv_u32x4_as_f32x4, \
      wasmv_u64x2_t: wasmv_u64x2_as_f32x4, \
      wasmv_f64x2_t: wasmv_f64x2_as_f32x4, \
      wasmv_b8x16_t: wasmv_b8x16_as_f32x4, \
      wasmv_b16x8_t: wasmv_b16x8_as_f32x4, \
      wasmv_b32x4_t: wasmv_b32x4_as_f32x4, \
      wasmv_b64x2_t: wasmv_b64x2_as_f32x4, \
             v128_t: wasmv_v128_as_f32x4) (v))
  #define wasmv_as_f64x2(v) \
    (_Generic( \
      wasmv_i8x16_t: wasmv_i8x16_as_f64x2, \
      wasmv_i16x8_t: wasmv_i16x8_as_f64x2, \
      wasmv_i32x4_t: wasmv_i32x4_as_f64x2, \
      wasmv_i64x2_t: wasmv_i64x2_as_f64x2, \
      wasmv_u8x16_t: wasmv_u8x16_as_f64x2, \
      wasmv_u16x8_t: wasmv_u16x8_as_f64x2, \
      wasmv_u32x4_t: wasmv_u32x4_as_f64x2, \
      wasmv_u64x2_t: wasmv_u64x2_as_f64x2, \
      wasmv_f32x4_t: wasmv_f32x4_as_f64x2, \
      wasmv_b8x16_t: wasmv_b8x16_as_f64x2, \
      wasmv_b16x8_t: wasmv_b16x8_as_f64x2, \
      wasmv_b32x4_t: wasmv_b32x4_as_f64x2, \
      wasmv_b64x2_t: wasmv_b64x2_as_f64x2, \
             v128_t: wasmv_v128_as_f64x2) (v))
  #define wasmv_as_b8x16(v) \
    (_Generic( \
      wasmv_i8x16_t: wasmv_i8x16_as_b8x16, \
      wasmv_i16x8_t: wasmv_i16x8_as_b8x16, \
      wasmv_i32x4_t: wasmv_i32x4_as_b8x16, \
      wasmv_i64x2_t: wasmv_i64x2_as_b8x16, \
      wasmv_u8x16_t: wasmv_u8x16_as_b8x16, \
      wasmv_u16x8_t: wasmv_u16x8_as_b8x16, \
      wasmv_u32x4_t: wasmv_u32x4_as_b8x16, \
      wasmv_u64x2_t: wasmv_u64x2_as_b8x16, \
      wasmv_f32x4_t: wasmv_f32x4_as_b8x16, \
      wasmv_f64x2_t: wasmv_f64x2_as_b8x16, \
      wasmv_b16x8_t: wasmv_b16x8_as_b8x16, \
      wasmv_b32x4_t: wasmv_b32x4_as_b8x16, \
      wasmv_b64x2_t: wasmv_b64x2_as_b8x16, \
             v128_t: wasmv_v128_as_b8x16) (v))
  #define wasmv_as_b16x8(v) \
    (_Generic( \
      wasmv_i8x16_t: wasmv_i8x16_as_b16x8, \
      wasmv_i16x8_t: wasmv_i16x8_as_b16x8, \
      wasmv_i32x4_t: wasmv_i32x4_as_b16x8, \
      wasmv_i64x2_t: wasmv_i64x2_as_b16x8, \
      wasmv_u8x16_t: wasmv_u8x16_as_b16x8, \
      wasmv_u16x8_t: wasmv_u16x8_as_b16x8, \
      wasmv_u32x4_t: wasmv_u32x4_as_b16x8, \
      wasmv_u64x2_t: wasmv_u64x2_as_b16x8, \
      wasmv_f32x4_t: wasmv_f32x4_as_b16x8, \
      wasmv_f64x2_t: wasmv_f64x2_as_b16x8, \
      wasmv_b8x16_t: wasmv_b8x16_as_b16x8, \
      wasmv_b32x4_t: wasmv_b32x4_as_b16x8, \
      wasmv_b64x2_t: wasmv_b64x2_as_b16x8, \
             v128_t: wasmv_v128_as_b16x8) (v))
  #define wasmv_as_b32x4(v) \
    (_Generic( \
      wasmv_i8x16_t: wasmv_i8x16_as_b32x4, \
      wasmv_i16x8_t: wasmv_i16x8_as_b32x4, \
      wasmv_i32x4_t: wasmv_i32x4_as_b32x4, \
      wasmv_i64x2_t: wasmv_i64x2_as_b32x4, \
      wasmv_u8x16_t: wasmv_u8x16_as_b32x4, \
      wasmv_u16x8_t: wasmv_u16x8_as_b32x4, \
      wasmv_u32x4_t: wasmv_u32x4_as_b32x4, \
      wasmv_u64x2_t: wasmv_u64x2_as_b32x4, \
      wasmv_f32x4_t: wasmv_f32x4_as_b32x4, \
      wasmv_f64x2_t: wasmv_f64x2_as_b32x4, \
      wasmv_b8x16_t: wasmv_b8x16_as_b32x4, \
      wasmv_b16x8_t: wasmv_b16x8_as_b32x4, \
      wasmv_b64x2_t: wasmv_b64x2_as_b32x4, \
             v128_t: wasmv_v128_as_b32x4) (v))
  #define wasmv_as_b64x2(v) \
    (_Generic( \
      wasmv_i8x16_t: wasmv_i8x16_as_b64x2, \
      wasmv_i16x8_t: wasmv_i16x8_as_b64x2, \
      wasmv_i32x4_t: wasmv_i32x4_as_b64x2, \
      wasmv_i64x2_t: wasmv_i64x2_as_b64x2, \
      wasmv_u8x16_t: wasmv_u8x16_as_b64x2, \
      wasmv_u16x8_t: wasmv_u16x8_as_b64x2, \
      wasmv_u32x4_t: wasmv_u32x4_as_b64x2, \
      wasmv_u64x2_t: wasmv_u64x2_as_b64x2, \
      wasmv_f32x4_t: wasmv_f32x4_as_b64x2, \
      wasmv_f64x2_t: wasmv_f64x2_as_b64x2, \
      wasmv_b8x16_t: wasmv_b8x16_as_b64x2, \
      wasmv_b16x8_t: wasmv_b16x8_as_b64x2, \
      wasmv_b32x4_t: wasmv_b32x4_as_b64x2, \
             v128_t: wasmv_v128_as_b64x2) (v))
  #define wasmv_as_v128(v) \
    (_Generic( \
      wasmv_i8x16_t: wasmv_i8x16_as_v128, \
      wasmv_i16x8_t: wasmv_i16x8_as_v128, \
      wasmv_i32x4_t: wasmv_i32x4_as_v128, \
      wasmv_i64x2_t: wasmv_i64x2_as_v128, \
      wasmv_u8x16_t: wasmv_u8x16_as_v128, \
      wasmv_u16x8_t: wasmv_u16x8_as_v128, \
      wasmv_u32x4_t: wasmv_u32x4_as_v128, \
      wasmv_u64x2_t: wasmv_u64x2_as_v128, \
      wasmv_f32x4_t: wasmv_f32x4_as_v128, \
      wasmv_f64x2_t: wasmv_f64x2_as_v128, \
      wasmv_b8x16_t: wasmv_b8x16_as_v128, \
      wasmv_b16x8_t: wasmv_b16x8_as_v128, \
      wasmv_b32x4_t: wasmv_b32x4_as_v128, \
      wasmv_b64x2_t: wasmv_b64x2_as_v128) (v))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_as_i8x16(wasmv_i16x8_t value) { return wasmv_i16x8_as_i8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_as_i8x16(wasmv_i32x4_t value) { return wasmv_i32x4_as_i8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_as_i8x16(wasmv_i64x2_t value) { return wasmv_i64x2_as_i8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_as_i8x16(wasmv_u8x16_t value) { return wasmv_u8x16_as_i8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_as_i8x16(wasmv_u16x8_t value) { return wasmv_u16x8_as_i8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_as_i8x16(wasmv_u32x4_t value) { return wasmv_u32x4_as_i8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_as_i8x16(wasmv_u64x2_t value) { return wasmv_u64x2_as_i8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_as_i8x16(wasmv_f32x4_t value) { return wasmv_f32x4_as_i8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_as_i8x16(wasmv_f64x2_t value) { return wasmv_f64x2_as_i8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_as_i8x16(wasmv_b8x16_t value) { return wasmv_b8x16_as_i8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_as_i8x16(wasmv_b16x8_t value) { return wasmv_b16x8_as_i8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_as_i8x16(wasmv_b32x4_t value) { return wasmv_b32x4_as_i8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_as_i8x16(wasmv_b64x2_t value) { return wasmv_b64x2_as_i8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_as_i8x16(       v128_t value) { return wasmv_v128_as_i8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_as_i16x8(wasmv_i8x16_t value) { return wasmv_i8x16_as_i16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_as_i16x8(wasmv_i32x4_t value) { return wasmv_i32x4_as_i16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_as_i16x8(wasmv_i64x2_t value) { return wasmv_i64x2_as_i16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_as_i16x8(wasmv_u8x16_t value) { return wasmv_u8x16_as_i16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_as_i16x8(wasmv_u16x8_t value) { return wasmv_u16x8_as_i16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_as_i16x8(wasmv_u32x4_t value) { return wasmv_u32x4_as_i16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_as_i16x8(wasmv_u64x2_t value) { return wasmv_u64x2_as_i16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_as_i16x8(wasmv_f32x4_t value) { return wasmv_f32x4_as_i16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_as_i16x8(wasmv_f64x2_t value) { return wasmv_f64x2_as_i16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_as_i16x8(wasmv_b8x16_t value) { return wasmv_b8x16_as_i16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_as_i16x8(wasmv_b16x8_t value) { return wasmv_b16x8_as_i16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_as_i16x8(wasmv_b32x4_t value) { return wasmv_b32x4_as_i16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_as_i16x8(wasmv_b64x2_t value) { return wasmv_b64x2_as_i16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_as_i16x8(       v128_t value) { return wasmv_v128_as_i16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_as_i32x4(wasmv_i8x16_t value) { return wasmv_i8x16_as_i32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_as_i32x4(wasmv_i16x8_t value) { return wasmv_i16x8_as_i32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_as_i32x4(wasmv_i64x2_t value) { return wasmv_i64x2_as_i32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_as_i32x4(wasmv_u8x16_t value) { return wasmv_u8x16_as_i32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_as_i32x4(wasmv_u16x8_t value) { return wasmv_u16x8_as_i32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_as_i32x4(wasmv_u32x4_t value) { return wasmv_u32x4_as_i32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_as_i32x4(wasmv_u64x2_t value) { return wasmv_u64x2_as_i32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_as_i32x4(wasmv_f32x4_t value) { return wasmv_f32x4_as_i32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_as_i32x4(wasmv_f64x2_t value) { return wasmv_f64x2_as_i32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_as_i32x4(wasmv_b8x16_t value) { return wasmv_b8x16_as_i32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_as_i32x4(wasmv_b16x8_t value) { return wasmv_b16x8_as_i32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_as_i32x4(wasmv_b32x4_t value) { return wasmv_b32x4_as_i32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_as_i32x4(wasmv_b64x2_t value) { return wasmv_b64x2_as_i32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_as_i32x4(       v128_t value) { return wasmv_v128_as_i32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_as_i64x2(wasmv_i8x16_t value) { return wasmv_i8x16_as_i64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_as_i64x2(wasmv_i16x8_t value) { return wasmv_i16x8_as_i64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_as_i64x2(wasmv_i32x4_t value) { return wasmv_i32x4_as_i64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_as_i64x2(wasmv_u8x16_t value) { return wasmv_u8x16_as_i64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_as_i64x2(wasmv_u16x8_t value) { return wasmv_u16x8_as_i64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_as_i64x2(wasmv_u32x4_t value) { return wasmv_u32x4_as_i64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_as_i64x2(wasmv_u64x2_t value) { return wasmv_u64x2_as_i64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_as_i64x2(wasmv_f32x4_t value) { return wasmv_f32x4_as_i64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_as_i64x2(wasmv_f64x2_t value) { return wasmv_f64x2_as_i64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_as_i64x2(wasmv_b8x16_t value) { return wasmv_b8x16_as_i64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_as_i64x2(wasmv_b16x8_t value) { return wasmv_b16x8_as_i64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_as_i64x2(wasmv_b32x4_t value) { return wasmv_b32x4_as_i64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_as_i64x2(wasmv_b64x2_t value) { return wasmv_b64x2_as_i64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_as_i64x2(       v128_t value) { return wasmv_v128_as_i64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_as_u8x16(wasmv_i8x16_t value) { return wasmv_i8x16_as_u8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_as_u8x16(wasmv_i16x8_t value) { return wasmv_i16x8_as_u8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_as_u8x16(wasmv_i32x4_t value) { return wasmv_i32x4_as_u8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_as_u8x16(wasmv_i64x2_t value) { return wasmv_i64x2_as_u8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_as_u8x16(wasmv_u16x8_t value) { return wasmv_u16x8_as_u8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_as_u8x16(wasmv_u32x4_t value) { return wasmv_u32x4_as_u8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_as_u8x16(wasmv_u64x2_t value) { return wasmv_u64x2_as_u8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_as_u8x16(wasmv_f32x4_t value) { return wasmv_f32x4_as_u8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_as_u8x16(wasmv_f64x2_t value) { return wasmv_f64x2_as_u8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_as_u8x16(wasmv_b8x16_t value) { return wasmv_b8x16_as_u8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_as_u8x16(wasmv_b16x8_t value) { return wasmv_b16x8_as_u8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_as_u8x16(wasmv_b32x4_t value) { return wasmv_b32x4_as_u8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_as_u8x16(wasmv_b64x2_t value) { return wasmv_b64x2_as_u8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_as_u8x16(       v128_t value) { return wasmv_v128_as_u8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_as_u16x8(wasmv_i8x16_t value) { return wasmv_i8x16_as_u16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_as_u16x8(wasmv_i16x8_t value) { return wasmv_i16x8_as_u16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_as_u16x8(wasmv_i32x4_t value) { return wasmv_i32x4_as_u16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_as_u16x8(wasmv_i64x2_t value) { return wasmv_i64x2_as_u16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_as_u16x8(wasmv_u8x16_t value) { return wasmv_u8x16_as_u16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_as_u16x8(wasmv_u32x4_t value) { return wasmv_u32x4_as_u16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_as_u16x8(wasmv_u64x2_t value) { return wasmv_u64x2_as_u16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_as_u16x8(wasmv_f32x4_t value) { return wasmv_f32x4_as_u16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_as_u16x8(wasmv_f64x2_t value) { return wasmv_f64x2_as_u16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_as_u16x8(wasmv_b8x16_t value) { return wasmv_b8x16_as_u16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_as_u16x8(wasmv_b16x8_t value) { return wasmv_b16x8_as_u16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_as_u16x8(wasmv_b32x4_t value) { return wasmv_b32x4_as_u16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_as_u16x8(wasmv_b64x2_t value) { return wasmv_b64x2_as_u16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_as_u16x8(       v128_t value) { return wasmv_v128_as_u16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_as_u32x4(wasmv_i8x16_t value) { return wasmv_i8x16_as_u32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_as_u32x4(wasmv_i16x8_t value) { return wasmv_i16x8_as_u32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_as_u32x4(wasmv_i32x4_t value) { return wasmv_i32x4_as_u32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_as_u32x4(wasmv_i64x2_t value) { return wasmv_i64x2_as_u32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_as_u32x4(wasmv_u8x16_t value) { return wasmv_u8x16_as_u32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_as_u32x4(wasmv_u16x8_t value) { return wasmv_u16x8_as_u32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_as_u32x4(wasmv_u64x2_t value) { return wasmv_u64x2_as_u32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_as_u32x4(wasmv_f32x4_t value) { return wasmv_f32x4_as_u32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_as_u32x4(wasmv_f64x2_t value) { return wasmv_f64x2_as_u32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_as_u32x4(wasmv_b8x16_t value) { return wasmv_b8x16_as_u32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_as_u32x4(wasmv_b16x8_t value) { return wasmv_b16x8_as_u32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_as_u32x4(wasmv_b32x4_t value) { return wasmv_b32x4_as_u32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_as_u32x4(wasmv_b64x2_t value) { return wasmv_b64x2_as_u32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_as_u32x4(       v128_t value) { return wasmv_v128_as_u32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_as_u64x2(wasmv_i8x16_t value) { return wasmv_i8x16_as_u64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_as_u64x2(wasmv_i16x8_t value) { return wasmv_i16x8_as_u64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_as_u64x2(wasmv_i32x4_t value) { return wasmv_i32x4_as_u64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_as_u64x2(wasmv_i64x2_t value) { return wasmv_i64x2_as_u64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_as_u64x2(wasmv_u8x16_t value) { return wasmv_u8x16_as_u64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_as_u64x2(wasmv_u16x8_t value) { return wasmv_u16x8_as_u64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_as_u64x2(wasmv_u32x4_t value) { return wasmv_u32x4_as_u64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_as_u64x2(wasmv_f32x4_t value) { return wasmv_f32x4_as_u64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_as_u64x2(wasmv_f64x2_t value) { return wasmv_f64x2_as_u64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_as_u64x2(wasmv_b8x16_t value) { return wasmv_b8x16_as_u64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_as_u64x2(wasmv_b16x8_t value) { return wasmv_b16x8_as_u64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_as_u64x2(wasmv_b32x4_t value) { return wasmv_b32x4_as_u64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_as_u64x2(wasmv_b64x2_t value) { return wasmv_b64x2_as_u64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_as_u64x2(       v128_t value) { return wasmv_v128_as_u64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_as_f32x4(wasmv_i8x16_t value) { return wasmv_i8x16_as_f32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_as_f32x4(wasmv_i16x8_t value) { return wasmv_i16x8_as_f32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_as_f32x4(wasmv_i32x4_t value) { return wasmv_i32x4_as_f32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_as_f32x4(wasmv_i64x2_t value) { return wasmv_i64x2_as_f32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_as_f32x4(wasmv_u8x16_t value) { return wasmv_u8x16_as_f32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_as_f32x4(wasmv_u16x8_t value) { return wasmv_u16x8_as_f32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_as_f32x4(wasmv_u32x4_t value) { return wasmv_u32x4_as_f32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_as_f32x4(wasmv_u64x2_t value) { return wasmv_u64x2_as_f32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_as_f32x4(wasmv_f64x2_t value) { return wasmv_f64x2_as_f32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_as_f32x4(wasmv_b8x16_t value) { return wasmv_b8x16_as_f32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_as_f32x4(wasmv_b16x8_t value) { return wasmv_b16x8_as_f32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_as_f32x4(wasmv_b32x4_t value) { return wasmv_b32x4_as_f32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_as_f32x4(wasmv_b64x2_t value) { return wasmv_b64x2_as_f32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_as_f32x4(       v128_t value) { return wasmv_v128_as_f32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_as_f64x2(wasmv_i8x16_t value) { return wasmv_i8x16_as_f64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_as_f64x2(wasmv_i16x8_t value) { return wasmv_i16x8_as_f64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_as_f64x2(wasmv_i32x4_t value) { return wasmv_i32x4_as_f64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_as_f64x2(wasmv_i64x2_t value) { return wasmv_i64x2_as_f64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_as_f64x2(wasmv_u8x16_t value) { return wasmv_u8x16_as_f64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_as_f64x2(wasmv_u16x8_t value) { return wasmv_u16x8_as_f64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_as_f64x2(wasmv_u32x4_t value) { return wasmv_u32x4_as_f64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_as_f64x2(wasmv_u64x2_t value) { return wasmv_u64x2_as_f64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_as_f64x2(wasmv_f32x4_t value) { return wasmv_f32x4_as_f64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_as_f64x2(wasmv_b8x16_t value) { return wasmv_b8x16_as_f64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_as_f64x2(wasmv_b16x8_t value) { return wasmv_b16x8_as_f64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_as_f64x2(wasmv_b32x4_t value) { return wasmv_b32x4_as_f64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_as_f64x2(wasmv_b64x2_t value) { return wasmv_b64x2_as_f64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_as_f64x2(       v128_t value) { return wasmv_v128_as_f64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_as_b8x16(wasmv_i8x16_t value) { return wasmv_i8x16_as_b8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_as_b8x16(wasmv_i16x8_t value) { return wasmv_i16x8_as_b8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_as_b8x16(wasmv_i32x4_t value) { return wasmv_i32x4_as_b8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_as_b8x16(wasmv_i64x2_t value) { return wasmv_i64x2_as_b8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_as_b8x16(wasmv_u8x16_t value) { return wasmv_u8x16_as_b8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_as_b8x16(wasmv_u16x8_t value) { return wasmv_u16x8_as_b8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_as_b8x16(wasmv_u32x4_t value) { return wasmv_u32x4_as_b8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_as_b8x16(wasmv_u64x2_t value) { return wasmv_u64x2_as_b8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_as_b8x16(wasmv_f32x4_t value) { return wasmv_f32x4_as_b8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_as_b8x16(wasmv_f64x2_t value) { return wasmv_f64x2_as_b8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_as_b8x16(wasmv_b16x8_t value) { return wasmv_b16x8_as_b8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_as_b8x16(wasmv_b32x4_t value) { return wasmv_b32x4_as_b8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_as_b8x16(wasmv_b64x2_t value) { return wasmv_b64x2_as_b8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_as_b8x16(       v128_t value) { return wasmv_v128_as_b8x16(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_as_b16x8(wasmv_i8x16_t value) { return wasmv_i8x16_as_b16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_as_b16x8(wasmv_i16x8_t value) { return wasmv_i16x8_as_b16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_as_b16x8(wasmv_i32x4_t value) { return wasmv_i32x4_as_b16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_as_b16x8(wasmv_i64x2_t value) { return wasmv_i64x2_as_b16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_as_b16x8(wasmv_u8x16_t value) { return wasmv_u8x16_as_b16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_as_b16x8(wasmv_u16x8_t value) { return wasmv_u16x8_as_b16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_as_b16x8(wasmv_u32x4_t value) { return wasmv_u32x4_as_b16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_as_b16x8(wasmv_u64x2_t value) { return wasmv_u64x2_as_b16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_as_b16x8(wasmv_f32x4_t value) { return wasmv_f32x4_as_b16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_as_b16x8(wasmv_f64x2_t value) { return wasmv_f64x2_as_b16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_as_b16x8(wasmv_b8x16_t value) { return wasmv_b8x16_as_b16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_as_b16x8(wasmv_b32x4_t value) { return wasmv_b32x4_as_b16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_as_b16x8(wasmv_b64x2_t value) { return wasmv_b64x2_as_b16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_as_b16x8(       v128_t value) { return wasmv_v128_as_b16x8(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_as_b32x4(wasmv_i8x16_t value) { return wasmv_i8x16_as_b32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_as_b32x4(wasmv_i16x8_t value) { return wasmv_i16x8_as_b32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_as_b32x4(wasmv_i32x4_t value) { return wasmv_i32x4_as_b32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_as_b32x4(wasmv_i64x2_t value) { return wasmv_i64x2_as_b32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_as_b32x4(wasmv_u8x16_t value) { return wasmv_u8x16_as_b32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_as_b32x4(wasmv_u16x8_t value) { return wasmv_u16x8_as_b32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_as_b32x4(wasmv_u32x4_t value) { return wasmv_u32x4_as_b32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_as_b32x4(wasmv_u64x2_t value) { return wasmv_u64x2_as_b32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_as_b32x4(wasmv_f32x4_t value) { return wasmv_f32x4_as_b32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_as_b32x4(wasmv_f64x2_t value) { return wasmv_f64x2_as_b32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_as_b32x4(wasmv_b8x16_t value) { return wasmv_b8x16_as_b32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_as_b32x4(wasmv_b16x8_t value) { return wasmv_b16x8_as_b32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_as_b32x4(wasmv_b64x2_t value) { return wasmv_b64x2_as_b32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_as_b32x4(       v128_t value) { return wasmv_v128_as_b32x4(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_as_b64x2(wasmv_i8x16_t value) { return wasmv_i8x16_as_b64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_as_b64x2(wasmv_i16x8_t value) { return wasmv_i16x8_as_b64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_as_b64x2(wasmv_i32x4_t value) { return wasmv_i32x4_as_b64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_as_b64x2(wasmv_i64x2_t value) { return wasmv_i64x2_as_b64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_as_b64x2(wasmv_u8x16_t value) { return wasmv_u8x16_as_b64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_as_b64x2(wasmv_u16x8_t value) { return wasmv_u16x8_as_b64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_as_b64x2(wasmv_u32x4_t value) { return wasmv_u32x4_as_b64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_as_b64x2(wasmv_u64x2_t value) { return wasmv_u64x2_as_b64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_as_b64x2(wasmv_f32x4_t value) { return wasmv_f32x4_as_b64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_as_b64x2(wasmv_f64x2_t value) { return wasmv_f64x2_as_b64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_as_b64x2(wasmv_b8x16_t value) { return wasmv_b8x16_as_b64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_as_b64x2(wasmv_b16x8_t value) { return wasmv_b16x8_as_b64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_as_b64x2(wasmv_b32x4_t value) { return wasmv_b32x4_as_b64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_as_b64x2(       v128_t value) { return wasmv_v128_as_b64x2(value); }
  WASMV_FUNCTION_ATTRIBUTES        v128_t  wasmv_as_v128(wasmv_i8x16_t value) { return wasmv_i8x16_as_v128(value); }
  WASMV_FUNCTION_ATTRIBUTES        v128_t  wasmv_as_v128(wasmv_i16x8_t value) { return wasmv_i16x8_as_v128(value); }
  WASMV_FUNCTION_ATTRIBUTES        v128_t  wasmv_as_v128(wasmv_i32x4_t value) { return wasmv_i32x4_as_v128(value); }
  WASMV_FUNCTION_ATTRIBUTES        v128_t  wasmv_as_v128(wasmv_i64x2_t value) { return wasmv_i64x2_as_v128(value); }
  WASMV_FUNCTION_ATTRIBUTES        v128_t  wasmv_as_v128(wasmv_u8x16_t value) { return wasmv_u8x16_as_v128(value); }
  WASMV_FUNCTION_ATTRIBUTES        v128_t  wasmv_as_v128(wasmv_u16x8_t value) { return wasmv_u16x8_as_v128(value); }
  WASMV_FUNCTION_ATTRIBUTES        v128_t  wasmv_as_v128(wasmv_u32x4_t value) { return wasmv_u32x4_as_v128(value); }
  WASMV_FUNCTION_ATTRIBUTES        v128_t  wasmv_as_v128(wasmv_u64x2_t value) { return wasmv_u64x2_as_v128(value); }
  WASMV_FUNCTION_ATTRIBUTES        v128_t  wasmv_as_v128(wasmv_f32x4_t value) { return wasmv_f32x4_as_v128(value); }
  WASMV_FUNCTION_ATTRIBUTES        v128_t  wasmv_as_v128(wasmv_f64x2_t value) { return wasmv_f64x2_as_v128(value); }
  WASMV_FUNCTION_ATTRIBUTES        v128_t  wasmv_as_v128(wasmv_b8x16_t value) { return wasmv_b8x16_as_v128(value); }
  WASMV_FUNCTION_ATTRIBUTES        v128_t  wasmv_as_v128(wasmv_b16x8_t value) { return wasmv_b16x8_as_v128(value); }
  WASMV_FUNCTION_ATTRIBUTES        v128_t  wasmv_as_v128(wasmv_b32x4_t value) { return wasmv_b32x4_as_v128(value); }
  WASMV_FUNCTION_ATTRIBUTES        v128_t  wasmv_as_v128(wasmv_b64x2_t value) { return wasmv_b64x2_as_v128(value); }
#endif

/* load
 *
 * Load data.
 *
 * WASM can load and store unaligned data, so these are really only
 * here to provide a type safe API. */

WASMV_FUNCTION_ATTRIBUTES
wasmv_i8x16_t
wasmv_i8x16_load(const int8_t src[WASMV_ARRAY_PARAM(16)]) {
  wasmv_i8x16_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i16x8_load(const int16_t src[WASMV_ARRAY_PARAM(8)]) {
  wasmv_i16x8_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i32x4_load(const int32_t src[WASMV_ARRAY_PARAM(4)]) {
  wasmv_i32x4_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_i64x2_load(const int64_t src[WASMV_ARRAY_PARAM(2)]) {
  wasmv_i64x2_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u8x16_t
wasmv_u8x16_load(const uint8_t src[WASMV_ARRAY_PARAM(16)]) {
  wasmv_u8x16_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_u16x8_load(const uint16_t src[WASMV_ARRAY_PARAM(8)]) {
  wasmv_u16x8_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_u32x4_load(const uint32_t src[WASMV_ARRAY_PARAM(4)]) {
  wasmv_u32x4_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u64x2_t
wasmv_u64x2_load(const uint64_t src[WASMV_ARRAY_PARAM(2)]) {
  wasmv_u64x2_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f32x4_t
wasmv_f32x4_load(const float src[WASMV_ARRAY_PARAM(4)]) {
  wasmv_f32x4_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f64x2_t
wasmv_f64x2_load(const double src[WASMV_ARRAY_PARAM(2)]) {
  wasmv_f64x2_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_load(src) \
    (_Generic((src), \
        int8_t*: wasmv_i8x16_load, \
       int16_t*: wasmv_i16x8_load, \
       int32_t*: wasmv_i32x4_load, \
       int64_t*: wasmv_i64x2_load, \
       uint8_t*: wasmv_u8x16_load, \
      uint16_t*: wasmv_u16x8_load, \
      uint32_t*: wasmv_u32x4_load, \
      uint64_t*: wasmv_u64x2_load, \
         float*: wasmv_f32x4_load, \
        double*: wasmv_f64x2_load) (src))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_load(const   int8_t src[WASMV_ARRAY_PARAM(16)]) { return wasmv_i8x16_load(src); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_load(const  int16_t src[WASMV_ARRAY_PARAM( 8)]) { return wasmv_i16x8_load(src); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_load(const  int32_t src[WASMV_ARRAY_PARAM( 4)]) { return wasmv_i32x4_load(src); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_load(const  int64_t src[WASMV_ARRAY_PARAM( 2)]) { return wasmv_i64x2_load(src); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_load(const  uint8_t src[WASMV_ARRAY_PARAM(16)]) { return wasmv_u8x16_load(src); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_load(const uint16_t src[WASMV_ARRAY_PARAM( 8)]) { return wasmv_u16x8_load(src); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_load(const uint32_t src[WASMV_ARRAY_PARAM( 4)]) { return wasmv_u32x4_load(src); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_load(const uint64_t src[WASMV_ARRAY_PARAM( 2)]) { return wasmv_u64x2_load(src); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_load(const    float src[WASMV_ARRAY_PARAM( 4)]) { return wasmv_f32x4_load(src); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_load(const   double src[WASMV_ARRAY_PARAM( 2)]) { return wasmv_f64x2_load(src); }
#endif

/* loadu
 *
 * Load unaligned data.
 *
 * Note that there are no overloads for these functions; the input type
 * is always a void pointer, so there is nothing to use to determine
 * the type. */

WASMV_FUNCTION_ATTRIBUTES
wasmv_i8x16_t
wasmv_i8x16_loadu(const void * src) {
  wasmv_i8x16_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i16x8_loadu(const void * src) {
  wasmv_i16x8_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i32x4_loadu(const void * src) {
  wasmv_i32x4_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_i64x2_loadu(const void * src) {
  wasmv_i64x2_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u8x16_t
wasmv_u8x16_loadu(const void * src) {
  wasmv_u8x16_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_u16x8_loadu(const void * src) {
  wasmv_u16x8_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_u32x4_loadu(const void * src) {
  wasmv_u32x4_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u64x2_t
wasmv_u64x2_loadu(const void * src) {
  wasmv_u64x2_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f32x4_t
wasmv_f32x4_loadu(const void * src) {
  wasmv_f32x4_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f64x2_t
wasmv_f64x2_loadu(const void * src) {
  wasmv_f64x2_t r;
  __builtin_memcpy(&r, src, sizeof(r));
  return r;
}

/* store
 *
 * Store data. */

WASMV_FUNCTION_ATTRIBUTES
void
wasmv_i8x16_store(int8_t dest[WASMV_ARRAY_PARAM(16)], wasmv_i8x16_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WASMV_FUNCTION_ATTRIBUTES
void
wasmv_i16x8_store(int16_t dest[WASMV_ARRAY_PARAM(16)], wasmv_i16x8_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WASMV_FUNCTION_ATTRIBUTES
void
wasmv_i32x4_store(int32_t dest[WASMV_ARRAY_PARAM(16)], wasmv_i32x4_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WASMV_FUNCTION_ATTRIBUTES
void
wasmv_i64x2_store(int64_t dest[WASMV_ARRAY_PARAM(16)], wasmv_i64x2_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WASMV_FUNCTION_ATTRIBUTES
void
wasmv_u8x16_store(uint8_t dest[WASMV_ARRAY_PARAM(16)], wasmv_u8x16_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WASMV_FUNCTION_ATTRIBUTES
void
wasmv_u16x8_store(uint16_t dest[WASMV_ARRAY_PARAM(16)], wasmv_u16x8_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WASMV_FUNCTION_ATTRIBUTES
void
wasmv_u32x4_store(uint32_t dest[WASMV_ARRAY_PARAM(16)], wasmv_u32x4_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WASMV_FUNCTION_ATTRIBUTES
void
wasmv_u64x2_store(uint64_t dest[WASMV_ARRAY_PARAM(16)], wasmv_u64x2_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WASMV_FUNCTION_ATTRIBUTES
void
wasmv_f32x4_store(float dest[WASMV_ARRAY_PARAM(16)], wasmv_f32x4_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WASMV_FUNCTION_ATTRIBUTES
void
wasmv_f64x2_store(double dest[WASMV_ARRAY_PARAM(16)], wasmv_f64x2_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_store(dest, src) \
    (_Generic((src), \
      wasmv_i8x16_t: wasmv_i8x16_store, \
      wasmv_i16x8_t: wasmv_i16x8_store, \
      wasmv_i32x4_t: wasmv_i32x4_store, \
      wasmv_i64x2_t: wasmv_i64x2_store, \
      wasmv_u8x16_t: wasmv_u8x16_store, \
      wasmv_u16x8_t: wasmv_u16x8_store, \
      wasmv_u32x4_t: wasmv_u32x4_store, \
      wasmv_u64x2_t: wasmv_u64x2_store, \
      wasmv_f32x4_t: wasmv_f32x4_store, \
      wasmv_f64x2_t: wasmv_f64x2_store) (dest, src))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES void wasmv_store(  int8_t dest[WASMV_ARRAY_PARAM(16)], wasmv_i8x16_t src) { wasmv_i8x16_store(dest, src); }
  WASMV_FUNCTION_ATTRIBUTES void wasmv_store( int16_t dest[WASMV_ARRAY_PARAM( 8)], wasmv_i16x8_t src) { wasmv_i16x8_store(dest, src); }
  WASMV_FUNCTION_ATTRIBUTES void wasmv_store( int32_t dest[WASMV_ARRAY_PARAM( 4)], wasmv_i32x4_t src) { wasmv_i32x4_store(dest, src); }
  WASMV_FUNCTION_ATTRIBUTES void wasmv_store( int64_t dest[WASMV_ARRAY_PARAM( 2)], wasmv_i64x2_t src) { wasmv_i64x2_store(dest, src); }
  WASMV_FUNCTION_ATTRIBUTES void wasmv_store( uint8_t dest[WASMV_ARRAY_PARAM(16)], wasmv_u8x16_t src) { wasmv_u8x16_store(dest, src); }
  WASMV_FUNCTION_ATTRIBUTES void wasmv_store(uint16_t dest[WASMV_ARRAY_PARAM( 8)], wasmv_u16x8_t src) { wasmv_u16x8_store(dest, src); }
  WASMV_FUNCTION_ATTRIBUTES void wasmv_store(uint32_t dest[WASMV_ARRAY_PARAM( 4)], wasmv_u32x4_t src) { wasmv_u32x4_store(dest, src); }
  WASMV_FUNCTION_ATTRIBUTES void wasmv_store(uint64_t dest[WASMV_ARRAY_PARAM( 2)], wasmv_u64x2_t src) { wasmv_u64x2_store(dest, src); }
  WASMV_FUNCTION_ATTRIBUTES void wasmv_store(   float dest[WASMV_ARRAY_PARAM( 4)], wasmv_f32x4_t src) { wasmv_f32x4_store(dest, src); }
  WASMV_FUNCTION_ATTRIBUTES void wasmv_store(  double dest[WASMV_ARRAY_PARAM( 2)], wasmv_f64x2_t src) { wasmv_f64x2_store(dest, src); }
#endif

/* store
 *
 * Store unaligned data. */

WASMV_FUNCTION_ATTRIBUTES
void
wasmv_i8x16_storeu(void * dest, wasmv_i8x16_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WASMV_FUNCTION_ATTRIBUTES
void
wasmv_i16x8_storeu(void * dest, wasmv_i16x8_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WASMV_FUNCTION_ATTRIBUTES
void
wasmv_i32x4_storeu(void * dest, wasmv_i32x4_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WASMV_FUNCTION_ATTRIBUTES
void
wasmv_i64x2_storeu(void * dest, wasmv_i64x2_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WASMV_FUNCTION_ATTRIBUTES
void
wasmv_u8x16_storeu(void * dest, wasmv_u8x16_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WASMV_FUNCTION_ATTRIBUTES
void
wasmv_u16x8_storeu(void * dest, wasmv_u16x8_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WASMV_FUNCTION_ATTRIBUTES
void
wasmv_u32x4_storeu(void * dest, wasmv_u32x4_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WASMV_FUNCTION_ATTRIBUTES
void
wasmv_u64x2_storeu(void * dest, wasmv_u64x2_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WASMV_FUNCTION_ATTRIBUTES
void
wasmv_f32x4_storeu(void * dest, wasmv_f32x4_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

WASMV_FUNCTION_ATTRIBUTES
void
wasmv_f64x2_storeu(void * dest, wasmv_f64x2_t src) {
  __builtin_memcpy(dest, &src, sizeof(src));
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_storeu(dest, src) \
    (_Generic((src), \
      wasmv_i8x16_t: wasmv_i8x16_storeu, \
      wasmv_i16x8_t: wasmv_i16x8_storeu, \
      wasmv_i32x4_t: wasmv_i32x4_storeu, \
      wasmv_i64x2_t: wasmv_i64x2_storeu, \
      wasmv_u8x16_t: wasmv_u8x16_storeu, \
      wasmv_u16x8_t: wasmv_u16x8_storeu, \
      wasmv_u32x4_t: wasmv_u32x4_storeu, \
      wasmv_u64x2_t: wasmv_u64x2_storeu, \
      wasmv_f32x4_t: wasmv_f32x4_storeu, \
      wasmv_f64x2_t: wasmv_f64x2_storeu) (dest, src))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES void wasmv_storeu(void * dest, wasmv_i8x16_t src) { wasmv_i8x16_storeu(dest, src); }
  WASMV_FUNCTION_ATTRIBUTES void wasmv_storeu(void * dest, wasmv_i16x8_t src) { wasmv_i16x8_storeu(dest, src); }
  WASMV_FUNCTION_ATTRIBUTES void wasmv_storeu(void * dest, wasmv_i32x4_t src) { wasmv_i32x4_storeu(dest, src); }
  WASMV_FUNCTION_ATTRIBUTES void wasmv_storeu(void * dest, wasmv_i64x2_t src) { wasmv_i64x2_storeu(dest, src); }
  WASMV_FUNCTION_ATTRIBUTES void wasmv_storeu(void * dest, wasmv_u8x16_t src) { wasmv_u8x16_storeu(dest, src); }
  WASMV_FUNCTION_ATTRIBUTES void wasmv_storeu(void * dest, wasmv_u16x8_t src) { wasmv_u16x8_storeu(dest, src); }
  WASMV_FUNCTION_ATTRIBUTES void wasmv_storeu(void * dest, wasmv_u32x4_t src) { wasmv_u32x4_storeu(dest, src); }
  WASMV_FUNCTION_ATTRIBUTES void wasmv_storeu(void * dest, wasmv_u64x2_t src) { wasmv_u64x2_storeu(dest, src); }
  WASMV_FUNCTION_ATTRIBUTES void wasmv_storeu(void * dest, wasmv_f32x4_t src) { wasmv_f32x4_storeu(dest, src); }
  WASMV_FUNCTION_ATTRIBUTES void wasmv_storeu(void * dest, wasmv_f64x2_t src) { wasmv_f64x2_storeu(dest, src); }
#endif

/* splat
 *
 * Create vector with identical lanes.
 */

WASMV_FUNCTION_ATTRIBUTES
wasmv_i8x16_t
wasmv_i8x16_splat(int8_t value) {
  return (wasmv_i8x16_t) { {
    value, value, value, value, value, value, value, value,
    value, value, value, value, value, value, value, value } };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i16x8_splat(int16_t value) {
  return (wasmv_i16x8_t) { {
    value, value, value, value, value, value, value, value } };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i32x4_splat(int32_t value) {
  return (wasmv_i32x4_t) { { value, value, value, value } };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_i64x2_splat(int64_t value) {
  return (wasmv_i64x2_t) { { value, value } };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u8x16_t
wasmv_u8x16_splat(uint8_t value) {
  return (wasmv_u8x16_t) { {
    value, value, value, value, value, value, value, value,
    value, value, value, value, value, value, value, value } };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_u16x8_splat(uint16_t value) {
  return (wasmv_u16x8_t) { {
    value, value, value, value, value, value, value, value } };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_u32x4_splat(uint32_t value) {
  return (wasmv_u32x4_t) { { value, value, value, value } };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u64x2_t
wasmv_u64x2_splat(uint64_t value) {
  return (wasmv_u64x2_t) { { value, value } };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f32x4_t
wasmv_f32x4_splat(float value) {
  return (wasmv_f32x4_t) { { value, value, value, value } };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f64x2_t
wasmv_f64x2_splat(double value) {
  return (wasmv_f64x2_t) { { value, value } };
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_splat(value) \
    (_Generic((value), \
        int8_t: wasmv_i8x16_splat, \
       int16_t: wasmv_i16x8_splat, \
       int32_t: wasmv_i32x4_splat, \
       int64_t: wasmv_i64x2_splat, \
       uint8_t: wasmv_u8x16_splat, \
      uint16_t: wasmv_u16x8_splat, \
      uint32_t: wasmv_u32x4_splat, \
      uint64_t: wasmv_u64x2_splat, \
         float: wasmv_f32x4_splat, \
        double: wasmv_f64x2_splat) (value))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_splat(  int8_t value) { return wasmv_i8x16_splat(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_splat( int16_t value) { return wasmv_i16x8_splat(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_splat( int32_t value) { return wasmv_i32x4_splat(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_splat( int64_t value) { return wasmv_i64x2_splat(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_splat( uint8_t value) { return wasmv_u8x16_splat(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_splat(uint16_t value) { return wasmv_u16x8_splat(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_splat(uint32_t value) { return wasmv_u32x4_splat(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_splat(uint64_t value) { return wasmv_u64x2_splat(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_splat(   float value) { return wasmv_f32x4_splat(value); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_splat(  double value) { return wasmv_f64x2_splat(value); }
#endif

/* make
 *
 * Create vector.
 *
 * Slow; splat + 15 replace_lanes
 */

WASMV_FUNCTION_ATTRIBUTES
wasmv_i8x16_t
wasmv_i8x16_make(
    int8_t c0, int8_t c1, int8_t  c2, int8_t  c3, int8_t  c4, int8_t  c5, int8_t  c6, int8_t  c7,
    int8_t c8, int8_t c9, int8_t c10, int8_t c11, int8_t c12, int8_t c13, int8_t c14, int8_t c15) {
  return (wasmv_i8x16_t) { {
    c0, c1,  c2,  c3,  c4,  c5,  c6,  c7,
    c8, c9, c10, c11, c12, c13, c14, c15 } };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i16x8_make(
    int16_t c0, int16_t c1, int16_t c2, int16_t c3, int16_t c4, int16_t c5, int16_t c6, int16_t c7) {
  return (wasmv_i16x8_t) { {
    c0, c1, c2, c3, c4, c5, c6, c7 } };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i32x4_make(int32_t c0, int32_t c1, int32_t c2, int32_t c3) {
  return (wasmv_i32x4_t) { { c0, c1, c2, c3 } };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_i64x2_make(int64_t c0, int64_t c1) {
  return (wasmv_i64x2_t) { { c0, c1 } };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u8x16_t
wasmv_u8x16_make(
    uint8_t c0, uint8_t c1, uint8_t  c2, uint8_t  c3, uint8_t  c4, uint8_t  c5, uint8_t  c6, uint8_t  c7,
    uint8_t c8, uint8_t c9, uint8_t c10, uint8_t c11, uint8_t c12, uint8_t c13, uint8_t c14, uint8_t c15) {
  return (wasmv_u8x16_t) { {
    c0, c1,  c2,  c3,  c4,  c5,  c6,  c7,
    c8, c9, c10, c11, c12, c13, c14, c15 } };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_u16x8_make(
    uint16_t c0, uint16_t c1, uint16_t c2, uint16_t c3, uint16_t c4, uint16_t c5, uint16_t c6, uint16_t c7) {
  return (wasmv_u16x8_t) { {
    c0, c1, c2, c3, c4, c5, c6, c7 } };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_u32x4_make(uint32_t c0, uint32_t c1, uint32_t c2, uint32_t c3) {
  return (wasmv_u32x4_t) { { c0, c1, c2, c3 } };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u64x2_t
wasmv_u64x2_make(uint64_t c0, uint64_t c1) {
  return (wasmv_u64x2_t) { { c0, c1 } };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f32x4_t
wasmv_f32x4_make(float c0, float c1, float c2, float c3) {
  return (wasmv_f32x4_t) { { c0, c1, c2, c3 } };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f64x2_t
wasmv_f64x2_make(double c0, double c1) {
  return (wasmv_f64x2_t) { { c0, c1 } };
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_make(c0, ...) \
    (_Generic((c0), \
        int8_t: wasmv_i8x16_make, \
       int16_t: wasmv_i16x8_make, \
       int32_t: wasmv_i32x4_make, \
       int64_t: wasmv_i64x2_make, \
       uint8_t: wasmv_u8x16_make, \
      uint16_t: wasmv_u16x8_make, \
      uint32_t: wasmv_u32x4_make, \
      uint64_t: wasmv_u64x2_make, \
         float: wasmv_f32x4_make, \
        double: wasmv_f64x2_make) (c0, __VA_ARGS__))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_make(
      int8_t c0, int8_t c1, int8_t  c2, int8_t  c3, int8_t  c4, int8_t  c5, int8_t  c6, int8_t  c7,
      int8_t c8, int8_t c9, int8_t c10, int8_t c11, int8_t c12, int8_t c13, int8_t c14, int8_t c15) {
    return wasmv_i8x16_make(
      c0, c1,  c2,  c3,  c4,  c5,  c6,  c7,
      c8, c9, c10, c11, c12, c13, c14, c15);
  }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_make(
      int16_t c0, int16_t c1, int16_t c2, int16_t c3, int16_t c4, int16_t c5, int16_t c6, int16_t c7) {
    return wasmv_i16x8_make(c0, c1, c2, c3, c4, c5, c6, c7);
  }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_make(int32_t c0, int32_t c1, int32_t c2, int32_t c3) {
    return wasmv_i32x4_make(c0, c1, c2, c3);
  }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_make(int64_t c0, int64_t c1) {
    return wasmv_i64x2_make(c0, c1);
  }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_make(
      uint8_t c0, uint8_t c1, uint8_t  c2, uint8_t  c3, uint8_t  c4, uint8_t  c5, uint8_t  c6, uint8_t  c7,
      uint8_t c8, uint8_t c9, uint8_t c10, uint8_t c11, uint8_t c12, uint8_t c13, uint8_t c14, uint8_t c15) {
    return wasmv_u8x16_make(
      c0, c1,  c2,  c3,  c4,  c5,  c6,  c7,
      c8, c9, c10, c11, c12, c13, c14, c15);
  }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_make(
      uint16_t c0, uint16_t c1, uint16_t c2, uint16_t c3, uint16_t c4, uint16_t c5, uint16_t c6, uint16_t c7) {
    return wasmv_u16x8_make(c0, c1, c2, c3, c4, c5, c6, c7);
  }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_make(uint32_t c0, uint32_t c1, uint32_t c2, uint32_t c3) {
    return wasmv_u32x4_make(c0, c1, c2, c3);
  }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_make(uint64_t c0, uint64_t c1) {
    return wasmv_u64x2_make(c0, c1);
  }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_make(float c0, float c1, float c2, float c3) {
    return wasmv_f32x4_make(c0, c1, c2, c3);
  }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_make(double c0, double c1) {
    return wasmv_f64x2_make(c0, c1);
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
  wasmv_i8x16_const( \
      c0, c1,  c2,  c3,  c4,  c5,  c6,  c7, \
      c8, c9, c10, c11, c12, c13, c14, c15) (__extension__ ({ \
    WASMV_REQUIRE_CONSTANT(c0); \
    WASMV_REQUIRE_CONSTANT(c1); \
    WASMV_REQUIRE_CONSTANT(c2); \
    WASMV_REQUIRE_CONSTANT(c3); \
    WASMV_REQUIRE_CONSTANT(c4); \
    WASMV_REQUIRE_CONSTANT(c5); \
    WASMV_REQUIRE_CONSTANT(c6); \
    WASMV_REQUIRE_CONSTANT(c7); \
    WASMV_REQUIRE_CONSTANT(c8); \
    WASMV_REQUIRE_CONSTANT(c9); \
    WASMV_REQUIRE_CONSTANT(c10); \
    WASMV_REQUIRE_CONSTANT(c11); \
    WASMV_REQUIRE_CONSTANT(c12); \
    WASMV_REQUIRE_CONSTANT(c13); \
    WASMV_REQUIRE_CONSTANT(c14); \
    WASMV_REQUIRE_CONSTANT(c15); \
    \
    (wasmv_i8x16_t) { \
      c0, c1,  c2,  c3,  c4,  c5,  c6,  c7, \
      c8, c9, c10, c11, c12, c13, c14, c15 }; \
  }))

#define \
  wasmv_i16x8_const(c0, c1,  c2,  c3,  c4,  c5,  c6,  c7) (__extension__ ({ \
    WASMV_REQUIRE_CONSTANT(c0); \
    WASMV_REQUIRE_CONSTANT(c1); \
    WASMV_REQUIRE_CONSTANT(c2); \
    WASMV_REQUIRE_CONSTANT(c3); \
    WASMV_REQUIRE_CONSTANT(c4); \
    WASMV_REQUIRE_CONSTANT(c5); \
    WASMV_REQUIRE_CONSTANT(c6); \
    WASMV_REQUIRE_CONSTANT(c7); \
    \
    (wasmv_i16x8_t) { c0, c1, c2, c3, c4, c5, c6, c7 }; \
  }))

#define \
  wasmv_i32x4_const(c0, c1,  c2,  c3) (__extension__ ({ \
    WASMV_REQUIRE_CONSTANT(c0); \
    WASMV_REQUIRE_CONSTANT(c1); \
    WASMV_REQUIRE_CONSTANT(c2); \
    WASMV_REQUIRE_CONSTANT(c3); \
    \
    (wasmv_i32x4_t) { c0, c1, c2, c3 }; \
  }))

/* extract_lane -- Extract lane as a scalar
 *
 * Extract the scalar value of lane specified in the immediate mode
 * operand lane in v.
 */

#define \
  wasmv_i8x16_extract_lane(v, lane) (__extension__ ({ \
    WASMV_REQUIRE_CONSTANT_RANGE(lane, 0, 15); \
    WASMV_REQUIRE_TYPE(wasmv_i8x16_t, v).values[lane]; \
  }))

#define \
  wasmv_i16x8_extract_lane(v, lane) (__extension__ ({ \
    WASMV_REQUIRE_CONSTANT_RANGE(lane, 0, 7); \
    WASMV_REQUIRE_TYPE(wasmv_i16x8_t, v).values[lane]; \
  }))

#define \
  wasmv_i32x4_extract_lane(v, lane) (__extension__ ({ \
    WASMV_REQUIRE_CONSTANT_RANGE(lane, 0, 3); \
    WASMV_REQUIRE_TYPE(wasmv_i32x4_t, v).values[lane]; \
  }))

#define \
  wasmv_i64x2_extract_lane(v, lane) (__extension__ ({ \
    WASMV_REQUIRE_CONSTANT_RANGE(lane, 0, 1); \
    WASMV_REQUIRE_TYPE(wasmv_i64x2_t, v).values[lane]; \
  }))

#define \
  wasmv_u8x16_extract_lane(v, lane) (__extension__ ({ \
    WASMV_REQUIRE_CONSTANT_RANGE(lane, 0, 15); \
    WASMV_REQUIRE_TYPE(wasmv_u8x16_t, v).values[lane]; \
  }))

#define \
  wasmv_u16x8_extract_lane(v, lane) (__extension__ ({ \
    WASMV_REQUIRE_CONSTANT_RANGE(lane, 0, 7); \
    WASMV_REQUIRE_TYPE(wasmv_u16x8_t, v).values[lane]; \
  }))

#define \
  wasmv_u32x4_extract_lane(v, lane) (__extension__ ({ \
    WASMV_REQUIRE_CONSTANT_RANGE(lane, 0, 3); \
    WASMV_REQUIRE_TYPE(wasmv_u32x4_t, v).values[lane]; \
  }))

#define \
  wasmv_u64x2_extract_lane(v, lane) (__extension__ ({ \
    WASMV_REQUIRE_CONSTANT_RANGE(lane, 0, 1); \
    WASMV_REQUIRE_TYPE(wasmv_u64x2_t, v).values[lane]; \
  }))

#define \
  wasmv_f32x4_extract_lane(v, lane) (__extension__ ({ \
    WASMV_REQUIRE_CONSTANT_RANGE(lane, 0, 3); \
    WASMV_REQUIRE_TYPE(wasmv_f32x4_t, v).values[lane]; \
  }))

#define \
  wasmv_f64x2_extract_lane(v, lane) (__extension__ ({ \
    WASMV_REQUIRE_CONSTANT_RANGE(lane, 0, 1); \
    WASMV_REQUIRE_TYPE(wasmv_f64x2_t, v).values[lane]; \
  }))

#if WASMV_OVERLOADS != WASMV_OVERLOADS_NONE
  #define wasmv_extract_lane(v, lane) \
    (__extension__ ({ \
      WASMV_REQUIRE_CONSTANT_RANGE(lane, 0, (sizeof(v.values) / sizeof(v.values[0])) - 1); \
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
  wasmv_i8x16_replace_lane(v, lane, value) (__extension__ ({ \
    WASMV_REQUIRE_CONSTANT_RANGE(lane, 0, 15); \
    wasmv_i8x16_t v_ = v; \
    v_.values[lane] = value; \
    v_; \
  }))

#define \
  wasmv_i16x8_replace_lane(v, lane, value) (__extension__ ({ \
    WASMV_REQUIRE_CONSTANT_RANGE(lane, 0, 15); \
    wasmv_i16x8_t v_ = v; \
    v_.values[lane] = value; \
    v_; \
  }))

#define \
  wasmv_i32x4_replace_lane(v, lane, value) (__extension__ ({ \
    WASMV_REQUIRE_CONSTANT_RANGE(lane, 0, 15); \
    wasmv_i32x4_t v_ = v; \
    v_.values[lane] = value; \
    v_; \
  }))

#define \
  wasmv_i64x2_replace_lane(v, lane, value) (__extension__ ({ \
    WASMV_REQUIRE_CONSTANT_RANGE(lane, 0, 15); \
    wasmv_i64x2_t v_ = v; \
    v_.values[lane] = value; \
    v_; \
  }))

#define \
  wasmv_u8x16_replace_lane(v, lane, value) (__extension__ ({ \
    WASMV_REQUIRE_CONSTANT_RANGE(lane, 0, 15); \
    wasmv_u8x16_t v_ = v; \
    v_.values[lane] = value; \
    v_; \
  }))

#define \
  wasmv_u16x8_replace_lane(v, lane, value) (__extension__ ({ \
    WASMV_REQUIRE_CONSTANT_RANGE(lane, 0, 15); \
    wasmv_u16x8_t v_ = v; \
    v_.values[lane] = value; \
    v_; \
  }))

#define \
  wasmv_u32x4_replace_lane(v, lane, value) (__extension__ ({ \
    WASMV_REQUIRE_CONSTANT_RANGE(lane, 0, 15); \
    wasmv_u32x4_t v_ = v; \
    v_.values[lane] = value; \
    v_; \
  }))

#define \
  wasmv_u64x2_replace_lane(v, lane, value) (__extension__ ({ \
    WASMV_REQUIRE_CONSTANT_RANGE(lane, 0, 15); \
    wasmv_u64x2_t v_ = v; \
    v_.values[lane] = value; \
    v_; \
  }))

#define \
  wasmv_f32x4_replace_lane(v, lane, value) (__extension__ ({ \
    WASMV_REQUIRE_CONSTANT_RANGE(lane, 0, 15); \
    wasmv_f32x4_t v_ = v; \
    v_.values[lane] = value; \
    v_; \
  }))

#define \
  wasmv_f64x2_replace_lane(v, lane, value) (__extension__ ({ \
    WASMV_REQUIRE_CONSTANT_RANGE(lane, 0, 15); \
    wasmv_f64x2_t v_ = v; \
    v_.values[lane] = value; \
    v_; \
  }))

#if WASMV_OVERLOADS != WASMV_OVERLOADS_NONE
  #define wasmv_replace_lane(v, lane, value) \
    (__extension__ ({ \
      WASMV_REQUIRE_CONSTANT_RANGE(lane, 0, (sizeof(v.values) / sizeof(v.values[0])) - 1); \
      v.values[lane] = (value); \
    }))
#endif

/* shuffle -- Shuffling using immediate indices
 *
 * Returns a new vector with lanes selected from the lanes of the two
 * input vectors a and b.
 */

#define \
  wasmv_i8x16_shuffle( \
      a, b, \
      c0, c1,  c2,  c3,  c4,  c5,  c6,  c7, \
      c8, c9, c10, c11, c12, c13, c14, c15) \
    ((wasmv_i8x16_t) { __builtin_shufflevector(\
        WASMV_REQUIRE_TYPE(wasmv_i8x16_t, a).values, \
        WASMV_REQUIRE_TYPE(wasmv_i8x16_t, b).values, \
        c0, c1,  c2,  c3,  c4,  c5,  c6,  c7, \
        c8, c9, c10, c11, c12, c13, c14, c15) } )

#define \
  wasmv_i16x8_shuffle( \
      a, b, c0, c1, c2, c3, c4, c5, c6, c7) \
    ((wasmv_i16x8_t) { __builtin_shufflevector(\
        WASMV_REQUIRE_TYPE(wasmv_i16x8_t, a).values, \
        WASMV_REQUIRE_TYPE(wasmv_i16x8_t, b).values, \
        c0, c1,  c2,  c3,  c4,  c5,  c6,  c7) } )

#define \
  wasmv_i32x4_shuffle(a, b, c0, c1, c2, c3) \
    ((wasmv_i32x4_t) { __builtin_shufflevector(\
        WASMV_REQUIRE_TYPE(wasmv_i32x4_t, a).values, \
        WASMV_REQUIRE_TYPE(wasmv_i32x4_t, b).values, \
        c0, c1, c2, c3) } )

#define \
  wasmv_i64x2_shuffle(a, b, c0, c1) \
    ((wasmv_i64x2_t) { __builtin_shufflevector(\
        WASMV_REQUIRE_TYPE(wasmv_i64x2_t, a).values, \
        WASMV_REQUIRE_TYPE(wasmv_i64x2_t, b).values, \
        c0, c1) } )

#define \
  wasmv_u8x16_shuffle( \
      a, b, \
      c0, c1,  c2,  c3,  c4,  c5,  c6,  c7, \
      c8, c9, c10, c11, c12, c13, c14, c15) \
    ((wasmv_u8x16_t) { __builtin_shufflevector(\
        WASMV_REQUIRE_TYPE(wasmv_u8x16_t, a).values, \
        WASMV_REQUIRE_TYPE(wasmv_u8x16_t, b).values, \
        c0, c1,  c2,  c3,  c4,  c5,  c6,  c7, \
        c8, c9, c10, c11, c12, c13, c14, c15) } )

#define \
  wasmv_u16x8_shuffle( \
      a, b, c0, c1, c2, c3, c4, c5, c6, c7) \
    ((wasmv_u16x8_t) { __builtin_shufflevector(\
        WASMV_REQUIRE_TYPE(wasmv_u16x8_t, a).values, \
        WASMV_REQUIRE_TYPE(wasmv_u16x8_t, b).values, \
        c0, c1,  c2,  c3,  c4,  c5,  c6,  c7) } )

#define \
  wasmv_u32x4_shuffle(a, b, c0, c1, c2, c3) \
    ((wasmv_u32x4_t) { __builtin_shufflevector(\
        WASMV_REQUIRE_TYPE(wasmv_u32x4_t, a).values, \
        WASMV_REQUIRE_TYPE(wasmv_u32x4_t, b).values, \
        c0, c1, c2, c3) } )

#define \
  wasmv_u64x2_shuffle(a, b, c0, c1) \
    ((wasmv_u64x2_t) { __builtin_shufflevector(\
        WASMV_REQUIRE_TYPE(wasmv_u64x2_t, a).values, \
        WASMV_REQUIRE_TYPE(wasmv_u64x2_t, b).values, \
        c0, c1) } )

#define \
  wasmv_f32x4_shuffle(a, b, c0, c1, c2, c3) \
    ((wasmv_f32x4_t) { __builtin_shufflevector(\
        WASMV_REQUIRE_TYPE(wasmv_f32x4_t, a).values, \
        WASMV_REQUIRE_TYPE(wasmv_f32x4_t, b).values, \
        c0, c1, c2, c3) } )

#define \
  wasmv_f64x2_shuffle(a, b, c0, c1) \
    ((wasmv_f64x2_t) { __builtin_shufflevector(\
        WASMV_REQUIRE_TYPE(wasmv_f64x2_t, a).values, \
        WASMV_REQUIRE_TYPE(wasmv_f64x2_t, b).values, \
        c0, c1) } )

#if WASMV_OVERLOADS != WASMV_OVERLOADS_NONE
  #define \
    wasmv_shuffle(a, b, ...) \
      ( \
        _Pragma("clang diagnostic push") \
        _Pragma("clang diagnostic ignored \"-Wc99-extensions\"") \
        (__typeof__(a)) { __builtin_shufflevector(\
          WASMV_REQUIRE_TYPE(__typeof__(b), a).values, \
          WASMV_REQUIRE_TYPE(__typeof__(a), b).values, \
          __VA_ARGS__) } \
        _Pragma("clang diagnostic pop") \
      )
#endif

/* swizzle -- Swizzling using variable indices
 *
 * Returns a new vector with lanes selected from the lanes of the first
 * input vector a specified in the second input vector s.
 */

WASMV_FUNCTION_ATTRIBUTES
wasmv_i8x16_t
wasmv_i8x16_swizzle(wasmv_i8x16_t a, wasmv_i8x16_t b) {
  return (wasmv_i8x16_t) { __builtin_wasm_swizzle_v8x16(a.values, b.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u8x16_t
wasmv_u8x16_swizzle(wasmv_u8x16_t a, wasmv_u8x16_t b) {
  return
    wasmv_i8x16_as_u8x16(
      wasmv_i8x16_swizzle(
        wasmv_u8x16_as_i8x16(a),
        wasmv_u8x16_as_i8x16(b)
      )
    );
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_swizzle(a, b) \
    (_Generic((a), \
      wasmv_i8x16_t: wasmv_i8x16_swizzle, \
      wasmv_u8x16_t: wasmv_u8x16_swizzle) (a, b))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_swizzle(wasmv_i8x16_t a, wasmv_i8x16_t b) { return wasmv_i8x16_swizzle(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_swizzle(wasmv_u8x16_t a, wasmv_u8x16_t b) { return wasmv_u8x16_swizzle(a, b); }
#endif

/* add — Addition. */

WASMV_FUNCTION_ATTRIBUTES
wasmv_i8x16_t
wasmv_i8x16_add(wasmv_i8x16_t a, wasmv_i8x16_t b) {
  return (wasmv_i8x16_t) { a.values + b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i16x8_add(wasmv_i16x8_t a, wasmv_i16x8_t b) {
  return (wasmv_i16x8_t) { a.values + b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i32x4_add(wasmv_i32x4_t a, wasmv_i32x4_t b) {
  return (wasmv_i32x4_t) { a.values + b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_i64x2_add(wasmv_i64x2_t a, wasmv_i64x2_t b) {
  return (wasmv_i64x2_t) { a.values + b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u8x16_t
wasmv_u8x16_add(wasmv_u8x16_t a, wasmv_u8x16_t b) {
  return (wasmv_u8x16_t) { a.values + b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_u16x8_add(wasmv_u16x8_t a, wasmv_u16x8_t b) {
  return (wasmv_u16x8_t) { a.values + b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_u32x4_add(wasmv_u32x4_t a, wasmv_u32x4_t b) {
  return (wasmv_u32x4_t) { a.values + b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u64x2_t
wasmv_u64x2_add(wasmv_u64x2_t a, wasmv_u64x2_t b) {
  return (wasmv_u64x2_t) { a.values + b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f32x4_t
wasmv_f32x4_add(wasmv_f32x4_t a, wasmv_f32x4_t b) {
  return (wasmv_f32x4_t) { a.values + b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f64x2_t
wasmv_f64x2_add(wasmv_f64x2_t a, wasmv_f64x2_t b) {
  return (wasmv_f64x2_t) { a.values + b.values };
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_add(a, b) \
    (_Generic((a), \
      wasmv_i8x16_t: wasmv_i8x16_add, \
      wasmv_i16x8_t: wasmv_i16x8_add, \
      wasmv_i32x4_t: wasmv_i32x4_add, \
      wasmv_i64x2_t: wasmv_i64x2_add, \
      wasmv_u8x16_t: wasmv_u8x16_add, \
      wasmv_u16x8_t: wasmv_u16x8_add, \
      wasmv_u32x4_t: wasmv_u32x4_add, \
      wasmv_u64x2_t: wasmv_u64x2_add, \
      wasmv_f32x4_t: wasmv_f32x4_add, \
      wasmv_f64x2_t: wasmv_f64x2_add) (a, b))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_add(wasmv_i8x16_t a, wasmv_i8x16_t b) { return wasmv_i8x16_add(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_add(wasmv_i16x8_t a, wasmv_i16x8_t b) { return wasmv_i16x8_add(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_add(wasmv_i32x4_t a, wasmv_i32x4_t b) { return wasmv_i32x4_add(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_add(wasmv_i64x2_t a, wasmv_i64x2_t b) { return wasmv_i64x2_add(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_add(wasmv_u8x16_t a, wasmv_u8x16_t b) { return wasmv_u8x16_add(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_add(wasmv_u16x8_t a, wasmv_u16x8_t b) { return wasmv_u16x8_add(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_add(wasmv_u32x4_t a, wasmv_u32x4_t b) { return wasmv_u32x4_add(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_add(wasmv_u64x2_t a, wasmv_u64x2_t b) { return wasmv_u64x2_add(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_add(wasmv_f32x4_t a, wasmv_f32x4_t b) { return wasmv_f32x4_add(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_add(wasmv_f64x2_t a, wasmv_f64x2_t b) { return wasmv_f64x2_add(a, b); }

  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t operator+(wasmv_i8x16_t a, wasmv_i8x16_t b) { return wasmv_i8x16_add(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t operator+(wasmv_i16x8_t a, wasmv_i16x8_t b) { return wasmv_i16x8_add(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t operator+(wasmv_i32x4_t a, wasmv_i32x4_t b) { return wasmv_i32x4_add(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t operator+(wasmv_i64x2_t a, wasmv_i64x2_t b) { return wasmv_i64x2_add(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t operator+(wasmv_u8x16_t a, wasmv_u8x16_t b) { return wasmv_u8x16_add(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t operator+(wasmv_u16x8_t a, wasmv_u16x8_t b) { return wasmv_u16x8_add(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t operator+(wasmv_u32x4_t a, wasmv_u32x4_t b) { return wasmv_u32x4_add(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t operator+(wasmv_u64x2_t a, wasmv_u64x2_t b) { return wasmv_u64x2_add(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t operator+(wasmv_f32x4_t a, wasmv_f32x4_t b) { return wasmv_f32x4_add(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t operator+(wasmv_f64x2_t a, wasmv_f64x2_t b) { return wasmv_f64x2_add(a, b); }
#endif

/* sub — Subtraction. */

WASMV_FUNCTION_ATTRIBUTES
wasmv_i8x16_t
wasmv_i8x16_sub(wasmv_i8x16_t a, wasmv_i8x16_t b) {
  return (wasmv_i8x16_t) { a.values - b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i16x8_sub(wasmv_i16x8_t a, wasmv_i16x8_t b) {
  return (wasmv_i16x8_t) { a.values - b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i32x4_sub(wasmv_i32x4_t a, wasmv_i32x4_t b) {
  return (wasmv_i32x4_t) { a.values - b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_i64x2_sub(wasmv_i64x2_t a, wasmv_i64x2_t b) {
  return (wasmv_i64x2_t) { a.values - b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u8x16_t
wasmv_u8x16_sub(wasmv_u8x16_t a, wasmv_u8x16_t b) {
  return (wasmv_u8x16_t) { a.values - b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_u16x8_sub(wasmv_u16x8_t a, wasmv_u16x8_t b) {
  return (wasmv_u16x8_t) { a.values - b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_u32x4_sub(wasmv_u32x4_t a, wasmv_u32x4_t b) {
  return (wasmv_u32x4_t) { a.values - b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u64x2_t
wasmv_u64x2_sub(wasmv_u64x2_t a, wasmv_u64x2_t b) {
  return (wasmv_u64x2_t) { a.values - b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f32x4_t
wasmv_f32x4_sub(wasmv_f32x4_t a, wasmv_f32x4_t b) {
  return (wasmv_f32x4_t) { a.values - b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f64x2_t
wasmv_f64x2_sub(wasmv_f64x2_t a, wasmv_f64x2_t b) {
  return (wasmv_f64x2_t) { a.values - b.values };
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_sub(a, b) \
    (_Generic((a), \
      wasmv_i8x16_t: wasmv_i8x16_sub, \
      wasmv_i16x8_t: wasmv_i16x8_sub, \
      wasmv_i32x4_t: wasmv_i32x4_sub, \
      wasmv_i64x2_t: wasmv_i64x2_sub, \
      wasmv_u8x16_t: wasmv_u8x16_sub, \
      wasmv_u16x8_t: wasmv_u16x8_sub, \
      wasmv_u32x4_t: wasmv_u32x4_sub, \
      wasmv_u64x2_t: wasmv_u64x2_sub, \
      wasmv_f32x4_t: wasmv_f32x4_sub, \
      wasmv_f64x2_t: wasmv_f64x2_sub) (a, b))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_sub(wasmv_i8x16_t a, wasmv_i8x16_t b) { return wasmv_i8x16_sub(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_sub(wasmv_i16x8_t a, wasmv_i16x8_t b) { return wasmv_i16x8_sub(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_sub(wasmv_i32x4_t a, wasmv_i32x4_t b) { return wasmv_i32x4_sub(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_sub(wasmv_i64x2_t a, wasmv_i64x2_t b) { return wasmv_i64x2_sub(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_sub(wasmv_u8x16_t a, wasmv_u8x16_t b) { return wasmv_u8x16_sub(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_sub(wasmv_u16x8_t a, wasmv_u16x8_t b) { return wasmv_u16x8_sub(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_sub(wasmv_u32x4_t a, wasmv_u32x4_t b) { return wasmv_u32x4_sub(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_sub(wasmv_u64x2_t a, wasmv_u64x2_t b) { return wasmv_u64x2_sub(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_sub(wasmv_f32x4_t a, wasmv_f32x4_t b) { return wasmv_f32x4_sub(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_sub(wasmv_f64x2_t a, wasmv_f64x2_t b) { return wasmv_f64x2_sub(a, b); }

  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t operator-(wasmv_i8x16_t a, wasmv_i8x16_t b) { return wasmv_i8x16_sub(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t operator-(wasmv_i16x8_t a, wasmv_i16x8_t b) { return wasmv_i16x8_sub(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t operator-(wasmv_i32x4_t a, wasmv_i32x4_t b) { return wasmv_i32x4_sub(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t operator-(wasmv_i64x2_t a, wasmv_i64x2_t b) { return wasmv_i64x2_sub(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t operator-(wasmv_u8x16_t a, wasmv_u8x16_t b) { return wasmv_u8x16_sub(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t operator-(wasmv_u16x8_t a, wasmv_u16x8_t b) { return wasmv_u16x8_sub(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t operator-(wasmv_u32x4_t a, wasmv_u32x4_t b) { return wasmv_u32x4_sub(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t operator-(wasmv_u64x2_t a, wasmv_u64x2_t b) { return wasmv_u64x2_sub(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t operator-(wasmv_f32x4_t a, wasmv_f32x4_t b) { return wasmv_f32x4_sub(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t operator-(wasmv_f64x2_t a, wasmv_f64x2_t b) { return wasmv_f64x2_sub(a, b); }
#endif

/* mul — Multiplication. */

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i16x8_mul(wasmv_i16x8_t a, wasmv_i16x8_t b) {
  return (wasmv_i16x8_t) { a.values * b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i32x4_mul(wasmv_i32x4_t a, wasmv_i32x4_t b) {
  return (wasmv_i32x4_t) { a.values * b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_i64x2_mul(wasmv_i64x2_t a, wasmv_i64x2_t b) {
  return (wasmv_i64x2_t) { a.values * b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f32x4_t
wasmv_f32x4_mul(wasmv_f32x4_t a, wasmv_f32x4_t b) {
  return (wasmv_f32x4_t) { a.values * b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f64x2_t
wasmv_f64x2_mul(wasmv_f64x2_t a, wasmv_f64x2_t b) {
  return (wasmv_f64x2_t) { a.values * b.values };
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_mul(a, b) \
    (_Generic((a), \
      wasmv_i16x8_t: wasmv_i16x8_mul, \
      wasmv_i32x4_t: wasmv_i32x4_mul, \
      wasmv_i64x2_t: wasmv_i64x2_mul, \
      wasmv_f32x4_t: wasmv_f32x4_mul, \
      wasmv_f64x2_t: wasmv_f64x2_mul) (a, b))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_mul(wasmv_i16x8_t a, wasmv_i16x8_t b) { return wasmv_i16x8_mul(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_mul(wasmv_i32x4_t a, wasmv_i32x4_t b) { return wasmv_i32x4_mul(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_mul(wasmv_i64x2_t a, wasmv_i64x2_t b) { return wasmv_i64x2_mul(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_mul(wasmv_f32x4_t a, wasmv_f32x4_t b) { return wasmv_f32x4_mul(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_mul(wasmv_f64x2_t a, wasmv_f64x2_t b) { return wasmv_f64x2_mul(a, b); }

  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t operator*(wasmv_i16x8_t a, wasmv_i16x8_t b) { return wasmv_i16x8_mul(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t operator*(wasmv_i32x4_t a, wasmv_i32x4_t b) { return wasmv_i32x4_mul(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t operator*(wasmv_i64x2_t a, wasmv_i64x2_t b) { return wasmv_i64x2_mul(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t operator*(wasmv_f32x4_t a, wasmv_f32x4_t b) { return wasmv_f32x4_mul(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t operator*(wasmv_f64x2_t a, wasmv_f64x2_t b) { return wasmv_f64x2_mul(a, b); }
#endif

/* div — Division. */

WASMV_FUNCTION_ATTRIBUTES
wasmv_f32x4_t
wasmv_f32x4_div(wasmv_f32x4_t a, wasmv_f32x4_t b) {
  return (wasmv_f32x4_t) { a.values * b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f64x2_t
wasmv_f64x2_div(wasmv_f64x2_t a, wasmv_f64x2_t b) {
  return (wasmv_f64x2_t) { a.values * b.values };
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_div(a, b) \
    (_Generic((a), \
      wasmv_f32x4_t: wasmv_f32x4_div, \
      wasmv_f64x2_t: wasmv_f64x2_div) (a, b))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_div(wasmv_f32x4_t a, wasmv_f32x4_t b) { return wasmv_f32x4_div(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_div(wasmv_f64x2_t a, wasmv_f64x2_t b) { return wasmv_f64x2_div(a, b); }

  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t operator/(wasmv_f32x4_t a, wasmv_f32x4_t b) { return wasmv_f32x4_div(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t operator/(wasmv_f64x2_t a, wasmv_f64x2_t b) { return wasmv_f64x2_div(a, b); }
#endif

/* neg — Negation. */

WASMV_FUNCTION_ATTRIBUTES
wasmv_i8x16_t
wasmv_i8x16_neg(wasmv_i8x16_t a) {
  return (wasmv_i8x16_t) { -a.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i16x8_neg(wasmv_i16x8_t a) {
  return (wasmv_i16x8_t) { -a.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i32x4_neg(wasmv_i32x4_t a) {
  return (wasmv_i32x4_t) { -a.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_i64x2_neg(wasmv_i64x2_t a) {
  return (wasmv_i64x2_t) { -a.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f32x4_t
wasmv_f32x4_neg(wasmv_f32x4_t a) {
  return (wasmv_f32x4_t) { -a.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f64x2_t
wasmv_f64x2_neg(wasmv_f64x2_t a) {
  return (wasmv_f64x2_t) { -a.values };
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_neg(a) \
    (_Generic((a), \
      wasmv_i8x16_t: wasmv_i8x16_neg, \
      wasmv_i16x8_t: wasmv_i16x8_neg, \
      wasmv_i32x4_t: wasmv_i32x4_neg, \
      wasmv_i64x2_t: wasmv_i64x2_neg, \
      wasmv_f32x4_t: wasmv_f32x4_neg, \
      wasmv_f64x2_t: wasmv_f64x2_neg) (a))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_neg(wasmv_i8x16_t a) { return wasmv_i8x16_neg(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_neg(wasmv_i16x8_t a) { return wasmv_i16x8_neg(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_neg(wasmv_i32x4_t a) { return wasmv_i32x4_neg(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_neg(wasmv_i64x2_t a) { return wasmv_i64x2_neg(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_neg(wasmv_f32x4_t a) { return wasmv_f32x4_neg(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_neg(wasmv_f64x2_t a) { return wasmv_f64x2_neg(a); }

  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t operator-(wasmv_i8x16_t a) { return wasmv_i8x16_neg(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t operator-(wasmv_i16x8_t a) { return wasmv_i16x8_neg(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t operator-(wasmv_i32x4_t a) { return wasmv_i32x4_neg(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t operator-(wasmv_i64x2_t a) { return wasmv_i64x2_neg(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t operator-(wasmv_f32x4_t a) { return wasmv_f32x4_neg(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t operator-(wasmv_f64x2_t a) { return wasmv_f64x2_neg(a); }
#endif

/* sqrt — Square root.
 *
 * Lane-wise IEEE squareRoot.
 */

WASMV_FUNCTION_ATTRIBUTES
wasmv_f32x4_t
wasmv_f32x4_sqrt(wasmv_f32x4_t a) {
  return (wasmv_f32x4_t) { __builtin_wasm_sqrt_f32x4(a.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f64x2_t
wasmv_f64x2_sqrt(wasmv_f64x2_t a) {
  return (wasmv_f64x2_t) { __builtin_wasm_sqrt_f64x2(a.values) };
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_sqrt(a) \
    (_Generic((a), \
      wasmv_f32x4_t: wasmv_f32x4_sqrt, \
      wasmv_f64x2_t: wasmv_f64x2_sqrt) (a))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_sqrt(wasmv_f32x4_t a) { return wasmv_f32x4_sqrt(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_sqrt(wasmv_f64x2_t a) { return wasmv_f64x2_sqrt(a); }
#endif

/* ceil — Round to integer above (ceiling)
 *
 * Lane-wise rounding to the nearest integral value not smaller than
 * the input.
 */

WASMV_FUNCTION_ATTRIBUTES
wasmv_f32x4_t
wasmv_f32x4_ceil(wasmv_f32x4_t a) {
  return (wasmv_f32x4_t) { __builtin_wasm_ceil_f32x4(a.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f64x2_t
wasmv_f64x2_ceil(wasmv_f64x2_t a) {
  return (wasmv_f64x2_t) { __builtin_wasm_ceil_f64x2(a.values) };
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_ceil(a) \
    (_Generic((a), \
      wasmv_f32x4_t: wasmv_f32x4_ceil, \
      wasmv_f64x2_t: wasmv_f64x2_ceil) (a))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_ceil(wasmv_f32x4_t a) { return wasmv_f32x4_ceil(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_ceil(wasmv_f64x2_t a) { return wasmv_f64x2_ceil(a); }
#endif

/* floor — Round to integer below (floor)
 *
 * Lane-wise rounding to the nearest integral value not greater than
 * the input.
 */

WASMV_FUNCTION_ATTRIBUTES
wasmv_f32x4_t
wasmv_f32x4_floor(wasmv_f32x4_t a) {
  return (wasmv_f32x4_t) { __builtin_wasm_floor_f32x4(a.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f64x2_t
wasmv_f64x2_floor(wasmv_f64x2_t a) {
  return (wasmv_f64x2_t) { __builtin_wasm_floor_f64x2(a.values) };
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_floor(a) \
    (_Generic((a), \
      wasmv_f32x4_t: wasmv_f32x4_floor, \
      wasmv_f64x2_t: wasmv_f64x2_floor) (a))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_floor(wasmv_f32x4_t a) { return wasmv_f32x4_floor(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_floor(wasmv_f64x2_t a) { return wasmv_f64x2_floor(a); }
#endif

/* trunc — Round to integer toward zero (truncate to integer)
 *
 * Lane-wise rounding to the nearest integral value with the magnitude
 * not larger than the input.
 */

WASMV_FUNCTION_ATTRIBUTES
wasmv_f32x4_t
wasmv_f32x4_trunc(wasmv_f32x4_t a) {
  return (wasmv_f32x4_t) { __builtin_wasm_trunc_f32x4(a.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f64x2_t
wasmv_f64x2_trunc(wasmv_f64x2_t a) {
  return (wasmv_f64x2_t) { __builtin_wasm_trunc_f64x2(a.values) };
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_trunc(a) \
    (_Generic((a), \
      wasmv_f32x4_t: wasmv_f32x4_trunc, \
      wasmv_f64x2_t: wasmv_f64x2_trunc) (a))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_trunc(wasmv_f32x4_t a) { return wasmv_f32x4_trunc(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_trunc(wasmv_f64x2_t a) { return wasmv_f64x2_trunc(a); }
#endif

/* nearest — Round to nearest integer, ties to even
 *
 * Lane-wise rounding to the nearest integral value; if two values are
 * equally near, rounds to the even one.
 */

WASMV_FUNCTION_ATTRIBUTES
wasmv_f32x4_t
wasmv_f32x4_nearest(wasmv_f32x4_t a) {
  return (wasmv_f32x4_t) { __builtin_wasm_nearest_f32x4(a.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f64x2_t
wasmv_f64x2_nearest(wasmv_f64x2_t a) {
  return (wasmv_f64x2_t) { __builtin_wasm_nearest_f64x2(a.values) };
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_nearest(a) \
    (_Generic((a), \
      wasmv_f32x4_t: wasmv_f32x4_nearest, \
      wasmv_f64x2_t: wasmv_f64x2_nearest) (a))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_nearest(wasmv_f32x4_t a) { return wasmv_f32x4_nearest(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_nearest(wasmv_f64x2_t a) { return wasmv_f64x2_nearest(a); }
#endif

/* extend_low -- Integer to integer extension
 *
 * Converts low half of the smaller lane vector to a larger lane
 * vector, sign extended or zero (unsigned) extended.
 */

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i8x16_extend_low(wasmv_i8x16_t a) {
  wasmv_i16x8_t r;
  const int8_t low __attribute__((__vector_size__(8))) = {
    a.values[0], a.values[1], a.values[2], a.values[3],
    a.values[4], a.values[5], a.values[6], a.values[7],
  };

  r.values = __builtin_convertvector(low, __typeof__(r.values));

  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i16x8_extend_low(wasmv_i16x8_t a) {
  wasmv_i32x4_t r;
  const int16_t low __attribute__((__vector_size__(8))) = {
    a.values[0], a.values[1], a.values[2], a.values[3],
  };

  r.values = __builtin_convertvector(low, __typeof__(r.values));

  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_i32x4_extend_low(wasmv_i32x4_t a) {
  wasmv_i64x2_t r;
  const int32_t low __attribute__((__vector_size__(8))) = {
    a.values[0], a.values[1],
  };

  r.values = __builtin_convertvector(low, __typeof__(r.values));

  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_u8x16_extend_low(wasmv_u8x16_t a) {
  wasmv_u16x8_t r;
  const uint8_t low __attribute__((__vector_size__(8))) = {
    a.values[0], a.values[1], a.values[2], a.values[3],
    a.values[4], a.values[5], a.values[6], a.values[7],
  };

  r.values = __builtin_convertvector(low, __typeof__(r.values));

  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_u16x8_extend_low(wasmv_u16x8_t a) {
  wasmv_u32x4_t r;
  const uint16_t low __attribute__((__vector_size__(8))) = {
    a.values[0], a.values[1], a.values[2], a.values[3],
  };

  r.values = __builtin_convertvector(low, __typeof__(r.values));

  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u64x2_t
wasmv_u32x4_extend_low(wasmv_u32x4_t a) {
  wasmv_u64x2_t r;
  const uint32_t low __attribute__((__vector_size__(8))) = {
    a.values[0], a.values[1],
  };

  r.values = __builtin_convertvector(low, __typeof__(r.values));

  return r;
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_extend_low(a) \
    (_Generic((a), \
      wasmv_i8x16_t: wasmv_i8x16_extend_low, \
      wasmv_i16x8_t: wasmv_i16x8_extend_low, \
      wasmv_i32x4_t: wasmv_i32x4_extend_low, \
      wasmv_u8x16_t: wasmv_u8x16_extend_low, \
      wasmv_u16x8_t: wasmv_u16x8_extend_low, \
      wasmv_u32x4_t: wasmv_u32x4_extend_low) (a))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_extend_low(wasmv_i8x16_t a) { return wasmv_i8x16_extend_low(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_extend_low(wasmv_i16x8_t a) { return wasmv_i16x8_extend_low(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_extend_low(wasmv_i32x4_t a) { return wasmv_i32x4_extend_low(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_extend_low(wasmv_u8x16_t a) { return wasmv_u8x16_extend_low(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_extend_low(wasmv_u16x8_t a) { return wasmv_u16x8_extend_low(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_extend_low(wasmv_u32x4_t a) { return wasmv_u32x4_extend_low(a); }
#endif

/* promote_low -- Single-precision floating point to double-precision
 *
 * Conversion of the two lower single-precision floating point lanes to
 * the two double-precision lanes of the result.
 */

WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WASMV_FUNCTION_ATTRIBUTES
wasmv_f64x2_t
wasmv_f32x4_promote_low(wasmv_f32x4_t a) {
  #if WASMV_BUILTIN_IS_NOOP
    return (wasmv_f64x2_t) { __builtin_wasm_promote_low_f32x4_f64x2(a.values) };
  #else
    wasmv_f64x2_t r;
    const float low __attribute__((__vector_size__(8))) = {
      a.values[0], a.values[1],
    };

    r.values = __builtin_convertvector(low, __typeof__(r.values));

    return r;
  #endif
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_promote_low(a) \
    (_Generic((a), \
      wasmv_f32x4_t: wasmv_f32x4_promote_low) (a))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_promote_low(wasmv_f32x4_t a) { return wasmv_f32x4_promote_low(a); }
#endif

/* extend_high -- Integer to integer extension
 *
 * Converts high half of the smaller lane vector to a larger lane
 * vector, sign extended or zero (unsigned) extended.
 */

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i8x16_extend_high(wasmv_i8x16_t a) {
  wasmv_i8x16_t high;
  high.values = __builtin_shufflevector(a.values, a.values, 8, 9, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1);
  return wasmv_i8x16_extend_low(high);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i16x8_extend_high(wasmv_i16x8_t a) {
  wasmv_i16x8_t high;
  high.values = __builtin_shufflevector(a.values, a.values, 4, 5, 6, 7, -1, -1, -1, -1);
  return wasmv_i16x8_extend_low(high);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_i32x4_extend_high(wasmv_i32x4_t a) {
  wasmv_i32x4_t high;
  high.values = __builtin_shufflevector(a.values, a.values, 2, 3, -1, -1);
  return wasmv_i32x4_extend_low(high);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_u8x16_extend_high(wasmv_u8x16_t a) {
  wasmv_u8x16_t high;
  high.values = __builtin_shufflevector(a.values, a.values, 8, 9, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1);
  return wasmv_u8x16_extend_low(high);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_u16x8_extend_high(wasmv_u16x8_t a) {
  wasmv_u16x8_t high;
  high.values = __builtin_shufflevector(a.values, a.values, 4, 5, 6, 7, -1, -1, -1, -1);
  return wasmv_u16x8_extend_low(high);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u64x2_t
wasmv_u32x4_extend_high(wasmv_u32x4_t a) {
  wasmv_u32x4_t high;
  high.values = __builtin_shufflevector(a.values, a.values, 2, 3, -1, -1);
  return wasmv_u32x4_extend_low(high);
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_extend_high(a) \
    (_Generic((a), \
      wasmv_i8x16_t: wasmv_i8x16_extend_high, \
      wasmv_i16x8_t: wasmv_i16x8_extend_high, \
      wasmv_i32x4_t: wasmv_i32x4_extend_high, \
      wasmv_u8x16_t: wasmv_u8x16_extend_high, \
      wasmv_u16x8_t: wasmv_u16x8_extend_high, \
      wasmv_u32x4_t: wasmv_u32x4_extend_high) (a))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_extend_high(wasmv_i8x16_t a) { return wasmv_i8x16_extend_high(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_extend_high(wasmv_i16x8_t a) { return wasmv_i16x8_extend_high(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_extend_high(wasmv_i32x4_t a) { return wasmv_i32x4_extend_high(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_extend_high(wasmv_u8x16_t a) { return wasmv_u8x16_extend_high(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_extend_high(wasmv_u16x8_t a) { return wasmv_u16x8_extend_high(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_extend_high(wasmv_u32x4_t a) { return wasmv_u32x4_extend_high(a); }
#endif

/* narrow -- Narrowing / demotion
 *
 * Converts two input vectors into a smaller lane vector by narrowing
 * each lane, signed or unsigned. The signed narrowing operation will
 * use signed saturation to handle overflow, 0x7f or 0x80 for i8x16,
 * the unsigned narrowing operation will use unsigned saturation to
 * handle overflow, 0x00 or 0xff for i8x16.
 */

WASMV_FUNCTION_ATTRIBUTES
wasmv_i8x16_t
wasmv_i16x8_narrow(wasmv_i16x8_t a, wasmv_i16x8_t b) {
  return (wasmv_i8x16_t) { __builtin_wasm_narrow_s_i8x16_i16x8(a.values, b.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i32x4_narrow(wasmv_i32x4_t a, wasmv_i32x4_t b) {
  return (wasmv_i16x8_t) { __builtin_wasm_narrow_s_i16x8_i32x4(a.values, b.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u8x16_t
wasmv_u16x8_narrow(wasmv_u16x8_t a, wasmv_u16x8_t b) {
  return (wasmv_u8x16_t) { __builtin_wasm_narrow_u_i8x16_i16x8(a.values, b.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_u32x4_narrow(wasmv_u32x4_t a, wasmv_u32x4_t b) {
  return (wasmv_u16x8_t) { __builtin_wasm_narrow_u_i16x8_i32x4(a.values, b.values) };
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_narrow(a, b) \
    (_Generic((a), \
      wasmv_i16x8_t: wasmv_i16x8_narrow, \
      wasmv_i32x4_t: wasmv_i32x4_narrow, \
      wasmv_u16x8_t: wasmv_u16x8_narrow, \
      wasmv_u32x4_t: wasmv_u32x4_narrow) (a, b))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_narrow(wasmv_i16x8_t a, wasmv_i16x8_t b) { return wasmv_i16x8_narrow(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_narrow(wasmv_i32x4_t a, wasmv_i32x4_t b) { return wasmv_i32x4_narrow(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_narrow(wasmv_u16x8_t a, wasmv_u16x8_t b) { return wasmv_u16x8_narrow(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_narrow(wasmv_u32x4_t a, wasmv_u32x4_t b) { return wasmv_u32x4_narrow(a, b); }
#endif

/* demote -- Double-precision floating point to single-precision
 *
 * Conversion of the two double-precision floating point lanes to two
 * lower single-precision lanes of the result. The two higher lanes of
 * the result are initialized to zero. If the conversion result is not
 * representable as a single-precision floating point number, it is
 * rounded to the nearest-even representable number.
 */

WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WASMV_FUNCTION_ATTRIBUTES
wasmv_f32x4_t
wasmv_f64x2_demote(wasmv_f64x2_t a) {
  #if WASMV_BUILTIN_IS_NOOP
    return (wasmv_f32x4_t) { __builtin_wasm_demote_zero_f64x2_f32x4(a.values) };
  #else
    return (wasmv_f32x4_t) {
      (float) a.values[0], (float) a.values[1], 0.0f, 0.0f
    };
  #endif
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_demote(a) \
    (_Generic((a), \
      wasmv_f64x2_t: wasmv_f64x2_demote) (a))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_demote(wasmv_f64x2_t a) { return wasmv_f64x2_demote(a); }
#endif

/* extmul_low -- Extended wultiplication.
 *
 * Lane-wise integer extended multiplication producing twice wider
 * result than the inputs.
 */

WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i8x16_extmul_low(wasmv_i8x16_t a, wasmv_i8x16_t b) {
  #if WASMV_BUILTIN_IS_NOOP
    return (wasmv_i16x8_t) { __builtin_wasm_extmul_low_i8x16_s_i16x8(a.values, b.values) };
  #else
    return wasmv_i16x8_mul(wasmv_i8x16_extend_low(a), wasmv_i8x16_extend_low(b));
  #endif
}

WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i16x8_extmul_low(wasmv_i16x8_t a, wasmv_i16x8_t b) {
  #if WASMV_BUILTIN_IS_NOOP
    return (wasmv_i16x8_t) { __builtin_wasm_extmul_low_i16x8_s_i32x4(a.values, b.values) };
  #else
    return wasmv_i32x4_mul(wasmv_i16x8_extend_low(a), wasmv_i16x8_extend_low(b));
  #endif
}

WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_i32x4_extmul_low(wasmv_i32x4_t a, wasmv_i32x4_t b) {
  #if WASMV_BUILTIN_IS_NOOP
    return (wasmv_i16x8_t) { __builtin_wasm_extmul_low_i32x4_s_i64x2(a.values, b.values) };
  #else
    return wasmv_i64x2_mul(wasmv_i32x4_extend_low(a), wasmv_i32x4_extend_low(b));
  #endif
}

WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_u8x16_extmul_low(wasmv_u8x16_t a, wasmv_u8x16_t b) {
  #if WASMV_BUILTIN_IS_NOOP
    return (wasmv_i16x8_t) { __builtin_wasm_extmul_low_i8x16_u_i16x8(a.values, b.values) };
  #else
    return (wasmv_u16x8_t) { wasmv_u8x16_extend_low(a).values * wasmv_u8x16_extend_low(b).values };
  #endif
}

WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_u16x8_extmul_low(wasmv_u16x8_t a, wasmv_u16x8_t b) {
  #if WASMV_BUILTIN_IS_NOOP
    return (wasmv_i16x8_t) { __builtin_wasm_extmul_low_i16x8_u_i32x4(a.values, b.values) };
  #else
    return (wasmv_u32x4_t) { wasmv_u16x8_extend_low(a).values * wasmv_u16x8_extend_low(b).values };
  #endif
}

WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WASMV_FUNCTION_ATTRIBUTES
wasmv_u64x2_t
wasmv_u32x4_extmul_low(wasmv_u32x4_t a, wasmv_u32x4_t b) {
  #if WASMV_BUILTIN_IS_NOOP
    return (wasmv_i16x8_t) { __builtin_wasm_extmul_low_i32x4_u_i64x2(a.values, b.values) };
  #else
    return (wasmv_u64x2_t) { wasmv_u32x4_extend_low(a).values * wasmv_u32x4_extend_low(b).values };
  #endif
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_extmul_low(a, b) \
    (_Generic((a), \
      wasmv_i8x16_t: wasmv_i8x16_extmul_low, \
      wasmv_i16x8_t: wasmv_i16x8_extmul_low, \
      wasmv_i32x4_t: wasmv_i32x4_extmul_low, \
      wasmv_u8x16_t: wasmv_u8x16_extmul_low, \
      wasmv_u16x8_t: wasmv_u16x8_extmul_low, \
      wasmv_u32x4_t: wasmv_u32x4_extmul_low) (a, b))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_extmul_low(wasmv_i8x16_t a, wasmv_i8x16_t b) { return wasmv_i8x16_extmul_low(a, b); }
  WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_extmul_low(wasmv_i16x8_t a, wasmv_i16x8_t b) { return wasmv_i16x8_extmul_low(a, b); }
  WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_extmul_low(wasmv_i32x4_t a, wasmv_i32x4_t b) { return wasmv_i32x4_extmul_low(a, b); }
  WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_extmul_low(wasmv_u8x16_t a, wasmv_u8x16_t b) { return wasmv_u8x16_extmul_low(a, b); }
  WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_extmul_low(wasmv_u16x8_t a, wasmv_u16x8_t b) { return wasmv_u16x8_extmul_low(a, b); }
  WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_extmul_low(wasmv_u32x4_t a, wasmv_u32x4_t b) { return wasmv_u32x4_extmul_low(a, b); }
#endif

/* extmul_high -- Extended wultiplication.
 *
 * Lane-wise integer extended multiplication producing twice wider
 * result than the inputs.
 */

WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i8x16_extmul_high(wasmv_i8x16_t a, wasmv_i8x16_t b) {
  return wasmv_i16x8_mul(wasmv_i8x16_extend_high(a), wasmv_i8x16_extend_high(b));
}

WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i16x8_extmul_high(wasmv_i16x8_t a, wasmv_i16x8_t b) {
  return wasmv_i32x4_mul(wasmv_i16x8_extend_high(a), wasmv_i16x8_extend_high(b));
}

WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_i32x4_extmul_high(wasmv_i32x4_t a, wasmv_i32x4_t b) {
  return wasmv_i64x2_mul(wasmv_i32x4_extend_high(a), wasmv_i32x4_extend_high(b));
}

WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_u8x16_extmul_high(wasmv_u8x16_t a, wasmv_u8x16_t b) {
  return (wasmv_u16x8_t) { wasmv_u8x16_extend_high(a).values * wasmv_u8x16_extend_high(b).values };
}

WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_u16x8_extmul_high(wasmv_u16x8_t a, wasmv_u16x8_t b) {
  return (wasmv_u32x4_t) { wasmv_u16x8_extend_high(a).values * wasmv_u16x8_extend_high(b).values };
}

WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WASMV_FUNCTION_ATTRIBUTES
wasmv_u64x2_t
wasmv_u32x4_extmul_high(wasmv_u32x4_t a, wasmv_u32x4_t b) {
  return (wasmv_u64x2_t) { wasmv_u32x4_extend_high(a).values * wasmv_u32x4_extend_high(b).values };
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_extmul_high(a, b) \
    (_Generic((a), \
      wasmv_i8x16_t: wasmv_i8x16_extmul_high, \
      wasmv_i16x8_t: wasmv_i16x8_extmul_high, \
      wasmv_i32x4_t: wasmv_i32x4_extmul_high, \
      wasmv_u8x16_t: wasmv_u8x16_extmul_high, \
      wasmv_u16x8_t: wasmv_u16x8_extmul_high, \
      wasmv_u32x4_t: wasmv_u32x4_extmul_high) (a, b))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_extmul_high(wasmv_i8x16_t a, wasmv_i8x16_t b) { return wasmv_i8x16_extmul_high(a, b); }
  WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_extmul_high(wasmv_i16x8_t a, wasmv_i16x8_t b) { return wasmv_i16x8_extmul_high(a, b); }
  WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_extmul_high(wasmv_i32x4_t a, wasmv_i32x4_t b) { return wasmv_i32x4_extmul_high(a, b); }
  WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_extmul_high(wasmv_u8x16_t a, wasmv_u8x16_t b) { return wasmv_u8x16_extmul_high(a, b); }
  WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_extmul_high(wasmv_u16x8_t a, wasmv_u16x8_t b) { return wasmv_u16x8_extmul_high(a, b); }
  WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_extmul_high(wasmv_u32x4_t a, wasmv_u32x4_t b) { return wasmv_u32x4_extmul_high(a, b); }
#endif

/* extadd_pairwise
 *
 * Lane-wise integer extended pairwise addition producing extended
 * results (twice wider results than the inputs).
 */

WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i8x16_extadd_pairwise(wasmv_i8x16_t v) {
  #if WASMV_BUILTIN_IS_NOOP
    return (wasmv_i16x8_t) { __builtin_wasm_extadd_pairwise_i8x16_s_i16x8(v.values) };
  #else
    wasmv_i16x8_t r;
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

WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i16x8_extadd_pairwise(wasmv_i16x8_t v) {
  #if WASMV_BUILTIN_IS_NOOP
    return (wasmv_i32x4_t) { __builtin_wasm_extadd_pairwise_i16x8_s_i32x4(v.values) };
  #else
    wasmv_i32x4_t r;
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

WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_u8x16_extadd_pairwise(wasmv_u8x16_t v) {
  #if WASMV_BUILTIN_IS_NOOP
    return (wasmv_i32x4_t) { __builtin_wasm_extadd_pairwise_i8x16_u_i16x8(v.values) };
  #else
    wasmv_u16x8_t r;
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

WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_u16x8_extadd_pairwise(wasmv_u16x8_t v) {
  #if WASMV_BUILTIN_IS_NOOP
    return (wasmv_i32x4_t) { __builtin_wasm_extadd_pairwise_i8x16_u_i16x8(v.values) };
  #else
    wasmv_u32x4_t r;
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

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_extadd_pairwise(a) \
    (_Generic((a), \
      wasmv_i8x16_t: wasmv_i8x16_extadd_pairwise, \
      wasmv_i16x8_t: wasmv_i16x8_extadd_pairwise, \
      wasmv_u8x16_t: wasmv_u8x16_extadd_pairwise, \
      wasmv_u16x8_t: wasmv_u16x8_extadd_pairwise) (a))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_extadd_pairwise(wasmv_i8x16_t v) { return wasmv_i8x16_extadd_pairwise(v); }
  WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_extadd_pairwise(wasmv_i16x8_t v) { return wasmv_i16x8_extadd_pairwise(v); }
  WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_extadd_pairwise(wasmv_u8x16_t v) { return wasmv_u8x16_extadd_pairwise(v); }
  WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_extadd_pairwise(wasmv_u16x8_t v) { return wasmv_u16x8_extadd_pairwise(v); }
#endif

/* dot — dot product
 *
 * Lane-wise multiply signed 16-bit integers in the two input vectors
 * and add adjacent pairs of the full 32-bit results.
 */

WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i16x8_dot(wasmv_i16x8_t a, wasmv_i16x8_t b) {
  #if WASMV_BUILTIN_IS_NOOP
    return (wasmv_i32x4_t) { __builtin_wasm_dot_s_i32x4_i16x8(a.values, b.values) };
  #else
    return wasmv_i16x8_extadd_pairwise(wasmv_i16x8_mul(a, b));
  #endif
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_dot(a, b) \
    (_Generic((a) \
      wasmv_i16x8_t: wasmv_i16x8_dot) (a, b))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_dot(wasmv_i16x8_t a, wasmv_i16x8_t b) { return wasmv_i16x8_dot(a, b); }
#endif

/* add_saturate -- Saturating integer addition
 *
 * Lane-wise saturating addition.
 */

WASMV_FUNCTION_ATTRIBUTES
wasmv_i8x16_t
wasmv_i8x16_add_saturate(wasmv_i8x16_t a, wasmv_i8x16_t b) {
  return (wasmv_i8x16_t) { __builtin_wasm_add_saturate_s_i8x16(a.values, b.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i16x8_add_saturate(wasmv_i16x8_t a, wasmv_i16x8_t b) {
  return (wasmv_i16x8_t) { __builtin_wasm_add_saturate_s_i16x8(a.values, b.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u8x16_t
wasmv_u8x16_add_saturate(wasmv_u8x16_t a, wasmv_u8x16_t b) {
  return (wasmv_u8x16_t) { __builtin_wasm_add_saturate_u_i8x16(a.values, b.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_u16x8_add_saturate(wasmv_u16x8_t a, wasmv_u16x8_t b) {
  return (wasmv_u16x8_t) { __builtin_wasm_add_saturate_u_i16x8(a.values, b.values) };
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_add_saturate(a, b) \
    (_Generic((a), \
      wasmv_i8x16_t: wasmv_i8x16_add_saturate, \
      wasmv_i16x8_t: wasmv_i16x8_add_saturate, \
      wasmv_u8x16_t: wasmv_u8x16_add_saturate, \
      wasmv_u16x8_t: wasmv_u16x8_add_saturate) (a, b))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_add_saturate(wasmv_i8x16_t a, wasmv_i8x16_t b) { return wasmv_i8x16_add_saturate(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_add_saturate(wasmv_i16x8_t a, wasmv_i16x8_t b) { return wasmv_i16x8_add_saturate(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_add_saturate(wasmv_u8x16_t a, wasmv_u8x16_t b) { return wasmv_u8x16_add_saturate(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_add_saturate(wasmv_u16x8_t a, wasmv_u16x8_t b) { return wasmv_u16x8_add_saturate(a, b); }
#endif

/* sub_saturate -- Saturating integer subtraction
 *
 * Lane-wise saturating subtraction.
 */

WASMV_FUNCTION_ATTRIBUTES
wasmv_i8x16_t
wasmv_i8x16_sub_saturate(wasmv_i8x16_t a, wasmv_i8x16_t b) {
  return (wasmv_i8x16_t) { __builtin_wasm_sub_saturate_s_i8x16(a.values, b.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i16x8_sub_saturate(wasmv_i16x8_t a, wasmv_i16x8_t b) {
  return (wasmv_i16x8_t) { __builtin_wasm_sub_saturate_s_i16x8(a.values, b.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u8x16_t
wasmv_u8x16_sub_saturate(wasmv_u8x16_t a, wasmv_u8x16_t b) {
  return (wasmv_u8x16_t) { __builtin_wasm_sub_saturate_u_i8x16(a.values, b.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_u16x8_sub_saturate(wasmv_u16x8_t a, wasmv_u16x8_t b) {
  return (wasmv_u16x8_t) { __builtin_wasm_sub_saturate_u_i16x8(a.values, b.values) };
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_sub_saturate(a, b) \
    (_Generic((a), \
      wasmv_i8x16_t: wasmv_i8x16_sub_saturate, \
      wasmv_i16x8_t: wasmv_i16x8_sub_saturate, \
      wasmv_u8x16_t: wasmv_u8x16_sub_saturate, \
      wasmv_u16x8_t: wasmv_u16x8_sub_saturate) (a, b))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_sub_saturate(wasmv_i8x16_t a, wasmv_i8x16_t b) { return wasmv_i8x16_sub_saturate(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_sub_saturate(wasmv_i16x8_t a, wasmv_i16x8_t b) { return wasmv_i16x8_sub_saturate(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_sub_saturate(wasmv_u8x16_t a, wasmv_u8x16_t b) { return wasmv_u8x16_sub_saturate(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_sub_saturate(wasmv_u16x8_t a, wasmv_u16x8_t b) { return wasmv_u16x8_sub_saturate(a, b); }
#endif

/* q15mulr_saturate -- Saturating integer Q-format rounding multiplication
 *
 * Lane-wise saturating rounding multiplication in Q15 format
 */

#if WASMV_BUILTIN_IS_NOOP

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i16x8_q15mulr_saturate(wasmv_i16x8_t a, wasmv_i16x8_t b) {
  return (wasmv_i16x8_t) { __builtin_wasm_q15mulr_saturate_s_i16x8(a.values, b.values) };
}

#endif

/* min -- Lane-wise integer minimum
 *
 * Compares lane-wise signed/unsigned integers, and returns the minimum
 * of each pair.
 */

WASMV_FUNCTION_ATTRIBUTES
wasmv_i8x16_t
wasmv_i8x16_min(wasmv_i8x16_t a, wasmv_i8x16_t b)  {
  return (wasmv_i8x16_t) { __builtin_wasm_min_s_i8x16(a.values, b.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i16x8_min(wasmv_i16x8_t a, wasmv_i16x8_t b)  {
  return (wasmv_i16x8_t) { __builtin_wasm_min_s_i16x8(a.values, b.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i32x4_min(wasmv_i32x4_t a, wasmv_i32x4_t b)  {
  return (wasmv_i32x4_t) { __builtin_wasm_min_s_i32x4(a.values, b.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u8x16_t
wasmv_u8x16_min(wasmv_u8x16_t a, wasmv_u8x16_t b)  {
  return (wasmv_u8x16_t) { __builtin_wasm_min_u_i8x16(a.values, b.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_u16x8_min(wasmv_u16x8_t a, wasmv_u16x8_t b)  {
  return (wasmv_u16x8_t) { __builtin_wasm_min_u_i16x8(a.values, b.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_u32x4_min(wasmv_u32x4_t a, wasmv_u32x4_t b)  {
  return (wasmv_u32x4_t) { __builtin_wasm_min_u_i32x4(a.values, b.values) };
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_min(a, b) \
    (_Generic((a), \
      wasmv_i8x16_t: wasmv_i8x16_min, \
      wasmv_i16x8_t: wasmv_i16x8_min, \
      wasmv_i32x4_t: wasmv_i32x4_min, \
      wasmv_u8x16_t: wasmv_u8x16_min, \
      wasmv_u16x8_t: wasmv_u16x8_min, \
      wasmv_u32x4_t: wasmv_u32x4_min) (a, b))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_min(wasmv_i8x16_t a, wasmv_i8x16_t b) { return wasmv_i8x16_min(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_min(wasmv_i16x8_t a, wasmv_i16x8_t b) { return wasmv_i16x8_min(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_min(wasmv_i32x4_t a, wasmv_i32x4_t b) { return wasmv_i32x4_min(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_min(wasmv_u8x16_t a, wasmv_u8x16_t b) { return wasmv_u8x16_min(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_min(wasmv_u16x8_t a, wasmv_u16x8_t b) { return wasmv_u16x8_min(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_min(wasmv_u32x4_t a, wasmv_u32x4_t b) { return wasmv_u32x4_min(a, b); }
#endif

/* max -- Lane-wise integer maximum
 *
 * Compares lane-wise signed/unsigned integers, and returns the maximum
 * of each pair.
 */

WASMV_FUNCTION_ATTRIBUTES
wasmv_i8x16_t
wasmv_i8x16_max(wasmv_i8x16_t a, wasmv_i8x16_t b)  {
  return (wasmv_i8x16_t) { __builtin_wasm_max_s_i8x16(a.values, b.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i16x8_max(wasmv_i16x8_t a, wasmv_i16x8_t b)  {
  return (wasmv_i16x8_t) { __builtin_wasm_max_s_i16x8(a.values, b.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i32x4_max(wasmv_i32x4_t a, wasmv_i32x4_t b)  {
  return (wasmv_i32x4_t) { __builtin_wasm_max_s_i32x4(a.values, b.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u8x16_t
wasmv_u8x16_max(wasmv_u8x16_t a, wasmv_u8x16_t b)  {
  return (wasmv_u8x16_t) { __builtin_wasm_max_u_i8x16(a.values, b.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_u16x8_max(wasmv_u16x8_t a, wasmv_u16x8_t b)  {
  return (wasmv_u16x8_t) { __builtin_wasm_max_u_i16x8(a.values, b.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_u32x4_max(wasmv_u32x4_t a, wasmv_u32x4_t b)  {
  return (wasmv_u32x4_t) { __builtin_wasm_max_u_i32x4(a.values, b.values) };
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_max(a, b) \
    (_Generic((a), \
      wasmv_i8x16_t: wasmv_i8x16_max, \
      wasmv_i16x8_t: wasmv_i16x8_max, \
      wasmv_i32x4_t: wasmv_i32x4_max, \
      wasmv_u8x16_t: wasmv_u8x16_max, \
      wasmv_u16x8_t: wasmv_u16x8_max, \
      wasmv_u32x4_t: wasmv_u32x4_max) (a, b))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_max(wasmv_i8x16_t a, wasmv_i8x16_t b) { return wasmv_i8x16_max(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_max(wasmv_i16x8_t a, wasmv_i16x8_t b) { return wasmv_i16x8_max(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_max(wasmv_i32x4_t a, wasmv_i32x4_t b) { return wasmv_i32x4_max(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_max(wasmv_u8x16_t a, wasmv_u8x16_t b) { return wasmv_u8x16_max(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_max(wasmv_u16x8_t a, wasmv_u16x8_t b) { return wasmv_u16x8_max(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_max(wasmv_u32x4_t a, wasmv_u32x4_t b) { return wasmv_u32x4_max(a, b); }
#endif

/* avgr -- Lane-wise integer rounding average
 *
 * Lane-wise rounding average.
 */

WASMV_FUNCTION_ATTRIBUTES
wasmv_u8x16_t
wasmv_u8x16_avgr(wasmv_u8x16_t a, wasmv_u8x16_t b)  {
  return (wasmv_u8x16_t) { __builtin_wasm_avgr_u_i8x16(a.values, b.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_u16x8_avgr(wasmv_u16x8_t a, wasmv_u16x8_t b)  {
  return (wasmv_u16x8_t) { __builtin_wasm_avgr_u_i16x8(a.values, b.values) };
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_avgr(a, b) \
    (_Generic((a), \
      wasmv_u8x16_t: wasmv_u8x16_avgr, \
      wasmv_u16x8_t: wasmv_u16x8_avgr) (a, b))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_avgr(wasmv_u8x16_t a, wasmv_u8x16_t b) { return wasmv_u8x16_avgr(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_avgr(wasmv_u16x8_t a, wasmv_u16x8_t b) { return wasmv_u16x8_avgr(a, b); }
#endif

/* abs -- Lane-wise integer absolute value
 *
 * Lane-wise wrapping absolute value.
 */

WASMV_FUNCTION_ATTRIBUTES
wasmv_i8x16_t
wasmv_i8x16_abs(wasmv_i8x16_t a) {
  return (wasmv_i8x16_t) { -a.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i16x8_abs(wasmv_i16x8_t a) {
  return (wasmv_i16x8_t) { -a.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i32x4_abs(wasmv_i32x4_t a) {
  return (wasmv_i32x4_t) { -a.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_i64x2_abs(wasmv_i64x2_t a) {
  return (wasmv_i64x2_t) { -a.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u8x16_t
wasmv_u8x16_abs(wasmv_u8x16_t a) {
  return (wasmv_u8x16_t) { -a.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f32x4_t
wasmv_f32x4_abs(wasmv_f32x4_t a) {
  return (wasmv_f32x4_t) { -a.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f64x2_t
wasmv_f64x2_abs(wasmv_f64x2_t a) {
  return (wasmv_f64x2_t) { -a.values };
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_abs(a) \
    (_Generic((a), \
      wasmv_i8x16_t: wasmv_i8x16_abs, \
      wasmv_i16x8_t: wasmv_i16x8_abs, \
      wasmv_i32x4_t: wasmv_i32x4_abs, \
      wasmv_i64x2_t: wasmv_i64x2_abs, \
      wasmv_f32x4_t: wasmv_f32x4_abs, \
      wasmv_f64x2_t: wasmv_f64x2_abs) (a))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_abs(wasmv_i8x16_t a) { return wasmv_i8x16_abs(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_abs(wasmv_i16x8_t a) { return wasmv_i16x8_abs(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_abs(wasmv_i32x4_t a) { return wasmv_i32x4_abs(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_abs(wasmv_i64x2_t a) { return wasmv_i64x2_abs(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_abs(wasmv_f32x4_t a) { return wasmv_f32x4_abs(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_abs(wasmv_f64x2_t a) { return wasmv_f64x2_abs(a); }
#endif

/* shl -- Left shift by scalar
 *
 * Shift the bits in each lane to the left by the same amount. The
 * shift count is taken modulo lane width.
 */

WASMV_FUNCTION_ATTRIBUTES
wasmv_i8x16_t
wasmv_i8x16_shl(wasmv_i8x16_t a, int count) {
  return (wasmv_i8x16_t) { a.values << (count & 15) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i16x8_shl(wasmv_i16x8_t a, int count) {
  return (wasmv_i16x8_t) { a.values << (count & 15) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i32x4_shl(wasmv_i32x4_t a, int count) {
  return (wasmv_i32x4_t) { a.values << (count & 15) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_i64x2_shl(wasmv_i64x2_t a, int count) {
  return (wasmv_i64x2_t) { a.values << (count & 15) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u8x16_t
wasmv_u8x16_shl(wasmv_u8x16_t a, int count) {
  return (wasmv_u8x16_t) { a.values << (count & 15) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_u16x8_shl(wasmv_u16x8_t a, int count) {
  return (wasmv_u16x8_t) { a.values << (count & 15) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_u32x4_shl(wasmv_u32x4_t a, int count) {
  return (wasmv_u32x4_t) { a.values << (count & 15) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u64x2_t
wasmv_u64x2_shl(wasmv_u64x2_t a, int count) {
  return (wasmv_u64x2_t) { a.values << (count & 15) };
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_shl(a, count) \
    (_Generic((a), \
      wasmv_i8x16_t: wasmv_i8x16_shl, \
      wasmv_i16x8_t: wasmv_i16x8_shl, \
      wasmv_i32x4_t: wasmv_i32x4_shl, \
      wasmv_i64x2_t: wasmv_i64x2_shl, \
      wasmv_u8x16_t: wasmv_u8x16_shl, \
      wasmv_u16x8_t: wasmv_u16x8_shl, \
      wasmv_u32x4_t: wasmv_u32x4_shl, \
      wasmv_u64x2_t: wasmv_u64x2_shl) (a, count))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_shl(wasmv_i8x16_t a, int count) { return wasmv_i8x16_shl(a, count); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_shl(wasmv_i16x8_t a, int count) { return wasmv_i16x8_shl(a, count); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_shl(wasmv_i32x4_t a, int count) { return wasmv_i32x4_shl(a, count); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_shl(wasmv_i64x2_t a, int count) { return wasmv_i64x2_shl(a, count); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_shl(wasmv_u8x16_t a, int count) { return wasmv_u8x16_shl(a, count); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_shl(wasmv_u16x8_t a, int count) { return wasmv_u16x8_shl(a, count); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_shl(wasmv_u32x4_t a, int count) { return wasmv_u32x4_shl(a, count); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_shl(wasmv_u64x2_t a, int count) { return wasmv_u64x2_shl(a, count); }

  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t operator<<(wasmv_i8x16_t a, int count) { return wasmv_i8x16_shl(a, count); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t operator<<(wasmv_i16x8_t a, int count) { return wasmv_i16x8_shl(a, count); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t operator<<(wasmv_i32x4_t a, int count) { return wasmv_i32x4_shl(a, count); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t operator<<(wasmv_i64x2_t a, int count) { return wasmv_i64x2_shl(a, count); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t operator<<(wasmv_u8x16_t a, int count) { return wasmv_u8x16_shl(a, count); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t operator<<(wasmv_u16x8_t a, int count) { return wasmv_u16x8_shl(a, count); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t operator<<(wasmv_u32x4_t a, int count) { return wasmv_u32x4_shl(a, count); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t operator<<(wasmv_u64x2_t a, int count) { return wasmv_u64x2_shl(a, count); }
#endif

/* shr -- Right shift by scalar
 *
 * Shift the bits in each lane to the right by the same amount. The
 * shift count is taken modulo lane width. This is an arithmetic right
 * shift for the signed variants and a logical right shift for the
 * unsigned variants.
 */

WASMV_FUNCTION_ATTRIBUTES
wasmv_i8x16_t
wasmv_i8x16_shr(wasmv_i8x16_t a, int count) {
  return (wasmv_i8x16_t) { a.values << (count & 15) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i16x8_shr(wasmv_i16x8_t a, int count) {
  return (wasmv_i16x8_t) { a.values << (count & 15) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i32x4_shr(wasmv_i32x4_t a, int count) {
  return (wasmv_i32x4_t) { a.values << (count & 15) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_i64x2_shr(wasmv_i64x2_t a, int count) {
  return (wasmv_i64x2_t) { a.values << (count & 15) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u8x16_t
wasmv_u8x16_shr(wasmv_u8x16_t a, int count) {
  return (wasmv_u8x16_t) { a.values << (count & 15) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_u16x8_shr(wasmv_u16x8_t a, int count) {
  return (wasmv_u16x8_t) { a.values << (count & 15) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_u32x4_shr(wasmv_u32x4_t a, int count) {
  return (wasmv_u32x4_t) { a.values << (count & 15) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u64x2_t
wasmv_u64x2_shr(wasmv_u64x2_t a, int count) {
  return (wasmv_u64x2_t) { a.values << (count & 15) };
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_shr(a, count) \
    (_Generic((a), \
      wasmv_i8x16_t: wasmv_i8x16_shr, \
      wasmv_i16x8_t: wasmv_i16x8_shr, \
      wasmv_i32x4_t: wasmv_i32x4_shr, \
      wasmv_i64x2_t: wasmv_i64x2_shr, \
      wasmv_u8x16_t: wasmv_u8x16_shr, \
      wasmv_u16x8_t: wasmv_u16x8_shr, \
      wasmv_u32x4_t: wasmv_u32x4_shr, \
      wasmv_u64x2_t: wasmv_u64x2_shr) (a, count))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_shr(wasmv_i8x16_t a, int count) { return wasmv_i8x16_shr(a, count); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_shr(wasmv_i16x8_t a, int count) { return wasmv_i16x8_shr(a, count); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_shr(wasmv_i32x4_t a, int count) { return wasmv_i32x4_shr(a, count); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_shr(wasmv_i64x2_t a, int count) { return wasmv_i64x2_shr(a, count); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_shr(wasmv_u8x16_t a, int count) { return wasmv_u8x16_shr(a, count); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_shr(wasmv_u16x8_t a, int count) { return wasmv_u16x8_shr(a, count); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_shr(wasmv_u32x4_t a, int count) { return wasmv_u32x4_shr(a, count); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_shr(wasmv_u64x2_t a, int count) { return wasmv_u64x2_shr(a, count); }

  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t operator>>(wasmv_i8x16_t a, int count) { return wasmv_i8x16_shr(a, count); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t operator>>(wasmv_i16x8_t a, int count) { return wasmv_i16x8_shr(a, count); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t operator>>(wasmv_i32x4_t a, int count) { return wasmv_i32x4_shr(a, count); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t operator>>(wasmv_i64x2_t a, int count) { return wasmv_i64x2_shr(a, count); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t operator>>(wasmv_u8x16_t a, int count) { return wasmv_u8x16_shr(a, count); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t operator>>(wasmv_u16x8_t a, int count) { return wasmv_u16x8_shr(a, count); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t operator>>(wasmv_u32x4_t a, int count) { return wasmv_u32x4_shr(a, count); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t operator>>(wasmv_u64x2_t a, int count) { return wasmv_u64x2_shr(a, count); }
#endif

/* and -- Bitwise AND */

WASMV_FUNCTION_ATTRIBUTES
wasmv_i8x16_t
wasmv_i8x16_and(wasmv_i8x16_t a, wasmv_i8x16_t b) {
  return (wasmv_i8x16_t) { a.values & b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i16x8_and(wasmv_i16x8_t a, wasmv_i16x8_t b) {
  return (wasmv_i16x8_t) { a.values & b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i32x4_and(wasmv_i32x4_t a, wasmv_i32x4_t b) {
  return (wasmv_i32x4_t) { a.values & b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_i64x2_and(wasmv_i64x2_t a, wasmv_i64x2_t b) {
  return (wasmv_i64x2_t) { a.values & b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u8x16_t
wasmv_u8x16_and(wasmv_u8x16_t a, wasmv_u8x16_t b) {
  return (wasmv_u8x16_t) { a.values & b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_u16x8_and(wasmv_u16x8_t a, wasmv_u16x8_t b) {
  return (wasmv_u16x8_t) { a.values & b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_u32x4_and(wasmv_u32x4_t a, wasmv_u32x4_t b) {
  return (wasmv_u32x4_t) { a.values & b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u64x2_t
wasmv_u64x2_and(wasmv_u64x2_t a, wasmv_u64x2_t b) {
  return (wasmv_u64x2_t) { a.values & b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b8x16_t
wasmv_b8x16_and(wasmv_b8x16_t a, wasmv_b8x16_t b) {
  return (wasmv_b8x16_t) { a.values & b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b16x8_t
wasmv_b16x8_and(wasmv_b16x8_t a, wasmv_b16x8_t b) {
  return (wasmv_b16x8_t) { a.values & b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b32x4_t
wasmv_b32x4_and(wasmv_b32x4_t a, wasmv_b32x4_t b) {
  return (wasmv_b32x4_t) { a.values & b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b64x2_t
wasmv_b64x2_and(wasmv_b64x2_t a, wasmv_b64x2_t b) {
  return (wasmv_b64x2_t) { a.values & b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i8x16_t
wasmv_i8x16_and_b8x16(wasmv_i8x16_t a, wasmv_b8x16_t b) {
  return (wasmv_i8x16_t) { a.values & b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i16x8_and_b16x8(wasmv_i16x8_t a, wasmv_b16x8_t b) {
  return (wasmv_i16x8_t) { a.values & b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i32x4_and_b32x4(wasmv_i32x4_t a, wasmv_b32x4_t b) {
  return (wasmv_i32x4_t) { a.values & b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_i64x2_and_b64x2(wasmv_i64x2_t a, wasmv_b64x2_t b) {
  return (wasmv_i64x2_t) { a.values & b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u8x16_t
wasmv_u8x16_and_b8x16(wasmv_u8x16_t a, wasmv_b8x16_t b) {
  return (wasmv_u8x16_t) { a.values & b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_u16x8_and_b16x8(wasmv_u16x8_t a, wasmv_b16x8_t b) {
  return (wasmv_u16x8_t) { a.values & b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_u32x4_and_b32x4(wasmv_u32x4_t a, wasmv_b32x4_t b) {
  return (wasmv_u32x4_t) { a.values & b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u64x2_t
wasmv_u64x2_and_b64x2(wasmv_u64x2_t a, wasmv_b64x2_t b) {
  return (wasmv_u64x2_t) { a.values & b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f32x4_t
wasmv_f32x4_and_b32x4(wasmv_f32x4_t a, wasmv_b32x4_t b) {
  return wasmv_u32x4_as_f32x4(wasmv_u32x4_and_b32x4(wasmv_f32x4_as_u32x4(a), b));
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f64x2_t
wasmv_f64x2_and_b64x2(wasmv_f64x2_t a, wasmv_b64x2_t b) {
  return wasmv_u64x2_as_f64x2(wasmv_u64x2_and_b64x2(wasmv_f64x2_as_u64x2(a), b));
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i8x16_t
wasmv_b8x16_and_i8x16(wasmv_b8x16_t a, wasmv_i8x16_t b) {
  return wasmv_i8x16_and_b8x16(b, a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_b16x8_and_i16x8(wasmv_b16x8_t a, wasmv_i16x8_t b) {
  return wasmv_i16x8_and_b16x8(b, a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_b32x4_and_i32x4(wasmv_b32x4_t a, wasmv_i32x4_t b) {
  return wasmv_i32x4_and_b32x4(b, a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_b64x2_and_i64x2(wasmv_b64x2_t a, wasmv_i64x2_t b) {
  return wasmv_i64x2_and_b64x2(b, a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u8x16_t
wasmv_b8x16_and_u8x16(wasmv_b8x16_t a, wasmv_u8x16_t b) {
  return wasmv_u8x16_and_b8x16(b, a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_b16x8_and_u16x8(wasmv_b16x8_t a, wasmv_u16x8_t b) {
  return wasmv_u16x8_and_b16x8(b, a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_b32x4_and_u32x4(wasmv_b32x4_t a, wasmv_u32x4_t b) {
  return wasmv_u32x4_and_b32x4(b, a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u64x2_t
wasmv_b64x2_and_u64x2(wasmv_b64x2_t a, wasmv_u64x2_t b) {
  return wasmv_u64x2_and_b64x2(b, a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f32x4_t
wasmv_b32x4_and_f32x4(wasmv_b32x4_t a, wasmv_f32x4_t b) {
  return wasmv_f32x4_and_b32x4(b, a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f64x2_t
wasmv_b64x2_and_f64x2(wasmv_b64x2_t a, wasmv_f64x2_t b) {
  return wasmv_f64x2_and_b64x2(b, a);
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_and(a, b) \
    (_Generic((a), \
      wasmv_i8x16_t: (_Generic((b), \
        wasmv_b8x16_t: wasmv_i8x16_and_b8x16, \
        default /* wasmv_i8x16_t */: wasmv_i8x16_and)), \
      wasmv_i16x8_t: (_Generic((b), \
        wasmv_b16x8_t: wasmv_i16x8_and_b16x8, \
        default /* wasmv_i16x8_t */: wasmv_i16x8_and)), \
      wasmv_i32x4_t: (_Generic((b), \
        wasmv_b32x4_t: wasmv_i32x4_and_b32x4, \
        default /* wasmv_i32x4_t */: wasmv_i32x4_and)), \
      wasmv_i64x2_t: (_Generic((b), \
        wasmv_b64x2_t: wasmv_i64x2_and_b64x2, \
        default /* wasmv_i64x2_t */: wasmv_i64x2_and)), \
      wasmv_u8x16_t: (_Generic((b), \
        wasmv_b8x16_t: wasmv_u8x16_and_b8x16, \
        default /* wasmv_u8x16_t */: wasmv_u8x16_and)), \
      wasmv_u16x8_t: (_Generic((b), \
        wasmv_b16x8_t: wasmv_u16x8_and_b16x8, \
        default /* wasmv_u16x8_t */: wasmv_u16x8_and)), \
      wasmv_u32x4_t: (_Generic((b), \
        wasmv_b32x4_t: wasmv_u32x4_and_b32x4, \
        default /* wasmv_u32x4_t */: wasmv_u32x4_and)), \
      wasmv_u64x2_t: (_Generic((b), \
        wasmv_b64x2_t: wasmv_u64x2_and_b64x2, \
        default /* wasmv_u64x2_t */: wasmv_u64x2_and)), \
      wasmv_f32x4_t: wasmv_f32x4_and_b32x4, \
      wasmv_f64x2_t: wasmv_f64x2_and_b64x2, \
      wasmv_b8x16_t: (_Generic((b), \
        wasmv_i8x16_t: wasmv_b8x16_and_i8x16, \
        wasmv_u8x16_t: wasmv_b8x16_and_u8x16, \
        default /* wasmv_b8x16_t */: wasmv_b8x16_and)), \
      wasmv_b16x8_t: (_Generic((b), \
        wasmv_i16x8_t: wasmv_b16x8_and_i16x8, \
        wasmv_u16x8_t: wasmv_b16x8_and_u16x8, \
        default /* wasmv_b16x8_t */: wasmv_b16x8_and)), \
      wasmv_b32x4_t: (_Generic((b), \
        wasmv_i32x4_t: wasmv_b32x4_and_i32x4, \
        wasmv_u32x4_t: wasmv_b32x4_and_u32x4, \
        wasmv_f32x4_t: wasmv_b32x4_and_f32x4, \
        default /* wasmv_b32x4_t */: wasmv_b32x4_and)), \
      wasmv_b64x2_t: (_Generic((b), \
        wasmv_i64x2_t: wasmv_b64x2_and_i64x2, \
        wasmv_u64x2_t: wasmv_b64x2_and_u64x2, \
        wasmv_f64x2_t: wasmv_b64x2_and_f64x2, \
        default /* wasmv_b64x2_t */: wasmv_b64x2_and))) (a, b))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_and(wasmv_i8x16_t a, wasmv_i8x16_t b) { return wasmv_i8x16_and(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_and(wasmv_i16x8_t a, wasmv_i16x8_t b) { return wasmv_i16x8_and(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_and(wasmv_i32x4_t a, wasmv_i32x4_t b) { return wasmv_i32x4_and(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_and(wasmv_i64x2_t a, wasmv_i64x2_t b) { return wasmv_i64x2_and(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_and(wasmv_u8x16_t a, wasmv_u8x16_t b) { return wasmv_u8x16_and(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_and(wasmv_u16x8_t a, wasmv_u16x8_t b) { return wasmv_u16x8_and(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_and(wasmv_u32x4_t a, wasmv_u32x4_t b) { return wasmv_u32x4_and(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_and(wasmv_u64x2_t a, wasmv_u64x2_t b) { return wasmv_u64x2_and(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_and(wasmv_b8x16_t a, wasmv_b8x16_t b) { return wasmv_b8x16_and(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_and(wasmv_b16x8_t a, wasmv_b16x8_t b) { return wasmv_b16x8_and(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_and(wasmv_b32x4_t a, wasmv_b32x4_t b) { return wasmv_b32x4_and(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_and(wasmv_b64x2_t a, wasmv_b64x2_t b) { return wasmv_b64x2_and(a, b); }

  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_and(wasmv_i8x16_t a, wasmv_b8x16_t b) { return wasmv_i8x16_and_b8x16(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_and(wasmv_i16x8_t a, wasmv_b16x8_t b) { return wasmv_i16x8_and_b16x8(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_and(wasmv_i32x4_t a, wasmv_b32x4_t b) { return wasmv_i32x4_and_b32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_and(wasmv_i64x2_t a, wasmv_b64x2_t b) { return wasmv_i64x2_and_b64x2(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_and(wasmv_u8x16_t a, wasmv_b8x16_t b) { return wasmv_u8x16_and_b8x16(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_and(wasmv_u16x8_t a, wasmv_b16x8_t b) { return wasmv_u16x8_and_b16x8(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_and(wasmv_u32x4_t a, wasmv_b32x4_t b) { return wasmv_u32x4_and_b32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_and(wasmv_u64x2_t a, wasmv_b64x2_t b) { return wasmv_u64x2_and_b64x2(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_and(wasmv_f32x4_t a, wasmv_b32x4_t b) { return wasmv_f32x4_and_b32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_and(wasmv_f64x2_t a, wasmv_b64x2_t b) { return wasmv_f64x2_and_b64x2(a, b); }

  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_and(wasmv_b8x16_t a, wasmv_i8x16_t b) { return wasmv_b8x16_and_i8x16(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_and(wasmv_b16x8_t a, wasmv_i16x8_t b) { return wasmv_b16x8_and_i16x8(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_and(wasmv_b32x4_t a, wasmv_i32x4_t b) { return wasmv_b32x4_and_i32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_and(wasmv_b64x2_t a, wasmv_i64x2_t b) { return wasmv_b64x2_and_i64x2(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_and(wasmv_b8x16_t a, wasmv_u8x16_t b) { return wasmv_b8x16_and_u8x16(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_and(wasmv_b16x8_t a, wasmv_u16x8_t b) { return wasmv_b16x8_and_u16x8(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_and(wasmv_b32x4_t a, wasmv_u32x4_t b) { return wasmv_b32x4_and_u32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_and(wasmv_b64x2_t a, wasmv_u64x2_t b) { return wasmv_b64x2_and_u64x2(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_and(wasmv_b32x4_t a, wasmv_f32x4_t b) { return wasmv_b32x4_and_f32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_and(wasmv_b64x2_t a, wasmv_f64x2_t b) { return wasmv_b64x2_and_f64x2(a, b); }

  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t operator&(wasmv_i8x16_t a, wasmv_i8x16_t b) { return wasmv_i8x16_and(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t operator&(wasmv_i16x8_t a, wasmv_i16x8_t b) { return wasmv_i16x8_and(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t operator&(wasmv_i32x4_t a, wasmv_i32x4_t b) { return wasmv_i32x4_and(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t operator&(wasmv_i64x2_t a, wasmv_i64x2_t b) { return wasmv_i64x2_and(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t operator&(wasmv_u8x16_t a, wasmv_u8x16_t b) { return wasmv_u8x16_and(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t operator&(wasmv_u16x8_t a, wasmv_u16x8_t b) { return wasmv_u16x8_and(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t operator&(wasmv_u32x4_t a, wasmv_u32x4_t b) { return wasmv_u32x4_and(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t operator&(wasmv_u64x2_t a, wasmv_u64x2_t b) { return wasmv_u64x2_and(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t operator&(wasmv_b8x16_t a, wasmv_b8x16_t b) { return wasmv_b8x16_and(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t operator&(wasmv_b16x8_t a, wasmv_b16x8_t b) { return wasmv_b16x8_and(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t operator&(wasmv_b32x4_t a, wasmv_b32x4_t b) { return wasmv_b32x4_and(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t operator&(wasmv_b64x2_t a, wasmv_b64x2_t b) { return wasmv_b64x2_and(a, b); }

  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t operator&(wasmv_i8x16_t a, wasmv_b8x16_t b) { return wasmv_i8x16_and_b8x16(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t operator&(wasmv_i16x8_t a, wasmv_b16x8_t b) { return wasmv_i16x8_and_b16x8(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t operator&(wasmv_i32x4_t a, wasmv_b32x4_t b) { return wasmv_i32x4_and_b32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t operator&(wasmv_i64x2_t a, wasmv_b64x2_t b) { return wasmv_i64x2_and_b64x2(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t operator&(wasmv_u8x16_t a, wasmv_b8x16_t b) { return wasmv_u8x16_and_b8x16(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t operator&(wasmv_u16x8_t a, wasmv_b16x8_t b) { return wasmv_u16x8_and_b16x8(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t operator&(wasmv_u32x4_t a, wasmv_b32x4_t b) { return wasmv_u32x4_and_b32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t operator&(wasmv_u64x2_t a, wasmv_b64x2_t b) { return wasmv_u64x2_and_b64x2(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t operator&(wasmv_f32x4_t a, wasmv_b32x4_t b) { return wasmv_f32x4_and_b32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t operator&(wasmv_f64x2_t a, wasmv_b64x2_t b) { return wasmv_f64x2_and_b64x2(a, b); }

  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t operator&(wasmv_b8x16_t a, wasmv_i8x16_t b) { return wasmv_b8x16_and_i8x16(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t operator&(wasmv_b16x8_t a, wasmv_i16x8_t b) { return wasmv_b16x8_and_i16x8(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t operator&(wasmv_b32x4_t a, wasmv_i32x4_t b) { return wasmv_b32x4_and_i32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t operator&(wasmv_b64x2_t a, wasmv_i64x2_t b) { return wasmv_b64x2_and_i64x2(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t operator&(wasmv_b8x16_t a, wasmv_u8x16_t b) { return wasmv_b8x16_and_u8x16(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t operator&(wasmv_b16x8_t a, wasmv_u16x8_t b) { return wasmv_b16x8_and_u16x8(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t operator&(wasmv_b32x4_t a, wasmv_u32x4_t b) { return wasmv_b32x4_and_u32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t operator&(wasmv_b64x2_t a, wasmv_u64x2_t b) { return wasmv_b64x2_and_u64x2(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t operator&(wasmv_b32x4_t a, wasmv_f32x4_t b) { return wasmv_b32x4_and_f32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t operator&(wasmv_b64x2_t a, wasmv_f64x2_t b) { return wasmv_b64x2_and_f64x2(a, b); }
#endif

/* or -- Bitwise OR */

WASMV_FUNCTION_ATTRIBUTES
wasmv_i8x16_t
wasmv_i8x16_or(wasmv_i8x16_t a, wasmv_i8x16_t b) {
  return (wasmv_i8x16_t) { a.values | b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i16x8_or(wasmv_i16x8_t a, wasmv_i16x8_t b) {
  return (wasmv_i16x8_t) { a.values | b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i32x4_or(wasmv_i32x4_t a, wasmv_i32x4_t b) {
  return (wasmv_i32x4_t) { a.values | b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_i64x2_or(wasmv_i64x2_t a, wasmv_i64x2_t b) {
  return (wasmv_i64x2_t) { a.values | b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u8x16_t
wasmv_u8x16_or(wasmv_u8x16_t a, wasmv_u8x16_t b) {
  return (wasmv_u8x16_t) { a.values | b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_u16x8_or(wasmv_u16x8_t a, wasmv_u16x8_t b) {
  return (wasmv_u16x8_t) { a.values | b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_u32x4_or(wasmv_u32x4_t a, wasmv_u32x4_t b) {
  return (wasmv_u32x4_t) { a.values | b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u64x2_t
wasmv_u64x2_or(wasmv_u64x2_t a, wasmv_u64x2_t b) {
  return (wasmv_u64x2_t) { a.values | b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b8x16_t
wasmv_b8x16_or(wasmv_b8x16_t a, wasmv_b8x16_t b) {
  return (wasmv_b8x16_t) { a.values | b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b16x8_t
wasmv_b16x8_or(wasmv_b16x8_t a, wasmv_b16x8_t b) {
  return (wasmv_b16x8_t) { a.values | b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b32x4_t
wasmv_b32x4_or(wasmv_b32x4_t a, wasmv_b32x4_t b) {
  return (wasmv_b32x4_t) { a.values | b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b64x2_t
wasmv_b64x2_or(wasmv_b64x2_t a, wasmv_b64x2_t b) {
  return (wasmv_b64x2_t) { a.values | b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i8x16_t
wasmv_i8x16_or_b8x16(wasmv_i8x16_t a, wasmv_b8x16_t b) {
  return (wasmv_i8x16_t) { a.values | b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i16x8_or_b16x8(wasmv_i16x8_t a, wasmv_b16x8_t b) {
  return (wasmv_i16x8_t) { a.values | b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i32x4_or_b32x4(wasmv_i32x4_t a, wasmv_b32x4_t b) {
  return (wasmv_i32x4_t) { a.values | b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_i64x2_or_b64x2(wasmv_i64x2_t a, wasmv_b64x2_t b) {
  return (wasmv_i64x2_t) { a.values | b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u8x16_t
wasmv_u8x16_or_b8x16(wasmv_u8x16_t a, wasmv_b8x16_t b) {
  return (wasmv_u8x16_t) { a.values | b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_u16x8_or_b16x8(wasmv_u16x8_t a, wasmv_b16x8_t b) {
  return (wasmv_u16x8_t) { a.values | b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_u32x4_or_b32x4(wasmv_u32x4_t a, wasmv_b32x4_t b) {
  return (wasmv_u32x4_t) { a.values | b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u64x2_t
wasmv_u64x2_or_b64x2(wasmv_u64x2_t a, wasmv_b64x2_t b) {
  return (wasmv_u64x2_t) { a.values | b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f32x4_t
wasmv_f32x4_or_b32x4(wasmv_f32x4_t a, wasmv_b32x4_t b) {
  return wasmv_u32x4_as_f32x4(wasmv_u32x4_or_b32x4(wasmv_f32x4_as_u32x4(a), b));
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f64x2_t
wasmv_f64x2_or_b64x2(wasmv_f64x2_t a, wasmv_b64x2_t b) {
  return wasmv_u64x2_as_f64x2(wasmv_u64x2_or_b64x2(wasmv_f64x2_as_u64x2(a), b));
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i8x16_t
wasmv_b8x16_or_i8x16(wasmv_b8x16_t a, wasmv_i8x16_t b) {
  return wasmv_i8x16_or_b8x16(b, a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_b16x8_or_i16x8(wasmv_b16x8_t a, wasmv_i16x8_t b) {
  return wasmv_i16x8_or_b16x8(b, a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_b32x4_or_i32x4(wasmv_b32x4_t a, wasmv_i32x4_t b) {
  return wasmv_i32x4_or_b32x4(b, a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_b64x2_or_i64x2(wasmv_b64x2_t a, wasmv_i64x2_t b) {
  return wasmv_i64x2_or_b64x2(b, a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u8x16_t
wasmv_b8x16_or_u8x16(wasmv_b8x16_t a, wasmv_u8x16_t b) {
  return wasmv_u8x16_or_b8x16(b, a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_b16x8_or_u16x8(wasmv_b16x8_t a, wasmv_u16x8_t b) {
  return wasmv_u16x8_or_b16x8(b, a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_b32x4_or_u32x4(wasmv_b32x4_t a, wasmv_u32x4_t b) {
  return wasmv_u32x4_or_b32x4(b, a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u64x2_t
wasmv_b64x2_or_u64x2(wasmv_b64x2_t a, wasmv_u64x2_t b) {
  return wasmv_u64x2_or_b64x2(b, a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f32x4_t
wasmv_b32x4_or_f32x4(wasmv_b32x4_t a, wasmv_f32x4_t b) {
  return wasmv_f32x4_or_b32x4(b, a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f64x2_t
wasmv_b64x2_or_f64x2(wasmv_b64x2_t a, wasmv_f64x2_t b) {
  return wasmv_f64x2_or_b64x2(b, a);
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_or(a, b) \
    (_Generic((a), \
      wasmv_i8x16_t: (_Generic((b), \
        wasmv_b8x16_t: wasmv_i8x16_or_b8x16, \
        default /* wasmv_i8x16_t */: wasmv_i8x16_or)), \
      wasmv_i16x8_t: (_Generic((b), \
        wasmv_b16x8_t: wasmv_i16x8_or_b16x8, \
        default /* wasmv_i16x8_t */: wasmv_i16x8_or)), \
      wasmv_i32x4_t: (_Generic((b), \
        wasmv_b32x4_t: wasmv_i32x4_or_b32x4, \
        default /* wasmv_i32x4_t */: wasmv_i32x4_or)), \
      wasmv_i64x2_t: (_Generic((b), \
        wasmv_b64x2_t: wasmv_i64x2_or_b64x2, \
        default /* wasmv_i64x2_t */: wasmv_i64x2_or)), \
      wasmv_u8x16_t: (_Generic((b), \
        wasmv_b8x16_t: wasmv_u8x16_or_b8x16, \
        default /* wasmv_u8x16_t */: wasmv_u8x16_or)), \
      wasmv_u16x8_t: (_Generic((b), \
        wasmv_b16x8_t: wasmv_u16x8_or_b16x8, \
        default /* wasmv_u16x8_t */: wasmv_u16x8_or)), \
      wasmv_u32x4_t: (_Generic((b), \
        wasmv_b32x4_t: wasmv_u32x4_or_b32x4, \
        default /* wasmv_u32x4_t */: wasmv_u32x4_or)), \
      wasmv_u64x2_t: (_Generic((b), \
        wasmv_b64x2_t: wasmv_u64x2_or_b64x2, \
        default /* wasmv_u64x2_t */: wasmv_u64x2_or)), \
      wasmv_f32x4_t: wasmv_f32x4_or_b32x4, \
      wasmv_f64x2_t: wasmv_f64x2_or_b64x2, \
      wasmv_b8x16_t: (_Generic((b), \
        wasmv_i8x16_t: wasmv_b8x16_or_i8x16, \
        wasmv_u8x16_t: wasmv_b8x16_or_u8x16, \
        default /* wasmv_b8x16_t */: wasmv_b8x16_or)), \
      wasmv_b16x8_t: (_Generic((b), \
        wasmv_i16x8_t: wasmv_b16x8_or_i16x8, \
        wasmv_u16x8_t: wasmv_b16x8_or_u16x8, \
        default /* wasmv_b16x8_t */: wasmv_b16x8_or)), \
      wasmv_b32x4_t: (_Generic((b), \
        wasmv_i32x4_t: wasmv_b32x4_or_i32x4, \
        wasmv_u32x4_t: wasmv_b32x4_or_u32x4, \
        wasmv_f32x4_t: wasmv_b32x4_or_f32x4, \
        default /* wasmv_b32x4_t */: wasmv_b32x4_or)), \
      wasmv_b64x2_t: (_Generic((b), \
        wasmv_i64x2_t: wasmv_b64x2_or_i64x2, \
        wasmv_u64x2_t: wasmv_b64x2_or_u64x2, \
        wasmv_f64x2_t: wasmv_b64x2_or_f64x2, \
        default /* wasmv_b64x2_t */: wasmv_b64x2_or))) (a, b))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_or(wasmv_i8x16_t a, wasmv_i8x16_t b) { return wasmv_i8x16_or(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_or(wasmv_i16x8_t a, wasmv_i16x8_t b) { return wasmv_i16x8_or(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_or(wasmv_i32x4_t a, wasmv_i32x4_t b) { return wasmv_i32x4_or(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_or(wasmv_i64x2_t a, wasmv_i64x2_t b) { return wasmv_i64x2_or(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_or(wasmv_u8x16_t a, wasmv_u8x16_t b) { return wasmv_u8x16_or(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_or(wasmv_u16x8_t a, wasmv_u16x8_t b) { return wasmv_u16x8_or(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_or(wasmv_u32x4_t a, wasmv_u32x4_t b) { return wasmv_u32x4_or(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_or(wasmv_u64x2_t a, wasmv_u64x2_t b) { return wasmv_u64x2_or(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_or(wasmv_b8x16_t a, wasmv_b8x16_t b) { return wasmv_b8x16_or(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_or(wasmv_b16x8_t a, wasmv_b16x8_t b) { return wasmv_b16x8_or(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_or(wasmv_b32x4_t a, wasmv_b32x4_t b) { return wasmv_b32x4_or(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_or(wasmv_b64x2_t a, wasmv_b64x2_t b) { return wasmv_b64x2_or(a, b); }

  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_or(wasmv_i8x16_t a, wasmv_b8x16_t b) { return wasmv_i8x16_or_b8x16(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_or(wasmv_i16x8_t a, wasmv_b16x8_t b) { return wasmv_i16x8_or_b16x8(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_or(wasmv_i32x4_t a, wasmv_b32x4_t b) { return wasmv_i32x4_or_b32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_or(wasmv_i64x2_t a, wasmv_b64x2_t b) { return wasmv_i64x2_or_b64x2(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_or(wasmv_u8x16_t a, wasmv_b8x16_t b) { return wasmv_u8x16_or_b8x16(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_or(wasmv_u16x8_t a, wasmv_b16x8_t b) { return wasmv_u16x8_or_b16x8(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_or(wasmv_u32x4_t a, wasmv_b32x4_t b) { return wasmv_u32x4_or_b32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_or(wasmv_u64x2_t a, wasmv_b64x2_t b) { return wasmv_u64x2_or_b64x2(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_or(wasmv_f32x4_t a, wasmv_b32x4_t b) { return wasmv_f32x4_or_b32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_or(wasmv_f64x2_t a, wasmv_b64x2_t b) { return wasmv_f64x2_or_b64x2(a, b); }

  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_or(wasmv_b8x16_t a, wasmv_i8x16_t b) { return wasmv_b8x16_or_i8x16(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_or(wasmv_b16x8_t a, wasmv_i16x8_t b) { return wasmv_b16x8_or_i16x8(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_or(wasmv_b32x4_t a, wasmv_i32x4_t b) { return wasmv_b32x4_or_i32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_or(wasmv_b64x2_t a, wasmv_i64x2_t b) { return wasmv_b64x2_or_i64x2(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_or(wasmv_b8x16_t a, wasmv_u8x16_t b) { return wasmv_b8x16_or_u8x16(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_or(wasmv_b16x8_t a, wasmv_u16x8_t b) { return wasmv_b16x8_or_u16x8(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_or(wasmv_b32x4_t a, wasmv_u32x4_t b) { return wasmv_b32x4_or_u32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_or(wasmv_b64x2_t a, wasmv_u64x2_t b) { return wasmv_b64x2_or_u64x2(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_or(wasmv_b32x4_t a, wasmv_f32x4_t b) { return wasmv_b32x4_or_f32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_or(wasmv_b64x2_t a, wasmv_f64x2_t b) { return wasmv_b64x2_or_f64x2(a, b); }

  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t operator|(wasmv_i8x16_t a, wasmv_i8x16_t b) { return wasmv_i8x16_or(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t operator|(wasmv_i16x8_t a, wasmv_i16x8_t b) { return wasmv_i16x8_or(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t operator|(wasmv_i32x4_t a, wasmv_i32x4_t b) { return wasmv_i32x4_or(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t operator|(wasmv_i64x2_t a, wasmv_i64x2_t b) { return wasmv_i64x2_or(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t operator|(wasmv_u8x16_t a, wasmv_u8x16_t b) { return wasmv_u8x16_or(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t operator|(wasmv_u16x8_t a, wasmv_u16x8_t b) { return wasmv_u16x8_or(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t operator|(wasmv_u32x4_t a, wasmv_u32x4_t b) { return wasmv_u32x4_or(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t operator|(wasmv_u64x2_t a, wasmv_u64x2_t b) { return wasmv_u64x2_or(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t operator|(wasmv_b8x16_t a, wasmv_b8x16_t b) { return wasmv_b8x16_or(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t operator|(wasmv_b16x8_t a, wasmv_b16x8_t b) { return wasmv_b16x8_or(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t operator|(wasmv_b32x4_t a, wasmv_b32x4_t b) { return wasmv_b32x4_or(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t operator|(wasmv_b64x2_t a, wasmv_b64x2_t b) { return wasmv_b64x2_or(a, b); }

  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t operator|(wasmv_i8x16_t a, wasmv_b8x16_t b) { return wasmv_i8x16_or_b8x16(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t operator|(wasmv_i16x8_t a, wasmv_b16x8_t b) { return wasmv_i16x8_or_b16x8(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t operator|(wasmv_i32x4_t a, wasmv_b32x4_t b) { return wasmv_i32x4_or_b32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t operator|(wasmv_i64x2_t a, wasmv_b64x2_t b) { return wasmv_i64x2_or_b64x2(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t operator|(wasmv_u8x16_t a, wasmv_b8x16_t b) { return wasmv_u8x16_or_b8x16(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t operator|(wasmv_u16x8_t a, wasmv_b16x8_t b) { return wasmv_u16x8_or_b16x8(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t operator|(wasmv_u32x4_t a, wasmv_b32x4_t b) { return wasmv_u32x4_or_b32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t operator|(wasmv_u64x2_t a, wasmv_b64x2_t b) { return wasmv_u64x2_or_b64x2(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t operator|(wasmv_f32x4_t a, wasmv_b32x4_t b) { return wasmv_f32x4_or_b32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t operator|(wasmv_f64x2_t a, wasmv_b64x2_t b) { return wasmv_f64x2_or_b64x2(a, b); }

  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t operator|(wasmv_b8x16_t a, wasmv_i8x16_t b) { return wasmv_b8x16_or_i8x16(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t operator|(wasmv_b16x8_t a, wasmv_i16x8_t b) { return wasmv_b16x8_or_i16x8(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t operator|(wasmv_b32x4_t a, wasmv_i32x4_t b) { return wasmv_b32x4_or_i32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t operator|(wasmv_b64x2_t a, wasmv_i64x2_t b) { return wasmv_b64x2_or_i64x2(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t operator|(wasmv_b8x16_t a, wasmv_u8x16_t b) { return wasmv_b8x16_or_u8x16(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t operator|(wasmv_b16x8_t a, wasmv_u16x8_t b) { return wasmv_b16x8_or_u16x8(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t operator|(wasmv_b32x4_t a, wasmv_u32x4_t b) { return wasmv_b32x4_or_u32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t operator|(wasmv_b64x2_t a, wasmv_u64x2_t b) { return wasmv_b64x2_or_u64x2(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t operator|(wasmv_b32x4_t a, wasmv_f32x4_t b) { return wasmv_b32x4_or_f32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t operator|(wasmv_b64x2_t a, wasmv_f64x2_t b) { return wasmv_b64x2_or_f64x2(a, b); }
#endif

/* xor -- Bitwise XOR */

WASMV_FUNCTION_ATTRIBUTES
wasmv_i8x16_t
wasmv_i8x16_xor(wasmv_i8x16_t a, wasmv_i8x16_t b) {
  return (wasmv_i8x16_t) { a.values ^ b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i16x8_xor(wasmv_i16x8_t a, wasmv_i16x8_t b) {
  return (wasmv_i16x8_t) { a.values ^ b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i32x4_xor(wasmv_i32x4_t a, wasmv_i32x4_t b) {
  return (wasmv_i32x4_t) { a.values ^ b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_i64x2_xor(wasmv_i64x2_t a, wasmv_i64x2_t b) {
  return (wasmv_i64x2_t) { a.values ^ b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u8x16_t
wasmv_u8x16_xor(wasmv_u8x16_t a, wasmv_u8x16_t b) {
  return (wasmv_u8x16_t) { a.values ^ b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_u16x8_xor(wasmv_u16x8_t a, wasmv_u16x8_t b) {
  return (wasmv_u16x8_t) { a.values ^ b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_u32x4_xor(wasmv_u32x4_t a, wasmv_u32x4_t b) {
  return (wasmv_u32x4_t) { a.values ^ b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u64x2_t
wasmv_u64x2_xor(wasmv_u64x2_t a, wasmv_u64x2_t b) {
  return (wasmv_u64x2_t) { a.values ^ b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b8x16_t
wasmv_b8x16_xor(wasmv_b8x16_t a, wasmv_b8x16_t b) {
  return (wasmv_b8x16_t) { a.values ^ b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b16x8_t
wasmv_b16x8_xor(wasmv_b16x8_t a, wasmv_b16x8_t b) {
  return (wasmv_b16x8_t) { a.values ^ b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b32x4_t
wasmv_b32x4_xor(wasmv_b32x4_t a, wasmv_b32x4_t b) {
  return (wasmv_b32x4_t) { a.values ^ b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b64x2_t
wasmv_b64x2_xor(wasmv_b64x2_t a, wasmv_b64x2_t b) {
  return (wasmv_b64x2_t) { a.values ^ b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i8x16_t
wasmv_i8x16_xor_b8x16(wasmv_i8x16_t a, wasmv_b8x16_t b) {
  return (wasmv_i8x16_t) { a.values ^ b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i16x8_xor_b16x8(wasmv_i16x8_t a, wasmv_b16x8_t b) {
  return (wasmv_i16x8_t) { a.values ^ b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i32x4_xor_b32x4(wasmv_i32x4_t a, wasmv_b32x4_t b) {
  return (wasmv_i32x4_t) { a.values ^ b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_i64x2_xor_b64x2(wasmv_i64x2_t a, wasmv_b64x2_t b) {
  return (wasmv_i64x2_t) { a.values ^ b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u8x16_t
wasmv_u8x16_xor_b8x16(wasmv_u8x16_t a, wasmv_b8x16_t b) {
  return (wasmv_u8x16_t) { a.values ^ b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_u16x8_xor_b16x8(wasmv_u16x8_t a, wasmv_b16x8_t b) {
  return (wasmv_u16x8_t) { a.values ^ b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_u32x4_xor_b32x4(wasmv_u32x4_t a, wasmv_b32x4_t b) {
  return (wasmv_u32x4_t) { a.values ^ b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u64x2_t
wasmv_u64x2_xor_b64x2(wasmv_u64x2_t a, wasmv_b64x2_t b) {
  return (wasmv_u64x2_t) { a.values ^ b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f32x4_t
wasmv_f32x4_xor_b32x4(wasmv_f32x4_t a, wasmv_b32x4_t b) {
  return wasmv_u32x4_as_f32x4(wasmv_u32x4_xor_b32x4(wasmv_f32x4_as_u32x4(a), b));
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f64x2_t
wasmv_f64x2_xor_b64x2(wasmv_f64x2_t a, wasmv_b64x2_t b) {
  return wasmv_u64x2_as_f64x2(wasmv_u64x2_xor_b64x2(wasmv_f64x2_as_u64x2(a), b));
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i8x16_t
wasmv_b8x16_xor_i8x16(wasmv_b8x16_t a, wasmv_i8x16_t b) {
  return wasmv_i8x16_xor_b8x16(b, a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_b16x8_xor_i16x8(wasmv_b16x8_t a, wasmv_i16x8_t b) {
  return wasmv_i16x8_xor_b16x8(b, a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_b32x4_xor_i32x4(wasmv_b32x4_t a, wasmv_i32x4_t b) {
  return wasmv_i32x4_xor_b32x4(b, a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_b64x2_xor_i64x2(wasmv_b64x2_t a, wasmv_i64x2_t b) {
  return wasmv_i64x2_xor_b64x2(b, a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u8x16_t
wasmv_b8x16_xor_u8x16(wasmv_b8x16_t a, wasmv_u8x16_t b) {
  return wasmv_u8x16_xor_b8x16(b, a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_b16x8_xor_u16x8(wasmv_b16x8_t a, wasmv_u16x8_t b) {
  return wasmv_u16x8_xor_b16x8(b, a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_b32x4_xor_u32x4(wasmv_b32x4_t a, wasmv_u32x4_t b) {
  return wasmv_u32x4_xor_b32x4(b, a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u64x2_t
wasmv_b64x2_xor_u64x2(wasmv_b64x2_t a, wasmv_u64x2_t b) {
  return wasmv_u64x2_xor_b64x2(b, a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f32x4_t
wasmv_b32x4_xor_f32x4(wasmv_b32x4_t a, wasmv_f32x4_t b) {
  return wasmv_f32x4_xor_b32x4(b, a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f64x2_t
wasmv_b64x2_xor_f64x2(wasmv_b64x2_t a, wasmv_f64x2_t b) {
  return wasmv_f64x2_xor_b64x2(b, a);
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_xor(a, b) \
    (_Generic((a), \
      wasmv_i8x16_t: (_Generic((b), \
        wasmv_b8x16_t: wasmv_i8x16_xor_b8x16, \
        default /* wasmv_i8x16_t */: wasmv_i8x16_xor)), \
      wasmv_i16x8_t: (_Generic((b), \
        wasmv_b16x8_t: wasmv_i16x8_xor_b16x8, \
        default /* wasmv_i16x8_t */: wasmv_i16x8_xor)), \
      wasmv_i32x4_t: (_Generic((b), \
        wasmv_b32x4_t: wasmv_i32x4_xor_b32x4, \
        default /* wasmv_i32x4_t */: wasmv_i32x4_xor)), \
      wasmv_i64x2_t: (_Generic((b), \
        wasmv_b64x2_t: wasmv_i64x2_xor_b64x2, \
        default /* wasmv_i64x2_t */: wasmv_i64x2_xor)), \
      wasmv_u8x16_t: (_Generic((b), \
        wasmv_b8x16_t: wasmv_u8x16_xor_b8x16, \
        default /* wasmv_u8x16_t */: wasmv_u8x16_xor)), \
      wasmv_u16x8_t: (_Generic((b), \
        wasmv_b16x8_t: wasmv_u16x8_xor_b16x8, \
        default /* wasmv_u16x8_t */: wasmv_u16x8_xor)), \
      wasmv_u32x4_t: (_Generic((b), \
        wasmv_b32x4_t: wasmv_u32x4_xor_b32x4, \
        default /* wasmv_u32x4_t */: wasmv_u32x4_xor)), \
      wasmv_u64x2_t: (_Generic((b), \
        wasmv_b64x2_t: wasmv_u64x2_xor_b64x2, \
        default /* wasmv_u64x2_t */: wasmv_u64x2_xor)), \
      wasmv_f32x4_t: wasmv_f32x4_xor_b32x4, \
      wasmv_f64x2_t: wasmv_f64x2_xor_b64x2, \
      wasmv_b8x16_t: (_Generic((b), \
        wasmv_i8x16_t: wasmv_b8x16_xor_i8x16, \
        wasmv_u8x16_t: wasmv_b8x16_xor_u8x16, \
        default /* wasmv_b8x16_t */: wasmv_b8x16_xor)), \
      wasmv_b16x8_t: (_Generic((b), \
        wasmv_i16x8_t: wasmv_b16x8_xor_i16x8, \
        wasmv_u16x8_t: wasmv_b16x8_xor_u16x8, \
        default /* wasmv_b16x8_t */: wasmv_b16x8_xor)), \
      wasmv_b32x4_t: (_Generic((b), \
        wasmv_i32x4_t: wasmv_b32x4_xor_i32x4, \
        wasmv_u32x4_t: wasmv_b32x4_xor_u32x4, \
        wasmv_f32x4_t: wasmv_b32x4_xor_f32x4, \
        default /* wasmv_b32x4_t */: wasmv_b32x4_xor)), \
      wasmv_b64x2_t: (_Generic((b), \
        wasmv_i64x2_t: wasmv_b64x2_xor_i64x2, \
        wasmv_u64x2_t: wasmv_b64x2_xor_u64x2, \
        wasmv_f64x2_t: wasmv_b64x2_xor_f64x2, \
        default /* wasmv_b64x2_t */: wasmv_b64x2_xor))) (a, b))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_xor(wasmv_i8x16_t a, wasmv_i8x16_t b) { return wasmv_i8x16_xor(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_xor(wasmv_i16x8_t a, wasmv_i16x8_t b) { return wasmv_i16x8_xor(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_xor(wasmv_i32x4_t a, wasmv_i32x4_t b) { return wasmv_i32x4_xor(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_xor(wasmv_i64x2_t a, wasmv_i64x2_t b) { return wasmv_i64x2_xor(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_xor(wasmv_u8x16_t a, wasmv_u8x16_t b) { return wasmv_u8x16_xor(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_xor(wasmv_u16x8_t a, wasmv_u16x8_t b) { return wasmv_u16x8_xor(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_xor(wasmv_u32x4_t a, wasmv_u32x4_t b) { return wasmv_u32x4_xor(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_xor(wasmv_u64x2_t a, wasmv_u64x2_t b) { return wasmv_u64x2_xor(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_xor(wasmv_b8x16_t a, wasmv_b8x16_t b) { return wasmv_b8x16_xor(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_xor(wasmv_b16x8_t a, wasmv_b16x8_t b) { return wasmv_b16x8_xor(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_xor(wasmv_b32x4_t a, wasmv_b32x4_t b) { return wasmv_b32x4_xor(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_xor(wasmv_b64x2_t a, wasmv_b64x2_t b) { return wasmv_b64x2_xor(a, b); }

  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_xor(wasmv_i8x16_t a, wasmv_b8x16_t b) { return wasmv_i8x16_xor_b8x16(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_xor(wasmv_i16x8_t a, wasmv_b16x8_t b) { return wasmv_i16x8_xor_b16x8(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_xor(wasmv_i32x4_t a, wasmv_b32x4_t b) { return wasmv_i32x4_xor_b32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_xor(wasmv_i64x2_t a, wasmv_b64x2_t b) { return wasmv_i64x2_xor_b64x2(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_xor(wasmv_u8x16_t a, wasmv_b8x16_t b) { return wasmv_u8x16_xor_b8x16(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_xor(wasmv_u16x8_t a, wasmv_b16x8_t b) { return wasmv_u16x8_xor_b16x8(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_xor(wasmv_u32x4_t a, wasmv_b32x4_t b) { return wasmv_u32x4_xor_b32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_xor(wasmv_u64x2_t a, wasmv_b64x2_t b) { return wasmv_u64x2_xor_b64x2(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_xor(wasmv_f32x4_t a, wasmv_b32x4_t b) { return wasmv_f32x4_xor_b32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_xor(wasmv_f64x2_t a, wasmv_b64x2_t b) { return wasmv_f64x2_xor_b64x2(a, b); }

  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_xor(wasmv_b8x16_t a, wasmv_i8x16_t b) { return wasmv_b8x16_xor_i8x16(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_xor(wasmv_b16x8_t a, wasmv_i16x8_t b) { return wasmv_b16x8_xor_i16x8(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_xor(wasmv_b32x4_t a, wasmv_i32x4_t b) { return wasmv_b32x4_xor_i32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_xor(wasmv_b64x2_t a, wasmv_i64x2_t b) { return wasmv_b64x2_xor_i64x2(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_xor(wasmv_b8x16_t a, wasmv_u8x16_t b) { return wasmv_b8x16_xor_u8x16(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_xor(wasmv_b16x8_t a, wasmv_u16x8_t b) { return wasmv_b16x8_xor_u16x8(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_xor(wasmv_b32x4_t a, wasmv_u32x4_t b) { return wasmv_b32x4_xor_u32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_xor(wasmv_b64x2_t a, wasmv_u64x2_t b) { return wasmv_b64x2_xor_u64x2(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_xor(wasmv_b32x4_t a, wasmv_f32x4_t b) { return wasmv_b32x4_xor_f32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_xor(wasmv_b64x2_t a, wasmv_f64x2_t b) { return wasmv_b64x2_xor_f64x2(a, b); }

  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t operator^(wasmv_i8x16_t a, wasmv_i8x16_t b) { return wasmv_i8x16_xor(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t operator^(wasmv_i16x8_t a, wasmv_i16x8_t b) { return wasmv_i16x8_xor(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t operator^(wasmv_i32x4_t a, wasmv_i32x4_t b) { return wasmv_i32x4_xor(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t operator^(wasmv_i64x2_t a, wasmv_i64x2_t b) { return wasmv_i64x2_xor(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t operator^(wasmv_u8x16_t a, wasmv_u8x16_t b) { return wasmv_u8x16_xor(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t operator^(wasmv_u16x8_t a, wasmv_u16x8_t b) { return wasmv_u16x8_xor(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t operator^(wasmv_u32x4_t a, wasmv_u32x4_t b) { return wasmv_u32x4_xor(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t operator^(wasmv_u64x2_t a, wasmv_u64x2_t b) { return wasmv_u64x2_xor(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t operator^(wasmv_b8x16_t a, wasmv_b8x16_t b) { return wasmv_b8x16_xor(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t operator^(wasmv_b16x8_t a, wasmv_b16x8_t b) { return wasmv_b16x8_xor(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t operator^(wasmv_b32x4_t a, wasmv_b32x4_t b) { return wasmv_b32x4_xor(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t operator^(wasmv_b64x2_t a, wasmv_b64x2_t b) { return wasmv_b64x2_xor(a, b); }

  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t operator^(wasmv_i8x16_t a, wasmv_b8x16_t b) { return wasmv_i8x16_xor_b8x16(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t operator^(wasmv_i16x8_t a, wasmv_b16x8_t b) { return wasmv_i16x8_xor_b16x8(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t operator^(wasmv_i32x4_t a, wasmv_b32x4_t b) { return wasmv_i32x4_xor_b32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t operator^(wasmv_i64x2_t a, wasmv_b64x2_t b) { return wasmv_i64x2_xor_b64x2(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t operator^(wasmv_u8x16_t a, wasmv_b8x16_t b) { return wasmv_u8x16_xor_b8x16(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t operator^(wasmv_u16x8_t a, wasmv_b16x8_t b) { return wasmv_u16x8_xor_b16x8(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t operator^(wasmv_u32x4_t a, wasmv_b32x4_t b) { return wasmv_u32x4_xor_b32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t operator^(wasmv_u64x2_t a, wasmv_b64x2_t b) { return wasmv_u64x2_xor_b64x2(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t operator^(wasmv_f32x4_t a, wasmv_b32x4_t b) { return wasmv_f32x4_xor_b32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t operator^(wasmv_f64x2_t a, wasmv_b64x2_t b) { return wasmv_f64x2_xor_b64x2(a, b); }

  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t operator^(wasmv_b8x16_t a, wasmv_i8x16_t b) { return wasmv_b8x16_xor_i8x16(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t operator^(wasmv_b16x8_t a, wasmv_i16x8_t b) { return wasmv_b16x8_xor_i16x8(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t operator^(wasmv_b32x4_t a, wasmv_i32x4_t b) { return wasmv_b32x4_xor_i32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t operator^(wasmv_b64x2_t a, wasmv_i64x2_t b) { return wasmv_b64x2_xor_i64x2(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t operator^(wasmv_b8x16_t a, wasmv_u8x16_t b) { return wasmv_b8x16_xor_u8x16(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t operator^(wasmv_b16x8_t a, wasmv_u16x8_t b) { return wasmv_b16x8_xor_u16x8(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t operator^(wasmv_b32x4_t a, wasmv_u32x4_t b) { return wasmv_b32x4_xor_u32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t operator^(wasmv_b64x2_t a, wasmv_u64x2_t b) { return wasmv_b64x2_xor_u64x2(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t operator^(wasmv_b32x4_t a, wasmv_f32x4_t b) { return wasmv_b32x4_xor_f32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t operator^(wasmv_b64x2_t a, wasmv_f64x2_t b) { return wasmv_b64x2_xor_f64x2(a, b); }
#endif

/* not -- Bitwise NOT */

WASMV_FUNCTION_ATTRIBUTES
wasmv_i8x16_t
wasmv_i8x16_not(wasmv_i8x16_t a) {
  return (wasmv_i8x16_t) { ~a.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i16x8_not(wasmv_i16x8_t a) {
  return (wasmv_i16x8_t) { ~a.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i32x4_not(wasmv_i32x4_t a) {
  return (wasmv_i32x4_t) { ~a.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_i64x2_not(wasmv_i64x2_t a) {
  return (wasmv_i64x2_t) { ~a.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u8x16_t
wasmv_u8x16_not(wasmv_u8x16_t a) {
  return (wasmv_u8x16_t) { ~a.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_u16x8_not(wasmv_u16x8_t a) {
  return (wasmv_u16x8_t) { ~a.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_u32x4_not(wasmv_u32x4_t a) {
  return (wasmv_u32x4_t) { ~a.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u64x2_t
wasmv_u64x2_not(wasmv_u64x2_t a) {
  return (wasmv_u64x2_t) { ~a.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b8x16_t
wasmv_b8x16_not(wasmv_b8x16_t a) {
  return (wasmv_b8x16_t) { ~a.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b16x8_t
wasmv_b16x8_not(wasmv_b16x8_t a) {
  return (wasmv_b16x8_t) { ~a.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b32x4_t
wasmv_b32x4_not(wasmv_b32x4_t a) {
  return (wasmv_b32x4_t) { ~a.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b64x2_t
wasmv_b64x2_not(wasmv_b64x2_t a) {
  return (wasmv_b64x2_t) { ~a.values };
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_not(a) \
    (_Generic((a), \
      wasmv_i8x16_t: wasmv_i8x16_not, \
      wasmv_i16x8_t: wasmv_i16x8_not, \
      wasmv_i32x4_t: wasmv_i32x4_not, \
      wasmv_i64x2_t: wasmv_i64x2_not, \
      wasmv_u8x16_t: wasmv_u8x16_not, \
      wasmv_u16x8_t: wasmv_u16x8_not, \
      wasmv_u32x4_t: wasmv_u32x4_not, \
      wasmv_u64x2_t: wasmv_u64x2_not, \
      wasmv_b8x16_t: wasmv_b8x16_not, \
      wasmv_b16x8_t: wasmv_b16x8_not, \
      wasmv_b32x4_t: wasmv_b32x4_not, \
      wasmv_b64x2_t: wasmv_b64x2_not) (a))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_not(wasmv_i8x16_t a) { return wasmv_i8x16_not(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_not(wasmv_i16x8_t a) { return wasmv_i16x8_not(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_not(wasmv_i32x4_t a) { return wasmv_i32x4_not(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_not(wasmv_i64x2_t a) { return wasmv_i64x2_not(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_not(wasmv_u8x16_t a) { return wasmv_u8x16_not(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_not(wasmv_u16x8_t a) { return wasmv_u16x8_not(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_not(wasmv_u32x4_t a) { return wasmv_u32x4_not(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_not(wasmv_u64x2_t a) { return wasmv_u64x2_not(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_not(wasmv_b8x16_t a) { return wasmv_b8x16_not(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_not(wasmv_b16x8_t a) { return wasmv_b16x8_not(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_not(wasmv_b32x4_t a) { return wasmv_b32x4_not(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_not(wasmv_b64x2_t a) { return wasmv_b64x2_not(a); }

  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t operator~(wasmv_i8x16_t a) { return wasmv_i8x16_not(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t operator~(wasmv_i16x8_t a) { return wasmv_i16x8_not(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t operator~(wasmv_i32x4_t a) { return wasmv_i32x4_not(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t operator~(wasmv_i64x2_t a) { return wasmv_i64x2_not(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t operator~(wasmv_u8x16_t a) { return wasmv_u8x16_not(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t operator~(wasmv_u16x8_t a) { return wasmv_u16x8_not(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t operator~(wasmv_u32x4_t a) { return wasmv_u32x4_not(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t operator~(wasmv_u64x2_t a) { return wasmv_u64x2_not(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t operator~(wasmv_b8x16_t a) { return wasmv_b8x16_not(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t operator~(wasmv_b16x8_t a) { return wasmv_b16x8_not(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t operator~(wasmv_b32x4_t a) { return wasmv_b32x4_not(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t operator~(wasmv_b64x2_t a) { return wasmv_b64x2_not(a); }
#endif

/* andnot -- Bitwise AND-NOT */

WASMV_FUNCTION_ATTRIBUTES
wasmv_i8x16_t
wasmv_i8x16_andnot(wasmv_i8x16_t a, wasmv_i8x16_t b) {
  return (wasmv_i8x16_t) { a.values & ~b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i16x8_andnot(wasmv_i16x8_t a, wasmv_i16x8_t b) {
  return (wasmv_i16x8_t) { a.values & ~b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i32x4_andnot(wasmv_i32x4_t a, wasmv_i32x4_t b) {
  return (wasmv_i32x4_t) { a.values & ~b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_i64x2_andnot(wasmv_i64x2_t a, wasmv_i64x2_t b) {
  return (wasmv_i64x2_t) { a.values & ~b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u8x16_t
wasmv_u8x16_andnot(wasmv_u8x16_t a, wasmv_u8x16_t b) {
  return (wasmv_u8x16_t) { a.values & ~b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_u16x8_andnot(wasmv_u16x8_t a, wasmv_u16x8_t b) {
  return (wasmv_u16x8_t) { a.values & ~b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_u32x4_andnot(wasmv_u32x4_t a, wasmv_u32x4_t b) {
  return (wasmv_u32x4_t) { a.values & ~b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u64x2_t
wasmv_u64x2_andnot(wasmv_u64x2_t a, wasmv_u64x2_t b) {
  return (wasmv_u64x2_t) { a.values & ~b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b8x16_t
wasmv_b8x16_andnot(wasmv_b8x16_t a, wasmv_b8x16_t b) {
  return (wasmv_b8x16_t) { a.values & ~b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b16x8_t
wasmv_b16x8_andnot(wasmv_b16x8_t a, wasmv_b16x8_t b) {
  return (wasmv_b16x8_t) { a.values & ~b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b32x4_t
wasmv_b32x4_andnot(wasmv_b32x4_t a, wasmv_b32x4_t b) {
  return (wasmv_b32x4_t) { a.values & ~b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b64x2_t
wasmv_b64x2_andnot(wasmv_b64x2_t a, wasmv_b64x2_t b) {
  return (wasmv_b64x2_t) { a.values & ~b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i8x16_t
wasmv_b8x16_andnot_i8x16(wasmv_b8x16_t a, wasmv_i8x16_t b) {
  return (wasmv_i8x16_t) { a.values & ~b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_b16x8_andnot_i16x8(wasmv_b16x8_t a, wasmv_i16x8_t b) {
  return (wasmv_i16x8_t) { a.values & ~b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_b32x4_andnot_i32x4(wasmv_b32x4_t a, wasmv_i32x4_t b) {
  return (wasmv_i32x4_t) { a.values & ~b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_b64x2_andnot_i64x2(wasmv_b64x2_t a, wasmv_i64x2_t b) {
  return (wasmv_i64x2_t) { a.values & ~b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u8x16_t
wasmv_b8x16_andnot_u8x16(wasmv_b8x16_t a, wasmv_u8x16_t b) {
  return (wasmv_u8x16_t) { a.values & ~b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_b16x8_andnot_u16x8(wasmv_b16x8_t a, wasmv_u16x8_t b) {
  return (wasmv_u16x8_t) { a.values & ~b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_b32x4_andnot_u32x4(wasmv_b32x4_t a, wasmv_u32x4_t b) {
  return (wasmv_u32x4_t) { a.values & ~b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u64x2_t
wasmv_b64x2_andnot_u64x2(wasmv_b64x2_t a, wasmv_u64x2_t b) {
  return (wasmv_u64x2_t) { a.values & ~b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i8x16_t
wasmv_i8x16_andnot_b8x16(wasmv_i8x16_t a, wasmv_b8x16_t b) {
  return (wasmv_i8x16_t) { a.values & ~b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i16x8_andnot_b16x8(wasmv_i16x8_t a, wasmv_b16x8_t b) {
  return (wasmv_i16x8_t) { a.values & ~b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i32x4_andnot_b32x4(wasmv_i32x4_t a, wasmv_b32x4_t b) {
  return (wasmv_i32x4_t) { a.values & ~b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_i64x2_andnot_b64x2(wasmv_i64x2_t a, wasmv_b64x2_t b) {
  return (wasmv_i64x2_t) { a.values & ~b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u8x16_t
wasmv_u8x16_andnot_b8x16(wasmv_u8x16_t a, wasmv_b8x16_t b) {
  return (wasmv_u8x16_t) { a.values & ~b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_u16x8_andnot_b16x8(wasmv_u16x8_t a, wasmv_b16x8_t b) {
  return (wasmv_u16x8_t) { a.values & ~b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_u32x4_andnot_b32x4(wasmv_u32x4_t a, wasmv_b32x4_t b) {
  return (wasmv_u32x4_t) { a.values & ~b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u64x2_t
wasmv_u64x2_andnot_b64x2(wasmv_u64x2_t a, wasmv_b64x2_t b) {
  return (wasmv_u64x2_t) { a.values & ~b.values };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f32x4_t
wasmv_f32x4_andnot_b32x4(wasmv_f32x4_t a, wasmv_b32x4_t b) {
  return wasmv_u32x4_as_f32x4(wasmv_u32x4_andnot_b32x4(wasmv_f32x4_as_u32x4(a), b));
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f64x2_t
wasmv_f64x2_andnot_b64x2(wasmv_f64x2_t a, wasmv_b64x2_t b) {
  return wasmv_u64x2_as_f64x2(wasmv_u64x2_andnot_b64x2(wasmv_f64x2_as_u64x2(a), b));
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_andnot(a, b) \
    (_Generic((a), \
      wasmv_i8x16_t: (_Generic((b), \
        wasmv_b8x16_t: wasmv_i8x16_andnot_b8x16, \
        default /* wasmv_i8x16_t */: wasmv_i8x16_andnot)), \
      wasmv_i16x8_t: (_Generic((b), \
        wasmv_b16x8_t: wasmv_i16x8_andnot_b16x8, \
        default /* wasmv_i16x8_t */: wasmv_i16x8_andnot)), \
      wasmv_i32x4_t: (_Generic((b), \
        wasmv_b32x4_t: wasmv_i32x4_andnot_b32x4, \
        default /* wasmv_i32x4_t */: wasmv_i32x4_andnot)), \
      wasmv_i64x2_t: (_Generic((b), \
        wasmv_b64x2_t: wasmv_i64x2_andnot_b64x2, \
        default /* wasmv_i64x2_t */: wasmv_i64x2_andnot)), \
      wasmv_u8x16_t: (_Generic((b), \
        wasmv_b8x16_t: wasmv_u8x16_andnot_b8x16, \
        default /* wasmv_u8x16_t */: wasmv_u8x16_andnot)), \
      wasmv_u16x8_t: (_Generic((b), \
        wasmv_b16x8_t: wasmv_u16x8_andnot_b16x8, \
        default /* wasmv_u16x8_t */: wasmv_u16x8_andnot)), \
      wasmv_u32x4_t: (_Generic((b), \
        wasmv_b32x4_t: wasmv_u32x4_andnot_b32x4, \
        default /* wasmv_u32x4_t */: wasmv_u32x4_andnot)), \
      wasmv_u64x2_t: (_Generic((b), \
        wasmv_b64x2_t: wasmv_u64x2_andnot_b64x2, \
        default /* wasmv_u64x2_t */: wasmv_u64x2_andnot)), \
      wasmv_f32x4_t: wasmv_f32x4_andnot_b32x4, \
      wasmv_f64x2_t: wasmv_f64x2_andnot_b64x2, \
      wasmv_b8x16_t: (_Generic((b), \
        wasmv_i8x16_t: wasmv_b8x16_andnot_i8x16, \
        wasmv_u8x16_t: wasmv_b8x16_andnot_u8x16, \
        default /* wasmv_b8x16_t */: wasmv_b8x16_andnot)), \
      wasmv_b16x8_t: (_Generic((b), \
        wasmv_i16x8_t: wasmv_b16x8_andnot_i16x8, \
        wasmv_u16x8_t: wasmv_b16x8_andnot_u16x8, \
        default /* wasmv_b16x8_t */: wasmv_b16x8_andnot)), \
      wasmv_b32x4_t: (_Generic((b), \
        wasmv_i32x4_t: wasmv_b32x4_andnot_i32x4, \
        wasmv_u32x4_t: wasmv_b32x4_andnot_u32x4, \
        default /* wasmv_b32x4_t */: wasmv_b32x4_andnot)), \
      wasmv_b64x2_t: (_Generic((b), \
        wasmv_i64x2_t: wasmv_b64x2_andnot_i64x2, \
        wasmv_u64x2_t: wasmv_b64x2_andnot_u64x2, \
        default /* wasmv_b64x2_t */: wasmv_b64x2_andnot))) (a, b))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_andnot(wasmv_i8x16_t a, wasmv_i8x16_t b) { return wasmv_i8x16_andnot(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_andnot(wasmv_i16x8_t a, wasmv_i16x8_t b) { return wasmv_i16x8_andnot(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_andnot(wasmv_i32x4_t a, wasmv_i32x4_t b) { return wasmv_i32x4_andnot(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_andnot(wasmv_i64x2_t a, wasmv_i64x2_t b) { return wasmv_i64x2_andnot(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_andnot(wasmv_u8x16_t a, wasmv_u8x16_t b) { return wasmv_u8x16_andnot(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_andnot(wasmv_u16x8_t a, wasmv_u16x8_t b) { return wasmv_u16x8_andnot(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_andnot(wasmv_u32x4_t a, wasmv_u32x4_t b) { return wasmv_u32x4_andnot(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_andnot(wasmv_u64x2_t a, wasmv_u64x2_t b) { return wasmv_u64x2_andnot(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_andnot(wasmv_b8x16_t a, wasmv_b8x16_t b) { return wasmv_b8x16_andnot(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_andnot(wasmv_b16x8_t a, wasmv_b16x8_t b) { return wasmv_b16x8_andnot(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_andnot(wasmv_b32x4_t a, wasmv_b32x4_t b) { return wasmv_b32x4_andnot(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_andnot(wasmv_b64x2_t a, wasmv_b64x2_t b) { return wasmv_b64x2_andnot(a, b); }

  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_andnot(wasmv_i8x16_t a, wasmv_b8x16_t b) { return wasmv_i8x16_andnot_b8x16(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_andnot(wasmv_i16x8_t a, wasmv_b16x8_t b) { return wasmv_i16x8_andnot_b16x8(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_andnot(wasmv_i32x4_t a, wasmv_b32x4_t b) { return wasmv_i32x4_andnot_b32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_andnot(wasmv_i64x2_t a, wasmv_b64x2_t b) { return wasmv_i64x2_andnot_b64x2(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_andnot(wasmv_u8x16_t a, wasmv_b8x16_t b) { return wasmv_u8x16_andnot_b8x16(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_andnot(wasmv_u16x8_t a, wasmv_b16x8_t b) { return wasmv_u16x8_andnot_b16x8(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_andnot(wasmv_u32x4_t a, wasmv_b32x4_t b) { return wasmv_u32x4_andnot_b32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_andnot(wasmv_u64x2_t a, wasmv_b64x2_t b) { return wasmv_u64x2_andnot_b64x2(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_andnot(wasmv_f32x4_t a, wasmv_b32x4_t b) { return wasmv_f32x4_andnot_b32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_andnot(wasmv_f64x2_t a, wasmv_b64x2_t b) { return wasmv_f64x2_andnot_b64x2(a, b); }

  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_andnot(wasmv_b8x16_t a, wasmv_i8x16_t b) { return wasmv_b8x16_andnot_i8x16(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_andnot(wasmv_b16x8_t a, wasmv_i16x8_t b) { return wasmv_b16x8_andnot_i16x8(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_andnot(wasmv_b32x4_t a, wasmv_i32x4_t b) { return wasmv_b32x4_andnot_i32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_andnot(wasmv_b64x2_t a, wasmv_i64x2_t b) { return wasmv_b64x2_andnot_i64x2(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_andnot(wasmv_b8x16_t a, wasmv_u8x16_t b) { return wasmv_b8x16_andnot_u8x16(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_andnot(wasmv_b16x8_t a, wasmv_u16x8_t b) { return wasmv_b16x8_andnot_u16x8(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_andnot(wasmv_b32x4_t a, wasmv_u32x4_t b) { return wasmv_b32x4_andnot_u32x4(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_andnot(wasmv_b64x2_t a, wasmv_u64x2_t b) { return wasmv_b64x2_andnot_u64x2(a, b); }
#endif

/* bitselect -- Bitwise select
 *
 * Use the bits in the control mask c to select the corresponding bit
 * from v1 when 1 and v2 when 0.
 */

WASMV_FUNCTION_ATTRIBUTES
wasmv_i8x16_t
wasmv_i8x16_bitselect(wasmv_i8x16_t a, wasmv_i8x16_t b, wasmv_i8x16_t c) {
  return (wasmv_i8x16_t) { (a.values & c.values) ^ (b.values & ~c.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i16x8_bitselect(wasmv_i16x8_t a, wasmv_i16x8_t b, wasmv_i16x8_t c) {
  return (wasmv_i16x8_t) { (a.values & c.values) ^ (b.values & ~c.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i32x4_bitselect(wasmv_i32x4_t a, wasmv_i32x4_t b, wasmv_i32x4_t c) {
  return (wasmv_i32x4_t) { (a.values & c.values) ^ (b.values & ~c.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_i64x2_bitselect(wasmv_i64x2_t a, wasmv_i64x2_t b, wasmv_i64x2_t c) {
  return (wasmv_i64x2_t) { (a.values & c.values) ^ (b.values & ~c.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u8x16_t
wasmv_u8x16_bitselect(wasmv_u8x16_t a, wasmv_u8x16_t b, wasmv_u8x16_t c) {
  return (wasmv_u8x16_t) { (a.values & c.values) ^ (b.values & ~c.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_u16x8_bitselect(wasmv_u16x8_t a, wasmv_u16x8_t b, wasmv_u16x8_t c) {
  return (wasmv_u16x8_t) { (a.values & c.values) ^ (b.values & ~c.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_u32x4_bitselect(wasmv_u32x4_t a, wasmv_u32x4_t b, wasmv_u32x4_t c) {
  return (wasmv_u32x4_t) { (a.values & c.values) ^ (b.values & ~c.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u64x2_t
wasmv_u64x2_bitselect(wasmv_u64x2_t a, wasmv_u64x2_t b, wasmv_u64x2_t c) {
  return (wasmv_u64x2_t) { (a.values & c.values) ^ (b.values & ~c.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f32x4_t
wasmv_f32x4_bitselect(wasmv_f32x4_t a, wasmv_f32x4_t b, wasmv_u32x4_t c) {
  return
    wasmv_u32x4_as_f32x4(
      wasmv_u32x4_bitselect(
        wasmv_f32x4_as_u32x4(a),
        wasmv_f32x4_as_u32x4(b),
        c
      )
    );
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f64x2_t
wasmv_f64x2_bitselect(wasmv_f64x2_t a, wasmv_f64x2_t b, wasmv_u64x2_t c) {
  return
    wasmv_u64x2_as_f64x2(
      wasmv_u64x2_bitselect(
        wasmv_f64x2_as_u64x2(a),
        wasmv_f64x2_as_u64x2(b),
        c
      )
    );
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_bitselect(a, b, c) \
    (_Generic((a), \
      wasmv_i8x16_t: wasmv_i8x16_bitselect, \
      wasmv_i16x8_t: wasmv_i16x8_bitselect, \
      wasmv_i32x4_t: wasmv_i32x4_bitselect, \
      wasmv_i64x2_t: wasmv_i64x2_bitselect, \
      wasmv_u8x16_t: wasmv_u8x16_bitselect, \
      wasmv_u16x8_t: wasmv_u16x8_bitselect, \
      wasmv_u32x4_t: wasmv_u32x4_bitselect, \
      wasmv_u64x2_t: wasmv_u64x2_bitselect, \
      wasmv_f32x4_t: wasmv_f32x4_bitselect, \
      wasmv_f64x2_t: wasmv_f64x2_bitselect) (a, b, c))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_bitselect(wasmv_i8x16_t a, wasmv_i8x16_t b, wasmv_i8x16_t c) { return wasmv_i8x16_bitselect(a, b, c); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_bitselect(wasmv_i16x8_t a, wasmv_i16x8_t b, wasmv_i16x8_t c) { return wasmv_i16x8_bitselect(a, b, c); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_bitselect(wasmv_i32x4_t a, wasmv_i32x4_t b, wasmv_i32x4_t c) { return wasmv_i32x4_bitselect(a, b, c); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_bitselect(wasmv_i64x2_t a, wasmv_i64x2_t b, wasmv_i64x2_t c) { return wasmv_i64x2_bitselect(a, b, c); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_bitselect(wasmv_u8x16_t a, wasmv_u8x16_t b, wasmv_u8x16_t c) { return wasmv_u8x16_bitselect(a, b, c); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_bitselect(wasmv_u16x8_t a, wasmv_u16x8_t b, wasmv_u16x8_t c) { return wasmv_u16x8_bitselect(a, b, c); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_bitselect(wasmv_u32x4_t a, wasmv_u32x4_t b, wasmv_u32x4_t c) { return wasmv_u32x4_bitselect(a, b, c); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_bitselect(wasmv_u64x2_t a, wasmv_u64x2_t b, wasmv_u64x2_t c) { return wasmv_u64x2_bitselect(a, b, c); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_bitselect(wasmv_f32x4_t a, wasmv_f32x4_t b, wasmv_u32x4_t c) { return wasmv_f32x4_bitselect(a, b, c); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_bitselect(wasmv_f64x2_t a, wasmv_f64x2_t b, wasmv_u64x2_t c) { return wasmv_f64x2_bitselect(a, b, c); }
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

WASMV_FUNCTION_ATTRIBUTES
wasmv_i8x16_t
wasmv_i8x16_blend(wasmv_i8x16_t a, wasmv_i8x16_t b, wasmv_b8x16_t c) {
  return (wasmv_i8x16_t) { (a.values & c.values) ^ (b.values & ~c.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i16x8_blend(wasmv_i16x8_t a, wasmv_i16x8_t b, wasmv_b16x8_t c) {
  return (wasmv_i16x8_t) { (a.values & c.values) ^ (b.values & ~c.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i32x4_blend(wasmv_i32x4_t a, wasmv_i32x4_t b, wasmv_b32x4_t c) {
  return (wasmv_i32x4_t) { (a.values & c.values) ^ (b.values & ~c.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_i64x2_blend(wasmv_i64x2_t a, wasmv_i64x2_t b, wasmv_b64x2_t c) {
  return (wasmv_i64x2_t) { (a.values & c.values) ^ (b.values & ~c.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u8x16_t
wasmv_u8x16_blend(wasmv_u8x16_t a, wasmv_u8x16_t b, wasmv_b8x16_t c) {
  return (wasmv_u8x16_t) { (a.values & c.values) ^ (b.values & ~c.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_u16x8_blend(wasmv_u16x8_t a, wasmv_u16x8_t b, wasmv_b16x8_t c) {
  return (wasmv_u16x8_t) { (a.values & c.values) ^ (b.values & ~c.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_u32x4_blend(wasmv_u32x4_t a, wasmv_u32x4_t b, wasmv_b32x4_t c) {
  return (wasmv_u32x4_t) { (a.values & c.values) ^ (b.values & ~c.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u64x2_t
wasmv_u64x2_blend(wasmv_u64x2_t a, wasmv_u64x2_t b, wasmv_b64x2_t c) {
  return (wasmv_u64x2_t) { (a.values & c.values) ^ (b.values & ~c.values) };
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f32x4_t
wasmv_f32x4_blend(wasmv_f32x4_t a, wasmv_f32x4_t b, wasmv_b32x4_t c) {
  return
    wasmv_u32x4_as_f32x4(
      wasmv_u32x4_blend(
        wasmv_f32x4_as_u32x4(a),
        wasmv_f32x4_as_u32x4(b),
        c
      )
    );
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f64x2_t
wasmv_f64x2_blend(wasmv_f64x2_t a, wasmv_f64x2_t b, wasmv_b64x2_t c) {
  return
    wasmv_u64x2_as_f64x2(
      wasmv_u64x2_blend(
        wasmv_f64x2_as_u64x2(a),
        wasmv_f64x2_as_u64x2(b),
        c
      )
    );
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_blend(a, b, c) \
    (_Generic((a), \
      wasmv_i8x16_t: wasmv_i8x16_blend, \
      wasmv_i16x8_t: wasmv_i16x8_blend, \
      wasmv_i32x4_t: wasmv_i32x4_blend, \
      wasmv_i64x2_t: wasmv_i64x2_blend, \
      wasmv_u8x16_t: wasmv_u8x16_blend, \
      wasmv_u16x8_t: wasmv_u16x8_blend, \
      wasmv_u32x4_t: wasmv_u32x4_blend, \
      wasmv_u64x2_t: wasmv_u64x2_blend, \
      wasmv_f32x4_t: wasmv_f32x4_blend, \
      wasmv_f64x2_t: wasmv_f64x2_blend) (a, b, c))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_blend(wasmv_i8x16_t a, wasmv_i8x16_t b, wasmv_b8x16_t c) { return wasmv_i8x16_blend(a, b, c); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_blend(wasmv_i16x8_t a, wasmv_i16x8_t b, wasmv_b16x8_t c) { return wasmv_i16x8_blend(a, b, c); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_blend(wasmv_i32x4_t a, wasmv_i32x4_t b, wasmv_b32x4_t c) { return wasmv_i32x4_blend(a, b, c); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_blend(wasmv_i64x2_t a, wasmv_i64x2_t b, wasmv_b64x2_t c) { return wasmv_i64x2_blend(a, b, c); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_blend(wasmv_u8x16_t a, wasmv_u8x16_t b, wasmv_b8x16_t c) { return wasmv_u8x16_blend(a, b, c); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_blend(wasmv_u16x8_t a, wasmv_u16x8_t b, wasmv_b16x8_t c) { return wasmv_u16x8_blend(a, b, c); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_blend(wasmv_u32x4_t a, wasmv_u32x4_t b, wasmv_b32x4_t c) { return wasmv_u32x4_blend(a, b, c); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_blend(wasmv_u64x2_t a, wasmv_u64x2_t b, wasmv_b64x2_t c) { return wasmv_u64x2_blend(a, b, c); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_blend(wasmv_f32x4_t a, wasmv_f32x4_t b, wasmv_b32x4_t c) { return wasmv_f32x4_blend(a, b, c); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_blend(wasmv_f64x2_t a, wasmv_f64x2_t b, wasmv_b64x2_t c) { return wasmv_f64x2_blend(a, b, c); }
#endif

/* popcnt -- Lane-wise Population Count
 *
 * Count the number of bits set to one within each lane.
 */

WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WASMV_FUNCTION_ATTRIBUTES
wasmv_i8x16_t
wasmv_i8x16_popcount(wasmv_i8x16_t a) {
  wasmv_i8x16_t r;

  for (__SIZE_TYPE__ i = 0 ; i < (sizeof(r.values) / sizeof(r.values[i])) ; i++) {
    r.values[i] = (int8_t) __builtin_popcount((unsigned int) a.values[i]);
  }

  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wconditional-uninitialized" /* ??? */
  return r;
  #pragma clang diagnostic pop
}

WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WASMV_FUNCTION_ATTRIBUTES
wasmv_u8x16_t
wasmv_u8x16_popcount(wasmv_u8x16_t a) {
  wasmv_u8x16_t r;

  for (__SIZE_TYPE__ i = 0 ; i < (sizeof(r.values) / sizeof(r.values[i])) ; i++) {
    r.values[i] = (uint8_t) __builtin_popcount((unsigned int) a.values[i]);
  }

  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wconditional-uninitialized" /* ??? */
  return r;
  #pragma clang diagnostic pop
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_popcount(a) \
    (_Generic((a), \
      wasmv_i8x16_t: wasmv_i8x16_popcount, \
      wasmv_u8x16_t: wasmv_u8x16_popcount) (a))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_popcount(wasmv_i8x16_t a) { return wasmv_i8x16_popcount(a); }
  WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_popcount(wasmv_u8x16_t a) { return wasmv_u8x16_popcount(a); }
#endif

/* cmpeq
 *
 * Compares operands, returning all ones if the elements are equal, or
 * all zeros if inequal.
 */

WASMV_FUNCTION_ATTRIBUTES
wasmv_b8x16_t
wasmv_i8x16_cmpeq(wasmv_i8x16_t a, wasmv_i8x16_t b) {
  wasmv_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values == b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b16x8_t
wasmv_i16x8_cmpeq(wasmv_i16x8_t a, wasmv_i16x8_t b) {
  wasmv_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values == b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b32x4_t
wasmv_i32x4_cmpeq(wasmv_i32x4_t a, wasmv_i32x4_t b) {
  wasmv_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values == b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b64x2_t
wasmv_i64x2_cmpeq(wasmv_i64x2_t a, wasmv_i64x2_t b) {
  wasmv_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values == b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b8x16_t
wasmv_u8x16_cmpeq(wasmv_u8x16_t a, wasmv_u8x16_t b) {
  wasmv_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values == b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b16x8_t
wasmv_u16x8_cmpeq(wasmv_u16x8_t a, wasmv_u16x8_t b) {
  wasmv_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values == b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b32x4_t
wasmv_u32x4_cmpeq(wasmv_u32x4_t a, wasmv_u32x4_t b) {
  wasmv_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values == b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b64x2_t
wasmv_u64x2_cmpeq(wasmv_u64x2_t a, wasmv_u64x2_t b) {
  wasmv_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values == b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b8x16_t
wasmv_b8x16_cmpeq(wasmv_b8x16_t a, wasmv_b8x16_t b) {
  wasmv_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values == b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b16x8_t
wasmv_b16x8_cmpeq(wasmv_b16x8_t a, wasmv_b16x8_t b) {
  wasmv_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values == b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b32x4_t
wasmv_b32x4_cmpeq(wasmv_b32x4_t a, wasmv_b32x4_t b) {
  wasmv_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values == b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b64x2_t
wasmv_b64x2_cmpeq(wasmv_b64x2_t a, wasmv_b64x2_t b) {
  wasmv_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values == b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b32x4_t
wasmv_f32x4_cmpeq(wasmv_f32x4_t a, wasmv_f32x4_t b) {
  wasmv_b32x4_t r;
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wfloat-equal"
  r.values = (__typeof__(r.values)) (a.values == b.values);
  #pragma clang diagnostic pop
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b64x2_t
wasmv_f64x2_cmpeq(wasmv_f64x2_t a, wasmv_f64x2_t b) {
  wasmv_b64x2_t r;
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wfloat-equal"
  r.values = (__typeof__(r.values)) (a.values == b.values);
  #pragma clang diagnostic pop
  return r;
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_cmpeq(a, b) \
    (_Generic((a), \
      wasmv_i8x16_t: wasmv_i8x16_cmpeq, \
      wasmv_i16x8_t: wasmv_i16x8_cmpeq, \
      wasmv_i32x4_t: wasmv_i32x4_cmpeq, \
      wasmv_i64x2_t: wasmv_i64x2_cmpeq, \
      wasmv_u8x16_t: wasmv_u8x16_cmpeq, \
      wasmv_u16x8_t: wasmv_u16x8_cmpeq, \
      wasmv_u32x4_t: wasmv_u32x4_cmpeq, \
      wasmv_u64x2_t: wasmv_u64x2_cmpeq, \
      wasmv_b8x16_t: wasmv_b8x16_cmpeq, \
      wasmv_b16x8_t: wasmv_b16x8_cmpeq, \
      wasmv_b32x4_t: wasmv_b32x4_cmpeq, \
      wasmv_b64x2_t: wasmv_b64x2_cmpeq, \
      wasmv_f32x4_t: wasmv_f32x4_cmpeq, \
      wasmv_f64x2_t: wasmv_f64x2_cmpeq) (a, b))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_cmpeq(wasmv_i8x16_t a, wasmv_i8x16_t b) { return wasmv_i8x16_cmpeq(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_cmpeq(wasmv_i16x8_t a, wasmv_i16x8_t b) { return wasmv_i16x8_cmpeq(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_cmpeq(wasmv_i32x4_t a, wasmv_i32x4_t b) { return wasmv_i32x4_cmpeq(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_cmpeq(wasmv_i64x2_t a, wasmv_i64x2_t b) { return wasmv_i64x2_cmpeq(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_cmpeq(wasmv_u8x16_t a, wasmv_u8x16_t b) { return wasmv_u8x16_cmpeq(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_cmpeq(wasmv_u16x8_t a, wasmv_u16x8_t b) { return wasmv_u16x8_cmpeq(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_cmpeq(wasmv_u32x4_t a, wasmv_u32x4_t b) { return wasmv_u32x4_cmpeq(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_cmpeq(wasmv_u64x2_t a, wasmv_u64x2_t b) { return wasmv_u64x2_cmpeq(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_cmpeq(wasmv_b8x16_t a, wasmv_b8x16_t b) { return wasmv_b8x16_cmpeq(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_cmpeq(wasmv_b16x8_t a, wasmv_b16x8_t b) { return wasmv_b16x8_cmpeq(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_cmpeq(wasmv_b32x4_t a, wasmv_b32x4_t b) { return wasmv_b32x4_cmpeq(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_cmpeq(wasmv_b64x2_t a, wasmv_b64x2_t b) { return wasmv_b64x2_cmpeq(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_cmpeq(wasmv_f32x4_t a, wasmv_f32x4_t b) { return wasmv_f32x4_cmpeq(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_cmpeq(wasmv_f64x2_t a, wasmv_f64x2_t b) { return wasmv_f64x2_cmpeq(a, b); }

  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t operator==(wasmv_i8x16_t a, wasmv_i8x16_t b) { return wasmv_i8x16_cmpeq(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t operator==(wasmv_i16x8_t a, wasmv_i16x8_t b) { return wasmv_i16x8_cmpeq(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t operator==(wasmv_i32x4_t a, wasmv_i32x4_t b) { return wasmv_i32x4_cmpeq(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t operator==(wasmv_i64x2_t a, wasmv_i64x2_t b) { return wasmv_i64x2_cmpeq(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t operator==(wasmv_u8x16_t a, wasmv_u8x16_t b) { return wasmv_u8x16_cmpeq(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t operator==(wasmv_u16x8_t a, wasmv_u16x8_t b) { return wasmv_u16x8_cmpeq(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t operator==(wasmv_u32x4_t a, wasmv_u32x4_t b) { return wasmv_u32x4_cmpeq(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t operator==(wasmv_u64x2_t a, wasmv_u64x2_t b) { return wasmv_u64x2_cmpeq(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t operator==(wasmv_b8x16_t a, wasmv_b8x16_t b) { return wasmv_b8x16_cmpeq(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t operator==(wasmv_b16x8_t a, wasmv_b16x8_t b) { return wasmv_b16x8_cmpeq(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t operator==(wasmv_b32x4_t a, wasmv_b32x4_t b) { return wasmv_b32x4_cmpeq(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t operator==(wasmv_b64x2_t a, wasmv_b64x2_t b) { return wasmv_b64x2_cmpeq(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t operator==(wasmv_f32x4_t a, wasmv_f32x4_t b) { return wasmv_f32x4_cmpeq(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t operator==(wasmv_f64x2_t a, wasmv_f64x2_t b) { return wasmv_f64x2_cmpeq(a, b); }
#endif

/* cmpne
 *
 * The ne operations produce the inverse of their eq counterparts.
 */

WASMV_FUNCTION_ATTRIBUTES
wasmv_b8x16_t
wasmv_i8x16_cmpne(wasmv_i8x16_t a, wasmv_i8x16_t b) {
  wasmv_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values != b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b16x8_t
wasmv_i16x8_cmpne(wasmv_i16x8_t a, wasmv_i16x8_t b) {
  wasmv_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values != b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b32x4_t
wasmv_i32x4_cmpne(wasmv_i32x4_t a, wasmv_i32x4_t b) {
  wasmv_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values != b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b64x2_t
wasmv_i64x2_cmpne(wasmv_i64x2_t a, wasmv_i64x2_t b) {
  wasmv_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values != b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b8x16_t
wasmv_u8x16_cmpne(wasmv_u8x16_t a, wasmv_u8x16_t b) {
  wasmv_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values != b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b16x8_t
wasmv_u16x8_cmpne(wasmv_u16x8_t a, wasmv_u16x8_t b) {
  wasmv_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values != b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b32x4_t
wasmv_u32x4_cmpne(wasmv_u32x4_t a, wasmv_u32x4_t b) {
  wasmv_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values != b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b64x2_t
wasmv_u64x2_cmpne(wasmv_u64x2_t a, wasmv_u64x2_t b) {
  wasmv_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values != b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b8x16_t
wasmv_b8x16_cmpne(wasmv_b8x16_t a, wasmv_b8x16_t b) {
  wasmv_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values != b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b16x8_t
wasmv_b16x8_cmpne(wasmv_b16x8_t a, wasmv_b16x8_t b) {
  wasmv_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values != b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b32x4_t
wasmv_b32x4_cmpne(wasmv_b32x4_t a, wasmv_b32x4_t b) {
  wasmv_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values != b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b64x2_t
wasmv_b64x2_cmpne(wasmv_b64x2_t a, wasmv_b64x2_t b) {
  wasmv_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values != b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b32x4_t
wasmv_f32x4_cmpne(wasmv_f32x4_t a, wasmv_f32x4_t b) {
  wasmv_b32x4_t r;
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wfloat-equal"
  r.values = (__typeof__(r.values)) (a.values != b.values);
  #pragma clang diagnostic pop
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b64x2_t
wasmv_f64x2_cmpne(wasmv_f64x2_t a, wasmv_f64x2_t b) {
  wasmv_b64x2_t r;
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wfloat-equal"
  r.values = (__typeof__(r.values)) (a.values != b.values);
  #pragma clang diagnostic pop
  return r;
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_cmpne(a, b) \
    (_Generic((a), \
      wasmv_i8x16_t: wasmv_i8x16_cmpne, \
      wasmv_i16x8_t: wasmv_i16x8_cmpne, \
      wasmv_i32x4_t: wasmv_i32x4_cmpne, \
      wasmv_i64x2_t: wasmv_i64x2_cmpne, \
      wasmv_u8x16_t: wasmv_u8x16_cmpne, \
      wasmv_u16x8_t: wasmv_u16x8_cmpne, \
      wasmv_u32x4_t: wasmv_u32x4_cmpne, \
      wasmv_u64x2_t: wasmv_u64x2_cmpne, \
      wasmv_b8x16_t: wasmv_b8x16_cmpne, \
      wasmv_b16x8_t: wasmv_b16x8_cmpne, \
      wasmv_b32x4_t: wasmv_b32x4_cmpne, \
      wasmv_b64x2_t: wasmv_b64x2_cmpne, \
      wasmv_f32x4_t: wasmv_f32x4_cmpne, \
      wasmv_f64x2_t: wasmv_f64x2_cmpne) (a, b))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_cmpne(wasmv_i8x16_t a, wasmv_i8x16_t b) { return wasmv_i8x16_cmpne(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_cmpne(wasmv_i16x8_t a, wasmv_i16x8_t b) { return wasmv_i16x8_cmpne(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_cmpne(wasmv_i32x4_t a, wasmv_i32x4_t b) { return wasmv_i32x4_cmpne(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_cmpne(wasmv_i64x2_t a, wasmv_i64x2_t b) { return wasmv_i64x2_cmpne(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_cmpne(wasmv_u8x16_t a, wasmv_u8x16_t b) { return wasmv_u8x16_cmpne(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_cmpne(wasmv_u16x8_t a, wasmv_u16x8_t b) { return wasmv_u16x8_cmpne(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_cmpne(wasmv_u32x4_t a, wasmv_u32x4_t b) { return wasmv_u32x4_cmpne(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_cmpne(wasmv_u64x2_t a, wasmv_u64x2_t b) { return wasmv_u64x2_cmpne(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_cmpne(wasmv_b8x16_t a, wasmv_b8x16_t b) { return wasmv_b8x16_cmpne(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_cmpne(wasmv_b16x8_t a, wasmv_b16x8_t b) { return wasmv_b16x8_cmpne(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_cmpne(wasmv_b32x4_t a, wasmv_b32x4_t b) { return wasmv_b32x4_cmpne(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_cmpne(wasmv_b64x2_t a, wasmv_b64x2_t b) { return wasmv_b64x2_cmpne(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_cmpne(wasmv_f32x4_t a, wasmv_f32x4_t b) { return wasmv_f32x4_cmpne(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_cmpne(wasmv_f64x2_t a, wasmv_f64x2_t b) { return wasmv_f64x2_cmpne(a, b); }

  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t operator!=(wasmv_i8x16_t a, wasmv_i8x16_t b) { return wasmv_i8x16_cmpne(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t operator!=(wasmv_i16x8_t a, wasmv_i16x8_t b) { return wasmv_i16x8_cmpne(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t operator!=(wasmv_i32x4_t a, wasmv_i32x4_t b) { return wasmv_i32x4_cmpne(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t operator!=(wasmv_i64x2_t a, wasmv_i64x2_t b) { return wasmv_i64x2_cmpne(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t operator!=(wasmv_u8x16_t a, wasmv_u8x16_t b) { return wasmv_u8x16_cmpne(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t operator!=(wasmv_u16x8_t a, wasmv_u16x8_t b) { return wasmv_u16x8_cmpne(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t operator!=(wasmv_u32x4_t a, wasmv_u32x4_t b) { return wasmv_u32x4_cmpne(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t operator!=(wasmv_u64x2_t a, wasmv_u64x2_t b) { return wasmv_u64x2_cmpne(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t operator!=(wasmv_b8x16_t a, wasmv_b8x16_t b) { return wasmv_b8x16_cmpne(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t operator!=(wasmv_b16x8_t a, wasmv_b16x8_t b) { return wasmv_b16x8_cmpne(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t operator!=(wasmv_b32x4_t a, wasmv_b32x4_t b) { return wasmv_b32x4_cmpne(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t operator!=(wasmv_b64x2_t a, wasmv_b64x2_t b) { return wasmv_b64x2_cmpne(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t operator!=(wasmv_f32x4_t a, wasmv_f32x4_t b) { return wasmv_f32x4_cmpne(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t operator!=(wasmv_f64x2_t a, wasmv_f64x2_t b) { return wasmv_f64x2_cmpne(a, b); }
#endif

/* cmplt */

WASMV_FUNCTION_ATTRIBUTES
wasmv_b8x16_t
wasmv_i8x16_cmplt(wasmv_i8x16_t a, wasmv_i8x16_t b) {
  wasmv_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values < b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b16x8_t
wasmv_i16x8_cmplt(wasmv_i16x8_t a, wasmv_i16x8_t b) {
  wasmv_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values < b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b32x4_t
wasmv_i32x4_cmplt(wasmv_i32x4_t a, wasmv_i32x4_t b) {
  wasmv_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values < b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b64x2_t
wasmv_i64x2_cmplt(wasmv_i64x2_t a, wasmv_i64x2_t b) {
  wasmv_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values < b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b8x16_t
wasmv_u8x16_cmplt(wasmv_u8x16_t a, wasmv_u8x16_t b) {
  wasmv_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values < b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b16x8_t
wasmv_u16x8_cmplt(wasmv_u16x8_t a, wasmv_u16x8_t b) {
  wasmv_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values < b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b32x4_t
wasmv_u32x4_cmplt(wasmv_u32x4_t a, wasmv_u32x4_t b) {
  wasmv_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values < b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b64x2_t
wasmv_u64x2_cmplt(wasmv_u64x2_t a, wasmv_u64x2_t b) {
  wasmv_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values < b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b8x16_t
wasmv_b8x16_cmplt(wasmv_b8x16_t a, wasmv_b8x16_t b) {
  wasmv_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values < b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b16x8_t
wasmv_b16x8_cmplt(wasmv_b16x8_t a, wasmv_b16x8_t b) {
  wasmv_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values < b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b32x4_t
wasmv_b32x4_cmplt(wasmv_b32x4_t a, wasmv_b32x4_t b) {
  wasmv_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values < b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b64x2_t
wasmv_b64x2_cmplt(wasmv_b64x2_t a, wasmv_b64x2_t b) {
  wasmv_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values < b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b32x4_t
wasmv_f32x4_cmplt(wasmv_f32x4_t a, wasmv_f32x4_t b) {
  wasmv_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values < b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b64x2_t
wasmv_f64x2_cmplt(wasmv_f64x2_t a, wasmv_f64x2_t b) {
  wasmv_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values < b.values);
  return r;
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_cmplt(a, b) \
    (_Generic((a), \
      wasmv_i8x16_t: wasmv_i8x16_cmplt, \
      wasmv_i16x8_t: wasmv_i16x8_cmplt, \
      wasmv_i32x4_t: wasmv_i32x4_cmplt, \
      wasmv_i64x2_t: wasmv_i64x2_cmplt, \
      wasmv_u8x16_t: wasmv_u8x16_cmplt, \
      wasmv_u16x8_t: wasmv_u16x8_cmplt, \
      wasmv_u32x4_t: wasmv_u32x4_cmplt, \
      wasmv_u64x2_t: wasmv_u64x2_cmplt, \
      wasmv_b8x16_t: wasmv_b8x16_cmplt, \
      wasmv_b16x8_t: wasmv_b16x8_cmplt, \
      wasmv_b32x4_t: wasmv_b32x4_cmplt, \
      wasmv_b64x2_t: wasmv_b64x2_cmplt, \
      wasmv_f32x4_t: wasmv_f32x4_cmplt, \
      wasmv_f64x2_t: wasmv_f64x2_cmplt) (a, b))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_cmplt(wasmv_i8x16_t a, wasmv_i8x16_t b) { return wasmv_i8x16_cmplt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_cmplt(wasmv_i16x8_t a, wasmv_i16x8_t b) { return wasmv_i16x8_cmplt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_cmplt(wasmv_i32x4_t a, wasmv_i32x4_t b) { return wasmv_i32x4_cmplt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_cmplt(wasmv_i64x2_t a, wasmv_i64x2_t b) { return wasmv_i64x2_cmplt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_cmplt(wasmv_u8x16_t a, wasmv_u8x16_t b) { return wasmv_u8x16_cmplt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_cmplt(wasmv_u16x8_t a, wasmv_u16x8_t b) { return wasmv_u16x8_cmplt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_cmplt(wasmv_u32x4_t a, wasmv_u32x4_t b) { return wasmv_u32x4_cmplt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_cmplt(wasmv_u64x2_t a, wasmv_u64x2_t b) { return wasmv_u64x2_cmplt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_cmplt(wasmv_b8x16_t a, wasmv_b8x16_t b) { return wasmv_b8x16_cmplt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_cmplt(wasmv_b16x8_t a, wasmv_b16x8_t b) { return wasmv_b16x8_cmplt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_cmplt(wasmv_b32x4_t a, wasmv_b32x4_t b) { return wasmv_b32x4_cmplt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_cmplt(wasmv_b64x2_t a, wasmv_b64x2_t b) { return wasmv_b64x2_cmplt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_cmplt(wasmv_f32x4_t a, wasmv_f32x4_t b) { return wasmv_f32x4_cmplt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_cmplt(wasmv_f64x2_t a, wasmv_f64x2_t b) { return wasmv_f64x2_cmplt(a, b); }

  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t operator<(wasmv_i8x16_t a, wasmv_i8x16_t b) { return wasmv_i8x16_cmplt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t operator<(wasmv_i16x8_t a, wasmv_i16x8_t b) { return wasmv_i16x8_cmplt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t operator<(wasmv_i32x4_t a, wasmv_i32x4_t b) { return wasmv_i32x4_cmplt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t operator<(wasmv_i64x2_t a, wasmv_i64x2_t b) { return wasmv_i64x2_cmplt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t operator<(wasmv_u8x16_t a, wasmv_u8x16_t b) { return wasmv_u8x16_cmplt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t operator<(wasmv_u16x8_t a, wasmv_u16x8_t b) { return wasmv_u16x8_cmplt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t operator<(wasmv_u32x4_t a, wasmv_u32x4_t b) { return wasmv_u32x4_cmplt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t operator<(wasmv_u64x2_t a, wasmv_u64x2_t b) { return wasmv_u64x2_cmplt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t operator<(wasmv_b8x16_t a, wasmv_b8x16_t b) { return wasmv_b8x16_cmplt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t operator<(wasmv_b16x8_t a, wasmv_b16x8_t b) { return wasmv_b16x8_cmplt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t operator<(wasmv_b32x4_t a, wasmv_b32x4_t b) { return wasmv_b32x4_cmplt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t operator<(wasmv_b64x2_t a, wasmv_b64x2_t b) { return wasmv_b64x2_cmplt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t operator<(wasmv_f32x4_t a, wasmv_f32x4_t b) { return wasmv_f32x4_cmplt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t operator<(wasmv_f64x2_t a, wasmv_f64x2_t b) { return wasmv_f64x2_cmplt(a, b); }
#endif

/* cmple */

WASMV_FUNCTION_ATTRIBUTES
wasmv_b8x16_t
wasmv_i8x16_cmple(wasmv_i8x16_t a, wasmv_i8x16_t b) {
  wasmv_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values <= b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b16x8_t
wasmv_i16x8_cmple(wasmv_i16x8_t a, wasmv_i16x8_t b) {
  wasmv_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values <= b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b32x4_t
wasmv_i32x4_cmple(wasmv_i32x4_t a, wasmv_i32x4_t b) {
  wasmv_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values <= b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b64x2_t
wasmv_i64x2_cmple(wasmv_i64x2_t a, wasmv_i64x2_t b) {
  wasmv_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values <= b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b8x16_t
wasmv_u8x16_cmple(wasmv_u8x16_t a, wasmv_u8x16_t b) {
  wasmv_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values <= b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b16x8_t
wasmv_u16x8_cmple(wasmv_u16x8_t a, wasmv_u16x8_t b) {
  wasmv_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values <= b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b32x4_t
wasmv_u32x4_cmple(wasmv_u32x4_t a, wasmv_u32x4_t b) {
  wasmv_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values <= b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b64x2_t
wasmv_u64x2_cmple(wasmv_u64x2_t a, wasmv_u64x2_t b) {
  wasmv_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values <= b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b8x16_t
wasmv_b8x16_cmple(wasmv_b8x16_t a, wasmv_b8x16_t b) {
  wasmv_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values <= b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b16x8_t
wasmv_b16x8_cmple(wasmv_b16x8_t a, wasmv_b16x8_t b) {
  wasmv_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values <= b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b32x4_t
wasmv_b32x4_cmple(wasmv_b32x4_t a, wasmv_b32x4_t b) {
  wasmv_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values <= b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b64x2_t
wasmv_b64x2_cmple(wasmv_b64x2_t a, wasmv_b64x2_t b) {
  wasmv_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values <= b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b32x4_t
wasmv_f32x4_cmple(wasmv_f32x4_t a, wasmv_f32x4_t b) {
  wasmv_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values <= b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b64x2_t
wasmv_f64x2_cmple(wasmv_f64x2_t a, wasmv_f64x2_t b) {
  wasmv_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values <= b.values);
  return r;
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_cmple(a, b) \
    (_Generic((a), \
      wasmv_i8x16_t: wasmv_i8x16_cmple, \
      wasmv_i16x8_t: wasmv_i16x8_cmple, \
      wasmv_i32x4_t: wasmv_i32x4_cmple, \
      wasmv_i64x2_t: wasmv_i64x2_cmple, \
      wasmv_u8x16_t: wasmv_u8x16_cmple, \
      wasmv_u16x8_t: wasmv_u16x8_cmple, \
      wasmv_u32x4_t: wasmv_u32x4_cmple, \
      wasmv_u64x2_t: wasmv_u64x2_cmple, \
      wasmv_b8x16_t: wasmv_b8x16_cmple, \
      wasmv_b16x8_t: wasmv_b16x8_cmple, \
      wasmv_b32x4_t: wasmv_b32x4_cmple, \
      wasmv_b64x2_t: wasmv_b64x2_cmple, \
      wasmv_f32x4_t: wasmv_f32x4_cmple, \
      wasmv_f64x2_t: wasmv_f64x2_cmple) (a, b))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_cmple(wasmv_i8x16_t a, wasmv_i8x16_t b) { return wasmv_i8x16_cmple(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_cmple(wasmv_i16x8_t a, wasmv_i16x8_t b) { return wasmv_i16x8_cmple(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_cmple(wasmv_i32x4_t a, wasmv_i32x4_t b) { return wasmv_i32x4_cmple(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_cmple(wasmv_i64x2_t a, wasmv_i64x2_t b) { return wasmv_i64x2_cmple(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_cmple(wasmv_u8x16_t a, wasmv_u8x16_t b) { return wasmv_u8x16_cmple(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_cmple(wasmv_u16x8_t a, wasmv_u16x8_t b) { return wasmv_u16x8_cmple(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_cmple(wasmv_u32x4_t a, wasmv_u32x4_t b) { return wasmv_u32x4_cmple(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_cmple(wasmv_u64x2_t a, wasmv_u64x2_t b) { return wasmv_u64x2_cmple(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_cmple(wasmv_b8x16_t a, wasmv_b8x16_t b) { return wasmv_b8x16_cmple(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_cmple(wasmv_b16x8_t a, wasmv_b16x8_t b) { return wasmv_b16x8_cmple(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_cmple(wasmv_b32x4_t a, wasmv_b32x4_t b) { return wasmv_b32x4_cmple(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_cmple(wasmv_b64x2_t a, wasmv_b64x2_t b) { return wasmv_b64x2_cmple(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_cmple(wasmv_f32x4_t a, wasmv_f32x4_t b) { return wasmv_f32x4_cmple(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_cmple(wasmv_f64x2_t a, wasmv_f64x2_t b) { return wasmv_f64x2_cmple(a, b); }

  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t operator<=(wasmv_i8x16_t a, wasmv_i8x16_t b) { return wasmv_i8x16_cmple(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t operator<=(wasmv_i16x8_t a, wasmv_i16x8_t b) { return wasmv_i16x8_cmple(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t operator<=(wasmv_i32x4_t a, wasmv_i32x4_t b) { return wasmv_i32x4_cmple(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t operator<=(wasmv_i64x2_t a, wasmv_i64x2_t b) { return wasmv_i64x2_cmple(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t operator<=(wasmv_u8x16_t a, wasmv_u8x16_t b) { return wasmv_u8x16_cmple(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t operator<=(wasmv_u16x8_t a, wasmv_u16x8_t b) { return wasmv_u16x8_cmple(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t operator<=(wasmv_u32x4_t a, wasmv_u32x4_t b) { return wasmv_u32x4_cmple(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t operator<=(wasmv_u64x2_t a, wasmv_u64x2_t b) { return wasmv_u64x2_cmple(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t operator<=(wasmv_b8x16_t a, wasmv_b8x16_t b) { return wasmv_b8x16_cmple(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t operator<=(wasmv_b16x8_t a, wasmv_b16x8_t b) { return wasmv_b16x8_cmple(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t operator<=(wasmv_b32x4_t a, wasmv_b32x4_t b) { return wasmv_b32x4_cmple(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t operator<=(wasmv_b64x2_t a, wasmv_b64x2_t b) { return wasmv_b64x2_cmple(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t operator<=(wasmv_f32x4_t a, wasmv_f32x4_t b) { return wasmv_f32x4_cmple(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t operator<=(wasmv_f64x2_t a, wasmv_f64x2_t b) { return wasmv_f64x2_cmple(a, b); }
#endif

/* cmpgt */

WASMV_FUNCTION_ATTRIBUTES
wasmv_b8x16_t
wasmv_i8x16_cmpgt(wasmv_i8x16_t a, wasmv_i8x16_t b) {
  wasmv_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values > b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b16x8_t
wasmv_i16x8_cmpgt(wasmv_i16x8_t a, wasmv_i16x8_t b) {
  wasmv_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values > b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b32x4_t
wasmv_i32x4_cmpgt(wasmv_i32x4_t a, wasmv_i32x4_t b) {
  wasmv_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values > b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b64x2_t
wasmv_i64x2_cmpgt(wasmv_i64x2_t a, wasmv_i64x2_t b) {
  wasmv_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values > b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b8x16_t
wasmv_u8x16_cmpgt(wasmv_u8x16_t a, wasmv_u8x16_t b) {
  wasmv_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values > b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b16x8_t
wasmv_u16x8_cmpgt(wasmv_u16x8_t a, wasmv_u16x8_t b) {
  wasmv_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values > b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b32x4_t
wasmv_u32x4_cmpgt(wasmv_u32x4_t a, wasmv_u32x4_t b) {
  wasmv_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values > b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b64x2_t
wasmv_u64x2_cmpgt(wasmv_u64x2_t a, wasmv_u64x2_t b) {
  wasmv_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values > b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b8x16_t
wasmv_b8x16_cmpgt(wasmv_b8x16_t a, wasmv_b8x16_t b) {
  wasmv_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values > b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b16x8_t
wasmv_b16x8_cmpgt(wasmv_b16x8_t a, wasmv_b16x8_t b) {
  wasmv_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values > b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b32x4_t
wasmv_b32x4_cmpgt(wasmv_b32x4_t a, wasmv_b32x4_t b) {
  wasmv_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values > b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b64x2_t
wasmv_b64x2_cmpgt(wasmv_b64x2_t a, wasmv_b64x2_t b) {
  wasmv_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values > b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b32x4_t
wasmv_f32x4_cmpgt(wasmv_f32x4_t a, wasmv_f32x4_t b) {
  wasmv_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values > b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b64x2_t
wasmv_f64x2_cmpgt(wasmv_f64x2_t a, wasmv_f64x2_t b) {
  wasmv_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values > b.values);
  return r;
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_cmpgt(a, b) \
    (_Generic((a), \
      wasmv_i8x16_t: wasmv_i8x16_cmpgt, \
      wasmv_i16x8_t: wasmv_i16x8_cmpgt, \
      wasmv_i32x4_t: wasmv_i32x4_cmpgt, \
      wasmv_i64x2_t: wasmv_i64x2_cmpgt, \
      wasmv_u8x16_t: wasmv_u8x16_cmpgt, \
      wasmv_u16x8_t: wasmv_u16x8_cmpgt, \
      wasmv_u32x4_t: wasmv_u32x4_cmpgt, \
      wasmv_u64x2_t: wasmv_u64x2_cmpgt, \
      wasmv_b8x16_t: wasmv_b8x16_cmpgt, \
      wasmv_b16x8_t: wasmv_b16x8_cmpgt, \
      wasmv_b32x4_t: wasmv_b32x4_cmpgt, \
      wasmv_b64x2_t: wasmv_b64x2_cmpgt, \
      wasmv_f32x4_t: wasmv_f32x4_cmpgt, \
      wasmv_f64x2_t: wasmv_f64x2_cmpgt) (a, b))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_cmpgt(wasmv_i8x16_t a, wasmv_i8x16_t b) { return wasmv_i8x16_cmpgt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_cmpgt(wasmv_i16x8_t a, wasmv_i16x8_t b) { return wasmv_i16x8_cmpgt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_cmpgt(wasmv_i32x4_t a, wasmv_i32x4_t b) { return wasmv_i32x4_cmpgt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_cmpgt(wasmv_i64x2_t a, wasmv_i64x2_t b) { return wasmv_i64x2_cmpgt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_cmpgt(wasmv_u8x16_t a, wasmv_u8x16_t b) { return wasmv_u8x16_cmpgt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_cmpgt(wasmv_u16x8_t a, wasmv_u16x8_t b) { return wasmv_u16x8_cmpgt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_cmpgt(wasmv_u32x4_t a, wasmv_u32x4_t b) { return wasmv_u32x4_cmpgt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_cmpgt(wasmv_u64x2_t a, wasmv_u64x2_t b) { return wasmv_u64x2_cmpgt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_cmpgt(wasmv_b8x16_t a, wasmv_b8x16_t b) { return wasmv_b8x16_cmpgt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_cmpgt(wasmv_b16x8_t a, wasmv_b16x8_t b) { return wasmv_b16x8_cmpgt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_cmpgt(wasmv_b32x4_t a, wasmv_b32x4_t b) { return wasmv_b32x4_cmpgt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_cmpgt(wasmv_b64x2_t a, wasmv_b64x2_t b) { return wasmv_b64x2_cmpgt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_cmpgt(wasmv_f32x4_t a, wasmv_f32x4_t b) { return wasmv_f32x4_cmpgt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_cmpgt(wasmv_f64x2_t a, wasmv_f64x2_t b) { return wasmv_f64x2_cmpgt(a, b); }

  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t operator>(wasmv_i8x16_t a, wasmv_i8x16_t b) { return wasmv_i8x16_cmpgt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t operator>(wasmv_i16x8_t a, wasmv_i16x8_t b) { return wasmv_i16x8_cmpgt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t operator>(wasmv_i32x4_t a, wasmv_i32x4_t b) { return wasmv_i32x4_cmpgt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t operator>(wasmv_i64x2_t a, wasmv_i64x2_t b) { return wasmv_i64x2_cmpgt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t operator>(wasmv_u8x16_t a, wasmv_u8x16_t b) { return wasmv_u8x16_cmpgt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t operator>(wasmv_u16x8_t a, wasmv_u16x8_t b) { return wasmv_u16x8_cmpgt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t operator>(wasmv_u32x4_t a, wasmv_u32x4_t b) { return wasmv_u32x4_cmpgt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t operator>(wasmv_u64x2_t a, wasmv_u64x2_t b) { return wasmv_u64x2_cmpgt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t operator>(wasmv_b8x16_t a, wasmv_b8x16_t b) { return wasmv_b8x16_cmpgt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t operator>(wasmv_b16x8_t a, wasmv_b16x8_t b) { return wasmv_b16x8_cmpgt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t operator>(wasmv_b32x4_t a, wasmv_b32x4_t b) { return wasmv_b32x4_cmpgt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t operator>(wasmv_b64x2_t a, wasmv_b64x2_t b) { return wasmv_b64x2_cmpgt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t operator>(wasmv_f32x4_t a, wasmv_f32x4_t b) { return wasmv_f32x4_cmpgt(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t operator>(wasmv_f64x2_t a, wasmv_f64x2_t b) { return wasmv_f64x2_cmpgt(a, b); }
#endif

/* cmpge */

WASMV_FUNCTION_ATTRIBUTES
wasmv_b8x16_t
wasmv_i8x16_cmpge(wasmv_i8x16_t a, wasmv_i8x16_t b) {
  wasmv_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values >= b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b16x8_t
wasmv_i16x8_cmpge(wasmv_i16x8_t a, wasmv_i16x8_t b) {
  wasmv_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values >= b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b32x4_t
wasmv_i32x4_cmpge(wasmv_i32x4_t a, wasmv_i32x4_t b) {
  wasmv_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values >= b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b64x2_t
wasmv_i64x2_cmpge(wasmv_i64x2_t a, wasmv_i64x2_t b) {
  wasmv_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values >= b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b8x16_t
wasmv_u8x16_cmpge(wasmv_u8x16_t a, wasmv_u8x16_t b) {
  wasmv_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values >= b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b16x8_t
wasmv_u16x8_cmpge(wasmv_u16x8_t a, wasmv_u16x8_t b) {
  wasmv_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values >= b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b32x4_t
wasmv_u32x4_cmpge(wasmv_u32x4_t a, wasmv_u32x4_t b) {
  wasmv_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values >= b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b64x2_t
wasmv_u64x2_cmpge(wasmv_u64x2_t a, wasmv_u64x2_t b) {
  wasmv_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values >= b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b8x16_t
wasmv_b8x16_cmpge(wasmv_b8x16_t a, wasmv_b8x16_t b) {
  wasmv_b8x16_t r;
  r.values = (__typeof__(r.values)) (a.values >= b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b16x8_t
wasmv_b16x8_cmpge(wasmv_b16x8_t a, wasmv_b16x8_t b) {
  wasmv_b16x8_t r;
  r.values = (__typeof__(r.values)) (a.values >= b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b32x4_t
wasmv_b32x4_cmpge(wasmv_b32x4_t a, wasmv_b32x4_t b) {
  wasmv_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values >= b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b64x2_t
wasmv_b64x2_cmpge(wasmv_b64x2_t a, wasmv_b64x2_t b) {
  wasmv_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values >= b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b32x4_t
wasmv_f32x4_cmpge(wasmv_f32x4_t a, wasmv_f32x4_t b) {
  wasmv_b32x4_t r;
  r.values = (__typeof__(r.values)) (a.values >= b.values);
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_b64x2_t
wasmv_f64x2_cmpge(wasmv_f64x2_t a, wasmv_f64x2_t b) {
  wasmv_b64x2_t r;
  r.values = (__typeof__(r.values)) (a.values >= b.values);
  return r;
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_cmpge(a, b) \
    (_Generic((a), \
      wasmv_i8x16_t: wasmv_i8x16_cmpge, \
      wasmv_i16x8_t: wasmv_i16x8_cmpge, \
      wasmv_i32x4_t: wasmv_i32x4_cmpge, \
      wasmv_i64x2_t: wasmv_i64x2_cmpge, \
      wasmv_u8x16_t: wasmv_u8x16_cmpge, \
      wasmv_u16x8_t: wasmv_u16x8_cmpge, \
      wasmv_u32x4_t: wasmv_u32x4_cmpge, \
      wasmv_u64x2_t: wasmv_u64x2_cmpge, \
      wasmv_b8x16_t: wasmv_b8x16_cmpge, \
      wasmv_b16x8_t: wasmv_b16x8_cmpge, \
      wasmv_b32x4_t: wasmv_b32x4_cmpge, \
      wasmv_b64x2_t: wasmv_b64x2_cmpge, \
      wasmv_f32x4_t: wasmv_f32x4_cmpge, \
      wasmv_f64x2_t: wasmv_f64x2_cmpge) (a, b))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_cmpge(wasmv_i8x16_t a, wasmv_i8x16_t b) { return wasmv_i8x16_cmpge(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_cmpge(wasmv_i16x8_t a, wasmv_i16x8_t b) { return wasmv_i16x8_cmpge(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_cmpge(wasmv_i32x4_t a, wasmv_i32x4_t b) { return wasmv_i32x4_cmpge(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_cmpge(wasmv_i64x2_t a, wasmv_i64x2_t b) { return wasmv_i64x2_cmpge(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_cmpge(wasmv_u8x16_t a, wasmv_u8x16_t b) { return wasmv_u8x16_cmpge(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_cmpge(wasmv_u16x8_t a, wasmv_u16x8_t b) { return wasmv_u16x8_cmpge(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_cmpge(wasmv_u32x4_t a, wasmv_u32x4_t b) { return wasmv_u32x4_cmpge(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_cmpge(wasmv_u64x2_t a, wasmv_u64x2_t b) { return wasmv_u64x2_cmpge(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t wasmv_cmpge(wasmv_b8x16_t a, wasmv_b8x16_t b) { return wasmv_b8x16_cmpge(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t wasmv_cmpge(wasmv_b16x8_t a, wasmv_b16x8_t b) { return wasmv_b16x8_cmpge(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_cmpge(wasmv_b32x4_t a, wasmv_b32x4_t b) { return wasmv_b32x4_cmpge(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_cmpge(wasmv_b64x2_t a, wasmv_b64x2_t b) { return wasmv_b64x2_cmpge(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t wasmv_cmpge(wasmv_f32x4_t a, wasmv_f32x4_t b) { return wasmv_f32x4_cmpge(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t wasmv_cmpge(wasmv_f64x2_t a, wasmv_f64x2_t b) { return wasmv_f64x2_cmpge(a, b); }

  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t operator>=(wasmv_i8x16_t a, wasmv_i8x16_t b) { return wasmv_i8x16_cmpge(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t operator>=(wasmv_i16x8_t a, wasmv_i16x8_t b) { return wasmv_i16x8_cmpge(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t operator>=(wasmv_i32x4_t a, wasmv_i32x4_t b) { return wasmv_i32x4_cmpge(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t operator>=(wasmv_i64x2_t a, wasmv_i64x2_t b) { return wasmv_i64x2_cmpge(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t operator>=(wasmv_u8x16_t a, wasmv_u8x16_t b) { return wasmv_u8x16_cmpge(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t operator>=(wasmv_u16x8_t a, wasmv_u16x8_t b) { return wasmv_u16x8_cmpge(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t operator>=(wasmv_u32x4_t a, wasmv_u32x4_t b) { return wasmv_u32x4_cmpge(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t operator>=(wasmv_u64x2_t a, wasmv_u64x2_t b) { return wasmv_u64x2_cmpge(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b8x16_t operator>=(wasmv_b8x16_t a, wasmv_b8x16_t b) { return wasmv_b8x16_cmpge(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b16x8_t operator>=(wasmv_b16x8_t a, wasmv_b16x8_t b) { return wasmv_b16x8_cmpge(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t operator>=(wasmv_b32x4_t a, wasmv_b32x4_t b) { return wasmv_b32x4_cmpge(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t operator>=(wasmv_b64x2_t a, wasmv_b64x2_t b) { return wasmv_b64x2_cmpge(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b32x4_t operator>=(wasmv_f32x4_t a, wasmv_f32x4_t b) { return wasmv_f32x4_cmpge(a, b); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_b64x2_t operator>=(wasmv_f64x2_t a, wasmv_f64x2_t b) { return wasmv_f64x2_cmpge(a, b); }
#endif

/* load_zero -- Load and Zero-Pad
 *
 * Load a single 32-bit or 64-bit element into the lowest bits of a
 * vector, and initialize all other bits of the vector to zero.
 */

WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i32x4_load_zero(const int32_t * a) {
  return (wasmv_i32x4_t) { { *a, 0, 0, 0 } };
}

WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_i64x2_load_zero(const int64_t * a) {
  return (wasmv_i64x2_t) { { *a, 0 } };
}

WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_u32x4_load_zero(const uint32_t * a) {
  return (wasmv_u32x4_t) { { *a, 0, 0, 0 } };
}

WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WASMV_FUNCTION_ATTRIBUTES
wasmv_u64x2_t
wasmv_u64x2_load_zero(const uint64_t * a) {
  return (wasmv_u64x2_t) { { *a, 0  } };
}

WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WASMV_FUNCTION_ATTRIBUTES
wasmv_f32x4_t
wasmv_f32x4_load_zero(const float * a) {
  return (wasmv_f32x4_t) { { *a, 0, 0, 0 } };
}

WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WASMV_FUNCTION_ATTRIBUTES
wasmv_f64x2_t
wasmv_f64x2_load_zero(const double * a) {
  return (wasmv_f64x2_t) { { *a, 0  } };
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_load_zero(a) \
    (_Generic((a), \
       int32_t*: wasmv_i32x4_load_zero, \
       int64_t*: wasmv_i64x2_load_zero, \
      uint32_t*: wasmv_u32x4_load_zero, \
      uint64_t*: wasmv_u64x2_load_zero, \
         float*: wasmv_f32x4_load_zero, \
        double*: wasmv_f64x2_load_zero) (a))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_load_zero( int32_t* a) { return wasmv_i32x4_load_zero(a); }
  WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_load_zero( int64_t* a) { return wasmv_i64x2_load_zero(a); }
  WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_load_zero(uint32_t* a) { return wasmv_u32x4_load_zero(a); }
  WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_load_zero(uint64_t* a) { return wasmv_u64x2_load_zero(a); }
  WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_load_zero(   float* a) { return wasmv_f32x4_load_zero(a); }
  WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_load_zero(  double* a) { return wasmv_f64x2_load_zero(a); }
#endif

/* loadu_zero -- Unaligned Load and Zero-Pad
 *
 * Load a single 32-bit or 64-bit element into the lowest bits of a
 * vector, and initialize all other bits of the vector to zero.
 */

WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i32x4_loadu_zero(const void * a) {
  int32_t v;
  __builtin_memcpy(&v, a, sizeof(v));
  return (wasmv_i32x4_t) { { v, 0, 0, 0 } };
}

WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_i64x2_loadu_zero(const void * a) {
  int64_t v;
  __builtin_memcpy(&v, a, sizeof(v));
  return (wasmv_i64x2_t) { { v, 0 } };
}

WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_u32x4_loadu_zero(const void * a) {
  uint32_t v;
  __builtin_memcpy(&v, a, sizeof(v));
  return (wasmv_u32x4_t) { { v, 0, 0, 0 } };
}

WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WASMV_FUNCTION_ATTRIBUTES
wasmv_u64x2_t
wasmv_u64x2_loadu_zero(const void * a) {
  uint64_t v;
  __builtin_memcpy(&v, a, sizeof(v));
  return (wasmv_u64x2_t) { { v, 0  } };
}

WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WASMV_FUNCTION_ATTRIBUTES
wasmv_f32x4_t
wasmv_f32x4_loadu_zero(const void * a) {
  float v;
  __builtin_memcpy(&v, a, sizeof(v));
  return (wasmv_f32x4_t) { { v, 0, 0, 0 } };
}

WASMV_UNIMPLEMENTED_SIMD128_ATTRIBUTE
WASMV_FUNCTION_ATTRIBUTES
wasmv_f64x2_t
wasmv_f64x2_loadu_zero(const void * a) {
  double v;
  __builtin_memcpy(&v, a, sizeof(v));
  return (wasmv_f64x2_t) { { v, 0  } };
}

/* load_splat -- Load and Splat
 *
 * Load a single element and splat to all lanes of a vector. The
 * natural alignment is the size of the element loaded.
 */

WASMV_FUNCTION_ATTRIBUTES
wasmv_i8x16_t
wasmv_i8x16_load_splat(const int8_t * a) {
  return wasmv_i8x16_splat(*a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i16x8_load_splat(const int16_t * a) {
  return wasmv_i16x8_splat(*a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i32x4_load_splat(const int32_t * a) {
  return wasmv_i32x4_splat(*a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_i64x2_load_splat(const int64_t * a) {
  return wasmv_i64x2_splat(*a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u8x16_t
wasmv_u8x16_load_splat(const uint8_t * a) {
  return wasmv_u8x16_splat(*a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_u16x8_load_splat(const uint16_t * a) {
  return wasmv_u16x8_splat(*a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_u32x4_load_splat(const uint32_t * a) {
  return wasmv_u32x4_splat(*a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u64x2_t
wasmv_u64x2_load_splat(const uint64_t * a) {
  return wasmv_u64x2_splat(*a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f32x4_t
wasmv_f32x4_load_splat(const float * a) {
  return wasmv_f32x4_splat(*a);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f64x2_t
wasmv_f64x2_load_splat(const double * a) {
  return wasmv_f64x2_splat(*a);
}

#if WASMV_OVERLOADS == WASMV_OVERLOADS_C11
  #define wasmv_load_splat(a) \
    (_Generic((a), \
        int8_t*: wasmv_i8x16_load_splat, \
       int16_t*: wasmv_i16x8_load_splat, \
       int32_t*: wasmv_i32x4_load_splat, \
       int64_t*: wasmv_i64x2_load_splat, \
      uint32_t*: wasmv_u32x4_load_splat, \
      uint64_t*: wasmv_u64x2_load_splat, \
         float*: wasmv_f32x4_load_splat, \
        double*: wasmv_f64x2_load_splat) (a))
#elif WASMV_OVERLOADS == WASMV_OVERLOADS_CXX
  WASMV_FUNCTION_ATTRIBUTES wasmv_i8x16_t wasmv_load_splat(  int8_t* a) { return wasmv_i8x16_load_splat(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i16x8_t wasmv_load_splat( int16_t* a) { return wasmv_i16x8_load_splat(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i32x4_t wasmv_load_splat( int32_t* a) { return wasmv_i32x4_load_splat(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_i64x2_t wasmv_load_splat( int64_t* a) { return wasmv_i64x2_load_splat(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u8x16_t wasmv_load_splat( uint8_t* a) { return wasmv_u8x16_load_splat(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u16x8_t wasmv_load_splat(uint16_t* a) { return wasmv_u16x8_load_splat(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u32x4_t wasmv_load_splat(uint32_t* a) { return wasmv_u32x4_load_splat(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_u64x2_t wasmv_load_splat(uint64_t* a) { return wasmv_u64x2_load_splat(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f32x4_t wasmv_load_splat(   float* a) { return wasmv_f32x4_load_splat(a); }
  WASMV_FUNCTION_ATTRIBUTES wasmv_f64x2_t wasmv_load_splat(  double* a) { return wasmv_f64x2_load_splat(a); }
#endif

/* loadu_splat -- Load and Splat
 *
 * Load a single element and splat to all lanes of a vector. The
 * natural alignment is the size of the element loaded.
 */

WASMV_FUNCTION_ATTRIBUTES
wasmv_i8x16_t
wasmv_i8x16_loadu_splat(const void * a) {
  int8_t v;
  __builtin_memcpy(&v, a, sizeof(v));
  return wasmv_i8x16_splat(v);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i16x8_loadu_splat(const void * a) {
  int16_t v;
  __builtin_memcpy(&v, a, sizeof(v));
  return wasmv_i16x8_splat(v);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i32x4_loadu_splat(const void * a) {
  int32_t v;
  __builtin_memcpy(&v, a, sizeof(v));
  return wasmv_i32x4_splat(v);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_i64x2_loadu_splat(const void * a) {
  int64_t v;
  __builtin_memcpy(&v, a, sizeof(v));
  return wasmv_i64x2_splat(v);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u8x16_t
wasmv_u8x16_loadu_splat(const void * a) {
  uint8_t v;
  __builtin_memcpy(&v, a, sizeof(v));
  return wasmv_u8x16_splat(v);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_u16x8_loadu_splat(const void * a) {
  uint16_t v;
  __builtin_memcpy(&v, a, sizeof(v));
  return wasmv_u16x8_splat(v);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_u32x4_loadu_splat(const void * a) {
  uint32_t v;
  __builtin_memcpy(&v, a, sizeof(v));
  return wasmv_u32x4_splat(v);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u64x2_t
wasmv_u64x2_loadu_splat(const void * a) {
  uint64_t v;
  __builtin_memcpy(&v, a, sizeof(v));
  return wasmv_u64x2_splat(v);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f32x4_t
wasmv_f32x4_loadu_splat(const void * a) {
  float v;
  __builtin_memcpy(&v, a, sizeof(v));
  return wasmv_f32x4_splat(v);
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_f64x2_t
wasmv_f64x2_loadu_splat(const void * a) {
  double v;
  __builtin_memcpy(&v, a, sizeof(v));
  return wasmv_f64x2_splat(v);
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

WASMV_FUNCTION_ATTRIBUTES
wasmv_i16x8_t
wasmv_i8x16_load_extend(int8_t values[WASMV_ARRAY_PARAM(8)]) {
  wasmv_i16x8_t r;
  int8_t vec __attribute__((__vector_size__(8)));
  __builtin_memcpy(&vec, values, sizeof(vec));
  r.values = __builtin_convertvector(vec, __typeof__(r.values));
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i32x4_t
wasmv_i16x8_load_extend(int16_t values[WASMV_ARRAY_PARAM(4)]) {
  wasmv_i32x4_t r;
  int16_t vec __attribute__((__vector_size__(8)));
  __builtin_memcpy(&vec, values, sizeof(vec));
  r.values = __builtin_convertvector(vec, __typeof__(r.values));
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_i64x2_t
wasmv_i32x4_load_extend(int32_t values[WASMV_ARRAY_PARAM(2)]) {
  wasmv_i64x2_t r;
  int32_t vec __attribute__((__vector_size__(8)));
  __builtin_memcpy(&vec, values, sizeof(vec));
  r.values = __builtin_convertvector(vec, __typeof__(r.values));
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u16x8_t
wasmv_u8x16_load_extend(int8_t values[WASMV_ARRAY_PARAM(8)]) {
  wasmv_u16x8_t r;
  int8_t vec __attribute__((__vector_size__(8)));
  __builtin_memcpy(&vec, values, sizeof(vec));
  r.values = __builtin_convertvector(vec, __typeof__(r.values));
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u32x4_t
wasmv_u16x8_load_extend(int16_t values[WASMV_ARRAY_PARAM(4)]) {
  wasmv_u32x4_t r;
  int16_t vec __attribute__((__vector_size__(8)));
  __builtin_memcpy(&vec, values, sizeof(vec));
  r.values = __builtin_convertvector(vec, __typeof__(r.values));
  return r;
}

WASMV_FUNCTION_ATTRIBUTES
wasmv_u64x2_t
wasmv_u32x4_load_extend(int32_t values[WASMV_ARRAY_PARAM(2)]) {
  wasmv_u64x2_t r;
  int32_t vec __attribute__((__vector_size__(8)));
  __builtin_memcpy(&vec, values, sizeof(vec));
  r.values = __builtin_convertvector(vec, __typeof__(r.values));
  return r;
}

#pragma clang diagnostic pop
