(module
 (type $v128_=>_i32 (func (param v128) (result i32)))
 (type $v128_=>_i64 (func (param v128) (result i64)))
 (type $v128_=>_f32 (func (param v128) (result f32)))
 (type $v128_=>_f64 (func (param v128) (result f64)))
 (import "env" "__linear_memory" (memory $mimport$0 0))
 (func $0 (param $0 v128) (result i32)
  (i8x16.extract_lane_s 15
   (local.get $0)
  )
 )
 (func $1 (param $0 v128) (result i32)
  (i16x8.extract_lane_s 7
   (local.get $0)
  )
 )
 (func $2 (param $0 v128) (result i32)
  (i32x4.extract_lane 3
   (local.get $0)
  )
 )
 (func $3 (param $0 v128) (result i64)
  (i64x2.extract_lane 1
   (local.get $0)
  )
 )
 (func $4 (param $0 v128) (result i32)
  (i8x16.extract_lane_u 15
   (local.get $0)
  )
 )
 (func $5 (param $0 v128) (result i32)
  (i16x8.extract_lane_u 7
   (local.get $0)
  )
 )
 (func $6 (param $0 v128) (result i32)
  (i32x4.extract_lane 3
   (local.get $0)
  )
 )
 (func $7 (param $0 v128) (result i64)
  (i64x2.extract_lane 1
   (local.get $0)
  )
 )
 (func $8 (param $0 v128) (result f32)
  (f32x4.extract_lane 3
   (local.get $0)
  )
 )
 (func $9 (param $0 v128) (result f64)
  (f64x2.extract_lane 1
   (local.get $0)
  )
 )
 (func $10 (param $0 v128) (result i32)
  (i8x16.extract_lane_s 15
   (local.get $0)
  )
 )
 (func $11 (param $0 v128) (result i32)
  (i16x8.extract_lane_s 7
   (local.get $0)
  )
 )
 (func $12 (param $0 v128) (result i32)
  (i32x4.extract_lane 3
   (local.get $0)
  )
 )
 (func $13 (param $0 v128) (result i64)
  (i64x2.extract_lane 1
   (local.get $0)
  )
 )
 (func $14 (param $0 v128) (result i32)
  (i8x16.extract_lane_u 15
   (local.get $0)
  )
 )
 (func $15 (param $0 v128) (result i32)
  (i16x8.extract_lane_u 7
   (local.get $0)
  )
 )
 (func $16 (param $0 v128) (result i32)
  (i32x4.extract_lane 3
   (local.get $0)
  )
 )
 (func $17 (param $0 v128) (result i64)
  (i64x2.extract_lane 1
   (local.get $0)
  )
 )
 (func $18 (param $0 v128) (result f32)
  (f32x4.extract_lane 3
   (local.get $0)
  )
 )
 (func $19 (param $0 v128) (result f64)
  (f64x2.extract_lane 1
   (local.get $0)
  )
 )
 ;; custom section "linking", size 808
 ;; custom section "producers", size 157
 ;; features section: simd
)

