(module
  (type (;0;) (func (param i32 v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $_Z11store_i8x16Pa11wav_i8x16_t (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=1)
  (func $_Z11store_i16x8Ps11wav_i16x8_t (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=2)
  (func $_Z11store_i32x4Pi11wav_i32x4_t (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=4)
  (func $_Z11store_i64x2Px11wav_i64x2_t (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=8)
  (func $_Z11store_u8x16Ph11wav_u8x16_t (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=1)
  (func $_Z11store_u16x8Pt11wav_u16x8_t (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=2)
  (func $_Z11store_u32x4Pj11wav_u32x4_t (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=4)
  (func $_Z11store_u64x2Py11wav_u64x2_t (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=8)
  (func $_Z11store_f32x4Pf11wav_f32x4_t (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=4)
  (func $_Z11store_f64x2Pd11wav_f64x2_t (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=8)
  (func $_Z13store_i8x16_oPa11wav_i8x16_t (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=1)
  (func $_Z13store_i16x8_oPs11wav_i16x8_t (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=2)
  (func $_Z13store_i32x4_oPi11wav_i32x4_t (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=4)
  (func $_Z13store_i64x2_oPx11wav_i64x2_t (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=8)
  (func $_Z13store_u8x16_oPh11wav_u8x16_t (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=1)
  (func $_Z13store_u16x8_oPt11wav_u16x8_t (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=2)
  (func $_Z13store_u32x4_oPj11wav_u32x4_t (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=4)
  (func $_Z13store_u64x2_oPy11wav_u64x2_t (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=8)
  (func $_Z13store_f32x4_oPf11wav_f32x4_t (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=4)
  (func $_Z13store_f64x2_oPd11wav_f64x2_t (type 0) (param i32 v128)
    local.get 0
    local.get 1
    v128.store align=8))
