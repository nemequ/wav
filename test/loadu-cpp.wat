(module
  (type (;0;) (func (param i32) (result v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $_Z11loadu_i8x16Pa (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=1)
  (func $_Z11loadu_i16x8Ps (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=1)
  (func $_Z11loadu_i32x4Pi (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=1)
  (func $_Z11loadu_i64x2Px (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=1)
  (func $_Z11loadu_u8x16Ph (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=1)
  (func $_Z11loadu_u16x8Pt (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=1)
  (func $_Z11loadu_u32x4Pj (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=1)
  (func $_Z11loadu_u64x2Py (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=1)
  (func $_Z11loadu_f32x4Pf (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=1)
  (func $_Z11loadu_f64x2Pd (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=1))
