#include "dglfixp.h"

fixed fix_sqrt(fixed x) {
    int32 t, q, b, r;
    r = x;
    b = 0x40000000;
    q = 0;
    while (b > 0x40) {
        t = q + b;
        if (r >= t) {
            r -= t;
            q = t + b;
        }
        r <<= 1;
        b >>= 1;
    }
    q >>= 8;
    return q;
}

fixed fix_atan2(fixed y, fixed x) {
    fixed absY, mask, angle, r, r3;

    mask = (y >> (sizeof(fixed)*8-1));
    absY = (y + mask) ^ mask;

    if (x >= 0) {
        r = fix_div(x - absY, x + absY);

    } else {
        r = fix_div(x + absY, absY - x);

    }

    return 0;
}

