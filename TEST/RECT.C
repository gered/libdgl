#include "rect.h"
#include "dgl.h"
#include <stdio.h>

// draws two rectangles. red pixels mark the extents of the rects. a
// duplicate of the first rectangle is drawn using the "fast" variant.
void test_rect(void) {
    int x1, y1, x2, y2;

    surface_clear(screen, 0);

    x1 = 10; y1 = 10; x2 = 90; y2 = 90;
    surface_pset(screen, x1 - 1, y1, 4);
    surface_pset(screen, x1, y1 - 1, 4);
    surface_pset(screen, x2 + 1, y1, 4);
    surface_pset(screen, x2, y1 - 1, 4);
    surface_pset(screen, x1 - 1, y2, 4);
    surface_pset(screen, x1, y2 + 1, 4);
    surface_pset(screen, x2 + 1, y2, 4);
    surface_pset(screen, x2, y2 + 1, 4);
    surface_rect(screen, x1, y1, x2, y2, 1);

    x1 = 10; y1 = 110; x2 = 90; y2 = 190;
    surface_pset(screen, x1 - 1, y1, 4);
    surface_pset(screen, x1, y1 - 1, 4);
    surface_pset(screen, x2 + 1, y1, 4);
    surface_pset(screen, x2, y1 - 1, 4);
    surface_pset(screen, x1 - 1, y2, 4);
    surface_pset(screen, x1, y2 + 1, 4);
    surface_pset(screen, x2 + 1, y2, 4);
    surface_pset(screen, x2, y2 + 1, 4);
    surface_rect_f(screen, x1, y1, x2, y2, 1);

    x1 = 190; y1 = 90; x2 = 110; y2 = 10;
    surface_pset(screen, x2 - 1, y2, 4);
    surface_pset(screen, x2, y2 - 1, 4);
    surface_pset(screen, x1 + 1, y2, 4);
    surface_pset(screen, x1, y2 - 1, 4);
    surface_pset(screen, x2 - 1, y1, 4);
    surface_pset(screen, x2, y1 + 1, 4);
    surface_pset(screen, x1 + 1, y1, 4);
    surface_pset(screen, x1, y1 + 1, 4);
    surface_rect(screen, x1, y1, x2, y2, 2);

    getch();
}

// draws 4 rects in each of the top-left and bottom-right corners (two in each
// corner will be completely out of bounds, so should not be visible). also,
// two long rects are drawn along the center of the screen and they each
// extend beyond the edges of the screen. red pixels are drawn to mark rect
// extents.
void test_rect_clipping(void) {
    int x1, y1, x2, y2;

    surface_clear(screen, 0);

    x1 = -8; y1 = 10; x2 = 7; y2 = 25;
    surface_pset(screen, x2, y1 - 1, 4);
    surface_pset(screen, x2 + 1, y1, 4);
    surface_pset(screen, x2 + 1, y2, 4);
    surface_pset(screen, x2, y2 + 1, 4);
    surface_rect(screen, x1, y1, x2, y2, 1);

    surface_rect(screen, -16, 30, -1, 46, 10);

    x1 = 20; y1 = -8; x2 = 35; y2 = 7;
    surface_pset(screen, x1 - 1, y2, 4);
    surface_pset(screen, x1, y2 + 1, 4);
    surface_pset(screen, x2 + 1, y2, 4);
    surface_pset(screen, x2, y2 + 1, 4);
    surface_rect(screen, x1, y1, x2, y2, 2);

    surface_rect(screen, 40, -16, 55, -1, 11);

    x1 = 313; y1 = 170; x2 = 328; y2 = 185;
    surface_pset(screen, x1, y1 - 1, 4);
    surface_pset(screen, x1 - 1, y1, 4);
    surface_pset(screen, x1 - 1, y2, 4);
    surface_pset(screen, x1, y2 + 1, 4);
    surface_rect(screen, x1, y1, x2, y2, 3);

    surface_rect(screen, 320, 150, 335, 165, 12);

    x1 = 285; y1 = 193; x2 = 300; y2 = 208;
    surface_pset(screen, x1 - 1, y1, 4);
    surface_pset(screen, x1, y1 - 1, 4);
    surface_pset(screen, x2 + 1, y1, 4);
    surface_pset(screen, x2, y1 - 1, 4);
    surface_rect(screen, x1, y1, x2, y2, 5);

    surface_rect(screen, 265, 200, 280, 215, 13);

    x1 = 150; y1 = -10; x2 = 170; y2 = 210;
    surface_pset(screen, x1 - 1, 10, 4);
    surface_pset(screen, x2 + 1, 10, 4);
    surface_rect(screen, x1, y1, x2, y2, 7);

    x1 = -10; y1 = 90; x2 = 330; y2 = 110;
    surface_pset(screen, 10, y1 - 1, 4);
    surface_pset(screen, 10, y2 + 1, 4);
    surface_rect(screen, x1, y1, x2, y2, 8);

    getch();
}

