#include "../wav.h"

wav_i8x16_t popcnt_i8x16(wav_i8x16_t a) { return wav_i8x16_popcnt(a); }
wav_u8x16_t popcnt_u8x16(wav_u8x16_t a) { return wav_u8x16_popcnt(a); }

wav_i8x16_t popcnt_i8x16_o(wav_i8x16_t a) { return wav_popcnt(a); }
wav_u8x16_t popcnt_u8x16_o(wav_u8x16_t a) { return wav_popcnt(a); }
