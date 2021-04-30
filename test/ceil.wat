(module
 (type $v128_=>_v128 (func (param v128) (result v128)))
 (import "env" "__linear_memory" (memory $mimport$0 0))
 (func $0 (param $0 v128) (result v128)
  (f32x4.ceil
   (local.get $0)
  )
 )
 (func $1 (param $0 v128) (result v128)
  (f64x2.ceil
   (local.get $0)
  )
 )
 (func $2 (param $0 v128) (result v128)
  (f32x4.ceil
   (local.get $0)
  )
 )
 (func $3 (param $0 v128) (result v128)
  (f64x2.ceil
   (local.get $0)
  )
 )
 ;; custom section "linking", size 68
 ;; custom section "producers", size 157
 ;; features section: simd
)

