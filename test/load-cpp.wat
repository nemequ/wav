(module
 (type $i32_=>_v128 (func (param i32) (result v128)))
 (import "env" "__linear_memory" (memory $mimport$0 0))
 (func $0 (param $0 i32) (result v128)
  (v128.load align=1
   (local.get $0)
  )
 )
 (func $1 (param $0 i32) (result v128)
  (v128.load align=2
   (local.get $0)
  )
 )
 (func $2 (param $0 i32) (result v128)
  (v128.load align=4
   (local.get $0)
  )
 )
 (func $3 (param $0 i32) (result v128)
  (v128.load align=8
   (local.get $0)
  )
 )
 (func $4 (param $0 i32) (result v128)
  (v128.load align=1
   (local.get $0)
  )
 )
 (func $5 (param $0 i32) (result v128)
  (v128.load align=2
   (local.get $0)
  )
 )
 (func $6 (param $0 i32) (result v128)
  (v128.load align=4
   (local.get $0)
  )
 )
 (func $7 (param $0 i32) (result v128)
  (v128.load align=8
   (local.get $0)
  )
 )
 (func $8 (param $0 i32) (result v128)
  (v128.load align=4
   (local.get $0)
  )
 )
 (func $9 (param $0 i32) (result v128)
  (v128.load align=8
   (local.get $0)
  )
 )
 (func $10 (param $0 i32) (result v128)
  (v128.load align=1
   (local.get $0)
  )
 )
 (func $11 (param $0 i32) (result v128)
  (v128.load align=2
   (local.get $0)
  )
 )
 (func $12 (param $0 i32) (result v128)
  (v128.load align=4
   (local.get $0)
  )
 )
 (func $13 (param $0 i32) (result v128)
  (v128.load align=8
   (local.get $0)
  )
 )
 (func $14 (param $0 i32) (result v128)
  (v128.load align=1
   (local.get $0)
  )
 )
 (func $15 (param $0 i32) (result v128)
  (v128.load align=2
   (local.get $0)
  )
 )
 (func $16 (param $0 i32) (result v128)
  (v128.load align=4
   (local.get $0)
  )
 )
 (func $17 (param $0 i32) (result v128)
  (v128.load align=8
   (local.get $0)
  )
 )
 (func $18 (param $0 i32) (result v128)
  (v128.load align=4
   (local.get $0)
  )
 )
 (func $19 (param $0 i32) (result v128)
  (v128.load align=8
   (local.get $0)
  )
 )
 ;; custom section "linking", size 428
 ;; custom section "producers", size 157
 ;; features section: simd
)

