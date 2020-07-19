#include "line.h"
#include "dgldraw.h"
#include "dglmath.h"
#include <stdio.h>

// draws two horizontal lines, with red pixels marking the extents of the
// lines. a third line is drawn using the "fast" variant.
void test_hline(void) {
    int x1, x2, y;

    surface_clear(screen, 0);

    x1 = 10; x2 = 100; y = 20;

    pset(screen, x1 - 1, y, 4);
    pset(screen, x2 + 1, y, 4);
    draw_hline(screen, x1, x2, y, 1);

    y = 30;

    pset(screen, x1 - 1, y, 4);
    pset(screen, x2 + 1, y, 4);
    draw_hline(screen, x2, x1, y, 2);

    x1 = 200; x2 = 300; y = 20;

    pset(screen, x1 - 1, y, 4);
    pset(screen, x2 + 1, y, 4);
    draw_hline_f(screen, x1, x2, y, 1);

    getch();
}

// draws 4 horizontal lines. two are completely outside the bounds of
// the screen and should not be visible at all. the remaining two will be
// partly out of bounds off the left and right edges. red pixel markers are
// drawn marking the visible ends of the two partly clipped lines
void test_hline_clipping(void) {
    int x1, x2, y;

    surface_clear(screen, 0);

    x1 = -50; x2 = 50; y = 6;
    pset(screen, x2 + 1, y, 4);
    draw_hline(screen, x1, x2, y, 1);

    x1 = 300; x2 = 340; y = 130;
    pset(screen, x1 - 1, y, 4);
    draw_hline(screen, x1, x2, y, 2);

    draw_hline(screen, 100, 200, -10, 3);
    draw_hline(screen, 20, 80, 250, 5);

    getch();
}

// draws two vertical lines, with red pixels marking the extents of
// the lines. a third line is drawn using the "fast" variant.
void test_vline(void) {
    int x, y1, y2;

    surface_clear(screen, 0);

    x = 50; y1 = 10; y2 = 100;

    pset(screen, x, y1 - 1, 4);
    pset(screen, x, y2 + 1, 4);
    draw_vline(screen, x, y1, y2, 1);

    x = 60;

    pset(screen, x, y1 - 1, 4);
    pset(screen, x, y2 + 1, 4);
    draw_vline(screen, x, y2, y1, 2);

    x = 150; y1 = 10; y2 = 100;

    pset(screen, x, y1 - 1, 4);
    pset(screen, x, y2 + 1, 4);
    draw_vline_f(screen, x, y1, y2, 1);

    getch();
}

// draws 4 vertical lines. two are completely outside the bounds of
// the screen and should not be visible at all. the remaining two will be
// partly out of bounds off the top and bottom edges. red pixel markers are
// drawn marking the visible ends of the two partly clipped lines
void test_vline_clipping(void) {
    int x, y1, y2;

    surface_clear(screen, 0);

    x = 20; y1 = -32; y2 = 32;
    pset(screen, x, y2 + 1, 4);
    draw_vline(screen, x, y1, y2, 1);

    x = 270; y1 = 245; y2 = 165;
    pset(screen, x, y2 - 1, 4);
    draw_vline(screen, x, y1, y2, 2);

    draw_vline(screen, -17, 10, 20, 3);
    draw_vline(screen, 400, 100, 300, 5);

    getch();
}

