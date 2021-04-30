(module
 (type $v128_i32_=>_v128 (func (param v128 i32) (result v128)))
 (type $v128_i64_=>_v128 (func (param v128 i64) (result v128)))
 (type $v128_f32_=>_v128 (func (param v128 f32) (result v128)))
 (type $v128_f64_=>_v128 (func (param v128 f64) (result v128)))
 (import "env" "__linear_memory" (memory $mimport$0 0))
 (import "env" "__stack_pointer" (global $gimport$0 (mut i32)))
 (func $0 (param $0 v128) (param $1 i32) (result v128)
  (local $2 i32)
  (i32.store8 offset=15
   (local.tee $2
    (i32.sub
     (global.get $gimport$0)
     (i32.const 16)
    )
   )
   (local.get $1)
  )
  (v128.load8_lane offset=15 15
   (local.get $2)
   (local.get $0)
  )
 )
 (func $1 (param $0 v128) (param $1 i32) (result v128)
  (local $2 i32)
  (i32.store16 offset=14
   (local.tee $2
    (i32.sub
     (global.get $gimport$0)
     (i32.const 16)
    )
   )
   (local.get $1)
  )
  (v128.load16_lane offset=14 7
   (local.get $2)
   (local.get $0)
  )
 )
 (func $2 (param $0 v128) (param $1 i32) (result v128)
  (local $2 i32)
  (i32.store offset=12
   (local.tee $2
    (i32.sub
     (global.get $gimport$0)
     (i32.const 16)
    )
   )
   (local.get $1)
  )
  (v128.load32_lane offset=12 3
   (local.get $2)
   (local.get $0)
  )
 )
 (func $3 (param $0 v128) (param $1 i64) (result v128)
  (local $2 i32)
  (i64.store offset=8
   (local.tee $2
    (i32.sub
     (global.get $gimport$0)
     (i32.const 16)
    )
   )
   (local.get $1)
  )
  (v128.load64_lane offset=8 1
   (local.get $2)
   (local.get $0)
  )
 )
 (func $4 (param $0 v128) (param $1 i32) (result v128)
  (local $2 i32)
  (i32.store8 offset=15
   (local.tee $2
    (i32.sub
     (global.get $gimport$0)
     (i32.const 16)
    )
   )
   (local.get $1)
  )
  (v128.load8_lane offset=15 15
   (local.get $2)
   (local.get $0)
  )
 )
 (func $5 (param $0 v128) (param $1 i32) (result v128)
  (local $2 i32)
  (i32.store16 offset=14
   (local.tee $2
    (i32.sub
     (global.get $gimport$0)
     (i32.const 16)
    )
   )
   (local.get $1)
  )
  (v128.load16_lane offset=14 7
   (local.get $2)
   (local.get $0)
  )
 )
 (func $6 (param $0 v128) (param $1 i32) (result v128)
  (local $2 i32)
  (i32.store offset=12
   (local.tee $2
    (i32.sub
     (global.get $gimport$0)
     (i32.const 16)
    )
   )
   (local.get $1)
  )
  (v128.load32_lane offset=12 3
   (local.get $2)
   (local.get $0)
  )
 )
 (func $7 (param $0 v128) (param $1 i64) (result v128)
  (local $2 i32)
  (i64.store offset=8
   (local.tee $2
    (i32.sub
     (global.get $gimport$0)
     (i32.const 16)
    )
   )
   (local.get $1)
  )
  (v128.load64_lane offset=8 1
   (local.get $2)
   (local.get $0)
  )
 )
 (func $8 (param $0 v128) (param $1 f32) (result v128)
  (local $2 i32)
  (f32.store offset=12
   (local.tee $2
    (i32.sub
     (global.get $gimport$0)
     (i32.const 16)
    )
   )
   (local.get $1)
  )
  (v128.load32_lane offset=12 3
   (local.get $2)
   (local.get $0)
  )
 )
 (func $9 (param $0 v128) (param $1 f64) (result v128)
  (local $2 i32)
  (f64.store offset=8
   (local.tee $2
    (i32.sub
     (global.get $gimport$0)
     (i32.const 16)
    )
   )
   (local.get $1)
  )
  (v128.load64_lane offset=8 1
   (local.get $2)
   (local.get $0)
  )
 )
 ;; custom section "linking", size 201
 ;; custom section "reloc.CODE", size 37
 ;; custom section "producers", size 157
 ;; features section: simd
)

