#ifndef DGL_VECTOR2_H_INCLUDED
#define DGL_VECTOR2_H_INCLUDED

#include <math.h>
#include "common.h"

typedef struct {
    int x;
    int y;
} VECTOR2I;

static inline VECTOR2I vector2i(int x, int y);
static inline void vector2i_set(VECTOR2I *v, int x, int y);
static inline boolean vector2i_equals(VECTOR2I a, VECTOR2I b);
static inline VECTOR2I vector2i_add(VECTOR2I a, VECTOR2I b);
static inline VECTOR2I vector2i_sub(VECTOR2I a, VECTOR2I b);
static inline VECTOR2I vector2i_mul(VECTOR2I a, VECTOR2I b);
static inline VECTOR2I vector2i_muls(VECTOR2I v, int n);
static inline VECTOR2I vector2i_div(VECTOR2I a, VECTOR2I b);
static inline VECTOR2I vector2i_divs(VECTOR2I v, int n);
static inline int vector2i_distance(VECTOR2I a, VECTOR2I b);
static inline int vector2i_distancesq(VECTOR2I a, VECTOR2I b);
static inline int vector2i_dot(VECTOR2I a, VECTOR2I b);
static inline int vector2i_length(VECTOR2I v);
static inline int vector2i_lengthsq(VECTOR2I v);
static inline VECTOR2I vector2i_lerp(VECTOR2I a, VECTOR2I b, float lerp);

#define ZERO_VECTOR2I   vector2i(0, 0)
#define UP_VECTOR2I     vector2i(0, -1);
#define DOWN_VECTOR2I   vector2i(0, 1);
#define LEFT_VECTOR2I   vector2i(-1, 0);
#define RIGHT_VECTOR2I  vector2i(1, 0);
#define UNIT_X_VECTOR2I vector2i(1, 0);
#define UNIT_Y_VECTOR2I vector2i(0, 1);

typedef struct {
    float x;
    float y;
} VECTOR2F;

static inline VECTOR2F vector2f(float x, float y);
static inline void vector2f_set(VECTOR2F *v, float x, float y);
static inline boolean vector2f_equals(VECTOR2F a, VECTOR2F b);
static inline VECTOR2F vector2f_add(VECTOR2F a, VECTOR2F b);
static inline VECTOR2F vector2f_sub(VECTOR2F a, VECTOR2F b);
static inline VECTOR2F vector2f_mul(VECTOR2F a, VECTOR2F b);
static inline VECTOR2F vector2f_muls(VECTOR2F v, float n);
static inline VECTOR2F vector2f_div(VECTOR2F a, VECTOR2F b);
static inline VECTOR2F vector2f_divs(VECTOR2F v, float n);
static inline float vector2f_distance(VECTOR2F a, VECTOR2F b);
static inline float vector2f_distancesq(VECTOR2F a, VECTOR2F b);
static inline float vector2f_dot(VECTOR2F a, VECTOR2F b);
static inline float vector2f_length(VECTOR2F v);
static inline float vector2f_lengthsq(VECTOR2F v);
static inline VECTOR2F vector2f_normalize(VECTOR2F v);
static inline VECTOR2F vector2f_set_length(VECTOR2F v, float length);
static inline VECTOR2F vector2f_lerp(VECTOR2F a, VECTOR2F b, float lerp);

#define ZERO_VECTOR2F   vector2f(0.0f, 0.0f)
#define UP_VECTOR2F     vector2f(0.0f, -1.0f);
#define DOWN_VECTOR2F   vector2f(0.0f, 1.0f);
#define LEFT_VECTOR2F   vector2f(-1.0f, 0.0f);
#define RIGHT_VECTOR2F  vector2f(1.0f, 0.0f);
#define UNIT_X_VECTOR2F vector2f(1.0f, 0.0f);
#define UNIT_Y_VECTOR2F vector2f(0.0f, 1.0f);

// --------------------------------------------------------------------------

static inline VECTOR2I vector2i(int x, int y) {
    VECTOR2I v;
    v.x = x;
    v.y = y;
    return v;
}

static inline VECTOR2F vector2f(float x, float y) {
    VECTOR2F v;
    v.x = x;
    v.y = y;
    return v;
}

static inline void vector2i_set(VECTOR2I *v, int x, int y) {
    v->x = x;
    v->y = y;
}

static inline void vector2f_set(VECTOR2F *v, float x, float y) {
    v->x = x;
    v->y = y;
}

static inline boolean vector2i_equals(VECTOR2I a, VECTOR2I b) {
    return (a.x == b.x && a.y == b.y);
}

static inline boolean vector2f_equals(VECTOR2F a, VECTOR2F b) {
    return (a.x == b.x && a.y == b.y);
}

