#ifndef LIBDGL_DGLMATH_H
#define LIBDGL_DGLMATH_H

#include "dglcmn.h"
#include "dglvec2.h"
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TOLERANCE                  0.00001f

#define PI                         3.1415927f
#define PI_OVER_180                (PI / 180.0f)

#define RADIANS_0                  0.0f
#define RADIANS_45                 (PI / 4.0f)
#define RADIANS_90                 (PI / 2.0f)
#define RADIANS_135                ((3.0f * PI) / 4.0f)
#define RADIANS_180                PI
#define RADIANS_225                ((5.0f * PI) / 4.0f)
#define RADIANS_270                ((3.0f * PI) / 2.0f)
#define RADIANS_315                ((7.0f * PI) / 4.0f)
#define RADIANS_360                (PI * 2.0f)

#define DEG_TO_RAD(degrees)        ((degrees) * PI_OVER_180)
#define RAD_TO_DEG(radians)        ((radians) * (1.0f / PI_OVER_180))
#define CLAMP(value, low, high)    (((value) < (low) ? (low) : ((value) > (high) ? (high) : (value))))
#define LERP(a, b, t)              ((a) + ((b) - (a)) * (t))
#define INVERSE_LERP(a, b, lerped) (((lerped) - (a)) / ((b) - (a)))

#define SCALE_RANGE(value, from_min, from_max, to_min, to_max) \
    (((value) / (((from_max) - (from_min)) / ((to_max) - (to_min)))) + (to_min))

float angle_between_i(int x1, int y1, int x2, int y2);
float angle_between_f(float x1, float y1, float x2, float y2);
int next_power_of_2(int n);
void point_on_circle(float radius, float radians, float *x, float *y);

static VEC2 direction_from_angle(float radians);
static float round(float value);
static float symmetrical_round(float value);
static bool close_enough(float a, float b, float tolerance);
static bool power_of_2(int n);
static float smooth_step(float low, float high, float t);

// --------------------------------------------------------------------------

static VEC2 direction_from_angle(float radians) {
    VEC2 direction;
    float x, y;
    point_on_circle(1.0f, radians, &x, &y);
    direction.x = FTOFIX(x);
    direction.y = FTOFIX(y);
    return direction;
}

static float round(float value) {
    return ceil(value + 0.5f);
}

static float symmetrical_round(float value) {
    if (value > 0.0f)
        return floor(value + 0.5f);
    else
        return ceil(value - 0.5f);
}

static bool close_enough(float a, float b, float tolerance) {
    //return fabs((a - b) / ((b == 0.0f) ? 1.0f : b)) < tolerance;
    // TODO: this is not the best way
    return fabs(a - b) <= tolerance;
}

static bool power_of_2(int n) {
    return (n != 0) && !(n & (n - 1));
}

static float smooth_step(float low, float high, float t) {
    float n = CLAMP(t, 0.0f, 1.0f);
    return LERP(low, high, (n * n) * (3.0f - (2.0f * n)));
}

#ifdef __cplusplus
}
#endif

#endif
