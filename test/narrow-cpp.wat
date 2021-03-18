(module
  (type (;0;) (func (param v128 v128) (result v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $_Z9add_i16x811wav_i16x8_tS_ (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.narrow_i16x8_s)
  (func $_Z9add_i32x411wav_i32x4_tS_ (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i16x8.narrow_i32x4_s)
  (func $_Z9add_u16x811wav_u16x8_tS_ (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.narrow_i16x8_u)
  (func $_Z9add_u32x411wav_u32x4_tS_ (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i16x8.narrow_i32x4_u)
  (func $_Z12add_i16x8_op11wav_i16x8_tS_ (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.narrow_i16x8_s)
  (func $_Z12add_i32x4_op11wav_i32x4_tS_ (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i16x8.narrow_i32x4_s)
  (func $_Z12add_u16x8_op11wav_u16x8_tS_ (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.narrow_i16x8_u)
  (func $_Z12add_u32x4_op11wav_u32x4_tS_ (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i16x8.narrow_i32x4_u))
