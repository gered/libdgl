#ifndef LIBDGL_DGLVEC2_H
#define LIBDGL_DGLVEC2_H

#include <math.h>
#include "dglcmn.h"
#include "dglfixp.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int x;
    int y;
} VEC2I;

static VEC2I vec2i(int x, int y);
static void vec2i_set(VEC2I *v, int x, int y);
static bool vec2i_equals(VEC2I a, VEC2I b);
static VEC2I vec2i_add(VEC2I a, VEC2I b);
static VEC2I vec2i_sub(VEC2I a, VEC2I b);
static VEC2I vec2i_mul(VEC2I a, VEC2I b);
static VEC2I vec2i_muls(VEC2I v, int n);
static VEC2I vec2i_div(VEC2I a, VEC2I b);
static VEC2I vec2i_divs(VEC2I v, int n);
static int vec2i_distance(VEC2I a, VEC2I b);
static int vec2i_distancesq(VEC2I a, VEC2I b);
static int vec2i_dot(VEC2I a, VEC2I b);
static int vec2i_length(VEC2I v);
static int vec2i_lengthsq(VEC2I v);
static VEC2I vec2i_lerp(VEC2I a, VEC2I b, float lerp);

#define VEC2I_ZERO           vec2i(0, 0)
#define VEC2I_UP             vec2i(0, -1);
#define VEC2I_DOWN           vec2i(0, 1);
#define VEC2I_LEFT           vec2i(-1, 0);
#define VEC2I_RIGHT          vec2i(1, 0);
#define VEC2I_UNIT_X         vec2i(1, 0);
#define VEC2I_UNIT_Y         vec2i(0, 1);


typedef struct {
    fixed x;
    fixed y;
} VEC2;

static VEC2 vec2(fixed x, fixed y);
static void vec2_set(VEC2 *v, fixed x, fixed y);
static bool vec2_equals(VEC2 a, VEC2 b);
static VEC2 vec2_add(VEC2 a, VEC2 b);
static VEC2 vec2_sub(VEC2 a, VEC2 b);
static VEC2 vec2_mul(VEC2 a, VEC2 b);
static VEC2 vec2_muls(VEC2 v, fixed n);
static VEC2 vec2_div(VEC2 a, VEC2 b);
static VEC2 vec2_divs(VEC2 v, fixed n);
static fixed vec2_distance(VEC2 a, VEC2 b);
static fixed vec2_distancesq(VEC2 a, VEC2 b);
static fixed vec2_dot(VEC2 a, VEC2 b);
static fixed vec2_length(VEC2 v);
static fixed vec2_lengthsq(VEC2 v);
static VEC2 vec2_normalize(VEC2 v);
static VEC2 vec2_set_length(VEC2 v, fixed length);
static VEC2 vec2_lerp(VEC2 a, VEC2 b, fixed lerp);

#define VEC2_ZERO            vec2(0, 0)
#define VEC2_UP              vec2(0, ITOFIX(-1));
#define VEC2_DOWN            vec2(0, ITOFIX(1));
#define VEC2_LEFT            vec2(ITOFIX(-1), 0);
#define VEC2_RIGHT           vec2(ITOFIX(1), 0);
#define VEC2_UNIT_X          vec2(ITOFIX(1), 0);
#define VEC2_UNIT_Y          vec2(0, ITOFIX(1));

// --------------------------------------------------------------------------

static VEC2I vec2i(int x, int y) {
    VEC2I v;
    v.x = x;
    v.y = y;
    return v;
}

static VEC2 vec2(fixed x, fixed y) {
    VEC2 v;
    v.x = x;
    v.y = y;
    return v;
}

static void vec2i_set(VEC2I *v, int x, int y) {
    v->x = x;
    v->y = y;
}

static void vec2_set(VEC2 *v, fixed x, fixed y) {
    v->x = x;
    v->y = y;
}

static bool vec2i_equals(VEC2I a, VEC2I b) {
    return (a.x == b.x && a.y == b.y);
}

static bool vec2_equals(VEC2 a, VEC2 b) {
    return (a.x == b.x && a.y == b.y);
}

