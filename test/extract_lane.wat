(module
  (type (;0;) (func (param v128) (result i32)))
  (type (;1;) (func (param v128) (result i64)))
  (type (;2;) (func (param v128) (result f32)))
  (type (;3;) (func (param v128) (result f64)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $extract_lane_i8x16 (type 0) (param v128) (result i32)
    local.get 0
    i8x16.extract_lane_s 15)
  (func $extract_lane_i16x8 (type 0) (param v128) (result i32)
    local.get 0
    i16x8.extract_lane_s 7)
  (func $extract_lane_i32x4 (type 0) (param v128) (result i32)
    local.get 0
    i32x4.extract_lane 3)
  (func $extract_lane_i64x2 (type 1) (param v128) (result i64)
    local.get 0
    i64x2.extract_lane 1)
  (func $extract_lane_u8x16 (type 0) (param v128) (result i32)
    local.get 0
    i8x16.extract_lane_u 15)
  (func $extract_lane_u16x8 (type 0) (param v128) (result i32)
    local.get 0
    i16x8.extract_lane_u 7)
  (func $extract_lane_u32x4 (type 0) (param v128) (result i32)
    local.get 0
    i32x4.extract_lane 3)
  (func $extract_lane_u64x2 (type 1) (param v128) (result i64)
    local.get 0
    i64x2.extract_lane 1)
  (func $extract_lane_f32x4 (type 2) (param v128) (result f32)
    local.get 0
    f32x4.extract_lane 3)
  (func $extract_lane_f64x2 (type 3) (param v128) (result f64)
    local.get 0
    f64x2.extract_lane 1)
  (func $extract_lane_i8x16_o (type 0) (param v128) (result i32)
    local.get 0
    i8x16.extract_lane_s 15)
  (func $extract_lane_i16x8_o (type 0) (param v128) (result i32)
    local.get 0
    i16x8.extract_lane_s 7)
  (func $extract_lane_i32x4_o (type 0) (param v128) (result i32)
    local.get 0
    i32x4.extract_lane 3)
  (func $extract_lane_i64x2_o (type 1) (param v128) (result i64)
    local.get 0
    i64x2.extract_lane 1)
  (func $extract_lane_u8x16_o (type 0) (param v128) (result i32)
    local.get 0
    i8x16.extract_lane_u 15)
  (func $extract_lane_u16x8_o (type 0) (param v128) (result i32)
    local.get 0
    i16x8.extract_lane_u 7)
  (func $extract_lane_u32x4_o (type 0) (param v128) (result i32)
    local.get 0
    i32x4.extract_lane 3)
  (func $extract_lane_u64x2_o (type 1) (param v128) (result i64)
    local.get 0
    i64x2.extract_lane 1)
  (func $extract_lane_f32x4_o (type 2) (param v128) (result f32)
    local.get 0
    f32x4.extract_lane 3)
  (func $extract_lane_f64x2_o (type 3) (param v128) (result f64)
    local.get 0
    f64x2.extract_lane 1))
