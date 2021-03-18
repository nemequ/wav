(module
  (type (;0;) (func (param v128 v128) (result v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $_Z10pmax_f32x411wav_f32x4_tS_ (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    f32x4.pmax)
  (func $_Z10pmax_f64x211wav_f64x2_tS_ (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    f64x2.pmax)
  (func $_Z12pmax_f32x4_o11wav_f32x4_tS_ (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    f32x4.pmax)
  (func $_Z12pmax_f64x2_o11wav_f64x2_tS_ (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    f64x2.pmax))