static VEC2I vec2i_add(VEC2I a, VEC2I b) {
    VEC2I result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

static VEC2 vec2_add(VEC2 a, VEC2 b) {
    VEC2 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

static VEC2I vec2i_sub(VEC2I a, VEC2I b) {
    VEC2I result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

static VEC2 vec2_sub(VEC2 a, VEC2 b) {
    VEC2 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

static VEC2I vec2i_mul(VEC2I a, VEC2I b) {
    VEC2I result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    return result;
}

static VEC2 vec2_mul(VEC2 a, VEC2 b) {
    VEC2 result;
    result.x = fix_mul(a.x, b.x);
    result.y = fix_mul(a.y, b.y);
    return result;
}

static VEC2I vec2i_muls(VEC2I v, int n) {
    VEC2I result;
    result.x = v.x * n;
    result.y = v.y * n;
    return result;
}

static VEC2 vec2_muls(VEC2 v, fixed n) {
    VEC2 result;
    result.x = fix_mul(v.x, n);
    result.y = fix_mul(v.y, n);
    return result;
}

static VEC2I vec2i_div(VEC2I a, VEC2I b) {
    VEC2I result;
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    return result;
}

static VEC2 vec2_div(VEC2 a, VEC2 b) {
    VEC2 result;
    result.x = fix_div(a.x, b.x);
    result.y = fix_div(a.y, b.y);
    return result;
}

static VEC2I vec2i_divs(VEC2I v, int n) {
    VEC2I result;
    result.x = v.x / n;
    result.y = v.y / n;
    return result;
}

static VEC2 vec2_divs(VEC2 v, fixed n) {
    VEC2 result;
    result.x = fix_div(v.x, n);
    result.y = fix_div(v.y, n);
    return result;
}

static int vec2i_distance(VEC2I a, VEC2I b) {
    return (int)sqrt(
        ((b.x - a.x) * (b.x - a.x)) +
        ((b.y - a.y) * (b.y - a.y))
        );
}

static fixed vec2_distance(VEC2 a, VEC2 b) {
    return fix_sqrt(
        fix_mul((b.x - a.x), (b.x - a.x)) +
        fix_mul((b.y - a.y), (b.y - a.y))
        );
}

static int vec2i_distancesq(VEC2I a, VEC2I b) {
    return
        ((b.x - a.x) * (b.x - a.x)) +
        ((b.y - a.y) * (b.y - a.y));
}

static fixed vec2_distancesq(VEC2 a, VEC2 b) {
    return
        fix_mul((b.x - a.x), (b.x - a.x)) +
        fix_mul((b.y - a.y), (b.y - a.y));
}

static int vec2i_dot(VEC2I a, VEC2I b) {
    return
        (a.x * b.x) +
        (a.y * b.y);
}

static fixed vec2_dot(VEC2 a, VEC2 b) {
    return
        fix_mul(a.x, b.x) +
        fix_mul(a.y, b.y);
}

static int vec2i_length(VEC2I v) {
    return sqrt(
        (v.x * v.x) +
        (v.y * v.y)
        );
}

static fixed vec2_length(VEC2 v) {
    return fix_sqrt(
        fix_mul(v.x, v.x) +
        fix_mul(v.y, v.y)
        );
}

static int vec2i_lengthsq(VEC2I v) {
    return
        (v.x * v.x) +
        (v.y * v.y);
}

static fixed vec2_lengthsq(VEC2 v) {
    return
        fix_mul(v.x, v.x) +
        fix_mul(v.y, v.y);
}

static VEC2 vec2_normalize(VEC2 v) {
    fixed inverse_length;
    VEC2 result;

    inverse_length = fix_div(FP_1, vec2_length(v));
    result.x = fix_mul(v.x, inverse_length);
    result.y = fix_mul(v.y, inverse_length);
    return result;
}

static VEC2 vec2_set_length(VEC2 v, fixed length) {
    fixed scale_factor;
    VEC2 result;

    scale_factor = fix_div(length, vec2_length(v));
    result.x = fix_mul(v.x, scale_factor);
    result.y = fix_mul(v.y, scale_factor);
    return result;
}

static VEC2I vec2i_lerp(VEC2I a, VEC2I b, float lerp) {
    VEC2I result;
    result.x = a.x + (b.x - a.x) * lerp;
    result.y = a.y + (b.y - a.y) * lerp;
    return result;
}

static VEC2 vec2_lerp(VEC2 a, VEC2 b, fixed lerp) {
    VEC2 result;
    result.x = a.x + fix_mul((b.x - a.x), lerp);
    result.y = a.y + fix_mul((b.y - a.y), lerp);
    return result;
}

#ifdef __cplusplus
}
#endif

#endif

