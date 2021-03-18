#include "../wav.h"

wav_i8x16_t neg_i8x16(wav_i8x16_t a) { return wav_i8x16_neg(a); }
wav_i16x8_t neg_i16x8(wav_i16x8_t a) { return wav_i16x8_neg(a); }
wav_i32x4_t neg_i32x4(wav_i32x4_t a) { return wav_i32x4_neg(a); }
wav_i64x2_t neg_i64x2(wav_i64x2_t a) { return wav_i64x2_neg(a); }
wav_f32x4_t neg_f32x4(wav_f32x4_t a) { return wav_f32x4_neg(a); }
wav_f64x2_t neg_f64x2(wav_f64x2_t a) { return wav_f64x2_neg(a); }

wav_i8x16_t neg_i8x16_o(wav_i8x16_t a) { return wav_neg(a); }
wav_i16x8_t neg_i16x8_o(wav_i16x8_t a) { return wav_neg(a); }
wav_i32x4_t neg_i32x4_o(wav_i32x4_t a) { return wav_neg(a); }
wav_i64x2_t neg_i64x2_o(wav_i64x2_t a) { return wav_neg(a); }
wav_f32x4_t neg_f32x4_o(wav_f32x4_t a) { return wav_neg(a); }
wav_f64x2_t neg_f64x2_o(wav_f64x2_t a) { return wav_neg(a); }

#if WAV_OPERATOR_OVERLOADING
wav_i8x16_t neg_i8x16_op(wav_i8x16_t a) { return -a; }
wav_i16x8_t neg_i16x8_op(wav_i16x8_t a) { return -a; }
wav_i32x4_t neg_i32x4_op(wav_i32x4_t a) { return -a; }
wav_i64x2_t neg_i64x2_op(wav_i64x2_t a) { return -a; }
wav_f32x4_t neg_f32x4_op(wav_f32x4_t a) { return -a; }
wav_f64x2_t neg_f64x2_op(wav_f64x2_t a) { return -a; }
#endif
