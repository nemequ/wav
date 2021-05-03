#include "../wav.h"

wav_i8x16_t load_i8x16(const   int8_t * mem) { return wav_i8x16_load(mem); }
wav_i16x8_t load_i16x8(const  int16_t * mem) { return wav_i16x8_load(mem); }
wav_i32x4_t load_i32x4(const  int32_t * mem) { return wav_i32x4_load(mem); }
wav_i64x2_t load_i64x2(const  int64_t * mem) { return wav_i64x2_load(mem); }
wav_u8x16_t load_u8x16(const  uint8_t * mem) { return wav_u8x16_load(mem); }
wav_u16x8_t load_u16x8(const uint16_t * mem) { return wav_u16x8_load(mem); }
wav_u32x4_t load_u32x4(const uint32_t * mem) { return wav_u32x4_load(mem); }
wav_u64x2_t load_u64x2(const uint64_t * mem) { return wav_u64x2_load(mem); }
wav_f32x4_t load_f32x4(const    float * mem) { return wav_f32x4_load(mem); }
wav_f64x2_t load_f64x2(const   double * mem) { return wav_f64x2_load(mem); }

wav_i8x16_t load_i8x16_o(const   int8_t * mem) { return wav_load(mem); }
wav_i16x8_t load_i16x8_o(const  int16_t * mem) { return wav_load(mem); }
wav_i32x4_t load_i32x4_o(const  int32_t * mem) { return wav_load(mem); }
wav_i64x2_t load_i64x2_o(const  int64_t * mem) { return wav_load(mem); }
wav_u8x16_t load_u8x16_o(const  uint8_t * mem) { return wav_load(mem); }
wav_u16x8_t load_u16x8_o(const uint16_t * mem) { return wav_load(mem); }
wav_u32x4_t load_u32x4_o(const uint32_t * mem) { return wav_load(mem); }
wav_u64x2_t load_u64x2_o(const uint64_t * mem) { return wav_load(mem); }
wav_f32x4_t load_f32x4_o(const    float * mem) { return wav_load(mem); }
wav_f64x2_t load_f64x2_o(const   double * mem) { return wav_load(mem); }
