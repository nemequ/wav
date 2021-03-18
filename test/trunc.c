#include "../wav.h"

wav_f32x4_t trunc_f32x4(wav_f32x4_t a) { return wav_f32x4_trunc(a); }
wav_f64x2_t trunc_f64x2(wav_f64x2_t a) { return wav_f64x2_trunc(a); }

wav_f32x4_t trunc_f32x4_o(wav_f32x4_t a) { return wav_trunc(a); }
wav_f64x2_t trunc_f64x2_o(wav_f64x2_t a) { return wav_trunc(a); }
