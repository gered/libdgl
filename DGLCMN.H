#ifndef DGL_DGLCMN_H_INCLUDED
#define DGL_DGLCMN_H_INCLUDED

typedef int             boolean;

typedef char            byte;
typedef	short           word;
typedef int             dword;

typedef unsigned char   ubyte;
typedef unsigned short  uword;
typedef unsigned int    udword;

typedef unsigned char   uchar;
typedef unsigned short  ushort;
typedef unsigned int    uint;

#define TRUE  1
#define FALSE 0

#define BIT_0               0x1
#define BIT_1               0x2
#define BIT_2               0x4
#define BIT_3               0x8
#define BIT_4               0x10
#define BIT_5               0x20
#define BIT_6               0x40
#define BIT_7               0x80

#define BIT_ISSET(bit, x)   ((x) & (bit))
#define BIT_SET(bit, x)     ((x) |= (bit))
#define BIT_CLEAR(bit, x)   ((x) &= ~(bit))
#define BIT_TOGGLE(bit, x)  ((x) ^= (bit))

#endif

