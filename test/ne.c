#include "../wav.h"

wav_b8x16_t ne_i8x16(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_ne(a, b); }
wav_b16x8_t ne_i16x8(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_ne(a, b); }
wav_b32x4_t ne_i32x4(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_ne(a, b); }
wav_b64x2_t ne_i64x2(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_ne(a, b); }
wav_b8x16_t ne_u8x16(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_ne(a, b); }
wav_b16x8_t ne_u16x8(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_ne(a, b); }
wav_b32x4_t ne_u32x4(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_ne(a, b); }
wav_b64x2_t ne_u64x2(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_ne(a, b); }
wav_b8x16_t ne_b8x16(wav_b8x16_t a, wav_b8x16_t b) { return wav_b8x16_ne(a, b); }
wav_b16x8_t ne_b16x8(wav_b16x8_t a, wav_b16x8_t b) { return wav_b16x8_ne(a, b); }
wav_b32x4_t ne_b32x4(wav_b32x4_t a, wav_b32x4_t b) { return wav_b32x4_ne(a, b); }
wav_b64x2_t ne_b64x2(wav_b64x2_t a, wav_b64x2_t b) { return wav_b64x2_ne(a, b); }
wav_b32x4_t ne_f32x4(wav_f32x4_t a, wav_f32x4_t b) { return wav_f32x4_ne(a, b); }
wav_b64x2_t ne_f64x2(wav_f64x2_t a, wav_f64x2_t b) { return wav_f64x2_ne(a, b); }

wav_b8x16_t ne_i8x16_o(wav_i8x16_t a, wav_i8x16_t b) { return wav_ne(a, b); }
wav_b16x8_t ne_i16x8_o(wav_i16x8_t a, wav_i16x8_t b) { return wav_ne(a, b); }
wav_b32x4_t ne_i32x4_o(wav_i32x4_t a, wav_i32x4_t b) { return wav_ne(a, b); }
wav_b64x2_t ne_i64x2_o(wav_i64x2_t a, wav_i64x2_t b) { return wav_ne(a, b); }
wav_b8x16_t ne_u8x16_o(wav_u8x16_t a, wav_u8x16_t b) { return wav_ne(a, b); }
wav_b16x8_t ne_u16x8_o(wav_u16x8_t a, wav_u16x8_t b) { return wav_ne(a, b); }
wav_b32x4_t ne_u32x4_o(wav_u32x4_t a, wav_u32x4_t b) { return wav_ne(a, b); }
wav_b64x2_t ne_u64x2_o(wav_u64x2_t a, wav_u64x2_t b) { return wav_ne(a, b); }
wav_b8x16_t ne_b8x16_o(wav_b8x16_t a, wav_b8x16_t b) { return wav_ne(a, b); }
wav_b16x8_t ne_b16x8_o(wav_b16x8_t a, wav_b16x8_t b) { return wav_ne(a, b); }
wav_b32x4_t ne_b32x4_o(wav_b32x4_t a, wav_b32x4_t b) { return wav_ne(a, b); }
wav_b64x2_t ne_b64x2_o(wav_b64x2_t a, wav_b64x2_t b) { return wav_ne(a, b); }
wav_b32x4_t ne_f32x4_o(wav_f32x4_t a, wav_f32x4_t b) { return wav_ne(a, b); }
wav_b64x2_t ne_f64x2_o(wav_f64x2_t a, wav_f64x2_t b) { return wav_ne(a, b); }

#if WAV_OPERATOR_OVERLOADING
wav_b8x16_t ne_i8x16_op(wav_i8x16_t a, wav_i8x16_t b) { return a != b; }
wav_b16x8_t ne_i16x8_op(wav_i16x8_t a, wav_i16x8_t b) { return a != b; }
wav_b32x4_t ne_i32x4_op(wav_i32x4_t a, wav_i32x4_t b) { return a != b; }
wav_b64x2_t ne_i64x2_op(wav_i64x2_t a, wav_i64x2_t b) { return a != b; }
wav_b8x16_t ne_u8x16_op(wav_u8x16_t a, wav_u8x16_t b) { return a != b; }
wav_b16x8_t ne_u16x8_op(wav_u16x8_t a, wav_u16x8_t b) { return a != b; }
wav_b32x4_t ne_u32x4_op(wav_u32x4_t a, wav_u32x4_t b) { return a != b; }
wav_b64x2_t ne_u64x2_op(wav_u64x2_t a, wav_u64x2_t b) { return a != b; }
wav_b8x16_t ne_b8x16_op(wav_b8x16_t a, wav_b8x16_t b) { return a != b; }
wav_b16x8_t ne_b16x8_op(wav_b16x8_t a, wav_b16x8_t b) { return a != b; }
wav_b32x4_t ne_b32x4_op(wav_b32x4_t a, wav_b32x4_t b) { return a != b; }
wav_b64x2_t ne_b64x2_op(wav_b64x2_t a, wav_b64x2_t b) { return a != b; }
wav_b32x4_t ne_f32x4_op(wav_f32x4_t a, wav_f32x4_t b) { return a != b; }
wav_b64x2_t ne_f64x2_op(wav_f64x2_t a, wav_f64x2_t b) { return a != b; }
#endif
