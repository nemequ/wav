#include "../wav.h"

wav_f32x4_t div_f32x4(wav_f32x4_t a, wav_f32x4_t b) { return wav_f32x4_div(a, b); }
wav_f64x2_t div_f64x2(wav_f64x2_t a, wav_f64x2_t b) { return wav_f64x2_div(a, b); }

wav_f32x4_t div_f32x4_o(wav_f32x4_t a, wav_f32x4_t b) { return wav_div(a, b); }
wav_f64x2_t div_f64x2_o(wav_f64x2_t a, wav_f64x2_t b) { return wav_div(a, b); }

#if WAV_OPERATOR_OVERLOADING
wav_f32x4_t div_f32x4_op(wav_f32x4_t a, wav_f32x4_t b) { return a / b; }
wav_f64x2_t div_f64x2_op(wav_f64x2_t a, wav_f64x2_t b) { return a / b; }
#endif
