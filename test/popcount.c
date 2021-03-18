#include "../wav.h"

#if defined(__wasm_unimplemented_simd128__)
wav_i8x16_t popcount_i8x16(wav_i8x16_t a) { return wav_i8x16_popcount(a); }
wav_u8x16_t popcount_u8x16(wav_u8x16_t a) { return wav_u8x16_popcount(a); }

wav_i8x16_t popcount_i8x16_o(wav_i8x16_t a) { return wav_popcount(a); }
wav_u8x16_t popcount_u8x16_o(wav_u8x16_t a) { return wav_popcount(a); }
#endif
