#include "../wav.h"

wav_i8x16_t swizzle_i8x16(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_swizzle(a, b); }

wav_i8x16_t swizzle_i8x16_o(wav_i8x16_t a, wav_i8x16_t b) { return wav_swizzle(a, b); }
