#include "../wav.h"

wav_i8x16_t narrow_i8x16(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_narrow(a, b); }
wav_i16x8_t narrow_i16x8(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_narrow(a, b); }
wav_u8x16_t narrow_u_i8x16(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_narrow_u(a, b); }
wav_u16x8_t narrow_u_i16x8(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_narrow_u(a, b); }

wav_i8x16_t narrow_i8x16_o(wav_i16x8_t a, wav_i16x8_t b) { return wav_narrow(a, b); }
wav_i16x8_t narrow_i16x8_o(wav_i32x4_t a, wav_i32x4_t b) { return wav_narrow(a, b); }
wav_u8x16_t narrow_u_i8x16_o(wav_i16x8_t a, wav_i16x8_t b) { return wav_narrow_u(a, b); }
wav_u16x8_t narrow_u_i16x8_o(wav_i32x4_t a, wav_i32x4_t b) { return wav_narrow_u(a, b); }
