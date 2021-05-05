#include "../wav.h"

wav_i32x4_t f32x4_trunc_sat_i32x4(wav_f32x4_t a) { return wav_f32x4_trunc_sat_i32x4(a); }
wav_u32x4_t f32x4_trunc_sat_u32x4(wav_f32x4_t a) { return wav_f32x4_trunc_sat_u32x4(a); }

wav_i32x4_t f32x4_trunc_sat_i32x4_o(wav_f32x4_t a) { return wav_trunc_sat_i32x4(a); }
wav_u32x4_t f32x4_trunc_sat_u32x4_o(wav_f32x4_t a) { return wav_trunc_sat_u32x4(a); }


wav_i32x4_t f64x2_trunc_sat_zero_i32x4(wav_f64x2_t a) { return wav_f64x2_trunc_sat_zero_i32x4(a); }
wav_u32x4_t f64x2_trunc_sat_zero_u32x4(wav_f64x2_t a) { return wav_f64x2_trunc_sat_zero_u32x4(a); }

wav_i32x4_t f64x2_trunc_sat_zero_i32x4_o(wav_f64x2_t a) { return wav_trunc_sat_zero_i32x4(a); }
wav_u32x4_t f64x2_trunc_sat_zero_u32x4_o(wav_f64x2_t a) { return wav_trunc_sat_zero_u32x4(a); }
