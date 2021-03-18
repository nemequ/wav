#include "../wav.h"

#if defined(__wasm_unimplemented_simd128__)
wav_f32x4_t demote_f64x2(wav_f64x2_t a) { return wav_f64x2_demote(a); }
#endif

#if defined(__wasm_unimplemented_simd128__)
wav_f32x4_t demote_f64x2_o(wav_f64x2_t a) { return wav_demote(a); }
#endif
