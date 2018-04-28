#ifndef DGL_DGLFIXP_H_INCLUDED
#define DGL_DGLFIXP_H_INCLUDED

#include "dglcmn.h"
#include <math.h>

typedef int fixed;

#define FP_INT_SHIFT   16
#define FP_FLOAT_SHIFT 65536.0f

static fixed fix_sin(fixed x);
static fixed fix_cos(fixed x);
static fixed fix_tan(fixed x);

fixed fix_sqrt(fixed x);

fixed fix_mul(fixed a, fixed b);
#pragma aux fix_mul =               \
    "imul ebx"                      \
    "shrd eax, edx, 16"             \
    parm [eax] [ebx]                \
    modify [eax ebx edx];

fixed fix_div(fixed a, fixed b);
#pragma aux fix_div =               \
    "cdq"                           \
    "shld edx, eax, 16"             \
    "sal eax, 16"                   \
    "idiv ebx"                      \
    parm [eax] [ebx]                \
    modify [eax ebx edx];

#define FTOFIX(f)   ((fixed)((f) * FP_FLOAT_SHIFT))
#define ITOFIX(i)   ((fixed)((i) << FP_INT_SHIFT))
#define FIXTOF(x)   ((float)((x) / FP_FLOAT_SHIFT))
#define FIXTOI(x)   ((int)(((x) + 0x8000) >> FP_INT_SHIFT))

#define FP_1        ITOFIX(1)
#define FP_2        ITOFIX(2)
#define FP_3        ITOFIX(3)
#define FP_4        ITOFIX(4)
#define FP_5        ITOFIX(5)
#define FP_6        ITOFIX(6)
#define FP_7        ITOFIX(7)
#define FP_8        ITOFIX(8)
#define FP_9        ITOFIX(9)
#define FP_10       ITOFIX(10)

#define FP_16       ITOFIX(16)
#define FP_32       ITOFIX(32)
#define FP_64       ITOFIX(64)
#define FP_128      ITOFIX(128)
#define FP_256      ITOFIX(256)

#define FP_0_1      FTOFIX(0.1f)
#define FP_0_2      FTOFIX(0.2f)
#define FP_0_3      FTOFIX(0.3f)
#define FP_0_4      FTOFIX(0.4f)
#define FP_0_5      FTOFIX(0.5f)
#define FP_0_6      FTOFIX(0.6f)
#define FP_0_7      FTOFIX(0.7f)
#define FP_0_8      FTOFIX(0.8f)
#define FP_0_9      FTOFIX(0.9f)

#define FP_0_25     FTOFIX(0.25f)
#define FP_0_75     FTOFIX(0.75f)

#define FP_1_OVER_3 FTOFIX(1.0f / 3.0f)
#define FP_2_OVER_3 FTOFIX(2.0f / 3.0f)

#define FP_PI       FTOFIX(3.1415927f)

// ---------------------------------------------------------------------------

static fixed fix_sin(fixed x) {
    return FTOFIX(sin(FIXTOF(x)));
}

static fixed fix_cos(fixed x) {
    return FTOFIX(cos(FIXTOF(x)));
}

static fixed fix_tan(fixed x) {
    return FTOFIX(tan(FIXTOF(x)));
}

#endif

