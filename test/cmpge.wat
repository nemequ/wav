(module
  (type (;0;) (func (param v128 v128) (result v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $cmpge_i8x16 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.ge_s)
  (func $cmpge_i16x8 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i16x8.ge_s)
  (func $cmpge_i32x4 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i32x4.ge_s)
  (func $cmpge_u8x16 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.ge_u)
  (func $cmpge_u16x8 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i16x8.ge_u)
  (func $cmpge_u32x4 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i32x4.ge_u)
  (func $cmpge_f32x4 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    f32x4.ge)
  (func $cmpge_i8x16_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.ge_s)
  (func $cmpge_i16x8_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i16x8.ge_s)
  (func $cmpge_i32x4_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i32x4.ge_s)
  (func $cmpge_u8x16_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.ge_u)
  (func $cmpge_u16x8_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i16x8.ge_u)
  (func $cmpge_u32x4_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i32x4.ge_u)
  (func $cmpge_f32x4_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    f32x4.ge))
