(module
  (type (;0;) (func (param v128) (result v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $popcnt_i8x16 (type 0) (param v128) (result v128)
    local.get 0
    i8x16.popcnt)
  (func $popcnt_u8x16 (type 0) (param v128) (result v128)
    local.get 0
    i8x16.popcnt)
  (func $popcnt_i8x16_o (type 0) (param v128) (result v128)
    local.get 0
    i8x16.popcnt)
  (func $popcnt_u8x16_o (type 0) (param v128) (result v128)
    local.get 0
    i8x16.popcnt))
