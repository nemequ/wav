(module
  (type (;0;) (func (param v128) (result v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $_Z10sqrt_f32x411wav_f32x4_t (type 0) (param v128) (result v128)
    local.get 0
    f32x4.sqrt)
  (func $_Z10sqrt_f64x211wav_f64x2_t (type 0) (param v128) (result v128)
    local.get 0
    f64x2.sqrt)
  (func $_Z12sqrt_f32x4_o11wav_f32x4_t (type 0) (param v128) (result v128)
    local.get 0
    f32x4.sqrt)
  (func $_Z12sqrt_f64x2_o11wav_f64x2_t (type 0) (param v128) (result v128)
    local.get 0
    f64x2.sqrt))
