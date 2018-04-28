#ifndef DGL_DGLMAT33_H_INCLUDED
#define DGL_DGLMAT33_H_INCLUDED

#include "dglcmn.h"
#include "dglmath.h"
#include "dglvec2.h"
#include <math.h>

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
    float m[9];
} MATRIX33;

static MATRIX33 matrix33(float m11, float m12, float m13,
                         float m21, float m22, float m23,
                         float m31, float m32, float m33);
static void matrix33_set(MATRIX33 *m,
                         float m11, float m12, float m13,
                         float m21, float m22, float m23,
                         float m31, float m32, float m33);

static MATRIX33 matrix33_from_euler_angles(float x, float y, float z);
static MATRIX33 matrix33_rotation_x(float radians);
static MATRIX33 matrix33_rotation_y(float radians);
static MATRIX33 matrix33_rotation_z(float radians);

static MATRIX33 matrix33_add(MATRIX33 a, MATRIX33 b);
static MATRIX33 matrix33_sub(MATRIX33 a, MATRIX33 b);
static MATRIX33 matrix33_mul(MATRIX33 a, MATRIX33 b);
static MATRIX33 matrix33_scale(MATRIX33 m, float scale);

static float matrix33_determinant(MATRIX33 m);
static MATRIX33 matrix33_inverse(MATRIX33 m);
static MATRIX33 matrix33_transpose(MATRIX33 m);
static VECTOR2F matrix33_transform(MATRIX33 m, VECTOR2F v);

static MATRIX33 matrix33_translation_2d(float x, float y);
static MATRIX33 matrix33_scaling_2d(float x, float y);
static MATRIX33 matrix33_rotation_2d(float radians);
static VECTOR2F matrix33_transform_2d(MATRIX33 m, VECTOR2F v);

#define IDENTITY_MATRIX33 matrix33(1.0f, 0.0f, 0.0f, \
                                   0.0f, 1.0f, 0.0f, \
                                   0.0f, 0.0f, 1.0f)

// --------------------------------------------------------------------------

