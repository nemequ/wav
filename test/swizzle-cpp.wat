(module
  (type (;0;) (func (param v128 v128) (result v128)))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func $_Z13swizzle_i8x1611wav_i8x16_tS_ (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.swizzle)
  (func $_Z15swizzle_i8x16_o11wav_i8x16_tS_ (type 0) (param v128 v128) (result v128)
    local.get 0
    local.get 1
    i8x16.swizzle))
