#include "../wav.h"

wav_i8x16_t load_i8x16(  int8_t * mem) { return wav_i8x16_load(mem); }
wav_i16x8_t load_i16x8( int16_t * mem) { return wav_i16x8_load(mem); }
wav_i32x4_t load_i32x4( int32_t * mem) { return wav_i32x4_load(mem); }
wav_i64x2_t load_i64x2( int64_t * mem) { return wav_i64x2_load(mem); }
wav_u8x16_t load_u8x16( uint8_t * mem) { return wav_u8x16_load(mem); }
wav_u16x8_t load_u16x8(uint16_t * mem) { return wav_u16x8_load(mem); }
wav_u32x4_t load_u32x4(uint32_t * mem) { return wav_u32x4_load(mem); }
wav_u64x2_t load_u64x2(uint64_t * mem) { return wav_u64x2_load(mem); }
wav_f32x4_t load_f32x4(   float * mem) { return wav_f32x4_load(mem); }
wav_f64x2_t load_f64x2(  double * mem) { return wav_f64x2_load(mem); }

wav_i8x16_t load_i8x16_o(  int8_t * mem) { return wav_load(mem); }
wav_i16x8_t load_i16x8_o( int16_t * mem) { return wav_load(mem); }
wav_i32x4_t load_i32x4_o( int32_t * mem) { return wav_load(mem); }
wav_i64x2_t load_i64x2_o( int64_t * mem) { return wav_load(mem); }
wav_u8x16_t load_u8x16_o( uint8_t * mem) { return wav_load(mem); }
wav_u16x8_t load_u16x8_o(uint16_t * mem) { return wav_load(mem); }
wav_u32x4_t load_u32x4_o(uint32_t * mem) { return wav_load(mem); }
wav_u64x2_t load_u64x2_o(uint64_t * mem) { return wav_load(mem); }
wav_f32x4_t load_f32x4_o(   float * mem) { return wav_load(mem); }
wav_f64x2_t load_f64x2_o(  double * mem) { return wav_load(mem); }
