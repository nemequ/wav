#include "../wav.h"

#if defined(__wasm_unimplemented_simd128__)
wav_i8x16_t load_lane_i8x16(wav_i8x16_t v,   int8_t value) { return wav_i8x16_load_lane(v, 15, &value); }
wav_i16x8_t load_lane_i16x8(wav_i16x8_t v,  int16_t value) { return wav_i16x8_load_lane(v,  7, &value); }
wav_i32x4_t load_lane_i32x4(wav_i32x4_t v,  int32_t value) { return wav_i32x4_load_lane(v,  3, &value); }
wav_i64x2_t load_lane_i64x2(wav_i64x2_t v,  int64_t value) { return wav_i64x2_load_lane(v,  1, &value); }
wav_u8x16_t load_lane_u8x16(wav_u8x16_t v,  uint8_t value) { return wav_u8x16_load_lane(v, 15, &value); }
wav_u16x8_t load_lane_u16x8(wav_u16x8_t v, uint16_t value) { return wav_u16x8_load_lane(v,  7, &value); }
wav_u32x4_t load_lane_u32x4(wav_u32x4_t v, uint32_t value) { return wav_u32x4_load_lane(v,  3, &value); }
wav_u64x2_t load_lane_u64x2(wav_u64x2_t v, uint64_t value) { return wav_u64x2_load_lane(v,  1, &value); }
wav_f32x4_t load_lane_f32x4(wav_f32x4_t v,    float value) { return wav_f32x4_load_lane(v,  3, &value); }
wav_f64x2_t load_lane_f64x2(wav_f64x2_t v,   double value) { return wav_f64x2_load_lane(v,  1, &value); }

wav_i8x16_t load_lane_i8x16_o(wav_i8x16_t v,   int8_t value) { return wav_load_lane(v, 15, &value); }
wav_i16x8_t load_lane_i16x8_o(wav_i16x8_t v,  int16_t value) { return wav_load_lane(v,  7, &value); }
wav_i32x4_t load_lane_i32x4_o(wav_i32x4_t v,  int32_t value) { return wav_load_lane(v,  3, &value); }
wav_i64x2_t load_lane_i64x2_o(wav_i64x2_t v,  int64_t value) { return wav_load_lane(v,  1, &value); }
wav_u8x16_t load_lane_u8x16_o(wav_u8x16_t v,  uint8_t value) { return wav_load_lane(v, 15, &value); }
wav_u16x8_t load_lane_u16x8_o(wav_u16x8_t v, uint16_t value) { return wav_load_lane(v,  7, &value); }
wav_u32x4_t load_lane_u32x4_o(wav_u32x4_t v, uint32_t value) { return wav_load_lane(v,  3, &value); }
wav_u64x2_t load_lane_u64x2_o(wav_u64x2_t v, uint64_t value) { return wav_load_lane(v,  1, &value); }
wav_f32x4_t load_lane_f32x4_o(wav_f32x4_t v,    float value) { return wav_load_lane(v,  3, &value); }
wav_f64x2_t load_lane_f64x2_o(wav_f64x2_t v,   double value) { return wav_load_lane(v,  1, &value); }
#endif
