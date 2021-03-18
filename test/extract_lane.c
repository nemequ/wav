#include "../wav.h"

  int8_t extract_lane_i8x16(wav_i8x16_t v) { return wav_i8x16_extract_lane(v, 15); }
 int16_t extract_lane_i16x8(wav_i16x8_t v) { return wav_i16x8_extract_lane(v,  7); }
 int32_t extract_lane_i32x4(wav_i32x4_t v) { return wav_i32x4_extract_lane(v,  3); }
 int64_t extract_lane_i64x2(wav_i64x2_t v) { return wav_i64x2_extract_lane(v,  1); }
 uint8_t extract_lane_u8x16(wav_u8x16_t v) { return wav_u8x16_extract_lane(v, 15); }
uint16_t extract_lane_u16x8(wav_u16x8_t v) { return wav_u16x8_extract_lane(v,  7); }
uint32_t extract_lane_u32x4(wav_u32x4_t v) { return wav_u32x4_extract_lane(v,  3); }
uint64_t extract_lane_u64x2(wav_u64x2_t v) { return wav_u64x2_extract_lane(v,  1); }
   float extract_lane_f32x4(wav_f32x4_t v) { return wav_f32x4_extract_lane(v,  3); }
  double extract_lane_f64x2(wav_f64x2_t v) { return wav_f64x2_extract_lane(v,  1); }

  int8_t extract_lane_i8x16_o(wav_i8x16_t v) { return wav_extract_lane(v, 15); }
 int16_t extract_lane_i16x8_o(wav_i16x8_t v) { return wav_extract_lane(v,  7); }
 int32_t extract_lane_i32x4_o(wav_i32x4_t v) { return wav_extract_lane(v,  3); }
 int64_t extract_lane_i64x2_o(wav_i64x2_t v) { return wav_extract_lane(v,  1); }
 uint8_t extract_lane_u8x16_o(wav_u8x16_t v) { return wav_extract_lane(v, 15); }
uint16_t extract_lane_u16x8_o(wav_u16x8_t v) { return wav_extract_lane(v,  7); }
uint32_t extract_lane_u32x4_o(wav_u32x4_t v) { return wav_extract_lane(v,  3); }
uint64_t extract_lane_u64x2_o(wav_u64x2_t v) { return wav_extract_lane(v,  1); }
   float extract_lane_f32x4_o(wav_f32x4_t v) { return wav_extract_lane(v,  3); }
  double extract_lane_f64x2_o(wav_f64x2_t v) { return wav_extract_lane(v,  1); }
