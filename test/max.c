#include "../wav.h"

wav_i8x16_t max_i8x16(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_max(a, b); }
wav_i16x8_t max_i16x8(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_max(a, b); }
wav_i32x4_t max_i32x4(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_max(a, b); }
wav_u8x16_t max_u8x16(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_max(a, b); }
wav_u16x8_t max_u16x8(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_max(a, b); }
wav_u32x4_t max_u32x4(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_max(a, b); }
wav_f32x4_t max_f32x4(wav_f32x4_t a, wav_f32x4_t b) { return wav_f32x4_max(a, b); }
wav_f64x2_t max_f64x2(wav_f64x2_t a, wav_f64x2_t b) { return wav_f64x2_max(a, b); }

wav_i8x16_t max_i8x16_o(wav_i8x16_t a, wav_i8x16_t b) { return wav_max(a, b); }
wav_i16x8_t max_i16x8_o(wav_i16x8_t a, wav_i16x8_t b) { return wav_max(a, b); }
wav_i32x4_t max_i32x4_o(wav_i32x4_t a, wav_i32x4_t b) { return wav_max(a, b); }
wav_u8x16_t max_u8x16_o(wav_u8x16_t a, wav_u8x16_t b) { return wav_max(a, b); }
wav_u16x8_t max_u16x8_o(wav_u16x8_t a, wav_u16x8_t b) { return wav_max(a, b); }
wav_u32x4_t max_u32x4_o(wav_u32x4_t a, wav_u32x4_t b) { return wav_max(a, b); }
wav_f32x4_t max_f32x4_o(wav_f32x4_t a, wav_f32x4_t b) { return wav_max(a, b); }
wav_f64x2_t max_f64x2_o(wav_f64x2_t a, wav_f64x2_t b) { return wav_max(a, b); }
