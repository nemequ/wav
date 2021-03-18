#include "../wav.h"

wav_i8x16_t loada_i8x16(  int8_t * mem) { return wav_i8x16_loada(mem); }
wav_i16x8_t loada_i16x8( int16_t * mem) { return wav_i16x8_loada(mem); }
wav_i32x4_t loada_i32x4( int32_t * mem) { return wav_i32x4_loada(mem); }
wav_i64x2_t loada_i64x2( int64_t * mem) { return wav_i64x2_loada(mem); }
wav_u8x16_t loada_u8x16( uint8_t * mem) { return wav_u8x16_loada(mem); }
wav_u16x8_t loada_u16x8(uint16_t * mem) { return wav_u16x8_loada(mem); }
wav_u32x4_t loada_u32x4(uint32_t * mem) { return wav_u32x4_loada(mem); }
wav_u64x2_t loada_u64x2(uint64_t * mem) { return wav_u64x2_loada(mem); }
wav_f32x4_t loada_f32x4(   float * mem) { return wav_f32x4_loada(mem); }
wav_f64x2_t loada_f64x2(  double * mem) { return wav_f64x2_loada(mem); }
