(module
  (type (;0;) (func (param v128 v128) (result v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $min_i8x16 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.min_s)
  (func $min_i16x8 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i16x8.min_s)
  (func $min_i32x4 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i32x4.min_s)
  (func $min_u8x16 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.min_u)
  (func $min_u16x8 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i16x8.min_u)
  (func $min_u32x4 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i32x4.min_u)
  (func $min_f32x4 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    f32x4.min)
  (func $min_f64x2 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    f64x2.min)
  (func $min_i8x16_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.min_s)
  (func $min_i16x8_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i16x8.min_s)
  (func $min_i32x4_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i32x4.min_s)
  (func $min_u8x16_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.min_u)
  (func $min_u16x8_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i16x8.min_u)
  (func $min_u32x4_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i32x4.min_u)
  (func $min_f32x4_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    f32x4.min)
  (func $min_f64x2_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    f64x2.min))
