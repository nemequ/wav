(module
  (type (;0;) (func (param v128) (result v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $neg_i8x16 (type 0) (param v128) (result v128)
    local.get 0
    i8x16.neg)
  (func $neg_i16x8 (type 0) (param v128) (result v128)
    local.get 0
    i16x8.neg)
  (func $neg_i32x4 (type 0) (param v128) (result v128)
    local.get 0
    i32x4.neg)
  (func $neg_i64x2 (type 0) (param v128) (result v128)
    local.get 0
    i64x2.neg)
  (func $neg_f32x4 (type 0) (param v128) (result v128)
    local.get 0
    f32x4.neg)
  (func $neg_f64x2 (type 0) (param v128) (result v128)
    local.get 0
    f64x2.neg)
  (func $neg_i8x16_o (type 0) (param v128) (result v128)
    local.get 0
    i8x16.neg)
  (func $neg_i16x8_o (type 0) (param v128) (result v128)
    local.get 0
    i16x8.neg)
  (func $neg_i32x4_o (type 0) (param v128) (result v128)
    local.get 0
    i32x4.neg)
  (func $neg_i64x2_o (type 0) (param v128) (result v128)
    local.get 0
    i64x2.neg)
  (func $neg_f32x4_o (type 0) (param v128) (result v128)
    local.get 0
    f32x4.neg)
  (func $neg_f64x2_o (type 0) (param v128) (result v128)
    local.get 0
    f64x2.neg))
