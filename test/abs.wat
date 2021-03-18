(module
  (type (;0;) (func (param v128) (result v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $abs_i8x16 (type 0) (param v128) (result v128)
    local.get 0
    i8x16.abs)
  (func $abs_i16x8 (type 0) (param v128) (result v128)
    local.get 0
    i16x8.abs)
  (func $abs_i32x4 (type 0) (param v128) (result v128)
    local.get 0
    i32x4.abs)
  (func $abs_f32x4 (type 0) (param v128) (result v128)
    local.get 0
    f32x4.abs)
  (func $abs_f64x2 (type 0) (param v128) (result v128)
    local.get 0
    f64x2.abs)
  (func $abs_i8x16_o (type 0) (param v128) (result v128)
    local.get 0
    i8x16.abs)
  (func $abs_i16x8_o (type 0) (param v128) (result v128)
    local.get 0
    i16x8.abs)
  (func $abs_i32x4_o (type 0) (param v128) (result v128)
    local.get 0
    i32x4.abs)
  (func $abs_f32x4_o (type 0) (param v128) (result v128)
    local.get 0
    f32x4.abs)
  (func $abs_f64x2_o (type 0) (param v128) (result v128)
    local.get 0
    f64x2.abs))
