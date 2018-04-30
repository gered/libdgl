#include "vector2.h"
#include "dgl.h"
#include "helpers.h"

void test_vector2i(void) {
    VECTOR2I a, b, c;
    int i;

    c = vector2i(3, 7);
    ASSERT(c.x == 3 && c.y == 7);

    vector2i_set(&c, 1, 2);
    ASSERT(c.x == 1 && c.y == 2);

    a = vector2i(1, 2);
    b = vector2i(1, 2);
    ASSERT(vector2i_equals(a, b));

    a = vector2i(3, 4);
    b = vector2i(1, 2);

    c = vector2i_add(a, b);
    ASSERT(c.x == 4 && c.y == 6);

    c = vector2i_sub(a, b);
    ASSERT(c.x == 2 && c.y == 2);

    c = vector2i_mul(a, b);
    ASSERT(c.x == 3 && c.y == 8);

    c = vector2i_muls(a, 2);
    ASSERT(c.x == 6 && c.y == 8);

    c = vector2i_div(a, b);
    ASSERT(c.x == 3 && c.y == 2);

    c = vector2i_divs(a, 2);
    ASSERT(c.x == 1 && c.y == 2);

    a = vector2i(1, 1);
    b = vector2i(1, 3);
    i = vector2i_distance(a, b);
    ASSERT(i == 2);

    i = vector2i_distancesq(a, b);
    ASSERT(i == 4);

    a = vector2i(-12, 16);
    b = vector2i(12, 9);
    i = vector2i_dot(a, b);
    ASSERT(i == 0);

    a = vector2i(-3, 0);
    i = vector2i_length(a);
    ASSERT(i == 3);

    i = vector2i_lengthsq(a);
    ASSERT(i == 9);

    a = vector2i(5, 0);
    b = vector2i(10, 0);
    c = vector2i_lerp(a, b, 0.5f);
    ASSERT(c.x == 7 && c.y == 0);
}

void test_vector2f(void) {
    VECTOR2F a, b, c;
    float f;

    c = vector2f(3.0f, 7.0f);
    ASSERT(F_EQU(c.x, 3.0f) && F_EQU(c.y, 7.0f));

    vector2f_set(&c, 1.0f, 2.0f);
    ASSERT(F_EQU(c.x, 1.0f) && F_EQU(c.y, 2.0f));

    a = vector2f(1.0f, 2.0f);
    b = vector2f(1.0f, 2.0f);
    ASSERT(vector2f_equals(a, b));

    a = vector2f(3.0f, 4.0f);
    b = vector2f(1.0f, 2.0f);

    c = vector2f_add(a, b);
    ASSERT(F_EQU(c.x, 4.0f) && F_EQU(c.y, 6.0f));

    c = vector2f_sub(a, b);
    ASSERT(F_EQU(c.x, 2.0f) && F_EQU(c.y, 2.0f));

    c = vector2f_mul(a, b);
    ASSERT(F_EQU(c.x, 3.0f) && F_EQU(c.y, 8.0f));

    c = vector2f_muls(a, 0.5f);
    ASSERT(F_EQU(c.x, 1.5f) && F_EQU(c.y, 2.0f));

    c = vector2f_div(a, b);
    ASSERT(F_EQU(c.x, 3.0f) && F_EQU(c.y, 2.0f));

    c = vector2f_divs(a, 0.5f);
    ASSERT(F_EQU(c.x, 6.0f) && F_EQU(c.y, 8.0f));

    a = vector2f(1.0f, 1.0f);
    b = vector2f(1.0f, 3.0f);
    f = vector2f_distance(a, b);
    ASSERT(F_EQU(f, 2.0f));

    f = vector2f_distancesq(a, b);
    ASSERT(F_EQU(f, 4.0f));

    a = vector2f(-12.0f, 16.0f);
    b = vector2f(12.0f, 9.0f);
    f = vector2f_dot(a, b);
    ASSERT(F_EQU(f, 0.0f));

    a = vector2f(-3.0f, 0.0f);
    f = vector2f_length(a);
    ASSERT(F_EQU(f, 3.0f));

    f = vector2f_lengthsq(a);
    ASSERT(F_EQU(f, 9.0f));

    a = vector2f(7.0f, 7.0f);
    c = vector2f_normalize(a);
    ASSERT(F_EQU(c.x, 0.70710f) && F_EQU(c.y, 0.70710f));

    a = vector2f(10.0f, 0.0f);
    c = vector2f_set_length(a, 2.0f);
    ASSERT(F_EQU(c.x, 2.0f) && F_EQU(c.y, 0.0f));

    a = vector2f(5.0f, 0.0f);
    b = vector2f(10.0f, 0.0f);
    c = vector2f_lerp(a, b, 0.5f);
    ASSERT(F_EQU(c.x, 7.5f) && F_EQU(c.y, 0.0f));
}

