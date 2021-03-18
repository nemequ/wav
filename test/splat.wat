(module
  (type (;0;) (func (param i32) (result v128)))
  (type (;1;) (func (param i64) (result v128)))
  (type (;2;) (func (param f32) (result v128)))
  (type (;3;) (func (param f64) (result v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $splat_i8x16 (type 0) (param i32) (result v128)
    local.get 0
    i8x16.splat)
  (func $splat_i16x8 (type 0) (param i32) (result v128)
    local.get 0
    i16x8.splat)
  (func $splat_i32x4 (type 0) (param i32) (result v128)
    local.get 0
    i32x4.splat)
  (func $splat_i64x2 (type 1) (param i64) (result v128)
    local.get 0
    i64x2.splat)
  (func $splat_u8x16 (type 0) (param i32) (result v128)
    local.get 0
    i8x16.splat)
  (func $splat_u16x8 (type 0) (param i32) (result v128)
    local.get 0
    i16x8.splat)
  (func $splat_u32x4 (type 0) (param i32) (result v128)
    local.get 0
    i32x4.splat)
  (func $splat_u64x2 (type 1) (param i64) (result v128)
    local.get 0
    i64x2.splat)
  (func $splat_f32x4 (type 2) (param f32) (result v128)
    local.get 0
    f32x4.splat)
  (func $splat_f64x2 (type 3) (param f64) (result v128)
    local.get 0
    f64x2.splat)
  (func $splat_i8x16_o (type 0) (param i32) (result v128)
    local.get 0
    i8x16.splat)
  (func $splat_i16x8_o (type 0) (param i32) (result v128)
    local.get 0
    i16x8.splat)
  (func $splat_i32x4_o (type 0) (param i32) (result v128)
    local.get 0
    i32x4.splat)
  (func $splat_i64x2_o (type 1) (param i64) (result v128)
    local.get 0
    i64x2.splat)
  (func $splat_u8x16_o (type 0) (param i32) (result v128)
    local.get 0
    i8x16.splat)
  (func $splat_u16x8_o (type 0) (param i32) (result v128)
    local.get 0
    i16x8.splat)
  (func $splat_u32x4_o (type 0) (param i32) (result v128)
    local.get 0
    i32x4.splat)
  (func $splat_u64x2_o (type 1) (param i64) (result v128)
    local.get 0
    i64x2.splat)
  (func $splat_f32x4_o (type 2) (param f32) (result v128)
    local.get 0
    f32x4.splat)
  (func $splat_f64x2_o (type 3) (param f64) (result v128)
    local.get 0
    f64x2.splat))
