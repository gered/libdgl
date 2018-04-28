#ifndef DGL_DGLVEC2_H_INCLUDED
#define DGL_DGLVEC2_H_INCLUDED

#include <math.h>
#include "dglcmn.h"
#include "dglfixp.h"


typedef struct {
    int x;
    int y;
} VECTOR2I;

static VECTOR2I vector2i(int x, int y);
static void vector2i_set(VECTOR2I *v, int x, int y);
static boolean vector2i_equals(VECTOR2I a, VECTOR2I b);
static VECTOR2I vector2i_add(VECTOR2I a, VECTOR2I b);
static VECTOR2I vector2i_sub(VECTOR2I a, VECTOR2I b);
static VECTOR2I vector2i_mul(VECTOR2I a, VECTOR2I b);
static VECTOR2I vector2i_muls(VECTOR2I v, int n);
static VECTOR2I vector2i_div(VECTOR2I a, VECTOR2I b);
static VECTOR2I vector2i_divs(VECTOR2I v, int n);
static int vector2i_distance(VECTOR2I a, VECTOR2I b);
static int vector2i_distancesq(VECTOR2I a, VECTOR2I b);
static int vector2i_dot(VECTOR2I a, VECTOR2I b);
static int vector2i_length(VECTOR2I v);
static int vector2i_lengthsq(VECTOR2I v);
static VECTOR2I vector2i_lerp(VECTOR2I a, VECTOR2I b, float lerp);

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

static VECTOR2F vector2f(float x, float y);
static void vector2f_set(VECTOR2F *v, float x, float y);
static boolean vector2f_equals(VECTOR2F a, VECTOR2F b);
static VECTOR2F vector2f_add(VECTOR2F a, VECTOR2F b);
static VECTOR2F vector2f_sub(VECTOR2F a, VECTOR2F b);
static VECTOR2F vector2f_mul(VECTOR2F a, VECTOR2F b);
static VECTOR2F vector2f_muls(VECTOR2F v, float n);
static VECTOR2F vector2f_div(VECTOR2F a, VECTOR2F b);
static VECTOR2F vector2f_divs(VECTOR2F v, float n);
static float vector2f_distance(VECTOR2F a, VECTOR2F b);
static float vector2f_distancesq(VECTOR2F a, VECTOR2F b);
static float vector2f_dot(VECTOR2F a, VECTOR2F b);
static float vector2f_length(VECTOR2F v);
static float vector2f_lengthsq(VECTOR2F v);
static VECTOR2F vector2f_normalize(VECTOR2F v);
static VECTOR2F vector2f_set_length(VECTOR2F v, float length);
static VECTOR2F vector2f_lerp(VECTOR2F a, VECTOR2F b, float lerp);

#define ZERO_VECTOR2F   vector2f(0.0f, 0.0f)
#define UP_VECTOR2F     vector2f(0.0f, -1.0f);
#define DOWN_VECTOR2F   vector2f(0.0f, 1.0f);
#define LEFT_VECTOR2F   vector2f(-1.0f, 0.0f);
#define RIGHT_VECTOR2F  vector2f(1.0f, 0.0f);
#define UNIT_X_VECTOR2F vector2f(1.0f, 0.0f);
#define UNIT_Y_VECTOR2F vector2f(0.0f, 1.0f);


typedef struct {
    fixed x;
    fixed y;
} VECTOR2FP;

static VECTOR2FP vector2fp(fixed x, fixed y);
static void vector2fp_set(VECTOR2FP *v, fixed x, fixed y);
static boolean vector2fp_equals(VECTOR2FP a, VECTOR2FP b);
static VECTOR2FP vector2fp_add(VECTOR2FP a, VECTOR2FP b);
static VECTOR2FP vector2fp_sub(VECTOR2FP a, VECTOR2FP b);
static VECTOR2FP vector2fp_mul(VECTOR2FP a, VECTOR2FP b);
static VECTOR2FP vector2fp_muls(VECTOR2FP v, fixed n);
static VECTOR2FP vector2fp_div(VECTOR2FP a, VECTOR2FP b);
static VECTOR2FP vector2fp_divs(VECTOR2FP v, fixed n);
static fixed vector2fp_distance(VECTOR2FP a, VECTOR2FP b);
static fixed vector2fp_distancesq(VECTOR2FP a, VECTOR2FP b);
static fixed vector2fp_dot(VECTOR2FP a, VECTOR2FP b);
static fixed vector2fp_length(VECTOR2FP v);
static fixed vector2fp_lengthsq(VECTOR2FP v);
static VECTOR2FP vector2fp_normalize(VECTOR2FP v);
static VECTOR2FP vector2fp_set_length(VECTOR2FP v, fixed length);
static VECTOR2FP vector2fp_lerp(VECTOR2FP a, VECTOR2FP b, fixed lerp);

