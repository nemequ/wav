#include "../wav.h"

wav_i32x4_t dot_i16x8(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_dot(a, b); }

wav_i32x4_t dot_i8x16_o(wav_i16x8_t a, wav_i16x8_t b) { return wav_dot(a, b); }
