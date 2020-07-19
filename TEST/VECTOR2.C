#include "vector2.h"
#include "dglvec2.h"
#include "helpers.h"

void test_vec2i(void) {
    VEC2I a, b, c;
    int i;

    c = vec2i(3, 7);
    ASSERT(c.x == 3 && c.y == 7);

    vec2i_set(&c, 1, 2);
    ASSERT(c.x == 1 && c.y == 2);

    a = vec2i(1, 2);
    b = vec2i(1, 2);
    ASSERT(vec2i_equals(a, b));

    a = vec2i(3, 4);
    b = vec2i(1, 2);

    c = vec2i_add(a, b);
    ASSERT(c.x == 4 && c.y == 6);

    c = vec2i_sub(a, b);
    ASSERT(c.x == 2 && c.y == 2);

    c = vec2i_mul(a, b);
    ASSERT(c.x == 3 && c.y == 8);

    c = vec2i_muls(a, 2);
    ASSERT(c.x == 6 && c.y == 8);

    c = vec2i_div(a, b);
    ASSERT(c.x == 3 && c.y == 2);

    c = vec2i_divs(a, 2);
    ASSERT(c.x == 1 && c.y == 2);

    a = vec2i(1, 1);
    b = vec2i(1, 3);
    i = vec2i_distance(a, b);
    ASSERT(i == 2);

    i = vec2i_distancesq(a, b);
    ASSERT(i == 4);

    a = vec2i(-12, 16);
    b = vec2i(12, 9);
    i = vec2i_dot(a, b);
    ASSERT(i == 0);

    a = vec2i(-3, 0);
    i = vec2i_length(a);
    ASSERT(i == 3);

    i = vec2i_lengthsq(a);
    ASSERT(i == 9);

    a = vec2i(5, 0);
    b = vec2i(10, 0);
    c = vec2i_lerp(a, b, 0.5f);
    ASSERT(c.x == 7 && c.y == 0);
}

void test_vec2(void) {
    VEC2 a, b, c;
    fixed f;

    c = vec2(FTOFIX(3.0f), FTOFIX(7.0f));
    ASSERT(c.x == FTOFIX(3.0f) && c.y == FTOFIX(7.0f));

    vec2_set(&c, FTOFIX(1.0f), FTOFIX(2.0f));
    ASSERT(c.x == FTOFIX(1.0f) && c.y == FTOFIX(2.0f));

    a = vec2(FTOFIX(1.0f), FTOFIX(2.0f));
    b = vec2(FTOFIX(1.0f), FTOFIX(2.0f));
    ASSERT(vec2_equals(a, b));

    a = vec2(FTOFIX(3.0f), FTOFIX(4.0f));
    b = vec2(FTOFIX(1.0f), FTOFIX(2.0f));

    c = vec2_add(a, b);
    ASSERT(c.x == FTOFIX(4.0f) && c.y == FTOFIX(6.0f));

    c = vec2_sub(a, b);
    ASSERT(c.x == FTOFIX(2.0f) && c.y == FTOFIX(2.0f));

    c = vec2_mul(a, b);
    ASSERT(c.x == FTOFIX(3.0f) && c.y == FTOFIX(8.0f));

    c = vec2_muls(a, FTOFIX(0.5f));
    ASSERT(c.x == FTOFIX(1.5f) && c.y == FTOFIX(2.0f));

    c = vec2_div(a, b);
    ASSERT(c.x == FTOFIX(3.0f) && c.y == FTOFIX(2.0f));

    c = vec2_divs(a, FTOFIX(0.5f));
    ASSERT(c.x == FTOFIX(6.0f) && c.y == FTOFIX(8.0f));

    a = vec2(FTOFIX(1.0f), FTOFIX(1.0f));
    b = vec2(FTOFIX(1.0f), FTOFIX(3.0f));
    f = vec2_distance(a, b);
    ASSERT(f == FTOFIX(2.0f));

    f = vec2_distancesq(a, b);
    ASSERT(f == FTOFIX(4.0f));

    a = vec2(FTOFIX(-12.0f), FTOFIX(16.0f));
    b = vec2(FTOFIX(12.0f), FTOFIX(9.0f));
    f = vec2_dot(a, b);
    ASSERT(f == 0);

    a = vec2(FTOFIX(-3.0f), FTOFIX(0.0f));
    f = vec2_length(a);
    ASSERT(f == FTOFIX(3.0f));

    f = vec2_lengthsq(a);
    ASSERT(f == FTOFIX(9.0f));

    a = vec2(FTOFIX(7.0f), FTOFIX(7.0f));
    c = vec2_normalize(a);
    ASSERT(c.x == FTOFIX(0.70710f) && c.y == FTOFIX(0.70710f));

    a = vec2(FTOFIX(10.0f), FTOFIX(0.0f));
    c = vec2_set_length(a, FTOFIX(2.0f));
    ASSERT(c.x == FTOFIX(2.0f) && c.y == FTOFIX(0.0f));

    a = vec2(FTOFIX(5.0f), FTOFIX(0.0f));
    b = vec2(FTOFIX(10.0f), FTOFIX(0.0f));
    c = vec2_lerp(a, b, FTOFIX(0.5f));
    ASSERT(c.x == FTOFIX(7.5f) && c.y == FTOFIX(0.0f));
}

