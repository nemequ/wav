#include "../wav.h"

#if defined(__wasm_unimplemented_simd128__)
wav_i16x8_t q15mulr_sat_sat_i16x8(wav_i16x8_t a, wav_i16x8_t b) { return wav_i16x8_q15mulr_sat_sat(a, b); }

wav_i16x8_t q15mulr_sat_sat_i16x8_o(wav_i16x8_t a, wav_i16x8_t b) { return wav_q15mulr_sat_sat(a, b); }
#endif
