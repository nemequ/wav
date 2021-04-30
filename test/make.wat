(module
 (type $i32_i32_i32_i32_=>_v128 (func (param i32 i32 i32 i32) (result v128)))
 (type $i32_i32_i32_i32_i32_i32_i32_i32_=>_v128 (func (param i32 i32 i32 i32 i32 i32 i32 i32) (result v128)))
 (type $i32_i32_i32_i32_i32_i32_i32_i32_i32_i32_i32_i32_i32_i32_i32_i32_=>_v128 (func (param i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32) (result v128)))
 (type $i64_i64_=>_v128 (func (param i64 i64) (result v128)))
 (type $f32_f32_f32_f32_=>_v128 (func (param f32 f32 f32 f32) (result v128)))
 (type $f64_f64_=>_v128 (func (param f64 f64) (result v128)))
 (import "env" "__linear_memory" (memory $mimport$0 0))
 (func $0 (param $0 i32) (param $1 i32) (param $2 i32) (param $3 i32) (param $4 i32) (param $5 i32) (param $6 i32) (param $7 i32) (param $8 i32) (param $9 i32) (param $10 i32) (param $11 i32) (param $12 i32) (param $13 i32) (param $14 i32) (param $15 i32) (result v128)
  (i8x16.replace_lane 15
   (i8x16.replace_lane 14
    (i8x16.replace_lane 13
     (i8x16.replace_lane 12
      (i8x16.replace_lane 11
       (i8x16.replace_lane 10
        (i8x16.replace_lane 9
         (i8x16.replace_lane 8
          (i8x16.replace_lane 7
           (i8x16.replace_lane 6
            (i8x16.replace_lane 5
             (i8x16.replace_lane 4
              (i8x16.replace_lane 3
               (i8x16.replace_lane 2
                (i8x16.replace_lane 1
                 (i8x16.splat
                  (local.get $0)
                 )
                 (local.get $1)
                )
                (local.get $2)
               )
               (local.get $3)
              )
              (local.get $4)
             )
             (local.get $5)
            )
            (local.get $6)
           )
           (local.get $7)
          )
          (local.get $8)
         )
         (local.get $9)
        )
        (local.get $10)
       )
       (local.get $11)
      )
      (local.get $12)
     )
     (local.get $13)
    )
    (local.get $14)
   )
   (local.get $15)
  )
 )
 (func $1 (param $0 i32) (param $1 i32) (param $2 i32) (param $3 i32) (param $4 i32) (param $5 i32) (param $6 i32) (param $7 i32) (result v128)
  (i16x8.replace_lane 7
   (i16x8.replace_lane 6
    (i16x8.replace_lane 5
     (i16x8.replace_lane 4
      (i16x8.replace_lane 3
       (i16x8.replace_lane 2
        (i16x8.replace_lane 1
         (i16x8.splat
          (local.get $0)
         )
         (local.get $1)
        )
        (local.get $2)
       )
       (local.get $3)
      )
      (local.get $4)
     )
     (local.get $5)
    )
    (local.get $6)
   )
   (local.get $7)
  )
 )
 (func $2 (param $0 i32) (param $1 i32) (param $2 i32) (param $3 i32) (result v128)
  (i32x4.replace_lane 3
   (i32x4.replace_lane 2
    (i32x4.replace_lane 1
     (i32x4.splat
      (local.get $0)
     )
     (local.get $1)
    )
    (local.get $2)
   )
   (local.get $3)
  )
 )
 (func $3 (param $0 i64) (param $1 i64) (result v128)
  (i64x2.replace_lane 1
   (i64x2.splat
    (local.get $0)
   )
   (local.get $1)
  )
 )
 (func $4 (param $0 i32) (param $1 i32) (param $2 i32) (param $3 i32) (param $4 i32) (param $5 i32) (param $6 i32) (param $7 i32) (param $8 i32) (param $9 i32) (param $10 i32) (param $11 i32) (param $12 i32) (param $13 i32) (param $14 i32) (param $15 i32) (result v128)
  (i8x16.replace_lane 15
   (i8x16.replace_lane 14
    (i8x16.replace_lane 13
     (i8x16.replace_lane 12
      (i8x16.replace_lane 11
       (i8x16.replace_lane 10
        (i8x16.replace_lane 9
         (i8x16.replace_lane 8
          (i8x16.replace_lane 7
           (i8x16.replace_lane 6
            (i8x16.replace_lane 5
             (i8x16.replace_lane 4
              (i8x16.replace_lane 3
               (i8x16.replace_lane 2
                (i8x16.replace_lane 1
                 (i8x16.splat
                  (local.get $0)
                 )
                 (local.get $1)
                )
                (local.get $2)
               )
               (local.get $3)
              )
              (local.get $4)
             )
             (local.get $5)
            )
            (local.get $6)
           )
           (local.get $7)
          )
          (local.get $8)
         )
         (local.get $9)
        )
        (local.get $10)
       )
       (local.get $11)
      )
      (local.get $12)
     )
     (local.get $13)
    )
    (local.get $14)
   )
   (local.get $15)
  )
 )
 (func $5 (param $0 i32) (param $1 i32) (param $2 i32) (param $3 i32) (param $4 i32) (param $5 i32) (param $6 i32) (param $7 i32) (result v128)
  (i16x8.replace_lane 7
   (i16x8.replace_lane 6
    (i16x8.replace_lane 5
     (i16x8.replace_lane 4
      (i16x8.replace_lane 3
       (i16x8.replace_lane 2
        (i16x8.replace_lane 1
         (i16x8.splat
          (local.get $0)
         )
         (local.get $1)
        )
        (local.get $2)
       )
       (local.get $3)
      )
      (local.get $4)
     )
     (local.get $5)
    )
    (local.get $6)
   )
   (local.get $7)
  )
 )
 (func $6 (param $0 i32) (param $1 i32) (param $2 i32) (param $3 i32) (result v128)
  (i32x4.replace_lane 3
   (i32x4.replace_lane 2
    (i32x4.replace_lane 1
     (i32x4.splat
      (local.get $0)
     )
     (local.get $1)
    )
    (local.get $2)
   )
   (local.get $3)
  )
 )
 (func $7 (param $0 i64) (param $1 i64) (result v128)
  (i64x2.replace_lane 1
   (i64x2.splat
    (local.get $0)
   )
   (local.get $1)
  )
 )
 (func $8 (param $0 f32) (param $1 f32) (param $2 f32) (param $3 f32) (result v128)
  (f32x4.replace_lane 3
   (f32x4.replace_lane 2
    (f32x4.replace_lane 1
     (f32x4.splat
      (local.get $0)
     )
     (local.get $1)
    )
    (local.get $2)
   )
   (local.get $3)
  )
 )
 (func $9 (param $0 f64) (param $1 f64) (result v128)
  (f64x2.replace_lane 1
   (f64x2.splat
    (local.get $0)
   )
   (local.get $1)
  )
 )
 (func $10 (param $0 i32) (param $1 i32) (param $2 i32) (param $3 i32) (param $4 i32) (param $5 i32) (param $6 i32) (param $7 i32) (param $8 i32) (param $9 i32) (param $10 i32) (param $11 i32) (param $12 i32) (param $13 i32) (param $14 i32) (param $15 i32) (result v128)
  (i8x16.replace_lane 15
   (i8x16.replace_lane 14
    (i8x16.replace_lane 13
     (i8x16.replace_lane 12
      (i8x16.replace_lane 11
       (i8x16.replace_lane 10
        (i8x16.replace_lane 9
         (i8x16.replace_lane 8
          (i8x16.replace_lane 7
           (i8x16.replace_lane 6
            (i8x16.replace_lane 5
             (i8x16.replace_lane 4
              (i8x16.replace_lane 3
               (i8x16.replace_lane 2
                (i8x16.replace_lane 1
                 (i8x16.splat
                  (local.get $0)
                 )
                 (local.get $1)
                )
                (local.get $2)
               )
               (local.get $3)
              )
              (local.get $4)
             )
             (local.get $5)
            )
            (local.get $6)
           )
           (local.get $7)
          )
          (local.get $8)
         )
         (local.get $9)
        )
        (local.get $10)
       )
       (local.get $11)
      )
      (local.get $12)
     )
     (local.get $13)
    )
    (local.get $14)
   )
   (local.get $15)
  )
 )
 (func $11 (param $0 i32) (param $1 i32) (param $2 i32) (param $3 i32) (param $4 i32) (param $5 i32) (param $6 i32) (param $7 i32) (result v128)
  (i16x8.replace_lane 7
   (i16x8.replace_lane 6
    (i16x8.replace_lane 5
     (i16x8.replace_lane 4
      (i16x8.replace_lane 3
       (i16x8.replace_lane 2
        (i16x8.replace_lane 1
         (i16x8.splat
          (local.get $0)
         )
         (local.get $1)
        )
        (local.get $2)
       )
       (local.get $3)
      )
      (local.get $4)
     )
     (local.get $5)
    )
    (local.get $6)
   )
   (local.get $7)
  )
 )
 (func $12 (param $0 i32) (param $1 i32) (param $2 i32) (param $3 i32) (result v128)
  (i32x4.replace_lane 3
   (i32x4.replace_lane 2
    (i32x4.replace_lane 1
     (i32x4.splat
      (local.get $0)
     )
     (local.get $1)
    )
    (local.get $2)
   )
   (local.get $3)
  )
 )
 (func $13 (param $0 i64) (param $1 i64) (result v128)
  (i64x2.replace_lane 1
   (i64x2.splat
    (local.get $0)
   )
   (local.get $1)
  )
 )
 (func $14 (param $0 i32) (param $1 i32) (param $2 i32) (param $3 i32) (param $4 i32) (param $5 i32) (param $6 i32) (param $7 i32) (param $8 i32) (param $9 i32) (param $10 i32) (param $11 i32) (param $12 i32) (param $13 i32) (param $14 i32) (param $15 i32) (result v128)
  (i8x16.replace_lane 15
   (i8x16.replace_lane 14
    (i8x16.replace_lane 13
     (i8x16.replace_lane 12
      (i8x16.replace_lane 11
       (i8x16.replace_lane 10
        (i8x16.replace_lane 9
         (i8x16.replace_lane 8
          (i8x16.replace_lane 7
           (i8x16.replace_lane 6
            (i8x16.replace_lane 5
             (i8x16.replace_lane 4
              (i8x16.replace_lane 3
               (i8x16.replace_lane 2
                (i8x16.replace_lane 1
                 (i8x16.splat
                  (local.get $0)
                 )
                 (local.get $1)
                )
                (local.get $2)
               )
               (local.get $3)
              )
              (local.get $4)
             )
             (local.get $5)
            )
            (local.get $6)
           )
           (local.get $7)
          )
          (local.get $8)
         )
         (local.get $9)
        )
        (local.get $10)
       )
       (local.get $11)
      )
      (local.get $12)
     )
     (local.get $13)
    )
    (local.get $14)
   )
   (local.get $15)
  )
 )
 (func $15 (param $0 i32) (param $1 i32) (param $2 i32) (param $3 i32) (param $4 i32) (param $5 i32) (param $6 i32) (param $7 i32) (result v128)
  (i16x8.replace_lane 7
   (i16x8.replace_lane 6
    (i16x8.replace_lane 5
     (i16x8.replace_lane 4
      (i16x8.replace_lane 3
       (i16x8.replace_lane 2
        (i16x8.replace_lane 1
         (i16x8.splat
          (local.get $0)
         )
         (local.get $1)
        )
        (local.get $2)
       )
       (local.get $3)
      )
      (local.get $4)
     )
     (local.get $5)
    )
    (local.get $6)
   )
   (local.get $7)
  )
 )
 (func $16 (param $0 i32) (param $1 i32) (param $2 i32) (param $3 i32) (result v128)
  (i32x4.replace_lane 3
   (i32x4.replace_lane 2
    (i32x4.replace_lane 1
     (i32x4.splat
      (local.get $0)
     )
     (local.get $1)
    )
    (local.get $2)
   )
   (local.get $3)
  )
 )
 (func $17 (param $0 i64) (param $1 i64) (result v128)
  (i64x2.replace_lane 1
   (i64x2.splat
    (local.get $0)
   )
   (local.get $1)
  )
 )
 (func $18 (param $0 f32) (param $1 f32) (param $2 f32) (param $3 f32) (result v128)
  (f32x4.replace_lane 3
   (f32x4.replace_lane 2
    (f32x4.replace_lane 1
     (f32x4.splat
      (local.get $0)
     )
     (local.get $1)
    )
    (local.get $2)
   )
   (local.get $3)
  )
 )
 (func $19 (param $0 f64) (param $1 f64) (result v128)
  (f64x2.replace_lane 1
   (f64x2.splat
    (local.get $0)
   )
   (local.get $1)
  )
 )
 ;; custom section "linking", size 308
 ;; custom section "producers", size 157
 ;; features section: simd
)

