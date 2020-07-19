#ifndef LIBDGL_DGLMTX33_H
#define LIBDGL_DGLMTX33_H

#include <math.h>
#include "dglcmn.h"
#include "dglfixp.h"
#include "dglvec2.h"

#ifdef __cplusplus
extern "C" {
#endif

#define _M33_11 0
#define _M33_12 3
#define _M33_13 6
#define _M33_21 1
#define _M33_22 4
#define _M33_23 7
#define _M33_31 2
#define _M33_32 5
#define _M33_33 8

typedef struct {
    fixed m[9];
} MTX33;

static MTX33 mtx33(
    fixed m11, fixed m12, fixed m13,
    fixed m21, fixed m22, fixed m23,
    fixed m31, fixed m32, fixed m33
);

static void mtx33_set(
    MTX33 *m,
    fixed m11, fixed m12, fixed m13,
    fixed m21, fixed m22, fixed m23,
    fixed m31, fixed m32, fixed m33
);

static MTX33 mtx33_from_angles(float x, float y, float z);
static MTX33 mtx33_rotation_x(float radians);
static MTX33 mtx33_rotation_y(float radians);
static MTX33 mtx33_rotation_z(float radians);

static MTX33 mtx33_add(MTX33 a, MTX33 b);
static MTX33 mtx33_sub(MTX33 a, MTX33 b);
static MTX33 mtx33_mul(MTX33 a, MTX33 b);
static MTX33 mtx33_scale(MTX33 m, fixed scale);

static fixed mtx33_determinant(MTX33 m);
static MTX33 mtx33_inverse(MTX33 m);
static MTX33 mtx33_transpose(MTX33 m);
static VEC2 mtx33_transform(MTX33 m, VEC2 v);

static MTX33 mtx33_translation_2d(fixed x, fixed y);
static MTX33 mtx33_scaling_2d(fixed x, fixed y);
static MTX33 mtx33_rotation_2d(float radians);
static VEC2 mtx33_transform_2d(MTX33 m, VEC2 v);

#define IDENTITY_MTX33 mtx33(1.0f, 0.0f, 0.0f, \
                             0.0f, 1.0f, 0.0f, \
                             0.0f, 0.0f, 1.0f)

// --------------------------------------------------------------------------

static MTX33 mtx33(
    fixed m11, fixed m12, fixed m13,
    fixed m21, fixed m22, fixed m23,
    fixed m31, fixed m32, fixed m33
) {
    MTX33 result;
    result.m[_M33_11] = m11;
    result.m[_M33_12] = m12;
    result.m[_M33_13] = m13;
    result.m[_M33_21] = m21;
    result.m[_M33_22] = m22;
    result.m[_M33_23] = m23;
    result.m[_M33_31] = m31;
    result.m[_M33_32] = m32;
    result.m[_M33_33] = m33;
    return result;
}

static void mtx33_set(
    MTX33 *m,
    fixed m11, fixed m12, fixed m13,
    fixed m21, fixed m22, fixed m23,
    fixed m31, fixed m32, fixed m33
) {
    m->m[_M33_11] = m11;
    m->m[_M33_12] = m12;
    m->m[_M33_13] = m13;
    m->m[_M33_21] = m21;
    m->m[_M33_22] = m22;
    m->m[_M33_23] = m23;
    m->m[_M33_31] = m31;
    m->m[_M33_32] = m32;
    m->m[_M33_33] = m33;
}

static MTX33 mtx33_from_angles(float x, float y, float z) {
    MTX33 rx, ry, rz;
    rx = mtx33_rotation_x(x);
    ry = mtx33_rotation_y(y);
    rz = mtx33_rotation_z(z);
    return mtx33_mul(mtx33_mul(rz, ry), rx);
}

