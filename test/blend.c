#include "../wav.h"

wav_i8x16_t blend_i8x16(wav_i8x16_t a, wav_i8x16_t b, wav_b8x16_t c) { return wav_i8x16_blend(a, b, c); }
wav_i16x8_t blend_i16x8(wav_i16x8_t a, wav_i16x8_t b, wav_b16x8_t c) { return wav_i16x8_blend(a, b, c); }
wav_i32x4_t blend_i32x4(wav_i32x4_t a, wav_i32x4_t b, wav_b32x4_t c) { return wav_i32x4_blend(a, b, c); }
wav_i64x2_t blend_i64x2(wav_i64x2_t a, wav_i64x2_t b, wav_b64x2_t c) { return wav_i64x2_blend(a, b, c); }
wav_u8x16_t blend_u8x16(wav_u8x16_t a, wav_u8x16_t b, wav_b8x16_t c) { return wav_u8x16_blend(a, b, c); }
wav_u16x8_t blend_u16x8(wav_u16x8_t a, wav_u16x8_t b, wav_b16x8_t c) { return wav_u16x8_blend(a, b, c); }
wav_u32x4_t blend_u32x4(wav_u32x4_t a, wav_u32x4_t b, wav_b32x4_t c) { return wav_u32x4_blend(a, b, c); }
wav_u64x2_t blend_u64x2(wav_u64x2_t a, wav_u64x2_t b, wav_b64x2_t c) { return wav_u64x2_blend(a, b, c); }
wav_f32x4_t blend_f32x4(wav_f32x4_t a, wav_f32x4_t b, wav_b32x4_t c) { return wav_f32x4_blend(a, b, c); }
wav_f64x2_t blend_f64x2(wav_f64x2_t a, wav_f64x2_t b, wav_b64x2_t c) { return wav_f64x2_blend(a, b, c); }

wav_i8x16_t blend_i8x16_o(wav_i8x16_t a, wav_i8x16_t b, wav_b8x16_t c) { return wav_blend(a, b, c); }
wav_i16x8_t blend_i16x8_o(wav_i16x8_t a, wav_i16x8_t b, wav_b16x8_t c) { return wav_blend(a, b, c); }
wav_i32x4_t blend_i32x4_o(wav_i32x4_t a, wav_i32x4_t b, wav_b32x4_t c) { return wav_blend(a, b, c); }
wav_i64x2_t blend_i64x2_o(wav_i64x2_t a, wav_i64x2_t b, wav_b64x2_t c) { return wav_blend(a, b, c); }
wav_u8x16_t blend_u8x16_o(wav_u8x16_t a, wav_u8x16_t b, wav_b8x16_t c) { return wav_blend(a, b, c); }
wav_u16x8_t blend_u16x8_o(wav_u16x8_t a, wav_u16x8_t b, wav_b16x8_t c) { return wav_blend(a, b, c); }
wav_u32x4_t blend_u32x4_o(wav_u32x4_t a, wav_u32x4_t b, wav_b32x4_t c) { return wav_blend(a, b, c); }
wav_u64x2_t blend_u64x2_o(wav_u64x2_t a, wav_u64x2_t b, wav_b64x2_t c) { return wav_blend(a, b, c); }
wav_f32x4_t blend_f32x4_o(wav_f32x4_t a, wav_f32x4_t b, wav_b32x4_t c) { return wav_blend(a, b, c); }
wav_f64x2_t blend_f64x2_o(wav_f64x2_t a, wav_f64x2_t b, wav_b64x2_t c) { return wav_blend(a, b, c); }
