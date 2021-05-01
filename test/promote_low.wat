(module
 (type $v128_=>_v128 (func (param v128) (result v128)))
 (import "env" "__linear_memory" (memory $mimport$0 0))
 (func $0 (param $0 v128) (result v128)
  (f64x2.promote_low_f32x4
   (local.get $0)
  )
 )
 (func $1 (param $0 v128) (result v128)
  (f64x2.promote_low_f32x4
   (local.get $0)
  )
 )
 ;; custom section "linking", size 52
 ;; custom section "producers", size 157
 ;; features section: simd
)

