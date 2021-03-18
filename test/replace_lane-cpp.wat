(module
  (type (;0;) (func (param v128 i32) (result v128)))
  (type (;1;) (func (param v128 i64) (result v128)))
  (type (;2;) (func (param v128 f32) (result v128)))
  (type (;3;) (func (param v128 f64) (result v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $_Z18replace_lane_i8x1611wav_i8x16_ta (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i8x16.replace_lane 15)
  (func $_Z18replace_lane_i16x811wav_i16x8_ts (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i16x8.replace_lane 7)
  (func $_Z18replace_lane_i32x411wav_i32x4_ti (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i32x4.replace_lane 3)
  (func $_Z18replace_lane_i64x211wav_i64x2_tx (type 1) (param v128 i64) (result v128)
    local.get 0
    local.get 1
    i64x2.replace_lane 1)
  (func $_Z18replace_lane_u8x1611wav_u8x16_th (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i8x16.replace_lane 15)
  (func $_Z18replace_lane_u16x811wav_u16x8_tt (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i16x8.replace_lane 7)
  (func $_Z18replace_lane_u32x411wav_u32x4_tj (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i32x4.replace_lane 3)
  (func $_Z18replace_lane_u64x211wav_u64x2_ty (type 1) (param v128 i64) (result v128)
    local.get 0
    local.get 1
    i64x2.replace_lane 1)
  (func $_Z18replace_lane_f32x411wav_f32x4_tf (type 2) (param v128 f32) (result v128)
    local.get 0
    local.get 1
    f32x4.replace_lane 3)
  (func $_Z18replace_lane_f64x211wav_f64x2_td (type 3) (param v128 f64) (result v128)
    local.get 0
    local.get 1
    f64x2.replace_lane 1)
  (func $_Z20replace_lane_i8x16_o11wav_i8x16_ta (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i8x16.replace_lane 15)
  (func $_Z20replace_lane_i16x8_o11wav_i16x8_ts (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i16x8.replace_lane 7)
  (func $_Z20replace_lane_i32x4_o11wav_i32x4_ti (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i32x4.replace_lane 3)
  (func $_Z20replace_lane_i64x2_o11wav_i64x2_tx (type 1) (param v128 i64) (result v128)
    local.get 0
    local.get 1
    i64x2.replace_lane 1)
  (func $_Z20replace_lane_u8x16_o11wav_u8x16_th (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i8x16.replace_lane 15)
  (func $_Z20replace_lane_u16x8_o11wav_u16x8_tt (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i16x8.replace_lane 7)
  (func $_Z20replace_lane_u32x4_o11wav_u32x4_tj (type 0) (param v128 i32) (result v128)
    local.get 0
    local.get 1
    i32x4.replace_lane 3)
  (func $_Z20replace_lane_u64x2_o11wav_u64x2_ty (type 1) (param v128 i64) (result v128)
    local.get 0
    local.get 1
    i64x2.replace_lane 1)
  (func $_Z20replace_lane_f32x4_o11wav_f32x4_tf (type 2) (param v128 f32) (result v128)
    local.get 0
    local.get 1
    f32x4.replace_lane 3)
  (func $_Z20replace_lane_f64x2_o11wav_f64x2_td (type 3) (param v128 f64) (result v128)
    local.get 0
    local.get 1
    f64x2.replace_lane 1))
