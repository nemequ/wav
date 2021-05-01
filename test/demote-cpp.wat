(module
 (type $v128_=>_v128 (func (param v128) (result v128)))
 (import "env" "__linear_memory" (memory $mimport$0 0))
 (func $0 (param $0 v128) (result v128)
  (f32x4.demote_f64x2_zero
   (local.get $0)
  )
 )
 (func $1 (param $0 v128) (result v128)
  (f32x4.demote_f64x2_zero
   (local.get $0)
  )
 )
 ;; custom section "linking", size 76
 ;; custom section "producers", size 157
 ;; features section: simd
)

