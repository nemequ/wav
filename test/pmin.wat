(module
 (type $v128_v128_=>_v128 (func (param v128 v128) (result v128)))
 (import "env" "__linear_memory" (memory $mimport$0 0))
 (func $0 (param $0 v128) (param $1 v128) (result v128)
  (f32x4.pmin
   (local.get $0)
   (local.get $1)
  )
 )
 (func $1 (param $0 v128) (param $1 v128) (result v128)
  (f64x2.pmin
   (local.get $0)
   (local.get $1)
  )
 )
 (func $2 (param $0 v128) (param $1 v128) (result v128)
  (f32x4.pmin
   (local.get $0)
   (local.get $1)
  )
 )
 (func $3 (param $0 v128) (param $1 v128) (result v128)
  (f64x2.pmin
   (local.get $0)
   (local.get $1)
  )
 )
 ;; custom section "linking", size 68
 ;; custom section "producers", size 157
 ;; features section: simd
)

