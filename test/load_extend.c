#include "../wav.h"

wav_i16x8_t load_extend_i16x8(  int8_t * mem) { return wav_i16x8_load_extend(mem); }
wav_i32x4_t load_extend_i32x4( int16_t * mem) { return wav_i32x4_load_extend(mem); }
wav_i64x2_t load_extend_i64x2( int32_t * mem) { return wav_i64x2_load_extend(mem); }
wav_u16x8_t load_extend_u16x8( uint8_t * mem) { return wav_u16x8_load_extend(mem); }
wav_u32x4_t load_extend_u32x4(uint16_t * mem) { return wav_u32x4_load_extend(mem); }
wav_u64x2_t load_extend_u64x2(uint32_t * mem) { return wav_u64x2_load_extend(mem); }

wav_i16x8_t load_extend_i16x8_o(  int8_t * mem) { return wav_load_extend(mem); }
wav_i32x4_t load_extend_i32x4_o( int16_t * mem) { return wav_load_extend(mem); }
wav_i64x2_t load_extend_i64x2_o( int32_t * mem) { return wav_load_extend(mem); }
wav_u16x8_t load_extend_u16x8_o( uint8_t * mem) { return wav_load_extend(mem); }
wav_u32x4_t load_extend_u32x4_o(uint16_t * mem) { return wav_load_extend(mem); }
wav_u64x2_t load_extend_u64x2_o(uint32_t * mem) { return wav_load_extend(mem); }
