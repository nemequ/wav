(module
  (type (;0;) (func (param i32) (result v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $_Z17loadu_splat_i8x16PKv (type 0) (param i32) (result v128)
    local.get 0
    v128.load8_splat)
  (func $_Z17loadu_splat_i16x8PKv (type 0) (param i32) (result v128)
    local.get 0
    v128.load16_splat align=1)
  (func $_Z17loadu_splat_i32x4PKv (type 0) (param i32) (result v128)
    local.get 0
    v128.load32_splat align=1)
  (func $_Z17loadu_splat_i64x2PKv (type 0) (param i32) (result v128)
    local.get 0
    v128.load64_splat align=1)
  (func $_Z17loadu_splat_u8x16PKv (type 0) (param i32) (result v128)
    local.get 0
    v128.load8_splat)
  (func $_Z17loadu_splat_u16x8PKv (type 0) (param i32) (result v128)
    local.get 0
    v128.load16_splat align=1)
  (func $_Z17loadu_splat_u32x4PKv (type 0) (param i32) (result v128)
    local.get 0
    v128.load32_splat align=1)
  (func $_Z17loadu_splat_u64x2PKv (type 0) (param i32) (result v128)
    local.get 0
    v128.load64_splat align=1)
  (func $_Z17loadu_splat_f32x4PKv (type 0) (param i32) (result v128)
    local.get 0
    v128.load32_splat align=1)
  (func $_Z17loadu_splat_f64x2PKv (type 0) (param i32) (result v128)
    local.get 0
    v128.load64_splat align=1))
