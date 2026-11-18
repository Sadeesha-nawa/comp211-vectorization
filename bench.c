#define _POSIX_C_SOURCE 200809L

#include "wave_update.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum { SAMPLE_COUNT = 262147, CALLS_PER_RUN = 500 };

static float decayed[SAMPLE_COUNT];
static float bounce[SAMPLE_COUNT];
static float updated[SAMPLE_COUNT];

static double monotonic_seconds(void)
{
    struct timespec time;

    if (clock_gettime(CLOCK_MONOTONIC, &time) != 0) {
        perror("clock_gettime");
        exit(EXIT_FAILURE);
    }

    return time.tv_sec + time.tv_nsec / 1e9;
}

int main(void)
{
    for (size_t i = 0; i < SAMPLE_COUNT; i++) {
        decayed[i] = (float)i;
        bounce[i] = 1.0f;
    }

    update_wave(updated, decayed, bounce, SAMPLE_COUNT);
    double start = monotonic_seconds();

    for (int call = 0; call < CALLS_PER_RUN; call++) {
        update_wave(updated, decayed, bounce, SAMPLE_COUNT);
    }

    double elapsed = monotonic_seconds() - start;

    for (size_t i = 0; i < SAMPLE_COUNT; i++) {
        float expected = decayed[i] + bounce[i];
        if (updated[i] != expected) {
            fprintf(stderr, "Incorrect result at index %zu\n", i);
            return EXIT_FAILURE;
        }
    }

    printf("%.3f\n", elapsed * 1e6 / CALLS_PER_RUN);
    return EXIT_SUCCESS;
}
