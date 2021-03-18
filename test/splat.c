#include "../wav.h"

wav_i8x16_t splat_i8x16(  int8_t value) { return wav_i8x16_splat(value); }
wav_i16x8_t splat_i16x8( int16_t value) { return wav_i16x8_splat(value); }
wav_i32x4_t splat_i32x4( int32_t value) { return wav_i32x4_splat(value); }
wav_i64x2_t splat_i64x2( int64_t value) { return wav_i64x2_splat(value); }
wav_u8x16_t splat_u8x16( uint8_t value) { return wav_u8x16_splat(value); }
wav_u16x8_t splat_u16x8(uint16_t value) { return wav_u16x8_splat(value); }
wav_u32x4_t splat_u32x4(uint32_t value) { return wav_u32x4_splat(value); }
wav_u64x2_t splat_u64x2(uint64_t value) { return wav_u64x2_splat(value); }
wav_f32x4_t splat_f32x4(   float value) { return wav_f32x4_splat(value); }
wav_f64x2_t splat_f64x2(  double value) { return wav_f64x2_splat(value); }

wav_i8x16_t splat_i8x16_o(  int8_t value) { return wav_splat(value); }
wav_i16x8_t splat_i16x8_o( int16_t value) { return wav_splat(value); }
wav_i32x4_t splat_i32x4_o( int32_t value) { return wav_splat(value); }
wav_i64x2_t splat_i64x2_o( int64_t value) { return wav_splat(value); }
wav_u8x16_t splat_u8x16_o( uint8_t value) { return wav_splat(value); }
wav_u16x8_t splat_u16x8_o(uint16_t value) { return wav_splat(value); }
wav_u32x4_t splat_u32x4_o(uint32_t value) { return wav_splat(value); }
wav_u64x2_t splat_u64x2_o(uint64_t value) { return wav_splat(value); }
wav_f32x4_t splat_f32x4_o(   float value) { return wav_splat(value); }
wav_f64x2_t splat_f64x2_o(  double value) { return wav_splat(value); }
