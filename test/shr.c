#include "../wav.h"

wav_i8x16_t shr_i8x16(wav_i8x16_t a, int count) { return wav_i8x16_shr(a, count); }
wav_i16x8_t shr_i16x8(wav_i16x8_t a, int count) { return wav_i16x8_shr(a, count); }
wav_i32x4_t shr_i32x4(wav_i32x4_t a, int count) { return wav_i32x4_shr(a, count); }
wav_i64x2_t shr_i64x2(wav_i64x2_t a, int count) { return wav_i64x2_shr(a, count); }
wav_u8x16_t shr_u8x16(wav_u8x16_t a, int count) { return wav_u8x16_shr(a, count); }
wav_u16x8_t shr_u16x8(wav_u16x8_t a, int count) { return wav_u16x8_shr(a, count); }
wav_u32x4_t shr_u32x4(wav_u32x4_t a, int count) { return wav_u32x4_shr(a, count); }
wav_u64x2_t shr_u64x2(wav_u64x2_t a, int count) { return wav_u64x2_shr(a, count); }

wav_i8x16_t shr_i8x16_o(wav_i8x16_t a, int count) { return wav_shr(a, count); }
wav_i16x8_t shr_i16x8_o(wav_i16x8_t a, int count) { return wav_shr(a, count); }
wav_i32x4_t shr_i32x4_o(wav_i32x4_t a, int count) { return wav_shr(a, count); }
wav_i64x2_t shr_i64x2_o(wav_i64x2_t a, int count) { return wav_shr(a, count); }
wav_u8x16_t shr_u8x16_o(wav_u8x16_t a, int count) { return wav_shr(a, count); }
wav_u16x8_t shr_u16x8_o(wav_u16x8_t a, int count) { return wav_shr(a, count); }
wav_u32x4_t shr_u32x4_o(wav_u32x4_t a, int count) { return wav_shr(a, count); }
wav_u64x2_t shr_u64x2_o(wav_u64x2_t a, int count) { return wav_shr(a, count); }

#if WAV_OPERATOR_OVERLOADING
wav_i8x16_t shr_i8x16_op(wav_i8x16_t a, int count) { return a << count; }
wav_i16x8_t shr_i16x8_op(wav_i16x8_t a, int count) { return a << count; }
wav_i32x4_t shr_i32x4_op(wav_i32x4_t a, int count) { return a << count; }
wav_i64x2_t shr_i64x2_op(wav_i64x2_t a, int count) { return a << count; }
wav_u8x16_t shr_u8x16_op(wav_u8x16_t a, int count) { return a << count; }
wav_u16x8_t shr_u16x8_op(wav_u16x8_t a, int count) { return a << count; }
wav_u32x4_t shr_u32x4_op(wav_u32x4_t a, int count) { return a << count; }
wav_u64x2_t shr_u64x2_op(wav_u64x2_t a, int count) { return a << count; }
#endif