#define ZERO_VECTOR2FP   vector2fp(0, 0)
#define UP_VECTOR2FP     vector2fp(0, (-1 << 16));
#define DOWN_VECTOR2FP   vector2fp(0, (1 << 16));
#define LEFT_VECTOR2FP   vector2fp((-1 << 16), 0);
#define RIGHT_VECTOR2FP  vector2fp((1 << 16), 0);
#define UNIT_X_VECTOR2FP vector2fp((1 << 16), 0);
#define UNIT_Y_VECTOR2FP vector2fp(0, (1 << 16));

// --------------------------------------------------------------------------

static VECTOR2I vector2i(int x, int y) {
    VECTOR2I v;
    v.x = x;
    v.y = y;
    return v;
}

static VECTOR2F vector2f(float x, float y) {
    VECTOR2F v;
    v.x = x;
    v.y = y;
    return v;
}

static VECTOR2FP vector2fp(fixed x, fixed y) {
    VECTOR2FP v;
    v.x = x;
    v.y = y;
    return v;
}

static void vector2i_set(VECTOR2I *v, int x, int y) {
    v->x = x;
    v->y = y;
}

static void vector2f_set(VECTOR2F *v, float x, float y) {
    v->x = x;
    v->y = y;
}

static void vector2fp_set(VECTOR2FP *v, fixed x, fixed y) {
    v->x = x;
    v->y = y;
}

static boolean vector2i_equals(VECTOR2I a, VECTOR2I b) {
    return (a.x == b.x && a.y == b.y);
}

static boolean vector2f_equals(VECTOR2F a, VECTOR2F b) {
    return (a.x == b.x && a.y == b.y);
}

static boolean vector2fp_equals(VECTOR2FP a, VECTOR2FP b) {
    return (a.x == b.x && a.y == b.y);
}

