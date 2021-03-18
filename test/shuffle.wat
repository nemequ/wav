(module
  (type (;0;) (func (param v128 v128) (result v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $shuffle_i8x16 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 0
    i8x16.shuffle 15 14 13 12 11 10 9 8 7 6 8 4 3 2 1 0)
  (func $shuffle_i16x8 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.shuffle 14 15 12 13 16 17 8 9 6 7 4 5 2 3 0 1)
  (func $shuffle_i32x4 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 0
    i8x16.shuffle 12 13 14 15 8 9 10 11 4 5 6 7 0 1 2 3)
  (func $shuffle_i64x2 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 0
    i8x16.shuffle 8 9 10 11 12 13 14 15 0 1 2 3 4 5 6 7)
  (func $shuffle_u8x16 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 0
    i8x16.shuffle 15 14 13 12 11 10 9 8 7 6 8 4 3 2 1 0)
  (func $shuffle_u16x8 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.shuffle 14 15 12 13 16 17 8 9 6 7 4 5 2 3 0 1)
  (func $shuffle_u32x4 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 0
    i8x16.shuffle 12 13 14 15 8 9 10 11 4 5 6 7 0 1 2 3)
  (func $shuffle_u64x2 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 0
    i8x16.shuffle 8 9 10 11 12 13 14 15 0 1 2 3 4 5 6 7)
  (func $shuffle_f32x4 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 0
    i8x16.shuffle 12 13 14 15 8 9 10 11 4 5 6 7 0 1 2 3)
  (func $shuffle_f64x2 (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 0
    i8x16.shuffle 8 9 10 11 12 13 14 15 0 1 2 3 4 5 6 7)
  (func $shuffle_i8x16_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 0
    i8x16.shuffle 15 14 13 12 11 10 9 8 7 6 8 4 3 2 1 0)
  (func $shuffle_i16x8_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.shuffle 14 15 12 13 16 17 8 9 6 7 4 5 2 3 0 1)
  (func $shuffle_i32x4_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 0
    i8x16.shuffle 12 13 14 15 8 9 10 11 4 5 6 7 0 1 2 3)
  (func $shuffle_i64x2_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 0
    i8x16.shuffle 8 9 10 11 12 13 14 15 0 1 2 3 4 5 6 7)
  (func $shuffle_u8x16_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 0
    i8x16.shuffle 15 14 13 12 11 10 9 8 7 6 8 4 3 2 1 0)
  (func $shuffle_u16x8_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.shuffle 14 15 12 13 16 17 8 9 6 7 4 5 2 3 0 1)
  (func $shuffle_u32x4_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 0
    i8x16.shuffle 12 13 14 15 8 9 10 11 4 5 6 7 0 1 2 3)
  (func $shuffle_u64x2_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 0
    i8x16.shuffle 8 9 10 11 12 13 14 15 0 1 2 3 4 5 6 7)
  (func $shuffle_f32x4_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 0
    i8x16.shuffle 12 13 14 15 8 9 10 11 4 5 6 7 0 1 2 3)
  (func $shuffle_f64x2_o (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 0
    i8x16.shuffle 8 9 10 11 12 13 14 15 0 1 2 3 4 5 6 7))
