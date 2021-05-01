#include "../wav.h"

void store_lane_i8x16(wav_i8x16_t v,   int8_t * value) { wav_i8x16_store_lane(v, 15, value); }
void store_lane_i16x8(wav_i16x8_t v,  int16_t * value) { wav_i16x8_store_lane(v,  7, value); }
void store_lane_i32x4(wav_i32x4_t v,  int32_t * value) { wav_i32x4_store_lane(v,  3, value); }
void store_lane_i64x2(wav_i64x2_t v,  int64_t * value) { wav_i64x2_store_lane(v,  1, value); }
void store_lane_u8x16(wav_u8x16_t v,  uint8_t * value) { wav_u8x16_store_lane(v, 15, value); }
void store_lane_u16x8(wav_u16x8_t v, uint16_t * value) { wav_u16x8_store_lane(v,  7, value); }
void store_lane_u32x4(wav_u32x4_t v, uint32_t * value) { wav_u32x4_store_lane(v,  3, value); }
void store_lane_u64x2(wav_u64x2_t v, uint64_t * value) { wav_u64x2_store_lane(v,  1, value); }
void store_lane_f32x4(wav_f32x4_t v,    float * value) { wav_f32x4_store_lane(v,  3, value); }
void store_lane_f64x2(wav_f64x2_t v,   double * value) { wav_f64x2_store_lane(v,  1, value); }

#if defined(__wasm_unimplemented_simd128__)
void store_lane_i8x16_o(wav_i8x16_t v,   int8_t * value) { wav_store_lane(v, 15, &value); }
void store_lane_i16x8_o(wav_i16x8_t v,  int16_t * value) { wav_store_lane(v,  7, &value); }
void store_lane_i32x4_o(wav_i32x4_t v,  int32_t * value) { wav_store_lane(v,  3, &value); }
void store_lane_i64x2_o(wav_i64x2_t v,  int64_t * value) { wav_store_lane(v,  1, &value); }
void store_lane_u8x16_o(wav_u8x16_t v,  uint8_t * value) { wav_store_lane(v, 15, &value); }
void store_lane_u16x8_o(wav_u16x8_t v, uint16_t * value) { wav_store_lane(v,  7, &value); }
void store_lane_u32x4_o(wav_u32x4_t v, uint32_t * value) { wav_store_lane(v,  3, &value); }
void store_lane_u64x2_o(wav_u64x2_t v, uint64_t * value) { wav_store_lane(v,  1, &value); }
void store_lane_f32x4_o(wav_f32x4_t v,    float * value) { wav_store_lane(v,  3, &value); }
void store_lane_f64x2_o(wav_f64x2_t v,   double * value) { wav_store_lane(v,  1, &value); }
#endif
