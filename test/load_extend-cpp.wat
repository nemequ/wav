(module
  (type (;0;) (func (param i32) (result v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $_Z17load_extend_i16x8Pa (type 0) (param i32) (result v128)
    local.get 0
    v128.load8x8_s align=1)
  (func $_Z17load_extend_i32x4Ps (type 0) (param i32) (result v128)
    local.get 0
    v128.load16x4_s align=2)
  (func $_Z17load_extend_i64x2Pi (type 0) (param i32) (result v128)
    local.get 0
    v128.load32x2_s align=4)
  (func $_Z17load_extend_u16x8Ph (type 0) (param i32) (result v128)
    local.get 0
    v128.load8x8_s align=1)
  (func $_Z17load_extend_u32x4Pt (type 0) (param i32) (result v128)
    local.get 0
    v128.load16x4_s align=2)
  (func $_Z17load_extend_u64x2Pj (type 0) (param i32) (result v128)
    local.get 0
    v128.load32x2_s align=4)
  (func $_Z19load_extend_i16x8_oPa (type 0) (param i32) (result v128)
    local.get 0
    v128.load8x8_s align=1)
  (func $_Z19load_extend_i32x4_oPs (type 0) (param i32) (result v128)
    local.get 0
    v128.load16x4_s align=2)
  (func $_Z19load_extend_i64x2_oPi (type 0) (param i32) (result v128)
    local.get 0
    v128.load32x2_s align=4)
  (func $_Z19load_extend_u16x8_oPh (type 0) (param i32) (result v128)
    local.get 0
    v128.load8x8_s align=1)
  (func $_Z19load_extend_u32x4_oPt (type 0) (param i32) (result v128)
    local.get 0
    v128.load16x4_s align=2)
  (func $_Z19load_extend_u64x2_oPj (type 0) (param i32) (result v128)
    local.get 0
    v128.load32x2_s align=4))
