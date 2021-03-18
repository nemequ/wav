#include "../wav.h"

#if defined(__wasm_unimplemented_simd128__)
bool any_true_i8x16(wav_i8x16_t a) { return wav_i8x16_any_true(a); }
bool any_true_i16x8(wav_i16x8_t a) { return wav_i16x8_any_true(a); }
bool any_true_i32x4(wav_i32x4_t a) { return wav_i32x4_any_true(a); }
bool any_true_i64x2(wav_i64x2_t a) { return wav_i64x2_any_true(a); }
bool any_true_u8x16(wav_u8x16_t a) { return wav_u8x16_any_true(a); }
bool any_true_u16x8(wav_u16x8_t a) { return wav_u16x8_any_true(a); }
bool any_true_u32x4(wav_u32x4_t a) { return wav_u32x4_any_true(a); }
bool any_true_u64x2(wav_u64x2_t a) { return wav_u64x2_any_true(a); }
bool any_true_b8x16(wav_b8x16_t a) { return wav_b8x16_any_true(a); }
bool any_true_b16x8(wav_b16x8_t a) { return wav_b16x8_any_true(a); }
bool any_true_b32x4(wav_b32x4_t a) { return wav_b32x4_any_true(a); }
bool any_true_b64x2(wav_b64x2_t a) { return wav_b64x2_any_true(a); }

bool any_true_i8x16_o(wav_i8x16_t a) { return wav_any_true(a); }
bool any_true_i16x8_o(wav_i16x8_t a) { return wav_any_true(a); }
bool any_true_i32x4_o(wav_i32x4_t a) { return wav_any_true(a); }
bool any_true_i64x2_o(wav_i64x2_t a) { return wav_any_true(a); }
bool any_true_u8x16_o(wav_u8x16_t a) { return wav_any_true(a); }
bool any_true_u16x8_o(wav_u16x8_t a) { return wav_any_true(a); }
bool any_true_u32x4_o(wav_u32x4_t a) { return wav_any_true(a); }
bool any_true_u64x2_o(wav_u64x2_t a) { return wav_any_true(a); }
bool any_true_b8x16_o(wav_b8x16_t a) { return wav_any_true(a); }
bool any_true_b16x8_o(wav_b16x8_t a) { return wav_any_true(a); }
bool any_true_b32x4_o(wav_b32x4_t a) { return wav_any_true(a); }
bool any_true_b64x2_o(wav_b64x2_t a) { return wav_any_true(a); }
#endif
