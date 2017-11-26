#ifndef DGL_UTIL_H_INCLUDED
#define DGL_UTIL_H_INCLUDED

#include "common.h"

#define SWAP(type, a, b) \
    do {                 \
        type __tmp = a;  \
        a = b;           \
        b = __tmp;       \
    } while (0)

#define SIGN(x) (((x) < 0) ? -1 : (((x) > 0) ? 1 : 0))

void* map_dos_memory(long physical_addr);

int sys_clock();
int sys_ticks();
float clock_ticks_to_seconds(int clocks);
float ticks_to_seconds(int ticks);

int rnd_int(int low, int high);
float rnd_float(float low, float high);

#endif

