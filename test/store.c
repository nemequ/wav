#include "../wav.h"

void store_i8x16(  int8_t * mem, wav_i8x16_t value) { wav_i8x16_store(mem, value); }
void store_i16x8( int16_t * mem, wav_i16x8_t value) { wav_i16x8_store(mem, value); }
void store_i32x4( int32_t * mem, wav_i32x4_t value) { wav_i32x4_store(mem, value); }
void store_i64x2( int64_t * mem, wav_i64x2_t value) { wav_i64x2_store(mem, value); }
void store_u8x16( uint8_t * mem, wav_u8x16_t value) { wav_u8x16_store(mem, value); }
void store_u16x8(uint16_t * mem, wav_u16x8_t value) { wav_u16x8_store(mem, value); }
void store_u32x4(uint32_t * mem, wav_u32x4_t value) { wav_u32x4_store(mem, value); }
void store_u64x2(uint64_t * mem, wav_u64x2_t value) { wav_u64x2_store(mem, value); }
void store_f32x4(   float * mem, wav_f32x4_t value) { wav_f32x4_store(mem, value); }
void store_f64x2(  double * mem, wav_f64x2_t value) { wav_f64x2_store(mem, value); }

void store_i8x16_o(  int8_t * mem, wav_i8x16_t value) { wav_store(mem, value); }
void store_i16x8_o( int16_t * mem, wav_i16x8_t value) { wav_store(mem, value); }
void store_i32x4_o( int32_t * mem, wav_i32x4_t value) { wav_store(mem, value); }
void store_i64x2_o( int64_t * mem, wav_i64x2_t value) { wav_store(mem, value); }
void store_u8x16_o( uint8_t * mem, wav_u8x16_t value) { wav_store(mem, value); }
void store_u16x8_o(uint16_t * mem, wav_u16x8_t value) { wav_store(mem, value); }
void store_u32x4_o(uint32_t * mem, wav_u32x4_t value) { wav_store(mem, value); }
void store_u64x2_o(uint64_t * mem, wav_u64x2_t value) { wav_store(mem, value); }
void store_f32x4_o(   float * mem, wav_f32x4_t value) { wav_store(mem, value); }
void store_f64x2_o(  double * mem, wav_f64x2_t value) { wav_store(mem, value); }
