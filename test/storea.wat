(module
  (type (;0;) (func (param i32 v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $storea_i8x16 (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store)
  (func $storea_i16x8 (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store)
  (func $storea_i32x4 (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store)
  (func $storea_i64x2 (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store)
  (func $storea_u8x16 (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store)
  (func $storea_u16x8 (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store)
  (func $storea_u32x4 (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store)
  (func $storea_u64x2 (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store)
  (func $storea_f32x4 (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store)
  (func $storea_f64x2 (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store))
