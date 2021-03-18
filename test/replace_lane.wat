(module
  (type (;0;) (func (param v128 i32) (result v128)))
  (type (;1;) (func (param v128 i64) (result v128)))
  (type (;2;) (func (param v128 f32) (result v128)))
  (type (;3;) (func (param v128 f64) (result v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $replace_lane_i8x16 (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i8x16.replace_lane 15)
  (func $replace_lane_i16x8 (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i16x8.replace_lane 7)
  (func $replace_lane_i32x4 (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i32x4.replace_lane 3)
  (func $replace_lane_i64x2 (type 1) (param v128 i64) (result v128)
    local.get 0
    local.get 1
    i64x2.replace_lane 1)
  (func $replace_lane_u8x16 (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i8x16.replace_lane 15)
  (func $replace_lane_u16x8 (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i16x8.replace_lane 7)
  (func $replace_lane_u32x4 (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i32x4.replace_lane 3)
  (func $replace_lane_u64x2 (type 1) (param v128 i64) (result v128)
    local.get 0
    local.get 1
    i64x2.replace_lane 1)
  (func $replace_lane_f32x4 (type 2) (param v128 f32) (result v128)
    local.get 0
    local.get 1
    f32x4.replace_lane 3)
  (func $replace_lane_f64x2 (type 3) (param v128 f64) (result v128)
    local.get 0
    local.get 1
    f64x2.replace_lane 1)
  (func $replace_lane_i8x16_o (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i8x16.replace_lane 15)
  (func $replace_lane_i16x8_o (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i16x8.replace_lane 7)
  (func $replace_lane_i32x4_o (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i32x4.replace_lane 3)
  (func $replace_lane_i64x2_o (type 1) (param v128 i64) (result v128)
    local.get 0
    local.get 1
    i64x2.replace_lane 1)
  (func $replace_lane_u8x16_o (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i8x16.replace_lane 15)
  (func $replace_lane_u16x8_o (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i16x8.replace_lane 7)
  (func $replace_lane_u32x4_o (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i32x4.replace_lane 3)
  (func $replace_lane_u64x2_o (type 1) (param v128 i64) (result v128)
    local.get 0
    local.get 1
    i64x2.replace_lane 1)
  (func $replace_lane_f32x4_o (type 2) (param v128 f32) (result v128)
    local.get 0
    local.get 1
    f32x4.replace_lane 3)
  (func $replace_lane_f64x2_o (type 3) (param v128 f64) (result v128)
    local.get 0
    local.get 1
    f64x2.replace_lane 1))
