(module
  (type (;0;) (func (param i32) (result v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $_Z10load_i8x16Pa (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=1)
  (func $_Z10load_i16x8Ps (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=2)
  (func $_Z10load_i32x4Pi (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=4)
  (func $_Z10load_i64x2Px (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=8)
  (func $_Z10load_u8x16Ph (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=1)
  (func $_Z10load_u16x8Pt (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=2)
  (func $_Z10load_u32x4Pj (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=4)
  (func $_Z10load_u64x2Py (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=8)
  (func $_Z10load_f32x4Pf (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=4)
  (func $_Z10load_f64x2Pd (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=8)
  (func $_Z12load_i8x16_oPa (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=1)
  (func $_Z12load_i16x8_oPs (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=2)
  (func $_Z12load_i32x4_oPi (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=4)
  (func $_Z12load_i64x2_oPx (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=8)
  (func $_Z12load_u8x16_oPh (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=1)
  (func $_Z12load_u16x8_oPt (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=2)
  (func $_Z12load_u32x4_oPj (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=4)
  (func $_Z12load_u64x2_oPy (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=8)
  (func $_Z12load_f32x4_oPf (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=4)
  (func $_Z12load_f64x2_oPd (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=8))
