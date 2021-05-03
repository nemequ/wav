#include "../wav.h"

wav_i8x16_t loadu_i8x16(const   int8_t * mem) { return wav_i8x16_loadu(mem); }
wav_i16x8_t loadu_i16x8(const  int16_t * mem) { return wav_i16x8_loadu(mem); }
wav_i32x4_t loadu_i32x4(const  int32_t * mem) { return wav_i32x4_loadu(mem); }
wav_i64x2_t loadu_i64x2(const  int64_t * mem) { return wav_i64x2_loadu(mem); }
wav_u8x16_t loadu_u8x16(const  uint8_t * mem) { return wav_u8x16_loadu(mem); }
wav_u16x8_t loadu_u16x8(const uint16_t * mem) { return wav_u16x8_loadu(mem); }
wav_u32x4_t loadu_u32x4(const uint32_t * mem) { return wav_u32x4_loadu(mem); }
wav_u64x2_t loadu_u64x2(const uint64_t * mem) { return wav_u64x2_loadu(mem); }
wav_f32x4_t loadu_f32x4(const    float * mem) { return wav_f32x4_loadu(mem); }
wav_f64x2_t loadu_f64x2(const   double * mem) { return wav_f64x2_loadu(mem); }