static MATRIX33 matrix33(float m11, float m12, float m13,
                         float m21, float m22, float m23,
                         float m31, float m32, float m33) {
    MATRIX33 result;
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

static void matrix33_set(MATRIX33 *m,
                         float m11, float m12, float m13,
                         float m21, float m22, float m23,
                         float m31, float m32, float m33) {
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

static MATRIX33 matrix33_from_euler_angles(float x, float y, float z) {
    MATRIX33 rx, ry, rz;
    rx = matrix33_rotation_x(x);
    ry = matrix33_rotation_y(y);
    rz = matrix33_rotation_z(z);
    return matrix33_mul(matrix33_mul(rz, ry), rx);
}

static MATRIX33 matrix33_rotation_x(float radians) {
    MATRIX33 result;
    float s, c;

    s = sin(radians);
    c = cos(radians);

    result.m[_M33_11] = 1.0f;
    result.m[_M33_12] = 0.0f;
    result.m[_M33_13] = 0.0f;

    result.m[_M33_21] = 0.0f;
    result.m[_M33_22] = c;
    result.m[_M33_23] = -s;

    result.m[_M33_31] = 0.0f;
    result.m[_M33_32] = s;
    result.m[_M33_33] = c;

    return result;
}

static MATRIX33 matrix33_rotation_y(float radians) {
    MATRIX33 result;
    float s, c;

    s = sin(radians);
    c = cos(radians);

    result.m[_M33_11] = c;
    result.m[_M33_12] = 0.0f;
    result.m[_M33_13] = s;

    result.m[_M33_21] = 0.0f;
    result.m[_M33_22] = 1.0f;
    result.m[_M33_23] = 0.0f;

    result.m[_M33_31] = -s;
    result.m[_M33_32] = 0.0f;
    result.m[_M33_33] = c;

    return result;
}

static MATRIX33 matrix33_rotation_z(float radians) {
    MATRIX33 result;
    float s, c;

    s = sin(radians);
    c = cos(radians);

    result.m[_M33_11] = c;
    result.m[_M33_12] = -s;
    result.m[_M33_13] = 0.0f;

    result.m[_M33_21] = s;
    result.m[_M33_22] = c;
    result.m[_M33_23] = 0.0f;

    result.m[_M33_31] = 0.0f;
    result.m[_M33_32] = 0.0f;
    result.m[_M33_33] = 1.0f;

    return result;
}

static MATRIX33 matrix33_add(MATRIX33 a, MATRIX33 b) {
    MATRIX33 result;

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

static MATRIX33 matrix33_sub(MATRIX33 a, MATRIX33 b) {
    MATRIX33 result;

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

static MATRIX33 matrix33_mul(MATRIX33 a, MATRIX33 b) {
    MATRIX33 result;

    result.m[_M33_11] = a.m[_M33_11] * b.m[_M33_11] + a.m[_M33_12] * b.m[_M33_21] + a.m[_M33_13] * b.m[_M33_31];
    result.m[_M33_12] = a.m[_M33_11] * b.m[_M33_12] + a.m[_M33_12] * b.m[_M33_22] + a.m[_M33_13] * b.m[_M33_32];
    result.m[_M33_13] = a.m[_M33_11] * b.m[_M33_13] + a.m[_M33_12] * b.m[_M33_23] + a.m[_M33_13] * b.m[_M33_33];

    result.m[_M33_21] = a.m[_M33_21] * b.m[_M33_11] + a.m[_M33_22] * b.m[_M33_21] + a.m[_M33_23] * b.m[_M33_31];
    result.m[_M33_22] = a.m[_M33_21] * b.m[_M33_12] + a.m[_M33_22] * b.m[_M33_22] + a.m[_M33_23] * b.m[_M33_32];
    result.m[_M33_23] = a.m[_M33_21] * b.m[_M33_13] + a.m[_M33_22] * b.m[_M33_23] + a.m[_M33_23] * b.m[_M33_33];

    result.m[_M33_31] = a.m[_M33_31] * b.m[_M33_11] + a.m[_M33_32] * b.m[_M33_21] + a.m[_M33_33] * b.m[_M33_31];
    result.m[_M33_32] = a.m[_M33_31] * b.m[_M33_12] + a.m[_M33_32] * b.m[_M33_22] + a.m[_M33_33] * b.m[_M33_32];
    result.m[_M33_33] = a.m[_M33_31] * b.m[_M33_13] + a.m[_M33_32] * b.m[_M33_23] + a.m[_M33_33] * b.m[_M33_33];

    return result;
}

static MATRIX33 matrix33_scale(MATRIX33 m, float scale) {
    MATRIX33 result;

    result.m[_M33_11] = m.m[_M33_11] * scale;
    result.m[_M33_12] = m.m[_M33_12] * scale;
    result.m[_M33_13] = m.m[_M33_13] * scale;
    result.m[_M33_21] = m.m[_M33_21] * scale;
    result.m[_M33_22] = m.m[_M33_22] * scale;
    result.m[_M33_23] = m.m[_M33_23] * scale;
    result.m[_M33_31] = m.m[_M33_31] * scale;
    result.m[_M33_32] = m.m[_M33_32] * scale;
    result.m[_M33_33] = m.m[_M33_33] * scale;

    return result;
}

static float matrix33_determinant(MATRIX33 m) {
    return
        m.m[_M33_11] * m.m[_M33_22] * m.m[_M33_33] +
        m.m[_M33_12] * m.m[_M33_23] * m.m[_M33_31] +
        m.m[_M33_13] * m.m[_M33_21] * m.m[_M33_32] -
        m.m[_M33_11] * m.m[_M33_23] * m.m[_M33_32] -
        m.m[_M33_12] * m.m[_M33_21] * m.m[_M33_33] -
        m.m[_M33_13] * m.m[_M33_22] * m.m[_M33_31];
}

static MATRIX33 matrix33_inverse(MATRIX33 m) {
    float d;
    MATRIX33 result;
    d = matrix33_determinant(m);
    if (close_enough(d, 0.0f, TOLERANCE))
        return IDENTITY_MATRIX33;
    else {
        d = 1.0f / d;

        result.m[_M33_11] = d * (m.m[_M33_22] * m.m[_M33_33] - m.m[_M33_32] * m.m[_M33_23]);
        result.m[_M33_21] = d * (m.m[_M33_31] * m.m[_M33_23] - m.m[_M33_21] * m.m[_M33_33]);
        result.m[_M33_31] = d * (m.m[_M33_21] * m.m[_M33_32] - m.m[_M33_31] * m.m[_M33_22]);
        result.m[_M33_21] = d * (m.m[_M33_32] * m.m[_M33_13] - m.m[_M33_12] * m.m[_M33_33]);
        result.m[_M33_22] = d * (m.m[_M33_11] * m.m[_M33_33] - m.m[_M33_31] * m.m[_M33_13]);
        result.m[_M33_23] = d * (m.m[_M33_31] * m.m[_M33_12] - m.m[_M33_11] * m.m[_M33_32]);
        result.m[_M33_31] = d * (m.m[_M33_12] * m.m[_M33_23] - m.m[_M33_22] * m.m[_M33_13]);
        result.m[_M33_32] = d * (m.m[_M33_21] * m.m[_M33_13] - m.m[_M33_11] * m.m[_M33_23]);
        result.m[_M33_33] = d * (m.m[_M33_11] * m.m[_M33_22] - m.m[_M33_21] * m.m[_M33_12]);

        return result;
    }
}

static MATRIX33 matrix33_transpose(MATRIX33 m) {
    MATRIX33 result;

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

static VECTOR2F matrix33_transform(MATRIX33 m, VECTOR2F v) {
    VECTOR2F result;

    result.x = v.x * m.m[_M33_11] + v.y * m.m[_M33_12] + m.m[_M33_13];
    result.y = v.x * m.m[_M33_21] + v.y * m.m[_M33_22] + m.m[_M33_23];

    return result;
}

static MATRIX33 matrix33_translation_2d(float x, float y) {
    MATRIX33 result;

    result.m[_M33_11] = 1.0f;
    result.m[_M33_12] = 0.0f;
    result.m[_M33_13] = 0.0f;

    result.m[_M33_21] = 0.0f;
    result.m[_M33_22] = 1.0f;
    result.m[_M33_23] = 0.0f;

    result.m[_M33_31] = x;
    result.m[_M33_32] = y;
    result.m[_M33_33] = 1.0f;

    return result;
}

static MATRIX33 matrix33_scaling_2d(float x, float y) {
    MATRIX33 result;

    result.m[_M33_11] = x;
    result.m[_M33_12] = 0.0f;
    result.m[_M33_13] = 0.0f;

    result.m[_M33_21] = 0.0f;
    result.m[_M33_22] = y;
    result.m[_M33_23] = 0.0f;

    result.m[_M33_31] = 0.0f;
    result.m[_M33_32] = 0.0f;
    result.m[_M33_33] = 1.0f;

    return result;
}

static MATRIX33 matrix33_rotation_2d(float radians) {
    return matrix33_rotation_z(radians);
}

static VECTOR2F matrix33_transform_2d(MATRIX33 m, VECTOR2F v) {
    VECTOR2F result;

    result.x = v.x * m.m[_M33_11] + v.y * m.m[_M33_12] + m.m[_M33_13];
    result.y = v.x * m.m[_M33_21] + v.y * m.m[_M33_22] + m.m[_M33_23];
    result.x += m.m[_M33_31];
    result.y += m.m[_M33_32];

    return result;
}

#endif

