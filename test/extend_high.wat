(module
  (type (;0;) (func (param v128) (result v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $extend_high_i16x8 (type 0) (param v128) (result v128)
    local.get 0
    i32x4.extend_high_i16x8_s)
  (func $extend_high_u8x16 (type 0) (param v128) (result v128)
    local.get 0
    i16x8.extend_high_i8x16_u)
  (func $extend_high_u16x8 (type 0) (param v128) (result v128)
    local.get 0
    i32x4.extend_high_i16x8_u)
  (func $extend_high_i8x16 (type 0) (param v128) (result v128)
    local.get 0
    i16x8.extend_high_i8x16_s)
  (func $extend_high_i16x8_o (type 0) (param v128) (result v128)
    local.get 0
    i32x4.extend_high_i16x8_s)
  (func $extend_high_u8x16_o (type 0) (param v128) (result v128)
    local.get 0
    i16x8.extend_high_i8x16_u)
  (func $extend_high_u16x8_o (type 0) (param v128) (result v128)
    local.get 0
    i32x4.extend_high_i16x8_u)
  (func $extend_high_i8x16_o (type 0) (param v128) (result v128)
    local.get 0
    i16x8.extend_high_i8x16_s))
