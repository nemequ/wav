(module
 (type $v128_=>_v128 (func (param v128) (result v128)))
 (import "env" "__linear_memory" (memory $mimport$0 0))
 (func $0 (param $0 v128) (result v128)
  (i8x16.abs
   (local.get $0)
  )
 )
 (func $1 (param $0 v128) (result v128)
  (i16x8.abs
   (local.get $0)
  )
 )
 (func $2 (param $0 v128) (result v128)
  (i32x4.abs
   (local.get $0)
  )
 )
 (func $3 (param $0 v128) (result v128)
  (f32x4.abs
   (local.get $0)
  )
 )
 (func $4 (param $0 v128) (result v128)
  (f64x2.abs
   (local.get $0)
  )
 )
 (func $5 (param $0 v128) (result v128)
  (i8x16.abs
   (local.get $0)
  )
 )
 (func $6 (param $0 v128) (result v128)
  (i16x8.abs
   (local.get $0)
  )
 )
 (func $7 (param $0 v128) (result v128)
  (i32x4.abs
   (local.get $0)
  )
 )
 (func $8 (param $0 v128) (result v128)
  (f32x4.abs
   (local.get $0)
  )
 )
 (func $9 (param $0 v128) (result v128)
  (f64x2.abs
   (local.get $0)
  )
 )
 ;; custom section "linking", size 313
 ;; custom section "producers", size 157
 ;; features section: simd
)

