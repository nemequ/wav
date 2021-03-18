(module
  (type (;0;) (func (param i32) (result v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $load_splat_i8x16 (type 0) (param i32) (result v128)
    local.get 0
    v128.load8_splat)
  (func $load_splat_i16x8 (type 0) (param i32) (result v128)
    local.get 0
    v128.load16_splat)
  (func $load_splat_i32x4 (type 0) (param i32) (result v128)
    local.get 0
    v128.load32_splat)
  (func $load_splat_i64x2 (type 0) (param i32) (result v128)
    local.get 0
    v128.load64_splat)
  (func $load_splat_u8x16 (type 0) (param i32) (result v128)
    local.get 0
    v128.load8_splat)
  (func $load_splat_u16x8 (type 0) (param i32) (result v128)
    local.get 0
    v128.load16_splat)
  (func $load_splat_u32x4 (type 0) (param i32) (result v128)
    local.get 0
    v128.load32_splat)
  (func $load_splat_u64x2 (type 0) (param i32) (result v128)
    local.get 0
    v128.load64_splat)
  (func $load_splat_f32x4 (type 0) (param i32) (result v128)
    local.get 0
    v128.load32_splat)
  (func $load_splat_f64x2 (type 0) (param i32) (result v128)
    local.get 0
    v128.load64_splat)
  (func $load_splat_i8x16_o (type 0) (param i32) (result v128)
    local.get 0
    v128.load8_splat)
  (func $load_splat_i16x8_o (type 0) (param i32) (result v128)
    local.get 0
    v128.load16_splat)
  (func $load_splat_i32x4_o (type 0) (param i32) (result v128)
    local.get 0
    v128.load32_splat)
  (func $load_splat_i64x2_o (type 0) (param i32) (result v128)
    local.get 0
    v128.load64_splat)
  (func $load_splat_u8x16_o (type 0) (param i32) (result v128)
    local.get 0
    v128.load8_splat)
  (func $load_splat_u16x8_o (type 0) (param i32) (result v128)
    local.get 0
    v128.load16_splat)
  (func $load_splat_u32x4_o (type 0) (param i32) (result v128)
    local.get 0
    v128.load32_splat)
  (func $load_splat_u64x2_o (type 0) (param i32) (result v128)
    local.get 0
    v128.load64_splat)
  (func $load_splat_f32x4_o (type 0) (param i32) (result v128)
    local.get 0
    v128.load32_splat)
  (func $load_splat_f64x2_o (type 0) (param i32) (result v128)
    local.get 0
    v128.load64_splat))
