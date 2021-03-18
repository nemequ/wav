#include "../wav.h"

#if defined(__wasm_unimplemented_simd128__)
wav_i16x8_t extmul_low_i8x16(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_extmul_low(a, b); }
wav_i32x4_t extmul_low_i16x8(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_extmul_low(a, b); }
wav_i64x2_t extmul_low_i32x4(wav_i32x4_t a, wav_i32x4_t b) { return wav_i32x4_extmul_low(a, b); }
wav_u16x8_t extmul_low_u8x16(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_extmul_low(a, b); }
wav_u32x4_t extmul_low_u16x8(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_extmul_low(a, b); }
wav_u64x2_t extmul_low_u32x4(wav_u32x4_t a, wav_u32x4_t b) { return wav_u32x4_extmul_low(a, b); }

wav_i16x8_t extmul_low_i8x16_o(wav_i8x16_t a, wav_i8x16_t b) { return wav_extmul_low(a, b); }
wav_i32x4_t extmul_low_i16x8_o(wav_i16x8_t a, wav_i16x8_t b) { return wav_extmul_low(a, b); }
wav_i64x2_t extmul_low_i32x4_o(wav_i32x4_t a, wav_i32x4_t b) { return wav_extmul_low(a, b); }
wav_u16x8_t extmul_low_u8x16_o(wav_u8x16_t a, wav_u8x16_t b) { return wav_extmul_low(a, b); }
wav_u32x4_t extmul_low_u16x8_o(wav_u16x8_t a, wav_u16x8_t b) { return wav_extmul_low(a, b); }
wav_u64x2_t extmul_low_u32x4_o(wav_u32x4_t a, wav_u32x4_t b) { return wav_extmul_low(a, b); }
#endif
