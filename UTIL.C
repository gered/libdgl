#include "util.h"
#include <stdlib.h>
#include <time.h>
#include <sys/nearptr.h>

#define SYS_CLOCKS_PER_SEC (1000.0f / 55.0f)

void* map_dos_memory(long physical_addr) {
    return (void*)(__djgpp_conventional_base + physical_addr);
}

int sys_clock() {
    //return *((int*)(map_dos_memory(0x046c)));
    return (int)clock();
}

int sys_ticks() {
    return (int)uclock();
}

float clock_ticks_to_seconds(int clocks) {
    //return clocks / SYS_CLOCKS_PER_SEC;
    return clocks / (float)CLOCKS_PER_SEC;
}

float ticks_to_seconds(int ticks) {
    return (float)ticks / UCLOCKS_PER_SEC;
}

int rnd_int(int low, int high) {
    return rand() % ((high - low) + 1) + low;
}

float rnd_float(float low, float high) {
    return low + (rand() / (float)RAND_MAX) * (high - low);
}

