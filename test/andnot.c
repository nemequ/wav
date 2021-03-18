#include "../wav.h"

wav_i8x16_t andnot_i8x16(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_andnot(a, b); }
wav_i16x8_t andnot_i16x8(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_andnot(a, b); }
wav_i32x4_t andnot_i32x4(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_andnot(a, b); }
wav_i64x2_t andnot_i64x2(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_andnot(a, b); }
wav_u8x16_t andnot_u8x16(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_andnot(a, b); }
wav_u16x8_t andnot_u16x8(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_andnot(a, b); }
wav_u32x4_t andnot_u32x4(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_andnot(a, b); }
wav_u64x2_t andnot_u64x2(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_andnot(a, b); }
wav_b8x16_t andnot_b8x16(wav_b8x16_t a, wav_b8x16_t b) { return wav_b8x16_andnot(a, b); }
wav_b16x8_t andnot_b16x8(wav_b16x8_t a, wav_b16x8_t b) { return wav_b16x8_andnot(a, b); }
wav_b32x4_t andnot_b32x4(wav_b32x4_t a, wav_b32x4_t b) { return wav_b32x4_andnot(a, b); }
wav_b64x2_t andnot_b64x2(wav_b64x2_t a, wav_b64x2_t b) { return wav_b64x2_andnot(a, b); }

wav_i8x16_t andnot_i8x16_b(wav_i8x16_t a, wav_b8x16_t b) { return wav_i8x16_andnot_b8x16(a, b); }
wav_i16x8_t andnot_i16x8_b(wav_i16x8_t a, wav_b16x8_t b) { return wav_i16x8_andnot_b16x8(a, b); }
wav_i32x4_t andnot_i32x4_b(wav_i32x4_t a, wav_b32x4_t b) { return wav_i32x4_andnot_b32x4(a, b); }
wav_i64x2_t andnot_i64x2_b(wav_i64x2_t a, wav_b64x2_t b) { return wav_i64x2_andnot_b64x2(a, b); }
wav_u8x16_t andnot_u8x16_b(wav_u8x16_t a, wav_b8x16_t b) { return wav_u8x16_andnot_b8x16(a, b); }
wav_u16x8_t andnot_u16x8_b(wav_u16x8_t a, wav_b16x8_t b) { return wav_u16x8_andnot_b16x8(a, b); }
wav_u32x4_t andnot_u32x4_b(wav_u32x4_t a, wav_b32x4_t b) { return wav_u32x4_andnot_b32x4(a, b); }
wav_u64x2_t andnot_u64x2_b(wav_u64x2_t a, wav_b64x2_t b) { return wav_u64x2_andnot_b64x2(a, b); }
wav_f32x4_t andnot_f32x4_b(wav_f32x4_t a, wav_b32x4_t b) { return wav_f32x4_andnot_b32x4(a, b); }
wav_f64x2_t andnot_f64x2_b(wav_f64x2_t a, wav_b64x2_t b) { return wav_f64x2_andnot_b64x2(a, b); }

wav_i8x16_t andnot_b8x16_i(wav_b8x16_t a, wav_i8x16_t b) { return wav_b8x16_andnot_i8x16(a, b); }
wav_i16x8_t andnot_b16x8_i(wav_b16x8_t a, wav_i16x8_t b) { return wav_b16x8_andnot_i16x8(a, b); }
wav_i32x4_t andnot_b32x4_i(wav_b32x4_t a, wav_i32x4_t b) { return wav_b32x4_andnot_i32x4(a, b); }
wav_i64x2_t andnot_b64x2_i(wav_b64x2_t a, wav_i64x2_t b) { return wav_b64x2_andnot_i64x2(a, b); }
wav_u8x16_t andnot_b8x16_u(wav_b8x16_t a, wav_u8x16_t b) { return wav_b8x16_andnot_u8x16(a, b); }
wav_u16x8_t andnot_b16x8_u(wav_b16x8_t a, wav_u16x8_t b) { return wav_b16x8_andnot_u16x8(a, b); }
wav_u32x4_t andnot_b32x4_u(wav_b32x4_t a, wav_u32x4_t b) { return wav_b32x4_andnot_u32x4(a, b); }
wav_u64x2_t andnot_b64x2_u(wav_b64x2_t a, wav_u64x2_t b) { return wav_b64x2_andnot_u64x2(a, b); }

