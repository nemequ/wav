(module
  (type (;0;) (func (param i32) (result v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $load_zero_i32x4 (type 0) (param i32) (result v128)
    local.get 0
    v128.load32_zero)
  (func $load_zero_i64x2 (type 0) (param i32) (result v128)
    local.get 0
    v128.load64_zero)
  (func $load_zero_u32x4 (type 0) (param i32) (result v128)
    local.get 0
    v128.load32_zero)
  (func $load_zero_u64x2 (type 0) (param i32) (result v128)
    local.get 0
    v128.load64_zero)
  (func $load_zero_f32x4 (type 0) (param i32) (result v128)
    local.get 0
    v128.load32_zero)
  (func $load_zero_f64x2 (type 0) (param i32) (result v128)
    local.get 0
    v128.load64_zero)
  (func $load_zero_i32x4_o (type 0) (param i32) (result v128)
    local.get 0
    v128.load32_zero)
  (func $load_zero_i64x2_o (type 0) (param i32) (result v128)
    local.get 0
    v128.load64_zero)
  (func $load_zero_u32x4_o (type 0) (param i32) (result v128)
    local.get 0
    v128.load32_zero)
  (func $load_zero_u64x2_o (type 0) (param i32) (result v128)
    local.get 0
    v128.load64_zero)
  (func $load_zero_f32x4_o (type 0) (param i32) (result v128)
    local.get 0
    v128.load32_zero)
  (func $load_zero_f64x2_o (type 0) (param i32) (result v128)
    local.get 0
    v128.load64_zero))
