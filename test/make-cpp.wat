(module
  (type (;0;) (func (param i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32) (result v128)))
  (type (;1;) (func (param i32 i32 i32 i32 i32 i32 i32 i32) (result v128)))
  (type (;2;) (func (param i32 i32 i32 i32) (result v128)))
  (type (;3;) (func (param i64 i64) (result v128)))
  (type (;4;) (func (param f32 f32 f32 f32) (result v128)))
  (type (;5;) (func (param f64 f64) (result v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $_Z10make_i8x16aaaaaaaaaaaaaaaa (type 0) (param i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32) (result v128)
    local.get 0
    i8x16.splat
    local.get 1
    i8x16.replace_lane 1
    local.get 2
    i8x16.replace_lane 2
    local.get 3
    i8x16.replace_lane 3
    local.get 4
    i8x16.replace_lane 4
    local.get 5
    i8x16.replace_lane 5
    local.get 6
    i8x16.replace_lane 6
    local.get 7
    i8x16.replace_lane 7
    local.get 8
    i8x16.replace_lane 8
    local.get 9
    i8x16.replace_lane 9
    local.get 10
    i8x16.replace_lane 10
    local.get 11
    i8x16.replace_lane 11
    local.get 12
    i8x16.replace_lane 12
    local.get 13
    i8x16.replace_lane 13
    local.get 14
    i8x16.replace_lane 14
    local.get 15
    i8x16.replace_lane 15)
  (func $_Z10make_i16x8ssssssss (type 1) (param i32 i32 i32 i32 i32 i32 i32 i32) (result v128)
    local.get 0
    i16x8.splat
    local.get 1
    i16x8.replace_lane 1
    local.get 2
    i16x8.replace_lane 2
    local.get 3
    i16x8.replace_lane 3
    local.get 4
    i16x8.replace_lane 4
    local.get 5
    i16x8.replace_lane 5
    local.get 6
    i16x8.replace_lane 6
    local.get 7
    i16x8.replace_lane 7)
  (func $_Z10make_i32x4iiii (type 2) (param i32 i32 i32 i32) (result v128)
    local.get 0
    i32x4.splat
    local.get 1
    i32x4.replace_lane 1
    local.get 2
    i32x4.replace_lane 2
    local.get 3
    i32x4.replace_lane 3)
  (func $_Z10make_i64x2xx (type 3) (param i64 i64) (result v128)
    local.get 0
    i64x2.splat
    local.get 1
    i64x2.replace_lane 1)
  (func $_Z10make_u8x16hhhhhhhhhhhhhhhh (type 0) (param i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32) (result v128)
    local.get 0
    i8x16.splat
    local.get 1
    i8x16.replace_lane 1
    local.get 2
    i8x16.replace_lane 2
    local.get 3
    i8x16.replace_lane 3
    local.get 4
    i8x16.replace_lane 4
    local.get 5
    i8x16.replace_lane 5
    local.get 6
    i8x16.replace_lane 6
    local.get 7
    i8x16.replace_lane 7
    local.get 8
    i8x16.replace_lane 8
    local.get 9
    i8x16.replace_lane 9
    local.get 10
    i8x16.replace_lane 10
    local.get 11
    i8x16.replace_lane 11
    local.get 12
    i8x16.replace_lane 12
    local.get 13
    i8x16.replace_lane 13
    local.get 14
    i8x16.replace_lane 14
    local.get 15
    i8x16.replace_lane 15)
  (func $_Z10make_u16x8tttttttt (type 1) (param i32 i32 i32 i32 i32 i32 i32 i32) (result v128)
    local.get 0
    i16x8.splat
    local.get 1
    i16x8.replace_lane 1
    local.get 2
    i16x8.replace_lane 2
    local.get 3
    i16x8.replace_lane 3
    local.get 4
    i16x8.replace_lane 4
    local.get 5
    i16x8.replace_lane 5
    local.get 6
    i16x8.replace_lane 6
    local.get 7
    i16x8.replace_lane 7)
  (func $_Z10make_u32x4jjjj (type 2) (param i32 i32 i32 i32) (result v128)
    local.get 0
    i32x4.splat
    local.get 1
    i32x4.replace_lane 1
    local.get 2
    i32x4.replace_lane 2
    local.get 3
    i32x4.replace_lane 3)
  (func $_Z10make_u64x2yy (type 3) (param i64 i64) (result v128)
    local.get 0
    i64x2.splat
    local.get 1
    i64x2.replace_lane 1)
  (func $_Z10make_f32x4ffff (type 4) (param f32 f32 f32 f32) (result v128)
    local.get 0
    f32x4.splat
    local.get 1
    f32x4.replace_lane 1
    local.get 2
    f32x4.replace_lane 2
    local.get 3
    f32x4.replace_lane 3)
  (func $_Z10make_f64x2dd (type 5) (param f64 f64) (result v128)
    local.get 0
    f64x2.splat
    local.get 1
    f64x2.replace_lane 1)
  (func $_Z12make_i8x16_oaaaaaaaaaaaaaaaa (type 0) (param i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32) (result v128)
    local.get 0
    i8x16.splat
    local.get 1
    i8x16.replace_lane 1
    local.get 2
    i8x16.replace_lane 2
    local.get 3
    i8x16.replace_lane 3
    local.get 4
    i8x16.replace_lane 4
    local.get 5
    i8x16.replace_lane 5
    local.get 6
    i8x16.replace_lane 6
    local.get 7
    i8x16.replace_lane 7
    local.get 8
    i8x16.replace_lane 8
    local.get 9
    i8x16.replace_lane 9
    local.get 10
    i8x16.replace_lane 10
    local.get 11
    i8x16.replace_lane 11
    local.get 12
    i8x16.replace_lane 12
    local.get 13
    i8x16.replace_lane 13
    local.get 14
    i8x16.replace_lane 14
    local.get 15
    i8x16.replace_lane 15)
  (func $_Z12make_i16x8_ossssssss (type 1) (param i32 i32 i32 i32 i32 i32 i32 i32) (result v128)
    local.get 0
    i16x8.splat
    local.get 1
    i16x8.replace_lane 1
    local.get 2
    i16x8.replace_lane 2
    local.get 3
    i16x8.replace_lane 3
    local.get 4
    i16x8.replace_lane 4
    local.get 5
    i16x8.replace_lane 5
    local.get 6
    i16x8.replace_lane 6
    local.get 7
    i16x8.replace_lane 7)
  (func $_Z12make_i32x4_oiiii (type 2) (param i32 i32 i32 i32) (result v128)
    local.get 0
    i32x4.splat
    local.get 1
    i32x4.replace_lane 1
    local.get 2
    i32x4.replace_lane 2
    local.get 3
    i32x4.replace_lane 3)
  (func $_Z12make_i64x2_oxx (type 3) (param i64 i64) (result v128)
    local.get 0
    i64x2.splat
    local.get 1
    i64x2.replace_lane 1)
  (func $_Z12make_u8x16_ohhhhhhhhhhhhhhhh (type 0) (param i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32) (result v128)
    local.get 0
    i8x16.splat
    local.get 1
    i8x16.replace_lane 1
    local.get 2
    i8x16.replace_lane 2
    local.get 3
    i8x16.replace_lane 3
    local.get 4
    i8x16.replace_lane 4
    local.get 5
    i8x16.replace_lane 5
    local.get 6
    i8x16.replace_lane 6
    local.get 7
    i8x16.replace_lane 7
    local.get 8
    i8x16.replace_lane 8
    local.get 9
    i8x16.replace_lane 9
    local.get 10
    i8x16.replace_lane 10
    local.get 11
    i8x16.replace_lane 11
    local.get 12
    i8x16.replace_lane 12
    local.get 13
    i8x16.replace_lane 13
    local.get 14
    i8x16.replace_lane 14
    local.get 15
    i8x16.replace_lane 15)
  (func $_Z12make_u16x8_otttttttt (type 1) (param i32 i32 i32 i32 i32 i32 i32 i32) (result v128)
    local.get 0
    i16x8.splat
    local.get 1
    i16x8.replace_lane 1
    local.get 2
    i16x8.replace_lane 2
    local.get 3
    i16x8.replace_lane 3
    local.get 4
    i16x8.replace_lane 4
    local.get 5
    i16x8.replace_lane 5
    local.get 6
    i16x8.replace_lane 6
    local.get 7
    i16x8.replace_lane 7)
  (func $_Z12make_u32x4_ojjjj (type 2) (param i32 i32 i32 i32) (result v128)
    local.get 0
    i32x4.splat
    local.get 1
    i32x4.replace_lane 1
    local.get 2
    i32x4.replace_lane 2
    local.get 3
    i32x4.replace_lane 3)
  (func $_Z12make_u64x2_oyy (type 3) (param i64 i64) (result v128)
    local.get 0
    i64x2.splat
    local.get 1
    i64x2.replace_lane 1)
  (func $_Z12make_f32x4_offff (type 4) (param f32 f32 f32 f32) (result v128)
    local.get 0
    f32x4.splat
    local.get 1
    f32x4.replace_lane 1
    local.get 2
    f32x4.replace_lane 2
    local.get 3
    f32x4.replace_lane 3)
  (func $_Z12make_f64x2_odd (type 5) (param f64 f64) (result v128)
    local.get 0
    f64x2.splat
    local.get 1
    f64x2.replace_lane 1))
