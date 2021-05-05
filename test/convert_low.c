#include "../wav.h"

wav_f64x2_t i32x4_convert_low_f64x2(wav_i32x4_t a) { return wav_i32x4_convert_low_f64x2(a); }
wav_f64x2_t u32x4_convert_low_f64x2(wav_u32x4_t a) { return wav_u32x4_convert_low_f64x2(a); }

wav_f64x2_t i32x4_convert_low_f64x2_o(wav_i32x4_t a) { return wav_convert_low_f64x2(a); }
wav_f64x2_t u32x4_convert_low_f64x2_o(wav_u32x4_t a) { return wav_convert_low_f64x2(a); }
