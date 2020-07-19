#include "dglutil.h"
#include <stdlib.h>

#define SYS_CLOCKS_PER_SEC (1000.0f / 55.0f)


uint32 sys_clock() {
    return *((uint32*)0x046c);
}

float clock_ticks_to_seconds(uint32 clocks) {
    return clocks / (float)SYS_CLOCKS_PER_SEC;
}

int rnd_int(int low, int high) {
    return rand() % ((high - low) + 1) + low;
}

float rnd_float(float low, float high) {
    return low + (rand() / (float)RAND_MAX) * (high - low);
}

