(module
  (type (;0;) (func (param v128 v128 v128) (result v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $blend_i8x16 (type 0) (param v128 v128 v128) (result v128)
    local.get 0
    local.get 1
    local.get 2
    v128.bitselect)
  (func $blend_i16x8 (type 0) (param v128 v128 v128) (result v128)
    local.get 0
    local.get 1
    local.get 2
    v128.bitselect)
  (func $blend_i32x4 (type 0) (param v128 v128 v128) (result v128)
    local.get 0
    local.get 1
    local.get 2
    v128.bitselect)
  (func $blend_i64x2 (type 0) (param v128 v128 v128) (result v128)
    local.get 0
    local.get 1
    local.get 2
    v128.bitselect)
  (func $blend_u8x16 (type 0) (param v128 v128 v128) (result v128)
    local.get 0
    local.get 1
    local.get 2
    v128.bitselect)
  (func $blend_u16x8 (type 0) (param v128 v128 v128) (result v128)
    local.get 0
    local.get 1
    local.get 2
    v128.bitselect)
  (func $blend_u32x4 (type 0) (param v128 v128 v128) (result v128)
    local.get 0
    local.get 1
    local.get 2
    v128.bitselect)
  (func $blend_u64x2 (type 0) (param v128 v128 v128) (result v128)
    local.get 0
    local.get 1
    local.get 2
    v128.bitselect)
  (func $blend_f32x4 (type 0) (param v128 v128 v128) (result v128)
    local.get 0
    local.get 1
    local.get 2
    v128.bitselect)
  (func $blend_f64x2 (type 0) (param v128 v128 v128) (result v128)
    local.get 0
    local.get 1
    local.get 2
    v128.bitselect)
  (func $blend_i8x16_o (type 0) (param v128 v128 v128) (result v128)
    local.get 0
    local.get 1
    local.get 2
    v128.bitselect)
  (func $blend_i16x8_o (type 0) (param v128 v128 v128) (result v128)
    local.get 0
    local.get 1
    local.get 2
    v128.bitselect)
  (func $blend_i32x4_o (type 0) (param v128 v128 v128) (result v128)
    local.get 0
    local.get 1
    local.get 2
    v128.bitselect)
  (func $blend_i64x2_o (type 0) (param v128 v128 v128) (result v128)
    local.get 0
    local.get 1
    local.get 2
    v128.bitselect)
  (func $blend_u8x16_o (type 0) (param v128 v128 v128) (result v128)
    local.get 0
    local.get 1
    local.get 2
    v128.bitselect)
  (func $blend_u16x8_o (type 0) (param v128 v128 v128) (result v128)
    local.get 0
    local.get 1
    local.get 2
    v128.bitselect)
  (func $blend_u32x4_o (type 0) (param v128 v128 v128) (result v128)
    local.get 0
    local.get 1
    local.get 2
    v128.bitselect)
  (func $blend_u64x2_o (type 0) (param v128 v128 v128) (result v128)
    local.get 0
    local.get 1
    local.get 2
    v128.bitselect)
  (func $blend_f32x4_o (type 0) (param v128 v128 v128) (result v128)
    local.get 0
    local.get 1
    local.get 2
    v128.bitselect)
  (func $blend_f64x2_o (type 0) (param v128 v128 v128) (result v128)
    local.get 0
    local.get 1
    local.get 2
    v128.bitselect))
