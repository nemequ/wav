#include "../wav.h"

wav_i8x16_t add_sat_i8x16(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_add_sat(a, b); }
wav_i16x8_t add_sat_i16x8(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_add_sat(a, b); }
wav_u8x16_t add_sat_u8x16(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_add_sat(a, b); }
wav_u16x8_t add_sat_u16x8(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_add_sat(a, b); }

wav_i8x16_t add_sat_i8x16_o(wav_i8x16_t a, wav_i8x16_t b) { return wav_add_sat(a, b); }
wav_i16x8_t add_sat_i16x8_o(wav_i16x8_t a, wav_i16x8_t b) { return wav_add_sat(a, b); }
wav_u8x16_t add_sat_u8x16_o(wav_u8x16_t a, wav_u8x16_t b) { return wav_add_sat(a, b); }
wav_u16x8_t add_sat_u16x8_o(wav_u16x8_t a, wav_u16x8_t b) { return wav_add_sat(a, b); }
