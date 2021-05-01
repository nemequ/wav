#include "../wav.h"

wav_f32x4_t demote_f64x2(wav_f64x2_t a) { return wav_f64x2_demote(a); }

wav_f32x4_t demote_f64x2_o(wav_f64x2_t a) { return wav_demote(a); }
