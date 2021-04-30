(module
 (type $v128_v128_=>_v128 (func (param v128 v128) (result v128)))
 (import "env" "__linear_memory" (memory $mimport$0 0))
 (func $0 (param $0 v128) (param $1 v128) (result v128)
  (i8x16.shuffle 15 14 13 12 11 10 9 8 7 6 8 4 3 2 1 0
   (local.get $0)
   (local.get $0)
  )
 )
 (func $1 (param $0 v128) (param $1 v128) (result v128)
  (i8x16.shuffle 14 15 12 13 16 17 8 9 6 7 4 5 2 3 0 1
   (local.get $0)
   (local.get $1)
  )
 )
 (func $2 (param $0 v128) (param $1 v128) (result v128)
  (i8x16.shuffle 12 13 14 15 8 9 10 11 4 5 6 7 0 1 2 3
   (local.get $0)
   (local.get $0)
  )
 )
 (func $3 (param $0 v128) (param $1 v128) (result v128)
  (i8x16.shuffle 8 9 10 11 12 13 14 15 0 1 2 3 4 5 6 7
   (local.get $0)
   (local.get $0)
  )
 )
 (func $4 (param $0 v128) (param $1 v128) (result v128)
  (i8x16.shuffle 15 14 13 12 11 10 9 8 7 6 8 4 3 2 1 0
   (local.get $0)
   (local.get $0)
  )
 )
 (func $5 (param $0 v128) (param $1 v128) (result v128)
  (i8x16.shuffle 14 15 12 13 16 17 8 9 6 7 4 5 2 3 0 1
   (local.get $0)
   (local.get $1)
  )
 )
 (func $6 (param $0 v128) (param $1 v128) (result v128)
  (i8x16.shuffle 12 13 14 15 8 9 10 11 4 5 6 7 0 1 2 3
   (local.get $0)
   (local.get $0)
  )
 )
 (func $7 (param $0 v128) (param $1 v128) (result v128)
  (i8x16.shuffle 8 9 10 11 12 13 14 15 0 1 2 3 4 5 6 7
   (local.get $0)
   (local.get $0)
  )
 )
 (func $8 (param $0 v128) (param $1 v128) (result v128)
  (i8x16.shuffle 12 13 14 15 8 9 10 11 4 5 6 7 0 1 2 3
   (local.get $0)
   (local.get $0)
  )
 )
 (func $9 (param $0 v128) (param $1 v128) (result v128)
  (i8x16.shuffle 8 9 10 11 12 13 14 15 0 1 2 3 4 5 6 7
   (local.get $0)
   (local.get $0)
  )
 )
 (func $10 (param $0 v128) (param $1 v128) (result v128)
  (i8x16.shuffle 15 14 13 12 11 10 9 8 7 6 8 4 3 2 1 0
   (local.get $0)
   (local.get $0)
  )
 )
 (func $11 (param $0 v128) (param $1 v128) (result v128)
  (i8x16.shuffle 14 15 12 13 16 17 8 9 6 7 4 5 2 3 0 1
   (local.get $0)
   (local.get $1)
  )
 )
 (func $12 (param $0 v128) (param $1 v128) (result v128)
  (i8x16.shuffle 12 13 14 15 8 9 10 11 4 5 6 7 0 1 2 3
   (local.get $0)
   (local.get $0)
  )
 )
 (func $13 (param $0 v128) (param $1 v128) (result v128)
  (i8x16.shuffle 8 9 10 11 12 13 14 15 0 1 2 3 4 5 6 7
   (local.get $0)
   (local.get $0)
  )
 )
 (func $14 (param $0 v128) (param $1 v128) (result v128)
  (i8x16.shuffle 15 14 13 12 11 10 9 8 7 6 8 4 3 2 1 0
   (local.get $0)
   (local.get $0)
  )
 )
 (func $15 (param $0 v128) (param $1 v128) (result v128)
  (i8x16.shuffle 14 15 12 13 16 17 8 9 6 7 4 5 2 3 0 1
   (local.get $0)
   (local.get $1)
  )
 )
 (func $16 (param $0 v128) (param $1 v128) (result v128)
  (i8x16.shuffle 12 13 14 15 8 9 10 11 4 5 6 7 0 1 2 3
   (local.get $0)
   (local.get $0)
  )
 )
 (func $17 (param $0 v128) (param $1 v128) (result v128)
  (i8x16.shuffle 8 9 10 11 12 13 14 15 0 1 2 3 4 5 6 7
   (local.get $0)
   (local.get $0)
  )
 )
 (func $18 (param $0 v128) (param $1 v128) (result v128)
  (i8x16.shuffle 12 13 14 15 8 9 10 11 4 5 6 7 0 1 2 3
   (local.get $0)
   (local.get $0)
  )
 )
 (func $19 (param $0 v128) (param $1 v128) (result v128)
  (i8x16.shuffle 8 9 10 11 12 13 14 15 0 1 2 3 4 5 6 7
   (local.get $0)
   (local.get $0)
  )
 )
 ;; custom section "linking", size 368
 ;; custom section "producers", size 157
 ;; features section: simd
)

