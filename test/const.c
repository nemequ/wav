#include "../wav.h"

#pragma clang diagnostic ignored "-Wold-style-cast"

wav_i8x16_t
const_i8x16(void) {
  return wav_i8x16_const(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
}

wav_i16x8_t
const_i16x8(void) {
  return wav_i16x8_const(0, 1, 2, 3, 4, 5, 6, 7);
}

wav_i32x4_t
const_i32x4(void) {
  return wav_i32x4_const(0, 1, 2, 3);
}

wav_i64x2_t
const_i64x2(void) {
  return wav_i64x2_const(0, 1);
}

wav_u8x16_t
const_u8x16(void) {
  return wav_u8x16_const(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
}

wav_u16x8_t
const_u16x8(void) {
  return wav_u16x8_const(0, 1, 2, 3, 4, 5, 6, 7);
}

wav_u32x4_t
const_u32x4(void) {
  return wav_u32x4_const(0, 1, 2, 3);
}

wav_u64x2_t
const_u64x2(void) {
  return wav_u64x2_const(0, 1);
}

wav_f32x4_t
const_f32x4(void) {
  return wav_f32x4_const(0, 1, 2, 3);
}

wav_f64x2_t
const_f64x2(void) {
  return wav_f64x2_const(0, 1);
}

wav_i8x16_t
const_i8x16_o(void) {
  return wav_const((int8_t) 0, (int8_t) 1, (int8_t) 2, (int8_t) 3, (int8_t) 4, (int8_t) 5, (int8_t) 6, (int8_t) 7, (int8_t) 8, (int8_t) 9, (int8_t) 10, (int8_t) 11, (int8_t) 12, (int8_t) 13, (int8_t) 14, (int8_t) 15);
}

wav_i16x8_t
const_i16x8_o(void) {
  return wav_const((int16_t) 0, (int16_t) 1, (int16_t) 2, (int16_t) 3, (int16_t) 4, (int16_t) 5, (int16_t) 6, (int16_t) 7);
}

wav_i32x4_t
const_i32x4_o(void) {
  return wav_const((int32_t) 0, (int32_t) 1, (int32_t) 2, (int32_t) 3);
}

wav_i64x2_t
const_i64x2_o(void) {
  return wav_const((int64_t) 0, (int64_t) 1);
}

wav_u8x16_t
const_u8x16_o(void) {
  return wav_const((uint8_t) 0, (uint8_t) 1, (uint8_t) 2, (uint8_t) 3, (uint8_t) 4, (uint8_t) 5, (uint8_t) 6, (uint8_t) 7, (uint8_t) 8, (uint8_t) 9, (uint8_t) 10, (uint8_t) 11, (uint8_t) 12, (uint8_t) 13, (uint8_t) 14, (uint8_t) 15);
}

wav_u16x8_t
const_u16x8_o(void) {
  return wav_const((uint16_t) 0, (uint16_t) 1, (uint16_t) 2, (uint16_t) 3, (uint16_t) 4, (uint16_t) 5, (uint16_t) 6, (uint16_t) 7);
}

wav_u32x4_t
const_u32x4_o(void) {
  return wav_const((uint32_t) 0, (uint32_t) 1, (uint32_t) 2, (uint32_t) 3);
}

wav_u64x2_t
const_u64x2_o(void) {
  return wav_const((uint64_t) 0, (uint64_t) 1);
}

wav_f32x4_t
const_f32x4_o(void) {
  return wav_const(0.0f, 1.0f, 2.0f, 3.0f);
}

wav_f64x2_t
const_f64x2_o(void) {
  return wav_const(0.0, 1.0);
}