static MTX33 mtx33_rotation_x(fixed radians) {
    MTX33 result;
    fixed s, c;

    s = FTOFIX(sin(radians));
    c = FTOFIX(cos(radians));

    result.m[_M33_11] = FP_1;
    result.m[_M33_12] = 0;
    result.m[_M33_13] = 0;

    result.m[_M33_21] = 0;
    result.m[_M33_22] = c;
    result.m[_M33_23] = -s;

    result.m[_M33_31] = 0;
    result.m[_M33_32] = s;
    result.m[_M33_33] = c;

    return result;
}

static MTX33 mtx33_rotation_y(float radians) {
    MTX33 result;
    fixed s, c;

    s = FTOFIX(sin(radians));
    c = FTOFIX(cos(radians));

    result.m[_M33_11] = c;
    result.m[_M33_12] = 0;
    result.m[_M33_13] = s;

    result.m[_M33_21] = 0;
    result.m[_M33_22] = FP_1;
    result.m[_M33_23] = 0;

    result.m[_M33_31] = -s;
    result.m[_M33_32] = 0;
    result.m[_M33_33] = c;

    return result;
}

static MTX33 mtx33_rotation_z(float radians) {
    MTX33 result;
    fixed s, c;

    s = FTOFIX(sin(radians));
    c = FTOFIX(cos(radians));

    result.m[_M33_11] = c;
    result.m[_M33_12] = -s;
    result.m[_M33_13] = 0;

    result.m[_M33_21] = s;
    result.m[_M33_22] = c;
    result.m[_M33_23] = 0;

    result.m[_M33_31] = 0;
    result.m[_M33_32] = 0;
    result.m[_M33_33] = FP_1;

    return result;
}

static MTX33 mtx33_add(MTX33 a, MTX33 b) {
    MTX33 result;

    result.m[_M33_11] = a.m[_M33_11] + b.m[_M33_11];
    result.m[_M33_12] = a.m[_M33_12] + b.m[_M33_12];
    result.m[_M33_13] = a.m[_M33_13] + b.m[_M33_13];
    result.m[_M33_21] = a.m[_M33_21] + b.m[_M33_21];
    result.m[_M33_22] = a.m[_M33_22] + b.m[_M33_22];
    result.m[_M33_23] = a.m[_M33_23] + b.m[_M33_23];
    result.m[_M33_31] = a.m[_M33_31] + b.m[_M33_31];
    result.m[_M33_32] = a.m[_M33_32] + b.m[_M33_32];
    result.m[_M33_33] = a.m[_M33_33] + b.m[_M33_33];

    return result;
}

static MTX33 mtx33_sub(MTX33 a, MTX33 b) {
    MTX33 result;

    result.m[_M33_11] = a.m[_M33_11] - b.m[_M33_11];
    result.m[_M33_12] = a.m[_M33_12] - b.m[_M33_12];
    result.m[_M33_13] = a.m[_M33_13] - b.m[_M33_13];
    result.m[_M33_21] = a.m[_M33_21] - b.m[_M33_21];
    result.m[_M33_22] = a.m[_M33_22] - b.m[_M33_22];
    result.m[_M33_23] = a.m[_M33_23] - b.m[_M33_23];
    result.m[_M33_31] = a.m[_M33_31] - b.m[_M33_31];
    result.m[_M33_32] = a.m[_M33_32] - b.m[_M33_32];
    result.m[_M33_33] = a.m[_M33_33] - b.m[_M33_33];

    return result;
}

