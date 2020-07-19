#include "helpers.h"
#include <conio.h>
#include <dos.h>
#include <stdio.h>

void clrscr(unsigned char attr) {
    union REGS r;
    r.h.ah = 6;
    r.h.al = 0;      // 0 = clear
    r.h.ch = 0;      // y1
    r.h.cl = 0;      // x1
    r.h.dh = 24;     // y2
    r.h.dl = 79;     // x2
    r.h.bh = attr;   // attr (char + color) to fill with
    int386(0x10, &r, &r);
    gotoxy(0, 0);
}

void gotoxy(int x, int y) {
    union REGS r;
    r.h.ah = 2;
    r.h.bh = 0;
    r.h.dh = y;
    r.h.dl = x;
    int386(0x10, &r, &r);
}

