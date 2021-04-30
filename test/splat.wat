(module
 (type $i32_=>_v128 (func (param i32) (result v128)))
 (type $i64_=>_v128 (func (param i64) (result v128)))
 (type $f32_=>_v128 (func (param f32) (result v128)))
 (type $f64_=>_v128 (func (param f64) (result v128)))
 (import "env" "__linear_memory" (memory $mimport$0 0))
 (func $0 (param $0 i32) (result v128)
  (i8x16.splat
   (local.get $0)
  )
 )
 (func $1 (param $0 i32) (result v128)
  (i16x8.splat
   (local.get $0)
  )
 )
 (func $2 (param $0 i32) (result v128)
  (i32x4.splat
   (local.get $0)
  )
 )
 (func $3 (param $0 i64) (result v128)
  (i64x2.splat
   (local.get $0)
  )
 )
 (func $4 (param $0 i32) (result v128)
  (i8x16.splat
   (local.get $0)
  )
 )
 (func $5 (param $0 i32) (result v128)
  (i16x8.splat
   (local.get $0)
  )
 )
 (func $6 (param $0 i32) (result v128)
  (i32x4.splat
   (local.get $0)
  )
 )
 (func $7 (param $0 i64) (result v128)
  (i64x2.splat
   (local.get $0)
  )
 )
 (func $8 (param $0 f32) (result v128)
  (f32x4.splat
   (local.get $0)
  )
 )
 (func $9 (param $0 f64) (result v128)
  (f64x2.splat
   (local.get $0)
  )
 )
 (func $10 (param $0 i32) (result v128)
  (i8x16.splat
   (local.get $0)
  )
 )
 (func $11 (param $0 i32) (result v128)
  (i16x8.splat
   (local.get $0)
  )
 )
 (func $12 (param $0 i32) (result v128)
  (i32x4.splat
   (local.get $0)
  )
 )
 (func $13 (param $0 i64) (result v128)
  (i64x2.splat
   (local.get $0)
  )
 )
 (func $14 (param $0 i32) (result v128)
  (i8x16.splat
   (local.get $0)
  )
 )
 (func $15 (param $0 i32) (result v128)
  (i16x8.splat
   (local.get $0)
  )
 )
 (func $16 (param $0 i32) (result v128)
  (i32x4.splat
   (local.get $0)
  )
 )
 (func $17 (param $0 i64) (result v128)
  (i64x2.splat
   (local.get $0)
  )
 )
 (func $18 (param $0 f32) (result v128)
  (f32x4.splat
   (local.get $0)
  )
 )
 (func $19 (param $0 f64) (result v128)
  (f64x2.splat
   (local.get $0)
  )
 )
 ;; custom section "linking", size 328
 ;; custom section "producers", size 157
 ;; features section: simd
)

