(module
 (type $i32_=>_v128 (func (param i32) (result v128)))
 (import "env" "__linear_memory" (memory $mimport$0 0))
 (func $0 (param $0 i32) (result v128)
  (v128.load8_splat
   (local.get $0)
  )
 )
 (func $1 (param $0 i32) (result v128)
  (v128.load16_splat align=1
   (local.get $0)
  )
 )
 (func $2 (param $0 i32) (result v128)
  (v128.load32_splat align=1
   (local.get $0)
  )
 )
 (func $3 (param $0 i32) (result v128)
  (v128.load64_splat align=1
   (local.get $0)
  )
 )
 (func $4 (param $0 i32) (result v128)
  (v128.load8_splat
   (local.get $0)
  )
 )
 (func $5 (param $0 i32) (result v128)
  (v128.load16_splat align=1
   (local.get $0)
  )
 )
 (func $6 (param $0 i32) (result v128)
  (v128.load32_splat align=1
   (local.get $0)
  )
 )
 (func $7 (param $0 i32) (result v128)
  (v128.load64_splat align=1
   (local.get $0)
  )
 )
 (func $8 (param $0 i32) (result v128)
  (v128.load32_splat align=1
   (local.get $0)
  )
 )
 (func $9 (param $0 i32) (result v128)
  (v128.load64_splat align=1
   (local.get $0)
  )
 )
 ;; custom section "linking", size 288
 ;; custom section "producers", size 157
 ;; features section: simd
)

