#ifndef DGL_TEST_HELPERS_H_INCLUDED
#define DGL_TEST_HELPERS_H_INCLUDED

#include <stdio.h>
#include <assert.h>
#include "dgl.h"

#define F_EQU(a, b) (close_enough((a), (b), TOLERANCE))
#define FFIX_EQU(a, b) (close_enough((a), (b), 0.05f))  // wow !

extern void _breakpoint();
#pragma aux _breakpoint = "int 0x03";

#define ASSERT(x) \
    do { \
        if (!(x)) { \
            printf("Assertion failed at %s:%d, %s\n", __FILE__, __LINE__, #x); \
            _breakpoint(); \
        } \
    } while (0)

#endif

void clrscr(unsigned char attr);
void gotoxy(int x, int y);

