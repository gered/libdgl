#ifndef LIBDGL_DGLCMN_H
#define LIBDGL_DGLCMN_H

#ifdef __cplusplus
extern "C" {
#endif

typedef char                 int8;
typedef short                int16;
typedef int                  int32;

typedef unsigned char        uint8;
typedef unsigned short       uint16;
typedef unsigned int         uint32;

#ifndef bool
typedef int                  bool;
#define true                 1
#define false                0
#endif

#define BIT_0                0x1
#define BIT_1                0x2
#define BIT_2                0x4
#define BIT_3                0x8
#define BIT_4                0x10
#define BIT_5                0x20
#define BIT_6                0x40
#define BIT_7                0x80

#define BIT_ISSET(bit, x)    ((x) & (bit))
#define BIT_SET(bit, x)      ((x) |= (bit))
#define BIT_CLEAR(bit, x)    ((x) &= ~(bit))
#define BIT_TOGGLE(bit, x)   ((x) ^= (bit))

#ifndef MAX
#define MAX(a, b)            (((a) > (b)) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a, b)            (((a) < (b)) ? (a) : (b))
#endif

#ifdef __cplusplus
}
#endif

#endif

