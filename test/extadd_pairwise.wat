(module
 (type $v128_=>_v128 (func (param v128) (result v128)))
 (import "env" "__linear_memory" (memory $mimport$0 0))
 (func $0 (param $0 v128) (result v128)
  (i16x8.extadd_pairwise_i8x16_s
   (local.get $0)
  )
 )
 (func $1 (param $0 v128) (result v128)
  (i32x4.extadd_pairwise_i16x8_s
   (local.get $0)
  )
 )
 (func $2 (param $0 v128) (result v128)
  (i16x8.extadd_pairwise_i8x16_u
   (local.get $0)
  )
 )
 (func $3 (param $0 v128) (result v128)
  (i32x4.extadd_pairwise_i16x8_u
   (local.get $0)
  )
 )
 (func $4 (param $0 v128) (result v128)
  (i16x8.extadd_pairwise_i8x16_s
   (local.get $0)
  )
 )
 (func $5 (param $0 v128) (result v128)
  (i32x4.extadd_pairwise_i16x8_s
   (local.get $0)
  )
 )
 (func $6 (param $0 v128) (result v128)
  (i16x8.extadd_pairwise_i8x16_u
   (local.get $0)
  )
 )
 (func $7 (param $0 v128) (result v128)
  (i32x4.extadd_pairwise_i16x8_u
   (local.get $0)
  )
 )
 ;; custom section "linking", size 216
 ;; custom section "producers", size 157
 ;; features section: simd
)
