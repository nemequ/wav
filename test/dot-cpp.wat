(module
 (type $v128_v128_=>_v128 (func (param v128 v128) (result v128)))
 (import "env" "__linear_memory" (memory $mimport$0 0))
 (func $0 (param $0 v128) (param $1 v128) (result v128)
  (i32x4.dot_i16x8_s
   (local.get $0)
   (local.get $1)
  )
 )
 (func $1 (param $0 v128) (param $1 v128) (result v128)
  (i32x4.dot_i16x8_s
   (local.get $0)
   (local.get $1)
  )
 )
 ;; custom section "linking", size 73
 ;; custom section "producers", size 157
 ;; features section: simd
)
