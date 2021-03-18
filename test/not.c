#include "../wav.h"

wav_i8x16_t not_i8x16(wav_i8x16_t a) { return wav_i8x16_not(a); }
wav_i16x8_t not_i16x8(wav_i16x8_t a) { return wav_i16x8_not(a); }
wav_i32x4_t not_i32x4(wav_i32x4_t a) { return wav_i32x4_not(a); }
wav_i64x2_t not_i64x2(wav_i64x2_t a) { return wav_i64x2_not(a); }
wav_u8x16_t not_u8x16(wav_u8x16_t a) { return wav_u8x16_not(a); }
wav_u16x8_t not_u16x8(wav_u16x8_t a) { return wav_u16x8_not(a); }
wav_u32x4_t not_u32x4(wav_u32x4_t a) { return wav_u32x4_not(a); }
wav_u64x2_t not_u64x2(wav_u64x2_t a) { return wav_u64x2_not(a); }
wav_b8x16_t not_b8x16(wav_b8x16_t a) { return wav_b8x16_not(a); }
wav_b16x8_t not_b16x8(wav_b16x8_t a) { return wav_b16x8_not(a); }
wav_b32x4_t not_b32x4(wav_b32x4_t a) { return wav_b32x4_not(a); }
wav_b64x2_t not_b64x2(wav_b64x2_t a) { return wav_b64x2_not(a); }

#if WAV_OPERATOR_OVERLOADING
wav_i8x16_t not_i8x16_op(wav_i8x16_t a) { return ~a; }
wav_i16x8_t not_i16x8_op(wav_i16x8_t a) { return ~a; }
wav_i32x4_t not_i32x4_op(wav_i32x4_t a) { return ~a; }
wav_i64x2_t not_i64x2_op(wav_i64x2_t a) { return ~a; }
wav_u8x16_t not_u8x16_op(wav_u8x16_t a) { return ~a; }
wav_u16x8_t not_u16x8_op(wav_u16x8_t a) { return ~a; }
wav_u32x4_t not_u32x4_op(wav_u32x4_t a) { return ~a; }
wav_u64x2_t not_u64x2_op(wav_u64x2_t a) { return ~a; }
wav_b8x16_t not_b8x16_op(wav_b8x16_t a) { return ~a; }
wav_b16x8_t not_b16x8_op(wav_b16x8_t a) { return ~a; }
wav_b32x4_t not_b32x4_op(wav_b32x4_t a) { return ~a; }
wav_b64x2_t not_b64x2_op(wav_b64x2_t a) { return ~a; }
#endif
