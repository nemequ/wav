#include "../wav.h"

#if defined(__wasm_unimplemented_simd128__)
wav_i16x8_t extadd_pairwise_i8x16(wav_i8x16_t a, wav_i8x16_t b) { return wav_i8x16_extadd_pairwise(a, b); }
wav_i32x4_t extadd_pairwise_i16x8(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_extadd_pairwise(a, b); }
wav_u16x8_t extadd_pairwise_u8x16(wav_u8x16_t a, wav_u8x16_t b) { return wav_u8x16_extadd_pairwise(a, b); }
wav_u32x4_t extadd_pairwise_u16x8(wav_u16x8_t a, wav_u16x8_t b) { return wav_u16x8_extadd_pairwise(a, b); }

wav_i16x8_t extadd_pairwise_i8x16_o(wav_i8x16_t a, wav_i8x16_t b) { return wav_extadd_pairwise(a, b); }
wav_i32x4_t extadd_pairwise_i16x8_o(wav_i16x8_t a, wav_i16x8_t b) { return wav_extadd_pairwise(a, b); }
wav_u16x8_t extadd_pairwise_u8x16_o(wav_u8x16_t a, wav_u8x16_t b) { return wav_extadd_pairwise(a, b); }
wav_u32x4_t extadd_pairwise_u16x8_o(wav_u16x8_t a, wav_u16x8_t b) { return wav_extadd_pairwise(a, b); }
#endif
