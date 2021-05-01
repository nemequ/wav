#include "../wav.h"

wav_f64x2_t promote_low_f32x4(wav_f32x4_t a) { return wav_f32x4_promote_low(a); }

wav_f64x2_t promote_low_f32x4_o(wav_f32x4_t a) { return wav_promote_low(a); }
