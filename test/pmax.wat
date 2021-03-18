(module
  (type (;0;) (func (param v128 v128) (result v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $pmax_f32x4 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    f32x4.pmax)
  (func $pmax_f64x2 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    f64x2.pmax)
  (func $pmax_f32x4_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    f32x4.pmax)
  (func $pmax_f64x2_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    f64x2.pmax))
