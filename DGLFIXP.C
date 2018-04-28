#include "dglfixp.h"

fixed fix_sqrt(fixed x) {
    int t, q, b, r;
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

