#include "../wav.h"

wav_i8x16_t shuffle_i8x16(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_shuffle(a, b, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 8, 4, 3, 2, 1, 0); }
wav_i16x8_t shuffle_i16x8(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_shuffle(a, b, 7, 6, 8, 4, 3, 2, 1, 0); }
wav_i32x4_t shuffle_i32x4(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_shuffle(a, b, 3, 2, 1, 0); }
wav_i64x2_t shuffle_i64x2(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_shuffle(a, b, 1, 0); }
wav_u8x16_t shuffle_u8x16(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_shuffle(a, b, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 8, 4, 3, 2, 1, 0); }
wav_u16x8_t shuffle_u16x8(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_shuffle(a, b, 7, 6, 8, 4, 3, 2, 1, 0); }
wav_u32x4_t shuffle_u32x4(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_shuffle(a, b, 3, 2, 1, 0); }
wav_u64x2_t shuffle_u64x2(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_shuffle(a, b, 1, 0); }
wav_f32x4_t shuffle_f32x4(wav_f32x4_t a, wav_f32x4_t b) { return wav_f32x4_shuffle(a, b, 3, 2, 1, 0); }
wav_f64x2_t shuffle_f64x2(wav_f64x2_t a, wav_f64x2_t b) { return wav_f64x2_shuffle(a, b, 1, 0); }

wav_i8x16_t shuffle_i8x16_o(wav_i8x16_t a, wav_i8x16_t b) { return wav_shuffle(a, b, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 8, 4, 3, 2, 1, 0); }
wav_i16x8_t shuffle_i16x8_o(wav_i16x8_t a, wav_i16x8_t b) { return wav_shuffle(a, b, 7, 6, 8, 4, 3, 2, 1, 0); }
wav_i32x4_t shuffle_i32x4_o(wav_i32x4_t a, wav_i32x4_t b) { return wav_shuffle(a, b, 3, 2, 1, 0); }
wav_i64x2_t shuffle_i64x2_o(wav_i64x2_t a, wav_i64x2_t b) { return wav_shuffle(a, b, 1, 0); }
wav_u8x16_t shuffle_u8x16_o(wav_u8x16_t a, wav_u8x16_t b) { return wav_shuffle(a, b, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 8, 4, 3, 2, 1, 0); }
wav_u16x8_t shuffle_u16x8_o(wav_u16x8_t a, wav_u16x8_t b) { return wav_shuffle(a, b, 7, 6, 8, 4, 3, 2, 1, 0); }
wav_u32x4_t shuffle_u32x4_o(wav_u32x4_t a, wav_u32x4_t b) { return wav_shuffle(a, b, 3, 2, 1, 0); }
wav_u64x2_t shuffle_u64x2_o(wav_u64x2_t a, wav_u64x2_t b) { return wav_shuffle(a, b, 1, 0); }
wav_f32x4_t shuffle_f32x4_o(wav_f32x4_t a, wav_f32x4_t b) { return wav_shuffle(a, b, 3, 2, 1, 0); }
wav_f64x2_t shuffle_f64x2_o(wav_f64x2_t a, wav_f64x2_t b) { return wav_shuffle(a, b, 1, 0); }