// draws two filled rectangles. red pixels mark the extents of the rects. a
// duplicate of the first rectangle is drawn using the "fast" variant.
void test_filled_rect(void) {
    int x1, y1, x2, y2;

    surface_clear(screen, 0);

    x1 = 10; y1 = 10; x2 = 90; y2 = 90;
    surface_pset(screen, x1 - 1, y1, 4);
    surface_pset(screen, x1, y1 - 1, 4);
    surface_pset(screen, x2 + 1, y1, 4);
    surface_pset(screen, x2, y1 - 1, 4);
    surface_pset(screen, x1 - 1, y2, 4);
    surface_pset(screen, x1, y2 + 1, 4);
    surface_pset(screen, x2 + 1, y2, 4);
    surface_pset(screen, x2, y2 + 1, 4);
    surface_filled_rect(screen, x1, y1, x2, y2, 1);

    x1 = 10; y1 = 110; x2 = 90; y2 = 190;
    surface_pset(screen, x1 - 1, y1, 4);
    surface_pset(screen, x1, y1 - 1, 4);
    surface_pset(screen, x2 + 1, y1, 4);
    surface_pset(screen, x2, y1 - 1, 4);
    surface_pset(screen, x1 - 1, y2, 4);
    surface_pset(screen, x1, y2 + 1, 4);
    surface_pset(screen, x2 + 1, y2, 4);
    surface_pset(screen, x2, y2 + 1, 4);
    surface_filled_rect_f(screen, x1, y1, x2, y2, 1);

    x1 = 190; y1 = 90; x2 = 110; y2 = 10;
    surface_pset(screen, x2 - 1, y2, 4);
    surface_pset(screen, x2, y2 - 1, 4);
    surface_pset(screen, x1 + 1, y2, 4);
    surface_pset(screen, x1, y2 - 1, 4);
    surface_pset(screen, x2 - 1, y1, 4);
    surface_pset(screen, x2, y1 + 1, 4);
    surface_pset(screen, x1 + 1, y1, 4);
    surface_pset(screen, x1, y1 + 1, 4);
    surface_filled_rect(screen, x1, y1, x2, y2, 2);

    getch();
}

// draws 4 rects in each of the top-left and bottom-right corners (two in each
// corner will be completely out of bounds, so should not be visible). also,
// two long rects are drawn along the center of the screen and they each
// extend beyond the edges of the screen. red pixels are drawn to mark rect
// extents.
void test_filled_rect_clipping(void) {
    int x1, y1, x2, y2;

    surface_clear(screen, 0);

    x1 = -8; y1 = 10; x2 = 7; y2 = 25;
    surface_pset(screen, x2, y1 - 1, 4);
    surface_pset(screen, x2 + 1, y1, 4);
    surface_pset(screen, x2 + 1, y2, 4);
    surface_pset(screen, x2, y2 + 1, 4);
    surface_filled_rect(screen, x1, y1, x2, y2, 1);

    surface_filled_rect(screen, -16, 30, -1, 46, 10);

    x1 = 20; y1 = -8; x2 = 35; y2 = 7;
    surface_pset(screen, x1 - 1, y2, 4);
    surface_pset(screen, x1, y2 + 1, 4);
    surface_pset(screen, x2 + 1, y2, 4);
    surface_pset(screen, x2, y2 + 1, 4);
    surface_filled_rect(screen, x1, y1, x2, y2, 2);

    surface_filled_rect(screen, 40, -16, 55, -1, 11);

    x1 = 313; y1 = 170; x2 = 328; y2 = 185;
    surface_pset(screen, x1, y1 - 1, 4);
    surface_pset(screen, x1 - 1, y1, 4);
    surface_pset(screen, x1 - 1, y2, 4);
    surface_pset(screen, x1, y2 + 1, 4);
    surface_filled_rect(screen, x1, y1, x2, y2, 3);

    surface_filled_rect(screen, 320, 150, 335, 165, 12);

    x1 = 285; y1 = 193; x2 = 300; y2 = 208;
    surface_pset(screen, x1 - 1, y1, 4);
    surface_pset(screen, x1, y1 - 1, 4);
    surface_pset(screen, x2 + 1, y1, 4);
    surface_pset(screen, x2, y1 - 1, 4);
    surface_filled_rect(screen, x1, y1, x2, y2, 5);

    surface_filled_rect(screen, 265, 200, 280, 215, 13);

    x1 = 150; y1 = -10; x2 = 170; y2 = 210;
    surface_pset(screen, x1 - 1, 10, 4);
    surface_pset(screen, x2 + 1, 10, 4);
    surface_filled_rect(screen, x1, y1, x2, y2, 7);

    x1 = -10; y1 = 90; x2 = 330; y2 = 110;
    surface_pset(screen, 10, y1 - 1, 4);
    surface_pset(screen, 10, y2 + 1, 4);
    surface_filled_rect(screen, x1, y1, x2, y2, 8);

    getch();
}

