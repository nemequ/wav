#include "../wav.h"

wav_i8x16_t
make_i8x16(
    int8_t c0, int8_t c1, int8_t  c2, int8_t  c3, int8_t  c4, int8_t  c5, int8_t  c6, int8_t  c7,
    int8_t c8, int8_t c9, int8_t c10, int8_t c11, int8_t c12, int8_t c13, int8_t c14, int8_t c15) {
  return wav_i8x16_make(c0, c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15);
}

wav_i16x8_t
make_i16x8(
    int16_t c0, int16_t c1, int16_t  c2, int16_t  c3, int16_t  c4, int16_t  c5, int16_t  c6, int16_t  c7) {
  return wav_i16x8_make(c0, c1, c2, c3, c4, c5, c6, c7);
}

wav_i32x4_t
make_i32x4(
    int32_t c0, int32_t c1, int32_t  c2, int32_t  c3) {
  return wav_i32x4_make(c0, c1, c2, c3);
}

wav_i64x2_t
make_i64x2(
    int64_t c0, int64_t c1) {
  return wav_i64x2_make(c0, c1);
}

wav_u8x16_t
make_u8x16(
    uint8_t c0, uint8_t c1, uint8_t  c2, uint8_t  c3, uint8_t  c4, uint8_t  c5, uint8_t  c6, uint8_t  c7,
    uint8_t c8, uint8_t c9, uint8_t c10, uint8_t c11, uint8_t c12, uint8_t c13, uint8_t c14, uint8_t c15) {
  return wav_u8x16_make(c0, c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15);
}

wav_u16x8_t
make_u16x8(
    uint16_t c0, uint16_t c1, uint16_t  c2, uint16_t  c3, uint16_t  c4, uint16_t  c5, uint16_t  c6, uint16_t  c7) {
  return wav_u16x8_make(c0, c1, c2, c3, c4, c5, c6, c7);
}

wav_u32x4_t
make_u32x4(
    uint32_t c0, uint32_t c1, uint32_t  c2, uint32_t  c3) {
  return wav_u32x4_make(c0, c1, c2, c3);
}

wav_u64x2_t
make_u64x2(
    uint64_t c0, uint64_t c1) {
  return wav_u64x2_make(c0, c1);
}

wav_f32x4_t
make_f32x4(
    float c0, float c1, float  c2, float  c3) {
  return wav_f32x4_make(c0, c1, c2, c3);
}

wav_f64x2_t
make_f64x2(
    double c0, double c1) {
  return wav_f64x2_make(c0, c1);
}

wav_i8x16_t
make_i8x16_o(
    int8_t c0, int8_t c1, int8_t  c2, int8_t  c3, int8_t  c4, int8_t  c5, int8_t  c6, int8_t  c7,
    int8_t c8, int8_t c9, int8_t c10, int8_t c11, int8_t c12, int8_t c13, int8_t c14, int8_t c15) {
  return wav_make(c0, c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15);
}

wav_i16x8_t
make_i16x8_o(
    int16_t c0, int16_t c1, int16_t  c2, int16_t  c3, int16_t  c4, int16_t  c5, int16_t  c6, int16_t  c7) {
  return wav_make(c0, c1, c2, c3, c4, c5, c6, c7);
}

wav_i32x4_t
make_i32x4_o(
    int32_t c0, int32_t c1, int32_t  c2, int32_t  c3) {
  return wav_make(c0, c1, c2, c3);
}

wav_i64x2_t
make_i64x2_o(
    int64_t c0, int64_t c1) {
  return wav_make(c0, c1);
}

wav_u8x16_t
make_u8x16_o(
    uint8_t c0, uint8_t c1, uint8_t  c2, uint8_t  c3, uint8_t  c4, uint8_t  c5, uint8_t  c6, uint8_t  c7,
    uint8_t c8, uint8_t c9, uint8_t c10, uint8_t c11, uint8_t c12, uint8_t c13, uint8_t c14, uint8_t c15) {
  return wav_make(c0, c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15);
}

wav_u16x8_t
make_u16x8_o(
    uint16_t c0, uint16_t c1, uint16_t  c2, uint16_t  c3, uint16_t  c4, uint16_t  c5, uint16_t  c6, uint16_t  c7) {
  return wav_make(c0, c1, c2, c3, c4, c5, c6, c7);
}

wav_u32x4_t
make_u32x4_o(
    uint32_t c0, uint32_t c1, uint32_t  c2, uint32_t  c3) {
  return wav_make(c0, c1, c2, c3);
}

wav_u64x2_t
make_u64x2_o(
    uint64_t c0, uint64_t c1) {
  return wav_make(c0, c1);
}

wav_f32x4_t
make_f32x4_o(
    float c0, float c1, float  c2, float  c3) {
  return wav_make(c0, c1, c2, c3);
}

wav_f64x2_t
make_f64x2_o(
    double c0, double c1) {
  return wav_make(c0, c1);
}
