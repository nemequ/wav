#include "../wav.h"

wav_i8x16_t bitselect_i8x16(wav_i8x16_t when_set, wav_i8x16_t when_unset, wav_i8x16_t control) { return wav_i8x16_bitselect(control, when_set, when_unset); }
wav_i16x8_t bitselect_i16x8(wav_i16x8_t when_set, wav_i16x8_t when_unset, wav_i16x8_t control) { return wav_i16x8_bitselect(control, when_set, when_unset); }
wav_i32x4_t bitselect_i32x4(wav_i32x4_t when_set, wav_i32x4_t when_unset, wav_i32x4_t control) { return wav_i32x4_bitselect(control, when_set, when_unset); }
wav_i64x2_t bitselect_i64x2(wav_i64x2_t when_set, wav_i64x2_t when_unset, wav_i64x2_t control) { return wav_i64x2_bitselect(control, when_set, when_unset); }
wav_u8x16_t bitselect_u8x16(wav_u8x16_t when_set, wav_u8x16_t when_unset, wav_u8x16_t control) { return wav_u8x16_bitselect(control, when_set, when_unset); }
wav_u16x8_t bitselect_u16x8(wav_u16x8_t when_set, wav_u16x8_t when_unset, wav_u16x8_t control) { return wav_u16x8_bitselect(control, when_set, when_unset); }
wav_u32x4_t bitselect_u32x4(wav_u32x4_t when_set, wav_u32x4_t when_unset, wav_u32x4_t control) { return wav_u32x4_bitselect(control, when_set, when_unset); }
wav_u64x2_t bitselect_u64x2(wav_u64x2_t when_set, wav_u64x2_t when_unset, wav_u64x2_t control) { return wav_u64x2_bitselect(control, when_set, when_unset); }
wav_f32x4_t bitselect_f32x4(wav_f32x4_t when_set, wav_f32x4_t when_unset, wav_u32x4_t control) { return wav_f32x4_bitselect(control, when_set, when_unset); }
wav_f64x2_t bitselect_f64x2(wav_f64x2_t when_set, wav_f64x2_t when_unset, wav_u64x2_t control) { return wav_f64x2_bitselect(control, when_set, when_unset); }

wav_i8x16_t bitselect_i8x16_o(wav_i8x16_t when_set, wav_i8x16_t when_unset, wav_i8x16_t control) { return wav_bitselect(control, when_set, when_unset); }
wav_i16x8_t bitselect_i16x8_o(wav_i16x8_t when_set, wav_i16x8_t when_unset, wav_i16x8_t control) { return wav_bitselect(control, when_set, when_unset); }
wav_i32x4_t bitselect_i32x4_o(wav_i32x4_t when_set, wav_i32x4_t when_unset, wav_i32x4_t control) { return wav_bitselect(control, when_set, when_unset); }
wav_i64x2_t bitselect_i64x2_o(wav_i64x2_t when_set, wav_i64x2_t when_unset, wav_i64x2_t control) { return wav_bitselect(control, when_set, when_unset); }
wav_u8x16_t bitselect_u8x16_o(wav_u8x16_t when_set, wav_u8x16_t when_unset, wav_u8x16_t control) { return wav_bitselect(control, when_set, when_unset); }
wav_u16x8_t bitselect_u16x8_o(wav_u16x8_t when_set, wav_u16x8_t when_unset, wav_u16x8_t control) { return wav_bitselect(control, when_set, when_unset); }
wav_u32x4_t bitselect_u32x4_o(wav_u32x4_t when_set, wav_u32x4_t when_unset, wav_u32x4_t control) { return wav_bitselect(control, when_set, when_unset); }
wav_u64x2_t bitselect_u64x2_o(wav_u64x2_t when_set, wav_u64x2_t when_unset, wav_u64x2_t control) { return wav_bitselect(control, when_set, when_unset); }
wav_f32x4_t bitselect_f32x4_o(wav_f32x4_t when_set, wav_f32x4_t when_unset, wav_u32x4_t control) { return wav_bitselect(control, when_set, when_unset); }
wav_f64x2_t bitselect_f64x2_o(wav_f64x2_t when_set, wav_f64x2_t when_unset, wav_u64x2_t control) { return wav_bitselect(control, when_set, when_unset); }
