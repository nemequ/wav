(module
  (type (;0;) (func (param v128 v128) (result v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $cmpeq_i8x16 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.eq)
  (func $cmpeq_i16x8 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i16x8.eq)
  (func $cmpeq_i32x4 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i32x4.eq)
  (func $cmpeq_u8x16 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.eq)
  (func $cmpeq_u16x8 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i16x8.eq)
  (func $cmpeq_u32x4 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i32x4.eq)
  (func $cmpeq_b8x16 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.eq)
  (func $cmpeq_b16x8 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i16x8.eq)
  (func $cmpeq_b32x4 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i32x4.eq)
  (func $cmpeq_f32x4 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    f32x4.eq)
  (func $cmpeq_i8x16_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.eq)
  (func $cmpeq_i16x8_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i16x8.eq)
  (func $cmpeq_i32x4_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i32x4.eq)
  (func $cmpeq_u8x16_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.eq)
  (func $cmpeq_u16x8_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i16x8.eq)
  (func $cmpeq_u32x4_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i32x4.eq)
  (func $cmpeq_b8x16_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.eq)
  (func $cmpeq_b16x8_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i16x8.eq)
  (func $cmpeq_b32x4_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i32x4.eq)
  (func $cmpeq_f32x4_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    f32x4.eq))
