#ifndef DGL_MATH_H_INCLUDED
#define DGL_MATH_H_INCLUDED

#include "common.h"
#include "vector2.h"
#include <math.h>

#define TOLERANCE                  0.00001f

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

static inline VECTOR2F direction_from_angle(float radians);
static inline float round(float value);
static inline float symmetrical_round(float value);
static inline boolean close_enough(float a, float b, float tolerance);
static inline boolean power_of_2(int n);
static inline float smooth_step(float low, float high, float t);

// --------------------------------------------------------------------------

static inline VECTOR2F direction_from_angle(float radians) {
    VECTOR2F direction;
    point_on_circle(1.0f, radians, &direction.x, &direction.y);
    return direction;
}

static inline float round(float value) {
    return ceil(value + 0.5f);
}

static inline float symmetrical_round(float value) {
    if (value > 0.0f)
        return floor(value + 0.5f);
    else
        return ceil(value - 0.5f);
}

static inline boolean close_enough(float a, float b, float tolerance) {
    return fabs((a - b) / ((b == 0.0f) ? 1.0f : b)) < tolerance;
}

static inline boolean power_of_2(int n) {
    return (n != 0) && !(n & (n - 1));
}

static inline float smooth_step(float low, float high, float t) {
    float n = CLAMP(t, 0.0f, 1.0f);
    return LERP(low, high, (n * n) * (3.0f - (2.0f * n)));
}

#endif
