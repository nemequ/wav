(module
  (type (;0;) (func (param v128 v128) (result v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $max_i8x16 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.max_s)
  (func $max_i16x8 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i16x8.max_s)
  (func $max_i32x4 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i32x4.max_s)
  (func $max_u8x16 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.max_u)
  (func $max_u16x8 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i16x8.max_u)
  (func $max_u32x4 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i32x4.max_u)
  (func $max_f32x4 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    f32x4.max)
  (func $max_f64x2 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    f64x2.max)
  (func $max_i8x16_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.max_s)
  (func $max_i16x8_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i16x8.max_s)
  (func $max_i32x4_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i32x4.max_s)
  (func $max_u8x16_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.max_u)
  (func $max_u16x8_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i16x8.max_u)
  (func $max_u32x4_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i32x4.max_u)
  (func $max_f32x4_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    f32x4.max)
  (func $max_f64x2_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    f64x2.max))
