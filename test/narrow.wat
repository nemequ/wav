(module
  (type (;0;) (func (param v128 v128) (result v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $add_i16x8 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.narrow_i16x8_s)
  (func $add_i32x4 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i16x8.narrow_i32x4_s)
  (func $add_u16x8 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.narrow_i16x8_u)
  (func $add_u32x4 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i16x8.narrow_i32x4_u)
  (func $add_i16x8_op (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.narrow_i16x8_s)
  (func $add_i32x4_op (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i16x8.narrow_i32x4_s)
  (func $add_u16x8_op (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.narrow_i16x8_u)
  (func $add_u32x4_op (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i16x8.narrow_i32x4_u))
