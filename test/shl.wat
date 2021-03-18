(module
  (type (;0;) (func (param v128 i32) (result v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $shl_i8x16 (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i8x16.shl)
  (func $shl_i16x8 (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i16x8.shl)
  (func $shl_i32x4 (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i32x4.shl)
  (func $shl_u8x16 (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i8x16.shl)
  (func $shl_u16x8 (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i16x8.shl)
  (func $shl_u32x4 (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i32x4.shl)
  (func $shl_i8x16_o (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i8x16.shl)
  (func $shl_i16x8_o (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i16x8.shl)
  (func $shl_i32x4_o (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i32x4.shl)
  (func $shl_u8x16_o (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i8x16.shl)
  (func $shl_u16x8_o (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i16x8.shl)
  (func $shl_u32x4_o (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i32x4.shl))
