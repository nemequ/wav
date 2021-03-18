#include "../wav.h"

void storeu_i8x16(  int8_t * mem, wav_i8x16_t value) { wav_i8x16_storeu(mem, value); }
void storeu_i16x8( int16_t * mem, wav_i16x8_t value) { wav_i16x8_storeu(mem, value); }
void storeu_i32x4( int32_t * mem, wav_i32x4_t value) { wav_i32x4_storeu(mem, value); }
void storeu_i64x2( int64_t * mem, wav_i64x2_t value) { wav_i64x2_storeu(mem, value); }
void storeu_u8x16( uint8_t * mem, wav_u8x16_t value) { wav_u8x16_storeu(mem, value); }
void storeu_u16x8(uint16_t * mem, wav_u16x8_t value) { wav_u16x8_storeu(mem, value); }
void storeu_u32x4(uint32_t * mem, wav_u32x4_t value) { wav_u32x4_storeu(mem, value); }
void storeu_u64x2(uint64_t * mem, wav_u64x2_t value) { wav_u64x2_storeu(mem, value); }
void storeu_f32x4(   float * mem, wav_f32x4_t value) { wav_f32x4_storeu(mem, value); }
void storeu_f64x2(  double * mem, wav_f64x2_t value) { wav_f64x2_storeu(mem, value); }
