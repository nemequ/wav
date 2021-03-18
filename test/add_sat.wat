(module
  (type (;0;) (func (param v128 v128) (result v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $add_sat_i8x16 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.add_sat_s)
  (func $add_sat_i16x8 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i16x8.add_sat_s)
  (func $add_sat_u8x16 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.add_sat_u)
  (func $add_sat_u16x8 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i16x8.add_sat_u)
  (func $add_sat_i8x16_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.add_sat_s)
  (func $add_sat_i16x8_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i16x8.add_sat_s)
  (func $add_sat_u8x16_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.add_sat_u)
  (func $add_sat_u16x8_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i16x8.add_sat_u))