static MTX33 mtx33_mul(MTX33 a, MTX33 b) {
    MTX33 result;

    result.m[_M33_11] = fix_mul(a.m[_M33_11], b.m[_M33_11]) + fix_mul(a.m[_M33_12], b.m[_M33_21]) + fix_mul(a.m[_M33_13], b.m[_M33_31]);
    result.m[_M33_12] = fix_mul(a.m[_M33_11], b.m[_M33_12]) + fix_mul(a.m[_M33_12], b.m[_M33_22]) + fix_mul(a.m[_M33_13], b.m[_M33_32]);
    result.m[_M33_13] = fix_mul(a.m[_M33_11], b.m[_M33_13]) + fix_mul(a.m[_M33_12], b.m[_M33_23]) + fix_mul(a.m[_M33_13], b.m[_M33_33]);

    result.m[_M33_21] = fix_mul(a.m[_M33_21], b.m[_M33_11]) + fix_mul(a.m[_M33_22], b.m[_M33_21]) + fix_mul(a.m[_M33_23], b.m[_M33_31]);
    result.m[_M33_22] = fix_mul(a.m[_M33_21], b.m[_M33_12]) + fix_mul(a.m[_M33_22], b.m[_M33_22]) + fix_mul(a.m[_M33_23], b.m[_M33_32]);
    result.m[_M33_23] = fix_mul(a.m[_M33_21], b.m[_M33_13]) + fix_mul(a.m[_M33_22], b.m[_M33_23]) + fix_mul(a.m[_M33_23], b.m[_M33_33]);

    result.m[_M33_31] = fix_mul(a.m[_M33_31], b.m[_M33_11]) + fix_mul(a.m[_M33_32], b.m[_M33_21]) + fix_mul(a.m[_M33_33], b.m[_M33_31]);
    result.m[_M33_32] = fix_mul(a.m[_M33_31], b.m[_M33_12]) + fix_mul(a.m[_M33_32], b.m[_M33_22]) + fix_mul(a.m[_M33_33], b.m[_M33_32]);
    result.m[_M33_33] = fix_mul(a.m[_M33_31], b.m[_M33_13]) + fix_mul(a.m[_M33_32], b.m[_M33_23]) + fix_mul(a.m[_M33_33], b.m[_M33_33]);

    return result;
}

static MTX33 mtx33_scale(MTX33 m, fixed scale) {
    MTX33 result;

    result.m[_M33_11] = fix_mul(m.m[_M33_11], scale);
    result.m[_M33_12] = fix_mul(m.m[_M33_12], scale);
    result.m[_M33_13] = fix_mul(m.m[_M33_13], scale);
    result.m[_M33_21] = fix_mul(m.m[_M33_21], scale);
    result.m[_M33_22] = fix_mul(m.m[_M33_22], scale);
    result.m[_M33_23] = fix_mul(m.m[_M33_23], scale);
    result.m[_M33_31] = fix_mul(m.m[_M33_31], scale);
    result.m[_M33_32] = fix_mul(m.m[_M33_32], scale);
    result.m[_M33_33] = fix_mul(m.m[_M33_33], scale);

    return result;
}

static fixed mtx33_determinant(MTX33 m) {
    return
        fix_mul(m.m[_M33_11], fix_mul(m.m[_M33_22], m.m[_M33_33])) +
        fix_mul(m.m[_M33_12], fix_mul(m.m[_M33_23], m.m[_M33_31])) +
        fix_mul(m.m[_M33_13], fix_mul(m.m[_M33_21], m.m[_M33_32])) -
        fix_mul(m.m[_M33_11], fix_mul(m.m[_M33_23], m.m[_M33_32])) -
        fix_mul(m.m[_M33_12], fix_mul(m.m[_M33_21], m.m[_M33_33])) -
        fix_mul(m.m[_M33_13], fix_mul(m.m[_M33_22], m.m[_M33_31]));
}