static inline VECTOR2I vector2i_add(VECTOR2I a, VECTOR2I b) {
    VECTOR2I result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

static inline VECTOR2F vector2f_add(VECTOR2F a, VECTOR2F b) {
    VECTOR2F result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

static inline VECTOR2I vector2i_sub(VECTOR2I a, VECTOR2I b) {
    VECTOR2I result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

static inline VECTOR2F vector2f_sub(VECTOR2F a, VECTOR2F b) {
    VECTOR2F result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

static inline VECTOR2I vector2i_mul(VECTOR2I a, VECTOR2I b) {
    VECTOR2I result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    return result;
}

static inline VECTOR2F vector2f_mul(VECTOR2F a, VECTOR2F b) {
    VECTOR2F result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    return result;
}

static inline VECTOR2I vector2i_muls(VECTOR2I v, int n) {
    VECTOR2I result;
    result.x = v.x * n;
    result.y = v.y * n;
    return result;
}

static inline VECTOR2F vector2f_muls(VECTOR2F v, float n) {
    VECTOR2F result;
    result.x = v.x * n;
    result.y = v.y * n;
    return result;
}

static inline VECTOR2I vector2i_div(VECTOR2I a, VECTOR2I b) {
    VECTOR2I result;
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    return result;
}

static inline VECTOR2F vector2f_div(VECTOR2F a, VECTOR2F b) {
    VECTOR2F result;
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    return result;
}

static inline VECTOR2I vector2i_divs(VECTOR2I v, int n) {
    VECTOR2I result;
    result.x = v.x / n;
    result.y = v.y / n;
    return result;
}

static inline VECTOR2F vector2f_divs(VECTOR2F v, float n) {
    VECTOR2F result;
    result.x = v.x / n;
    result.y = v.y / n;
    return result;
}

static inline int vector2i_distance(VECTOR2I a, VECTOR2I b) {
    return (int)sqrt(
        ((b.x - a.x) * (b.x - a.x)) +
        ((b.y - a.y) * (b.y - a.y))
        );
}

static inline float vector2f_distance(VECTOR2F a, VECTOR2F b) {
    return (float)sqrt(
        ((b.x - a.x) * (b.x - a.x)) +
        ((b.y - a.y) * (b.y - a.y))
        );
}

static inline int vector2i_distancesq(VECTOR2I a, VECTOR2I b) {
    return
        ((b.x - a.x) * (b.x - a.x)) +
        ((b.y - a.y) * (b.y - a.y));
}

static inline float vector2f_distancesq(VECTOR2F a, VECTOR2F b) {
    return
        ((b.x - a.x) * (b.x - a.x)) +
        ((b.y - a.y) * (b.y - a.y));
}

static inline int vector2i_dot(VECTOR2I a, VECTOR2I b) {
    return
        (a.x * b.x) +
        (a.y * b.y);
}

static inline float vector2f_dot(VECTOR2F a, VECTOR2F b) {
    return
        (a.x * b.x) +
        (a.y * b.y);
}

static inline int vector2i_length(VECTOR2I v) {
    return sqrt(
        (v.x * v.x) +
        (v.y * v.y)
        );
}

static inline float vector2f_length(VECTOR2F v) {
    return (float)sqrt(
        (v.x * v.x) +
        (v.y * v.y)
        );
}

static inline int vector2i_lengthsq(VECTOR2I v) {
    return
        (v.x * v.x) +
        (v.y * v.y);
}

static inline float vector2f_lengthsq(VECTOR2F v) {
    return
        (v.x * v.x) +
        (v.y * v.y);
}

static inline VECTOR2F vector2f_normalize(VECTOR2F v) {
    float inverse_length;
    VECTOR2F result;

    inverse_length = 1.0f / vector2f_length(v);
    result.x = v.x * inverse_length;
    result.y = v.y * inverse_length;
    return result;
}

static inline VECTOR2F vector2f_set_length(VECTOR2F v, float length) {
    float scale_factor;
    VECTOR2F result;

    scale_factor = length / vector2f_length(v);
    result.x = v.x * scale_factor;
    result.y = v.y * scale_factor;
    return result;
}

static inline VECTOR2I vector2i_lerp(VECTOR2I a, VECTOR2I b, float lerp) {
    VECTOR2I result;
    result.x = a.x + (b.x - a.x) * lerp;
    result.y = a.y + (b.y - a.y) * lerp;
    return result;
}

static inline VECTOR2F vector2f_lerp(VECTOR2F a, VECTOR2F b, float lerp) {
    VECTOR2F result;
    result.x = a.x + (b.x - a.x) * lerp;
    result.y = a.y + (b.y - a.y) * lerp;
    return result;
}

#endif

