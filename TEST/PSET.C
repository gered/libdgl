#include "pset.h"
#include "dgldraw.h"
#include <stdio.h>
#include "helpers.h"

// pixel drawing done largely using pointers directly, with the assistance
// of helper functions to get correct initial pointer positions.
// starting at (10,10), draw 256 colour palette in a 16x16 pixel box.
// draw white "guide" lines from (0,0) to (10,10)
void test_pixels_1(void) {
    uint8 *p;
    int i, x, y;

    surface_clear(screen, 0);

    p = surface_pointer(screen, 10, 10);
    ASSERT(p == (screen->pixels + (10 * screen->width) + 10));

    i = 0;
    for (y = 0; y < 16; ++y) {
        for (x = 0; x < 16; ++x) {
            *p = (uint8)i;
            ++p;
            ++i;
        }
        p += (screen->width - 16);
    }

    p = surface_pointer(screen, 0, 0);
    for (i = 0; i < 10; ++i) {
        *p = 15;
        p += (screen->width + 1);
    }
    p = surface_pointer(screen, 10, 0);
    for (i = 0; i < 10; ++i) {
        *p = 15;
        p += screen->width;
    }
    p = surface_pointer(screen, 0, 10);
    for (i = 0; i < 10; ++i) {
        *p = 15;
        ++p;
    }

    getch();
}

// pixel drawing using pset, and pixel getting using point
// draws two blue, green, cyan, red pixels each in each screen corner.
// the second pixel colour is drawn by sampling the first.
void test_pixels_2(void) {
    uint8 c1, c2, c3, c4;

    surface_clear(screen, 0);

    pset(screen, 0, 0, 1);
    pset(screen, 319, 0, 2);
    pset(screen, 0, 199, 3);
    pset(screen, 319, 199, 4);

    pset_f(screen, 10, 0, 1);
    pset_f(screen, 309, 0, 2);
    pset_f(screen, 10, 199, 3);
    pset_f(screen, 309, 199, 4);

    c1 = pget(screen, 0, 0);
    c2 = pget(screen, 319, 0);
    c3 = pget(screen, 0, 199);
    c4 = pget(screen, 319, 199);
    ASSERT(c1 == 1 && c2 == 2 && c3 == 3 && c4 == 4);

    pset(screen, 1, 1, c1);
    pset(screen, 318, 1, c2);
    pset(screen, 1, 198, c3);
    pset(screen, 318, 198, c4);

    c1 = pget_f(screen, 10, 0);
    c2 = pget_f(screen, 309, 0);
    c3 = pget_f(screen, 10, 199);
    c4 = pget_f(screen, 309, 199);
    ASSERT(c1 == 1 && c2 == 2 && c3 == 3 && c4 == 4);

    pset_f(screen, 11, 1, c1);
    pset_f(screen, 308, 1, c2);
    pset_f(screen, 11, 198, c3);
    pset_f(screen, 308, 198, c4);

    getch();
}

// pixel drawing with clipping (using pset).
// draws 4 white lines 10 pixels long, going 5 pixels out of bounds off each
// edge (top, left, bottom, right) of the screen. red marker pixels are at
// the inside edge of each line to mark a known reference point
void test_pixels_clipping(void) {
    int i, x, y;

    surface_clear(screen, 0);

    pset(screen, 5, 100, 4);
    pset(screen, 314, 100, 4);
    pset(screen, 160, 5, 4);
    pset(screen, 160, 194, 4);

    for (i = 0; i < 10; ++i) {
        pset(screen, i - 5, 100, 15);
        pset(screen, 315 + i, 100, 15);
        pset(screen, 160, i - 5, 15);
        pset(screen, 160, 195 + i, 15);
    }

    getch();
}