static MTX33 mtx33_inverse(MTX33 m) {
    fixed d;
    MTX33 result;
    d = mtx33_determinant(m);
    if (d == 0)
        return IDENTITY_MTX33;
    else {
        d = fix_div(FP_1, d);

        result.m[_M33_11] = fix_mul(d, (fix_mul(m.m[_M33_22], m.m[_M33_33]) - fix_mul(m.m[_M33_32], m.m[_M33_23])));
        result.m[_M33_21] = fix_mul(d, (fix_mul(m.m[_M33_31], m.m[_M33_23]) - fix_mul(m.m[_M33_21], m.m[_M33_33])));
        result.m[_M33_31] = fix_mul(d, (fix_mul(m.m[_M33_21], m.m[_M33_32]) - fix_mul(m.m[_M33_31], m.m[_M33_22])));
        result.m[_M33_21] = fix_mul(d, (fix_mul(m.m[_M33_32], m.m[_M33_13]) - fix_mul(m.m[_M33_12], m.m[_M33_33])));
        result.m[_M33_22] = fix_mul(d, (fix_mul(m.m[_M33_11], m.m[_M33_33]) - fix_mul(m.m[_M33_31], m.m[_M33_13])));
        result.m[_M33_23] = fix_mul(d, (fix_mul(m.m[_M33_31], m.m[_M33_12]) - fix_mul(m.m[_M33_11], m.m[_M33_32])));
        result.m[_M33_31] = fix_mul(d, (fix_mul(m.m[_M33_12], m.m[_M33_23]) - fix_mul(m.m[_M33_22], m.m[_M33_13])));
        result.m[_M33_32] = fix_mul(d, (fix_mul(m.m[_M33_21], m.m[_M33_13]) - fix_mul(m.m[_M33_11], m.m[_M33_23])));
        result.m[_M33_33] = fix_mul(d, (fix_mul(m.m[_M33_11], m.m[_M33_22]) - fix_mul(m.m[_M33_21], m.m[_M33_12])));

        return result;
    }
}

static MTX33 mtx33_transpose(MTX33 m) {
    MTX33 result;

    result.m[_M33_11] = m.m[_M33_11];
    result.m[_M33_12] = m.m[_M33_21];
    result.m[_M33_13] = m.m[_M33_31];

    result.m[_M33_21] = m.m[_M33_12];
    result.m[_M33_22] = m.m[_M33_22];
    result.m[_M33_23] = m.m[_M33_32];

    result.m[_M33_31] = m.m[_M33_13];
    result.m[_M33_32] = m.m[_M33_23];
    result.m[_M33_33] = m.m[_M33_33];

    return result;
}

static VEC2 mtx33_transform(MTX33 m, VEC2 v) {
    VEC2 result;

    result.x = fix_mul(v.x, m.m[_M33_11]) + fix_mul(v.y, m.m[_M33_12]) + m.m[_M33_13];
    result.y = fix_mul(v.x, m.m[_M33_21]) + fix_mul(v.y, m.m[_M33_22]) + m.m[_M33_23];

    return result;
}

static MTX33 mtx33_translation_2d(fixed x, fixed y) {
    MTX33 result;

    result.m[_M33_11] = FP_1;
    result.m[_M33_12] = 0;
    result.m[_M33_13] = 0;

    result.m[_M33_21] = 0;
    result.m[_M33_22] = FP_1;
    result.m[_M33_23] = 0;

    result.m[_M33_31] = x;
    result.m[_M33_32] = y;
    result.m[_M33_33] = FP_1;

    return result;
}

static MTX33 mtx33_scaling_2d(fixed x, fixed y) {
    MTX33 result;

    result.m[_M33_11] = x;
    result.m[_M33_12] = 0;
    result.m[_M33_13] = 0;

    result.m[_M33_21] = 0;
    result.m[_M33_22] = y;
    result.m[_M33_23] = 0;

    result.m[_M33_31] = 0;
    result.m[_M33_32] = 0;
    result.m[_M33_33] = FP_1;

    return result;
}

static MTX33 mtx33_rotation_2d(float radians) {
    return mtx33_rotation_z(radians);
}

static VEC2 mtx33_transform_2d(MTX33 m, VEC2 v) {
    VEC2 result;

    result.x = fix_mul(v.x, m.m[_M33_11]) + fix_mul(v.y, m.m[_M33_12]) + m.m[_M33_13];
    result.y = fix_mul(v.x, m.m[_M33_21]) + fix_mul(v.y, m.m[_M33_22]) + m.m[_M33_23];
    result.x += m.m[_M33_31];
    result.y += m.m[_M33_32];

    return result;
}

#ifdef __cplusplus
}
#endif

#endif

