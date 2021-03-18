#include "../wav.h"

wav_i32x4_t load_zero_i32x4( int32_t * mem) { return wav_i32x4_load_zero(mem); }
wav_i64x2_t load_zero_i64x2( int64_t * mem) { return wav_i64x2_load_zero(mem); }
wav_u32x4_t load_zero_u32x4(uint32_t * mem) { return wav_u32x4_load_zero(mem); }
wav_u64x2_t load_zero_u64x2(uint64_t * mem) { return wav_u64x2_load_zero(mem); }
wav_f32x4_t load_zero_f32x4(   float * mem) { return wav_f32x4_load_zero(mem); }
wav_f64x2_t load_zero_f64x2(  double * mem) { return wav_f64x2_load_zero(mem); }

wav_i32x4_t load_zero_i32x4_o( int32_t * mem) { return wav_load_zero(mem); }
wav_i64x2_t load_zero_i64x2_o( int64_t * mem) { return wav_load_zero(mem); }
wav_u32x4_t load_zero_u32x4_o(uint32_t * mem) { return wav_load_zero(mem); }
wav_u64x2_t load_zero_u64x2_o(uint64_t * mem) { return wav_load_zero(mem); }
wav_f32x4_t load_zero_f32x4_o(   float * mem) { return wav_load_zero(mem); }
wav_f64x2_t load_zero_f64x2_o(  double * mem) { return wav_load_zero(mem); }
