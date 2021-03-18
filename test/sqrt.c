#include "../wav.h"

wav_f32x4_t sqrt_f32x4(wav_f32x4_t a) { return wav_f32x4_sqrt(a); }
wav_f64x2_t sqrt_f64x2(wav_f64x2_t a) { return wav_f64x2_sqrt(a); }

wav_f32x4_t sqrt_f32x4_o(wav_f32x4_t a) { return wav_sqrt(a); }
wav_f64x2_t sqrt_f64x2_o(wav_f64x2_t a) { return wav_sqrt(a); }
