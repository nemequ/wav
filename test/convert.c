#include "../wav.h"

wav_f32x4_t i32x4_convert_f32x4(wav_i32x4_t a) { return wav_i32x4_convert_f32x4(a); }
wav_f32x4_t u32x4_convert_f32x4(wav_u32x4_t a) { return wav_u32x4_convert_f32x4(a); }

wav_f32x4_t i32x4_convert_f32x4_o(wav_i32x4_t a) { return wav_convert_f32x4(a); }
wav_f32x4_t u32x4_convert_f32x4_o(wav_u32x4_t a) { return wav_convert_f32x4(a); }