static VECTOR2I vector2i_add(VECTOR2I a, VECTOR2I b) {
    VECTOR2I result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

static VECTOR2F vector2f_add(VECTOR2F a, VECTOR2F b) {
    VECTOR2F result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

static VECTOR2FP vector2fp_add(VECTOR2FP a, VECTOR2FP b) {
    VECTOR2FP result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

static VECTOR2I vector2i_sub(VECTOR2I a, VECTOR2I b) {
    VECTOR2I result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

static VECTOR2F vector2f_sub(VECTOR2F a, VECTOR2F b) {
    VECTOR2F result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

static VECTOR2FP vector2fp_sub(VECTOR2FP a, VECTOR2FP b) {
    VECTOR2FP result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

static VECTOR2I vector2i_mul(VECTOR2I a, VECTOR2I b) {
    VECTOR2I result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    return result;
}

static VECTOR2F vector2f_mul(VECTOR2F a, VECTOR2F b) {
    VECTOR2F result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    return result;
}

static VECTOR2FP vector2fp_mul(VECTOR2FP a, VECTOR2FP b) {
    VECTOR2FP result;
    result.x = fix_mul(a.x, b.x);
    result.y = fix_mul(a.y, b.y);
    return result;
}

static VECTOR2I vector2i_muls(VECTOR2I v, int n) {
    VECTOR2I result;
    result.x = v.x * n;
    result.y = v.y * n;
    return result;
}

static VECTOR2F vector2f_muls(VECTOR2F v, float n) {
    VECTOR2F result;
    result.x = v.x * n;
    result.y = v.y * n;
    return result;
}

static VECTOR2FP vector2fp_muls(VECTOR2FP v, fixed n) {
    VECTOR2FP result;
    result.x = fix_mul(v.x, n);
    result.y = fix_mul(v.y, n);
    return result;
}

static VECTOR2I vector2i_div(VECTOR2I a, VECTOR2I b) {
    VECTOR2I result;
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    return result;
}

static VECTOR2F vector2f_div(VECTOR2F a, VECTOR2F b) {
    VECTOR2F result;
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    return result;
}

static VECTOR2FP vector2fp_div(VECTOR2FP a, VECTOR2FP b) {
    VECTOR2FP result;
    result.x = fix_div(a.x, b.x);
    result.y = fix_div(a.y, b.y);
    return result;
}

static VECTOR2I vector2i_divs(VECTOR2I v, int n) {
    VECTOR2I result;
    result.x = v.x / n;
    result.y = v.y / n;
    return result;
}

static VECTOR2F vector2f_divs(VECTOR2F v, float n) {
    VECTOR2F result;
    result.x = v.x / n;
    result.y = v.y / n;
    return result;
}

static VECTOR2FP vector2fp_divs(VECTOR2FP v, fixed n) {
    VECTOR2FP result;
    result.x = fix_div(v.x, n);
    result.y = fix_div(v.y, n);
    return result;
}

static int vector2i_distance(VECTOR2I a, VECTOR2I b) {
    return (int)sqrt(
        ((b.x - a.x) * (b.x - a.x)) +
        ((b.y - a.y) * (b.y - a.y))
        );
}

static float vector2f_distance(VECTOR2F a, VECTOR2F b) {
    return (float)sqrt(
        ((b.x - a.x) * (b.x - a.x)) +
        ((b.y - a.y) * (b.y - a.y))
        );
}

static fixed vector2fp_distance(VECTOR2FP a, VECTOR2FP b) {
    return fix_sqrt(
        fix_mul((b.x - a.x), (b.x - a.x)) +
        fix_mul((b.y - a.y), (b.y - a.y))
        );
}

static int vector2i_distancesq(VECTOR2I a, VECTOR2I b) {
    return
        ((b.x - a.x) * (b.x - a.x)) +
        ((b.y - a.y) * (b.y - a.y));
}

static float vector2f_distancesq(VECTOR2F a, VECTOR2F b) {
    return
        ((b.x - a.x) * (b.x - a.x)) +
        ((b.y - a.y) * (b.y - a.y));
}

static fixed vector2fp_distancesq(VECTOR2FP a, VECTOR2FP b) {
    return
        fix_mul((b.x - a.x), (b.x - a.x)) +
        fix_mul((b.y - a.y), (b.y - a.y));
}

static int vector2i_dot(VECTOR2I a, VECTOR2I b) {
    return
        (a.x * b.x) +
        (a.y * b.y);
}

static float vector2f_dot(VECTOR2F a, VECTOR2F b) {
    return
        (a.x * b.x) +
        (a.y * b.y);
}

static fixed vector2fp_dot(VECTOR2FP a, VECTOR2FP b) {
    return
        fix_mul(a.x, b.x) +
        fix_mul(a.y, b.y);
}

static int vector2i_length(VECTOR2I v) {
    return sqrt(
        (v.x * v.x) +
        (v.y * v.y)
        );
}

static float vector2f_length(VECTOR2F v) {
    return (float)sqrt(
        (v.x * v.x) +
        (v.y * v.y)
        );
}

static fixed vector2fp_length(VECTOR2FP v) {
    return fix_sqrt(
        fix_mul(v.x, v.x) +
        fix_mul(v.y, v.y)
        );
}

static int vector2i_lengthsq(VECTOR2I v) {
    return
        (v.x * v.x) +
        (v.y * v.y);
}

static float vector2f_lengthsq(VECTOR2F v) {
    return
        (v.x * v.x) +
        (v.y * v.y);
}

static fixed vector2fp_lengthsq(VECTOR2FP v) {
    return
        fix_mul(v.x, v.x) +
        fix_mul(v.y, v.y);
}

static VECTOR2F vector2f_normalize(VECTOR2F v) {
    float inverse_length;
    VECTOR2F result;

    inverse_length = 1.0f / vector2f_length(v);
    result.x = v.x * inverse_length;
    result.y = v.y * inverse_length;
    return result;
}

static VECTOR2FP vector2fp_normalize(VECTOR2FP v) {
    fixed inverse_length;
    VECTOR2FP result;

    inverse_length = fix_div(FP_1, vector2fp_length(v));
    result.x = fix_mul(v.x, inverse_length);
    result.y = fix_mul(v.y, inverse_length);
    return result;
}

static VECTOR2F vector2f_set_length(VECTOR2F v, float length) {
    float scale_factor;
    VECTOR2F result;

    scale_factor = length / vector2f_length(v);
    result.x = v.x * scale_factor;
    result.y = v.y * scale_factor;
    return result;
}

static VECTOR2FP vector2fp_set_length(VECTOR2FP v, fixed length) {
    fixed scale_factor;
    VECTOR2FP result;

    scale_factor = fix_div(length, vector2fp_length(v));
    result.x = fix_mul(v.x, scale_factor);
    result.y = fix_mul(v.y, scale_factor);
    return result;
}

static VECTOR2I vector2i_lerp(VECTOR2I a, VECTOR2I b, float lerp) {
    VECTOR2I result;
    result.x = a.x + (b.x - a.x) * lerp;
    result.y = a.y + (b.y - a.y) * lerp;
    return result;
}

static VECTOR2F vector2f_lerp(VECTOR2F a, VECTOR2F b, float lerp) {
    VECTOR2F result;
    result.x = a.x + (b.x - a.x) * lerp;
    result.y = a.y + (b.y - a.y) * lerp;
    return result;
}

static VECTOR2FP vector2fp_lerp(VECTOR2FP a, VECTOR2FP b, fixed lerp) {
    VECTOR2FP result;
    result.x = a.x + fix_mul((b.x - a.x), lerp);
    result.y = a.y + fix_mul((b.y - a.y), lerp);
    return result;
}

#endif

