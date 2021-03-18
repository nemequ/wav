#include "../wav.h"

wav_i8x16_t abs_i8x16(wav_i8x16_t a) { return wav_i8x16_abs(a); }
wav_i16x8_t abs_i16x8(wav_i16x8_t a) { return wav_i16x8_abs(a); }
wav_i32x4_t abs_i32x4(wav_i32x4_t a) { return wav_i32x4_abs(a); }
#if defined(__wasm_unimplemented_simd128__)
wav_i64x2_t abs_i64x2(wav_i64x2_t a) { return wav_i64x2_abs(a); }
#endif
wav_f32x4_t abs_f32x4(wav_f32x4_t a) { return wav_f32x4_abs(a); }
wav_f64x2_t abs_f64x2(wav_f64x2_t a) { return wav_f64x2_abs(a); }

wav_i8x16_t abs_i8x16_o(wav_i8x16_t a) { return wav_abs(a); }
wav_i16x8_t abs_i16x8_o(wav_i16x8_t a) { return wav_abs(a); }
wav_i32x4_t abs_i32x4_o(wav_i32x4_t a) { return wav_abs(a); }
#if defined(__wasm_unimplemented_simd128__)
wav_i64x2_t abs_i64x2_o(wav_i64x2_t a) { return wav_abs(a); }
#endif
wav_f32x4_t abs_f32x4_o(wav_f32x4_t a) { return wav_abs(a); }
wav_f64x2_t abs_f64x2_o(wav_f64x2_t a) { return wav_abs(a); }
