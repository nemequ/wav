#include "../wav.h"

wav_i8x16_t add_i16x8(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_narrow(a, b); }
wav_i16x8_t add_i32x4(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_narrow(a, b); }
wav_u8x16_t add_u16x8(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_narrow(a, b); }
wav_u16x8_t add_u32x4(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_narrow(a, b); }


wav_i8x16_t add_i16x8_op(wav_i16x8_t a, wav_i16x8_t b) { return wav_narrow(a, b); }
wav_i16x8_t add_i32x4_op(wav_i32x4_t a, wav_i32x4_t b) { return wav_narrow(a, b); }
wav_u8x16_t add_u16x8_op(wav_u16x8_t a, wav_u16x8_t b) { return wav_narrow(a, b); }
wav_u16x8_t add_u32x4_op(wav_u32x4_t a, wav_u32x4_t b) { return wav_narrow(a, b); }
