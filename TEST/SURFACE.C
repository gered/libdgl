#include "surface.h"
#include "dgl.h"
#include <stdio.h>

extern SURFACE *backbuffer;

// fill screen entirely with blue
void test_surface_clear(void) {
    surface_clear(screen, 1);

    getch();
}

// fill screen entirely with green, using the backbuffer first
void test_surface_copy(void) {
    surface_clear(backbuffer, 2);
    surface_copy(backbuffer, screen);

    getch();
}

