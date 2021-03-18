#include "../wav.h"

wav_i32x4_t extend_low_i16x8(wav_i16x8_t a) { return wav_i16x8_extend_low(a); }
#if defined(__wasm_unimplemented_simd128__)
wav_i64x2_t extend_low_i32x4(wav_i32x4_t a) { return wav_i32x4_extend_low(a); }
#endif
wav_u16x8_t extend_low_u8x16(wav_u8x16_t a) { return wav_u8x16_extend_low(a); }
wav_u32x4_t extend_low_u16x8(wav_u16x8_t a) { return wav_u16x8_extend_low(a); }
wav_i16x8_t extend_low_i8x16(wav_i8x16_t a) { return wav_i8x16_extend_low(a); }
#if defined(__wasm_unimplemented_simd128__)
wav_u64x2_t extend_low_u32x4(wav_u32x4_t a) { return wav_u32x4_extend_low(a); }
#endif

wav_i32x4_t extend_low_i16x8_o(wav_i16x8_t a) { return wav_extend_low(a); }
#if defined(__wasm_unimplemented_simd128__)
wav_i64x2_t extend_low_i32x4_o(wav_i32x4_t a) { return wav_extend_low(a); }
#endif
wav_u16x8_t extend_low_u8x16_o(wav_u8x16_t a) { return wav_extend_low(a); }
wav_u32x4_t extend_low_u16x8_o(wav_u16x8_t a) { return wav_extend_low(a); }
wav_i16x8_t extend_low_i8x16_o(wav_i8x16_t a) { return wav_extend_low(a); }
#if defined(__wasm_unimplemented_simd128__)
wav_u64x2_t extend_low_u32x4_o(wav_u32x4_t a) { return wav_extend_low(a); }
#endif
