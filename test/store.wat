(module
  (type (;0;) (func (param i32 v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $store_i8x16 (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=1)
  (func $store_i16x8 (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=2)
  (func $store_i32x4 (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=4)
  (func $store_i64x2 (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=8)
  (func $store_u8x16 (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=1)
  (func $store_u16x8 (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=2)
  (func $store_u32x4 (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=4)
  (func $store_u64x2 (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=8)
  (func $store_f32x4 (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=4)
  (func $store_f64x2 (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=8)
  (func $store_i8x16_o (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=1)
  (func $store_i16x8_o (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=2)
  (func $store_i32x4_o (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=4)
  (func $store_i64x2_o (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=8)
  (func $store_u8x16_o (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=1)
  (func $store_u16x8_o (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=2)
  (func $store_u32x4_o (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=4)
  (func $store_u64x2_o (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=8)
  (func $store_f32x4_o (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=4)
  (func $store_f64x2_o (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=8))
