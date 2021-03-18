(module
  (type (;0;) (func (param i32) (result v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $load_i8x16 (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=1)
  (func $load_i16x8 (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=2)
  (func $load_i32x4 (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=4)
  (func $load_i64x2 (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=8)
  (func $load_u8x16 (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=1)
  (func $load_u16x8 (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=2)
  (func $load_u32x4 (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=4)
  (func $load_u64x2 (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=8)
  (func $load_f32x4 (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=4)
  (func $load_f64x2 (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=8)
  (func $load_i8x16_o (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=1)
  (func $load_i16x8_o (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=2)
  (func $load_i32x4_o (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=4)
  (func $load_i64x2_o (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=8)
  (func $load_u8x16_o (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=1)
  (func $load_u16x8_o (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=2)
  (func $load_u32x4_o (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=4)
  (func $load_u64x2_o (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=8)
  (func $load_f32x4_o (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=4)
  (func $load_f64x2_o (type 0) (param i32) (result v128)
    local.get 0
    v128.load align=8))
