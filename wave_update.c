#include "wave_update.h"

void update_wave(float *restrict updated, const float *decayed,
                 const float *bounce, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        updated[i] = decayed[i] + bounce[i];
    }
}