// draws a series of lines, both straight and diagonal as well as
// specifying the two line endpoints in either order (e.g. left-right
// and right-left). draws red marker pixels around the end points of each
// line. duplicates of some lines are drawn using the "fast" variant.
void test_line(void) {
    int x1, x2, y1, y2;

    surface_clear(screen, 0);

    x1 = 10; y1 = 10; x2 = 20; y2 = 20;
    pset(screen, x1 - 1, y1, 4);
    pset(screen, x1, y1 - 1, 4);
    pset(screen, x2 + 1, y2, 4);
    pset(screen, x2, y2 + 1, 4);
    draw_line(screen, x1, y1, x2, y2, 1);

    x1 = 10; y1 = 100; x2 = 20; y2 = 150;
    pset(screen, x1 - 1, y1, 4);
    pset(screen, x1, y1 - 1, 4);
    pset(screen, x2 + 1, y2, 4);
    pset(screen, x2, y2 + 1, 4);
    draw_line(screen, x1, y1, x2, y2, 2);

    x1 = 60; y1 = 150; x2 = 50; y2 = 100;
    pset(screen, x1 + 1, y1, 4);
    pset(screen, x1, y1 + 1, 4);
    pset(screen, x2 - 1, y2, 4);
    pset(screen, x2, y2 - 1, 4);
    draw_line(screen, x1, y1, x2, y2, 3);

    x1 = 50; y1 = 10; x2 = 100; y2 = 10;
    pset(screen, x1 - 1, y1, 4);
    pset(screen, x2 + 1, y1, 4);
    draw_line(screen, x1, y1, x2, y2, 5);

    x1 = 100; y1 = 50; x2 = 20; y2 = 50;
    pset(screen, x1 + 1, y1, 4);
    pset(screen, x2 - 1, y1, 4);
    draw_line(screen, x1, y1, x2, y2, 6);

    x1 = 290; y1 = 10; x2 = 290; y2 = 100;
    pset(screen, x1, y1 - 1, 4);
    pset(screen, x2, y2 + 1, 4);
    draw_line(screen, x1, y1, x2, y2, 7);

    x1 = 310; y1 = 100; x2 = 310; y2 = 10;
    pset(screen, x1, y1 + 1, 4);
    pset(screen, x2, y2 - 1, 4);
    draw_line(screen, x1, y1, x2, y2, 8);


    x1 = 30; y1 = 10; x2 = 40; y2 = 20;
    pset(screen, x1 - 1, y1, 4);
    pset(screen, x1, y1 - 1, 4);
    pset(screen, x2 + 1, y2, 4);
    pset(screen, x2, y2 + 1, 4);
    draw_line_f(screen, x1, y1, x2, y2, 1);

    x1 = 30; y1 = 100; x2 = 40; y2 = 150;
    pset(screen, x1 - 1, y1, 4);
    pset(screen, x1, y1 - 1, 4);
    pset(screen, x2 + 1, y2, 4);
    pset(screen, x2, y2 + 1, 4);
    draw_line_f(screen, x1, y1, x2, y2, 2);

    x1 = 50; y1 = 20; x2 = 100; y2 = 20;
    pset(screen, x1 - 1, y1, 4);
    pset(screen, x2 + 1, y1, 4);
    draw_line_f(screen, x1, y1, x2, y2, 5);

    x1 = 300; y1 = 10; x2 = 300; y2 = 100;
    pset(screen, x1, y1 - 1, 4);
    pset(screen, x2, y2 + 1, 4);
    draw_line_f(screen, x1, y1, x2, y2, 7);

    getch();
}

// draws a series of lines fanned out in a circle originating from the
// center of the screen. each line extends well past the boundaries of the
// screen and so should be partially clipped. additionally, 4 straight lines
// are drawn completely outside the bounds of the screen and should not be
// visible.
void test_line_clipping(void) {
    int angle, color;
    float x, y;

    surface_clear(screen, 0);

    draw_line(screen, 10, -30, 100, -30, 1);
    draw_line(screen, 70, 250, 170, 250, 2);
    draw_line(screen, -100, 120, -100, 199, 3);
    draw_line(screen, 320, 99, 320, 199, 5);

    for (angle = 0, color = 32; angle <= 360; angle += 10, ++color) {
        point_on_circle(400, DEG_TO_RAD(angle), &x, &y);
        draw_line(screen, 160, 100, (int)x + 160, (int)y + 100, color);
    }

    getch();
}

