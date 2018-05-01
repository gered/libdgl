#include "fixed.h"
#include "dgl.h"
#include <stdio.h>
#include "helpers.h"

void test_fixed(void) {
    fixed a, b, c;
    float af, bf, cf, f;
    float aaf, bbf, ccf;
    int i;

    a = FTOFIX(PI);
    f = FIXTOF(a);
    ASSERT(FFIX_EQU(PI, f));

    a = FTOFIX(-0.38f);
    f = FIXTOF(a);
    ASSERT(FFIX_EQU(-0.38f, f));

    a = ITOFIX(17);
    i = FIXTOI(a);
    ASSERT(17 == i);

    a = ITOFIX(-17);
    i = FIXTOI(a);
    ASSERT(-17 == i);

    a = FTOFIX(1.5f);
    b = FTOFIX(2.3f);
    c = a + b;
    f = FIXTOF(c);
    ASSERT(FFIX_EQU((1.5f + 2.3f), f));

    a = FTOFIX(0.2f);
    b = FTOFIX(1.7f);
    c = a - b;
    f = FIXTOF(c);
    ASSERT(FFIX_EQU((0.2f - 1.7f), f));

    a = FTOFIX(16.0f);
    c = a >> 2;
    f = FIXTOF(c);
    ASSERT(FFIX_EQU((16 >> 2), f));

    a = FTOFIX(7.1f);
    b = ITOFIX(2);
    c = fix_mul(a, b);
    f = FIXTOF(c);
    ASSERT(FFIX_EQU((7.1f * 2), f));

    a = FTOFIX(77.7f);
    b = ITOFIX(3);
    c = fix_div(a, b);
    f = FIXTOF(c);
    ASSERT(FFIX_EQU((77.7f / 3), f));

    for (i = 0; i <= 360; i += 45) {
        f = DEG_TO_RAD(i + 1);
        a = fix_sin(FTOFIX(f));
        af = sin(f);
        aaf = FIXTOF(a);
        b = fix_cos(FTOFIX(f));
        bf = cos(f);
        bbf = FIXTOF(b);
        c = fix_tan(FTOFIX(f));
        cf = tan(f);
        ccf = FIXTOF(c);
        ASSERT(FFIX_EQU(aaf, af));
        ASSERT(FFIX_EQU(bbf, bf));
        ASSERT(FFIX_EQU(ccf, cf));
    }
}

