(module
  (type (;0;) (func (param v128) (result i32)))
  (type (;1;) (func (param v128) (result i64)))
  (type (;2;) (func (param v128) (result f32)))
  (type (;3;) (func (param v128) (result f64)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $_Z18extract_lane_i8x1611wav_i8x16_t (type 0) (param v128) (result i32)
    local.get 0
    i8x16.extract_lane_s 15)
  (func $_Z18extract_lane_i16x811wav_i16x8_t (type 0) (param v128) (result i32)
    local.get 0
    i16x8.extract_lane_s 7)
  (func $_Z18extract_lane_i32x411wav_i32x4_t (type 0) (param v128) (result i32)
    local.get 0
    i32x4.extract_lane 3)
  (func $_Z18extract_lane_i64x211wav_i64x2_t (type 1) (param v128) (result i64)
    local.get 0
    i64x2.extract_lane 1)
  (func $_Z18extract_lane_u8x1611wav_u8x16_t (type 0) (param v128) (result i32)
    local.get 0
    i8x16.extract_lane_u 15)
  (func $_Z18extract_lane_u16x811wav_u16x8_t (type 0) (param v128) (result i32)
    local.get 0
    i16x8.extract_lane_u 7)
  (func $_Z18extract_lane_u32x411wav_u32x4_t (type 0) (param v128) (result i32)
    local.get 0
    i32x4.extract_lane 3)
  (func $_Z18extract_lane_u64x211wav_u64x2_t (type 1) (param v128) (result i64)
    local.get 0
    i64x2.extract_lane 1)
  (func $_Z18extract_lane_f32x411wav_f32x4_t (type 2) (param v128) (result f32)
    local.get 0
    f32x4.extract_lane 3)
  (func $_Z18extract_lane_f64x211wav_f64x2_t (type 3) (param v128) (result f64)
    local.get 0
    f64x2.extract_lane 1)
  (func $_Z20extract_lane_i8x16_o11wav_i8x16_t (type 0) (param v128) (result i32)
    local.get 0
    i8x16.extract_lane_s 15)
  (func $_Z20extract_lane_i16x8_o11wav_i16x8_t (type 0) (param v128) (result i32)
    local.get 0
    i16x8.extract_lane_s 7)
  (func $_Z20extract_lane_i32x4_o11wav_i32x4_t (type 0) (param v128) (result i32)
    local.get 0
    i32x4.extract_lane 3)
  (func $_Z20extract_lane_i64x2_o11wav_i64x2_t (type 1) (param v128) (result i64)
    local.get 0
    i64x2.extract_lane 1)
  (func $_Z20extract_lane_u8x16_o11wav_u8x16_t (type 0) (param v128) (result i32)
    local.get 0
    i8x16.extract_lane_u 15)
  (func $_Z20extract_lane_u16x8_o11wav_u16x8_t (type 0) (param v128) (result i32)
    local.get 0
    i16x8.extract_lane_u 7)
  (func $_Z20extract_lane_u32x4_o11wav_u32x4_t (type 0) (param v128) (result i32)
    local.get 0
    i32x4.extract_lane 3)
  (func $_Z20extract_lane_u64x2_o11wav_u64x2_t (type 1) (param v128) (result i64)
    local.get 0
    i64x2.extract_lane 1)
  (func $_Z20extract_lane_f32x4_o11wav_f32x4_t (type 2) (param v128) (result f32)
    local.get 0
    f32x4.extract_lane 3)
  (func $_Z20extract_lane_f64x2_o11wav_f64x2_t (type 3) (param v128) (result f64)
    local.get 0
    f64x2.extract_lane 1))
