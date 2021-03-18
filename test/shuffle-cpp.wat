(module
  (type (;0;) (func (param v128 v128) (result v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $_Z13shuffle_i8x1611wav_i8x16_tS_ (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 0
    i8x16.shuffle 15 14 13 12 11 10 9 8 7 6 8 4 3 2 1 0)
  (func $_Z13shuffle_i16x811wav_i16x8_tS_ (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.shuffle 14 15 12 13 16 17 8 9 6 7 4 5 2 3 0 1)
  (func $_Z13shuffle_i32x411wav_i32x4_tS_ (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 0
    i8x16.shuffle 12 13 14 15 8 9 10 11 4 5 6 7 0 1 2 3)
  (func $_Z13shuffle_i64x211wav_i64x2_tS_ (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 0
    i8x16.shuffle 8 9 10 11 12 13 14 15 0 1 2 3 4 5 6 7)
  (func $_Z13shuffle_u8x1611wav_u8x16_tS_ (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 0
    i8x16.shuffle 15 14 13 12 11 10 9 8 7 6 8 4 3 2 1 0)
  (func $_Z13shuffle_u16x811wav_u16x8_tS_ (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.shuffle 14 15 12 13 16 17 8 9 6 7 4 5 2 3 0 1)
  (func $_Z13shuffle_u32x411wav_u32x4_tS_ (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 0
    i8x16.shuffle 12 13 14 15 8 9 10 11 4 5 6 7 0 1 2 3)
  (func $_Z13shuffle_u64x211wav_u64x2_tS_ (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 0
    i8x16.shuffle 8 9 10 11 12 13 14 15 0 1 2 3 4 5 6 7)
  (func $_Z13shuffle_f32x411wav_f32x4_tS_ (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 0
    i8x16.shuffle 12 13 14 15 8 9 10 11 4 5 6 7 0 1 2 3)
  (func $_Z13shuffle_f64x211wav_f64x2_tS_ (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 0
    i8x16.shuffle 8 9 10 11 12 13 14 15 0 1 2 3 4 5 6 7)
  (func $_Z15shuffle_i8x16_o11wav_i8x16_tS_ (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 0
    i8x16.shuffle 15 14 13 12 11 10 9 8 7 6 8 4 3 2 1 0)
  (func $_Z15shuffle_i16x8_o11wav_i16x8_tS_ (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.shuffle 14 15 12 13 16 17 8 9 6 7 4 5 2 3 0 1)
  (func $_Z15shuffle_i32x4_o11wav_i32x4_tS_ (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 0
    i8x16.shuffle 12 13 14 15 8 9 10 11 4 5 6 7 0 1 2 3)
  (func $_Z15shuffle_i64x2_o11wav_i64x2_tS_ (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 0
    i8x16.shuffle 8 9 10 11 12 13 14 15 0 1 2 3 4 5 6 7)
  (func $_Z15shuffle_u8x16_o11wav_u8x16_tS_ (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 0
    i8x16.shuffle 15 14 13 12 11 10 9 8 7 6 8 4 3 2 1 0)
  (func $_Z15shuffle_u16x8_o11wav_u16x8_tS_ (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.shuffle 14 15 12 13 16 17 8 9 6 7 4 5 2 3 0 1)
  (func $_Z15shuffle_u32x4_o11wav_u32x4_tS_ (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 0
    i8x16.shuffle 12 13 14 15 8 9 10 11 4 5 6 7 0 1 2 3)
  (func $_Z15shuffle_u64x2_o11wav_u64x2_tS_ (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 0
    i8x16.shuffle 8 9 10 11 12 13 14 15 0 1 2 3 4 5 6 7)
  (func $_Z15shuffle_f32x4_o11wav_f32x4_tS_ (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 0
    i8x16.shuffle 12 13 14 15 8 9 10 11 4 5 6 7 0 1 2 3)
  (func $_Z15shuffle_f64x2_o11wav_f64x2_tS_ (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 0
    i8x16.shuffle 8 9 10 11 12 13 14 15 0 1 2 3 4 5 6 7))
