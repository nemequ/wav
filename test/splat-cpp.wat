(module
  (type (;0;) (func (param i32) (result v128)))
  (type (;1;) (func (param i64) (result v128)))
  (type (;2;) (func (param f32) (result v128)))
  (type (;3;) (func (param f64) (result v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $_Z11splat_i8x16a (type 0) (param i32) (result v128)
    local.get 0
    i8x16.splat)
  (func $_Z11splat_i16x8s (type 0) (param i32) (result v128)
    local.get 0
    i16x8.splat)
  (func $_Z11splat_i32x4i (type 0) (param i32) (result v128)
    local.get 0
    i32x4.splat)
  (func $_Z11splat_i64x2x (type 1) (param i64) (result v128)
    local.get 0
    i64x2.splat)
  (func $_Z11splat_u8x16h (type 0) (param i32) (result v128)
    local.get 0
    i8x16.splat)
  (func $_Z11splat_u16x8t (type 0) (param i32) (result v128)
    local.get 0
    i16x8.splat)
  (func $_Z11splat_u32x4j (type 0) (param i32) (result v128)
    local.get 0
    i32x4.splat)
  (func $_Z11splat_u64x2y (type 1) (param i64) (result v128)
    local.get 0
    i64x2.splat)
  (func $_Z11splat_f32x4f (type 2) (param f32) (result v128)
    local.get 0
    f32x4.splat)
  (func $_Z11splat_f64x2d (type 3) (param f64) (result v128)
    local.get 0
    f64x2.splat)
  (func $_Z13splat_i8x16_oa (type 0) (param i32) (result v128)
    local.get 0
    i8x16.splat)
  (func $_Z13splat_i16x8_os (type 0) (param i32) (result v128)
    local.get 0
    i16x8.splat)
  (func $_Z13splat_i32x4_oi (type 0) (param i32) (result v128)
    local.get 0
    i32x4.splat)
  (func $_Z13splat_i64x2_ox (type 1) (param i64) (result v128)
    local.get 0
    i64x2.splat)
  (func $_Z13splat_u8x16_oh (type 0) (param i32) (result v128)
    local.get 0
    i8x16.splat)
  (func $_Z13splat_u16x8_ot (type 0) (param i32) (result v128)
    local.get 0
    i16x8.splat)
  (func $_Z13splat_u32x4_oj (type 0) (param i32) (result v128)
    local.get 0
    i32x4.splat)
  (func $_Z13splat_u64x2_oy (type 1) (param i64) (result v128)
    local.get 0
    i64x2.splat)
  (func $_Z13splat_f32x4_of (type 2) (param f32) (result v128)
    local.get 0
    f32x4.splat)
  (func $_Z13splat_f64x2_od (type 3) (param f64) (result v128)
    local.get 0
    f64x2.splat))
