(module
  (type (;0;) (func (param v128 v128) (result v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $mul_i16x8 (type 0) (param v128 v128) (result v128)
    local.get 1
    local.get 0
    i16x8.mul)
  (func $mul_i32x4 (type 0) (param v128 v128) (result v128)
    local.get 1
    local.get 0
    i32x4.mul)
  (func $mul_i64x2 (type 0) (param v128 v128) (result v128)
    local.get 1
    local.get 0
    i64x2.mul)
  (func $mul_u16x8 (type 0) (param v128 v128) (result v128)
    local.get 1
    local.get 0
    i16x8.mul)
  (func $mul_u32x4 (type 0) (param v128 v128) (result v128)
    local.get 1
    local.get 0
    i32x4.mul)
  (func $mul_u64x2 (type 0) (param v128 v128) (result v128)
    local.get 1
    local.get 0
    i64x2.mul)
  (func $mul_f32x4 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    f32x4.mul)
  (func $mul_f64x2 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    f64x2.mul)
  (func $mul_i16x8_o (type 0) (param v128 v128) (result v128)
    local.get 1
    local.get 0
    i16x8.mul)
  (func $mul_i32x4_o (type 0) (param v128 v128) (result v128)
    local.get 1
    local.get 0
    i32x4.mul)
  (func $mul_i64x2_o (type 0) (param v128 v128) (result v128)
    local.get 1
    local.get 0
    i64x2.mul)
  (func $mul_u16x8_o (type 0) (param v128 v128) (result v128)
    local.get 1
    local.get 0
    i16x8.mul)
  (func $mul_u32x4_o (type 0) (param v128 v128) (result v128)
    local.get 1
    local.get 0
    i32x4.mul)
  (func $mul_u64x2_o (type 0) (param v128 v128) (result v128)
    local.get 1
    local.get 0
    i64x2.mul)
  (func $mul_f32x4_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    f32x4.mul)
  (func $mul_f64x2_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    f64x2.mul))
