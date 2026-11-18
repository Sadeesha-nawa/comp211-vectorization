#ifndef WAVE_UPDATE_H
#define WAVE_UPDATE_H

#include <stddef.h>

/* The output must use storage separate from both inputs. */
void update_wave(float *restrict updated, const float *decayed,
                 const float *bounce, size_t n);

#endif
