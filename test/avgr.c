#include "../wav.h"

wav_u8x16_t avgr_u8x16(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_avgr(a, b); }
wav_u16x8_t avgr_u16x8(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_avgr(a, b); }

wav_u8x16_t avgr_u8x16_o(wav_u8x16_t a, wav_u8x16_t b) { return wav_avgr(a, b); }
wav_u16x8_t avgr_u16x8_o(wav_u16x8_t a, wav_u16x8_t b) { return wav_avgr(a, b); }

