#include "../wav.h"

wav_i16x8_t mul_i16x8(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_mul(a, b); }
wav_i32x4_t mul_i32x4(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_mul(a, b); }
wav_i64x2_t mul_i64x2(wav_i64x2_t a, wav_i64x2_t b) { return wav_i64x2_mul(a, b); }
wav_u16x8_t mul_u16x8(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_mul(a, b); }
wav_u32x4_t mul_u32x4(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_mul(a, b); }
wav_u64x2_t mul_u64x2(wav_u64x2_t a, wav_u64x2_t b) { return wav_u64x2_mul(a, b); }
wav_f32x4_t mul_f32x4(wav_f32x4_t a, wav_f32x4_t b) { return wav_f32x4_mul(a, b); }
wav_f64x2_t mul_f64x2(wav_f64x2_t a, wav_f64x2_t b) { return wav_f64x2_mul(a, b); }

wav_i16x8_t mul_i16x8_o(wav_i16x8_t a, wav_i16x8_t b) { return wav_mul(a, b); }
wav_i32x4_t mul_i32x4_o(wav_i32x4_t a, wav_i32x4_t b) { return wav_mul(a, b); }
wav_i64x2_t mul_i64x2_o(wav_i64x2_t a, wav_i64x2_t b) { return wav_mul(a, b); }
wav_u16x8_t mul_u16x8_o(wav_u16x8_t a, wav_u16x8_t b) { return wav_mul(a, b); }
wav_u32x4_t mul_u32x4_o(wav_u32x4_t a, wav_u32x4_t b) { return wav_mul(a, b); }
wav_u64x2_t mul_u64x2_o(wav_u64x2_t a, wav_u64x2_t b) { return wav_mul(a, b); }
wav_f32x4_t mul_f32x4_o(wav_f32x4_t a, wav_f32x4_t b) { return wav_mul(a, b); }
wav_f64x2_t mul_f64x2_o(wav_f64x2_t a, wav_f64x2_t b) { return wav_mul(a, b); }

#if WAV_OPERATOR_OVERLOADING
wav_i16x8_t mul_i16x8_op(wav_i16x8_t a, wav_i16x8_t b) { return a * b; }
wav_i32x4_t mul_i32x4_op(wav_i32x4_t a, wav_i32x4_t b) { return a * b; }
wav_i64x2_t mul_i64x2_op(wav_i64x2_t a, wav_i64x2_t b) { return a * b; }
wav_u16x8_t mul_u16x8_op(wav_u16x8_t a, wav_u16x8_t b) { return a * b; }
wav_u32x4_t mul_u32x4_op(wav_u32x4_t a, wav_u32x4_t b) { return a * b; }
wav_u64x2_t mul_u64x2_op(wav_u64x2_t a, wav_u64x2_t b) { return a * b; }
wav_f32x4_t mul_f32x4_op(wav_f32x4_t a, wav_f32x4_t b) { return a * b; }
wav_f64x2_t mul_f64x2_op(wav_f64x2_t a, wav_f64x2_t b) { return a * b; }
#endif
