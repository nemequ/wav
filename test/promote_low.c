#include "../wav.h"

#if defined(__wasm_unimplemented_simd128__)
wav_u64x2_t promote_low_u32x4(wav_u32x4_t a) { return wav_u32x4_promote_low(a); }
#endif

#if defined(__wasm_unimplemented_simd128__)
wav_u64x2_t promote_low_u32x4_o(wav_u32x4_t a) { return wav_promote_low(a); }
#endif
