(module
 (type $i32_v128_=>_none (func (param i32 v128)))
 (import "env" "__linear_memory" (memory $mimport$0 0))
 (func $0 (param $0 i32) (param $1 v128)
  (v128.store align=1
   (local.get $0)
   (local.get $1)
  )
 )
 (func $1 (param $0 i32) (param $1 v128)
  (v128.store align=2
   (local.get $0)
   (local.get $1)
  )
 )
 (func $2 (param $0 i32) (param $1 v128)
  (v128.store align=4
   (local.get $0)
   (local.get $1)
  )
 )
 (func $3 (param $0 i32) (param $1 v128)
  (v128.store align=8
   (local.get $0)
   (local.get $1)
  )
 )
 (func $4 (param $0 i32) (param $1 v128)
  (v128.store align=1
   (local.get $0)
   (local.get $1)
  )
 )
 (func $5 (param $0 i32) (param $1 v128)
  (v128.store align=2
   (local.get $0)
   (local.get $1)
  )
 )
 (func $6 (param $0 i32) (param $1 v128)
  (v128.store align=4
   (local.get $0)
   (local.get $1)
  )
 )
 (func $7 (param $0 i32) (param $1 v128)
  (v128.store align=8
   (local.get $0)
   (local.get $1)
  )
 )
 (func $8 (param $0 i32) (param $1 v128)
  (v128.store align=4
   (local.get $0)
   (local.get $1)
  )
 )
 (func $9 (param $0 i32) (param $1 v128)
  (v128.store align=8
   (local.get $0)
   (local.get $1)
  )
 )
 (func $10 (param $0 i32) (param $1 v128)
  (v128.store align=1
   (local.get $0)
   (local.get $1)
  )
 )
 (func $11 (param $0 i32) (param $1 v128)
  (v128.store align=2
   (local.get $0)
   (local.get $1)
  )
 )
 (func $12 (param $0 i32) (param $1 v128)
  (v128.store align=4
   (local.get $0)
   (local.get $1)
  )
 )
 (func $13 (param $0 i32) (param $1 v128)
  (v128.store align=8
   (local.get $0)
   (local.get $1)
  )
 )
 (func $14 (param $0 i32) (param $1 v128)
  (v128.store align=1
   (local.get $0)
   (local.get $1)
  )
 )
 (func $15 (param $0 i32) (param $1 v128)
  (v128.store align=2
   (local.get $0)
   (local.get $1)
  )
 )
 (func $16 (param $0 i32) (param $1 v128)
  (v128.store align=4
   (local.get $0)
   (local.get $1)
  )
 )
 (func $17 (param $0 i32) (param $1 v128)
  (v128.store align=8
   (local.get $0)
   (local.get $1)
  )
 )
 (func $18 (param $0 i32) (param $1 v128)
  (v128.store align=4
   (local.get $0)
   (local.get $1)
  )
 )
 (func $19 (param $0 i32) (param $1 v128)
  (v128.store align=8
   (local.get $0)
   (local.get $1)
  )
 )
 ;; custom section "linking", size 328
 ;; custom section "producers", size 157
 ;; features section: simd
)

