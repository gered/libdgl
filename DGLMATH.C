#include "dglmath.h"
#include <math.h>

float angle_between_i(int x1, int y1, int x2, int y2) {
    int delta_x = x2 - x1;
    int delta_y = y2 - y1;
    if (delta_x == 0 && delta_y == 0)
        return 0.0f;
    else
        return atan2(delta_y, delta_x);
}

float angle_between_f(float x1, float y1, float x2, float y2) {
    float delta_x = x2 - x1;
    float delta_y = y2 - y1;
    if (close_enough(delta_x, 0.0f, TOLERANCE) && close_enough(delta_y, 0.0f, TOLERANCE))
        return 0.0f;
    else
        return atan2(delta_y, delta_x);
}

int next_power_of_2(int n) {
    int i = n & (~n + 1);
    while (i < n) {
        i <<= 1;
    }
    return i;
}

void point_on_circle(float radius, float radians, float *x, float *y) {
    *x = radius * cos(radians);
    *y = radius * sin(radians);
}

