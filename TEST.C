#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <assert.h>
#include "dgl.h"

SURFACE *backbuffer = NULL;

#define F_EQU(a, b) (close_enough((a), (b), TOLERANCE))
#define FFIX_EQU(a, b) (close_enough((a), (b), 0.01f))

extern void _breakpoint();
#pragma aux _breakpoint = "int 0x03";


#define ASSERT(x) \
    do { \
        if (!(x)) { \
            printf("Assertion failed at %s:%d, %s\n", __FILE__, __LINE__, #x); \
            _breakpoint(); \
        } \
    } while (0)

void break_handler(int sig) {
    exit(0);
}

SURFACE* get_sprite(int width, int height) {
    int x_third, y_third;
    SURFACE *sprite = surface_create(width, height);
    ASSERT(sprite != NULL);
    ASSERT(sprite->width == width);
    ASSERT(sprite->height == height);

    x_third = width / 3;
    y_third = height / 3;

    surface_filled_rect(sprite, 0, 0, x_third, y_third, 1);
    surface_filled_rect(sprite, x_third * 2, y_third * 2, width - 1, height - 1, 2);
    surface_filled_rect(sprite, 0, (y_third * 2), x_third, height - 1, 3);
    surface_filled_rect(sprite, x_third * 2, 0, width - 1, y_third, 4);
    surface_filled_rect(sprite, x_third, y_third, x_third * 2, y_third * 2, 5);
    surface_rect(sprite, 0, 0, width - 1, height - 1, 6);

    return sprite;
}

SURFACE* get_image(int width, int height) {
    int x, y;
    SURFACE *image = surface_create(width, height);
    ASSERT(image != NULL);
    ASSERT(image->width == width);
    ASSERT(image->height == height);

    surface_filled_rect(image, 0, 0, width / 2, height / 2, 1);
    surface_filled_rect(image, width / 2, 0, width - 1, height / 2, 2);
    surface_filled_rect(image, 0, height / 2, width / 2, height - 1, 3);
    surface_filled_rect(image, width / 2, height / 2, width - 1, height - 1, 4);
    surface_rect(image, 0, 0, width - 1, height - 1, 5);

    return image;
}

// ---------------------------------------------------------------------------

void test_fixed(void) {
    fixed a, b, c;
    float af, bf, cf, f;
    int i;

    a = FTOFIX(PI);
    f = FIXTOF(a);
    ASSERT(FFIX_EQU(PI, f));

    a = FTOFIX(-0.37f);
    f = FIXTOF(a);
    ASSERT(FFIX_EQU(-0.38f, f));

    a = ITOFIX(17);
    i = FIXTOI(a);
    ASSERT(17 == i);

    a = ITOFIX(-17);
    i = FIXTOI(a);
    ASSERT(-17 == i);

    a = FTOFIX(1.5f);
    b = FTOFIX(2.3f);
    c = a + b;
    f = FIXTOF(c);
    ASSERT(FFIX_EQU((1.5f + 2.3f), f));

    a = FTOFIX(0.2f);
    b = FTOFIX(1.7f);
    c = a - b;
    f = FIXTOF(c);
    ASSERT(FFIX_EQU((0.2f - 1.7f), f));

    a = FTOFIX(16.0f);
    c = a >> 2;
    f = FIXTOF(c);
    ASSERT(FFIX_EQU((16 >> 2), f));

    a = FTOFIX(7.1f);
    b = ITOFIX(2);
    c = fix_mul(a, b);
    f = FIXTOF(c);
    ASSERT(FFIX_EQU((7.1f * 2), f));

    a = FTOFIX(77.7f);
    b = ITOFIX(3);
    c = fix_div(a, b);
    f = FIXTOF(c);
    ASSERT(FFIX_EQU((77.7f / 3), f));

    for (i = 0; i <= 360; i += 45) {
        f = DEG_TO_RAD(i + 1);
        a = fix_sin(FTOFIX(f));
        af = sin(f);
        b = fix_cos(FTOFIX(f));
        bf = cos(f);
        c = fix_tan(FTOFIX(f));
        cf = tan(f);
        ASSERT(FFIX_EQU(FIXTOF(a), af));
        ASSERT(FFIX_EQU(FIXTOF(b), bf));
        ASSERT(FFIX_EQU(FIXTOF(c), cf));
    }
}

void test_vector2i(void) {
    VECTOR2I a, b, c;
    int i;

    c = vector2i(3, 7);
    ASSERT(c.x == 3 && c.y == 7);

    vector2i_set(&c, 1, 2);
    ASSERT(c.x == 1 && c.y == 2);

    a = vector2i(1, 2);
    b = vector2i(1, 2);
    ASSERT(vector2i_equals(a, b));

    a = vector2i(3, 4);
    b = vector2i(1, 2);

    c = vector2i_add(a, b);
    ASSERT(c.x == 4 && c.y == 6);

    c = vector2i_sub(a, b);
    ASSERT(c.x == 2 && c.y == 2);

    c = vector2i_mul(a, b);
    ASSERT(c.x == 3 && c.y == 8);

    c = vector2i_muls(a, 2);
    ASSERT(c.x == 6 && c.y == 8);

    c = vector2i_div(a, b);
    ASSERT(c.x == 3 && c.y == 2);

    c = vector2i_divs(a, 2);
    ASSERT(c.x == 1 && c.y == 2);

    a = vector2i(1, 1);
    b = vector2i(1, 3);
    i = vector2i_distance(a, b);
    ASSERT(i == 2);

    i = vector2i_distancesq(a, b);
    ASSERT(i == 4);

    a = vector2i(-12, 16);
    b = vector2i(12, 9);
    i = vector2i_dot(a, b);
    ASSERT(i == 0);

    a = vector2i(-3, 0);
    i = vector2i_length(a);
    ASSERT(i == 3);

    i = vector2i_lengthsq(a);
    ASSERT(i == 9);

    a = vector2i(5, 0);
    b = vector2i(10, 0);
    c = vector2i_lerp(a, b, 0.5f);
    ASSERT(c.x == 7 && c.y == 0);
}

void test_vector2f(void) {
    VECTOR2F a, b, c;
    float f;

    c = vector2f(3.0f, 7.0f);
    ASSERT(F_EQU(c.x, 3.0f) && F_EQU(c.y, 7.0f));

    vector2f_set(&c, 1.0f, 2.0f);
    ASSERT(F_EQU(c.x, 1.0f) && F_EQU(c.y, 2.0f));

    a = vector2f(1.0f, 2.0f);
    b = vector2f(1.0f, 2.0f);
    ASSERT(vector2f_equals(a, b));

    a = vector2f(3.0f, 4.0f);
    b = vector2f(1.0f, 2.0f);

    c = vector2f_add(a, b);
    ASSERT(F_EQU(c.x, 4.0f) && F_EQU(c.y, 6.0f));

    c = vector2f_sub(a, b);
    ASSERT(F_EQU(c.x, 2.0f) && F_EQU(c.y, 2.0f));

    c = vector2f_mul(a, b);
    ASSERT(F_EQU(c.x, 3.0f) && F_EQU(c.y, 8.0f));

    c = vector2f_muls(a, 0.5f);
    ASSERT(F_EQU(c.x, 1.5f) && F_EQU(c.y, 2.0f));

    c = vector2f_div(a, b);
    ASSERT(F_EQU(c.x, 3.0f) && F_EQU(c.y, 2.0f));

    c = vector2f_divs(a, 0.5f);
    ASSERT(F_EQU(c.x, 6.0f) && F_EQU(c.y, 8.0f));

    a = vector2f(1.0f, 1.0f);
    b = vector2f(1.0f, 3.0f);
    f = vector2f_distance(a, b);
    ASSERT(F_EQU(f, 2.0f));

    f = vector2f_distancesq(a, b);
    ASSERT(F_EQU(f, 4.0f));

    a = vector2f(-12.0f, 16.0f);
    b = vector2f(12.0f, 9.0f);
    f = vector2f_dot(a, b);
    ASSERT(F_EQU(f, 0.0f));

    a = vector2f(-3.0f, 0.0f);
    f = vector2f_length(a);
    ASSERT(F_EQU(f, 3.0f));

    f = vector2f_lengthsq(a);
    ASSERT(F_EQU(f, 9.0f));

    a = vector2f(7.0f, 7.0f);
    c = vector2f_normalize(a);
    ASSERT(F_EQU(c.x, 0.70710f) && F_EQU(c.y, 0.70710f));

    a = vector2f(10.0f, 0.0f);
    c = vector2f_set_length(a, 2.0f);
    ASSERT(F_EQU(c.x, 2.0f) && F_EQU(c.y, 0.0f));

    a = vector2f(5.0f, 0.0f);
    b = vector2f(10.0f, 0.0f);
    c = vector2f_lerp(a, b, 0.5f);
    ASSERT(F_EQU(c.x, 7.5f) && F_EQU(c.y, 0.0f));
}

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

// pixel drawing done largely using pointers directly, with the assistance
// of helper functions to get correct initial pointer positions.
// starting at (10,10), draw 256 colour palette in a 16x16 pixel box.
// draw white "guide" lines from (0,0) to (10,10)
void test_pixels_1(void) {
    byte *p;
    int i, x, y;

    surface_clear(screen, 0);

    p = surface_pointer(screen, 10, 10);
    ASSERT(p == (screen->pixels + (10 * screen->width) + 10));

    i = 0;
    for (y = 0; y < 16; ++y) {
        for (x = 0; x < 16; ++x) {
            *p = (byte)i;
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
    byte c1, c2, c3, c4;

    surface_clear(screen, 0);

    surface_pset(screen, 0, 0, 1);
    surface_pset(screen, 319, 0, 2);
    surface_pset(screen, 0, 199, 3);
    surface_pset(screen, 319, 199, 4);

    surface_pset_f(screen, 10, 0, 1);
    surface_pset_f(screen, 309, 0, 2);
    surface_pset_f(screen, 10, 199, 3);
    surface_pset_f(screen, 309, 199, 4);

    c1 = surface_point(screen, 0, 0);
    c2 = surface_point(screen, 319, 0);
    c3 = surface_point(screen, 0, 199);
    c4 = surface_point(screen, 319, 199);
    ASSERT(c1 == 1 && c2 == 2 && c3 == 3 && c4 == 4);

    surface_pset(screen, 1, 1, c1);
    surface_pset(screen, 318, 1, c2);
    surface_pset(screen, 1, 198, c3);
    surface_pset(screen, 318, 198, c4);

    c1 = surface_point_f(screen, 10, 0);
    c2 = surface_point_f(screen, 309, 0);
    c3 = surface_point_f(screen, 10, 199);
    c4 = surface_point_f(screen, 309, 199);
    ASSERT(c1 == 1 && c2 == 2 && c3 == 3 && c4 == 4);

    surface_pset_f(screen, 11, 1, c1);
    surface_pset_f(screen, 308, 1, c2);
    surface_pset_f(screen, 11, 198, c3);
    surface_pset_f(screen, 308, 198, c4);

    getch();
}

// pixel drawing with clipping (using pset).
// draws 4 white lines 10 pixels long, going 5 pixels out of bounds off each
// edge (top, left, bottom, right) of the screen. red marker pixels are at
// the inside edge of each line to mark a known reference point
void test_pixels_clipping(void) {
    int i, x, y;

    surface_clear(screen, 0);

    surface_pset(screen, 5, 100, 4);
    surface_pset(screen, 314, 100, 4);
    surface_pset(screen, 160, 5, 4);
    surface_pset(screen, 160, 194, 4);

    for (i = 0; i < 10; ++i) {
        surface_pset(screen, i - 5, 100, 15);
        surface_pset(screen, 315 + i, 100, 15);
        surface_pset(screen, 160, i - 5, 15);
        surface_pset(screen, 160, 195 + i, 15);
    }

    getch();
}

// draws two horizontal lines, with red pixels marking the extents of the
// lines. a third line is drawn using the "fast" variant.
void test_hline(void) {
    int x1, x2, y;

    surface_clear(screen, 0);

    x1 = 10; x2 = 100; y = 20;

    surface_pset(screen, x1 - 1, y, 4);
    surface_pset(screen, x2 + 1, y, 4);
    surface_hline(screen, x1, x2, y, 1);

    y = 30;

    surface_pset(screen, x1 - 1, y, 4);
    surface_pset(screen, x2 + 1, y, 4);
    surface_hline(screen, x2, x1, y, 2);

    x1 = 200; x2 = 300; y = 20;

    surface_pset(screen, x1 - 1, y, 4);
    surface_pset(screen, x2 + 1, y, 4);
    surface_hline_f(screen, x1, x2, y, 1);

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
    surface_pset(screen, x2 + 1, y, 4);
    surface_hline(screen, x1, x2, y, 1);

    x1 = 300; x2 = 340; y = 130;
    surface_pset(screen, x1 - 1, y, 4);
    surface_hline(screen, x1, x2, y, 2);

    surface_hline(screen, 100, 200, -10, 3);
    surface_hline(screen, 20, 80, 250, 5);

    getch();
}

// draws two vertical lines, with red pixels marking the extents of
// the lines. a third line is drawn using the "fast" variant.
void test_vline(void) {
    int x, y1, y2;

    surface_clear(screen, 0);

    x = 50; y1 = 10; y2 = 100;

    surface_pset(screen, x, y1 - 1, 4);
    surface_pset(screen, x, y2 + 1, 4);
    surface_vline(screen, x, y1, y2, 1);

    x = 60;

    surface_pset(screen, x, y1 - 1, 4);
    surface_pset(screen, x, y2 + 1, 4);
    surface_vline(screen, x, y2, y1, 2);

    x = 150; y1 = 10; y2 = 100;

    surface_pset(screen, x, y1 - 1, 4);
    surface_pset(screen, x, y2 + 1, 4);
    surface_vline_f(screen, x, y1, y2, 1);

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
    surface_pset(screen, x, y2 + 1, 4);
    surface_vline(screen, x, y1, y2, 1);

    x = 270; y1 = 245; y2 = 165;
    surface_pset(screen, x, y2 - 1, 4);
    surface_vline(screen, x, y1, y2, 2);

    surface_vline(screen, -17, 10, 20, 3);
    surface_vline(screen, 400, 100, 300, 5);

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
    surface_pset(screen, x1 - 1, y1, 4);
    surface_pset(screen, x1, y1 - 1, 4);
    surface_pset(screen, x2 + 1, y2, 4);
    surface_pset(screen, x2, y2 + 1, 4);
    surface_line(screen, x1, y1, x2, y2, 1);

    x1 = 10; y1 = 100; x2 = 20; y2 = 150;
    surface_pset(screen, x1 - 1, y1, 4);
    surface_pset(screen, x1, y1 - 1, 4);
    surface_pset(screen, x2 + 1, y2, 4);
    surface_pset(screen, x2, y2 + 1, 4);
    surface_line(screen, x1, y1, x2, y2, 2);

    x1 = 60; y1 = 150; x2 = 50; y2 = 100;
    surface_pset(screen, x1 + 1, y1, 4);
    surface_pset(screen, x1, y1 + 1, 4);
    surface_pset(screen, x2 - 1, y2, 4);
    surface_pset(screen, x2, y2 - 1, 4);
    surface_line(screen, x1, y1, x2, y2, 3);

    x1 = 50; y1 = 10; x2 = 100; y2 = 10;
    surface_pset(screen, x1 - 1, y1, 4);
    surface_pset(screen, x2 + 1, y1, 4);
    surface_line(screen, x1, y1, x2, y2, 5);

    x1 = 100; y1 = 50; x2 = 20; y2 = 50;
    surface_pset(screen, x1 + 1, y1, 4);
    surface_pset(screen, x2 - 1, y1, 4);
    surface_line(screen, x1, y1, x2, y2, 6);

    x1 = 290; y1 = 10; x2 = 290; y2 = 100;
    surface_pset(screen, x1, y1 - 1, 4);
    surface_pset(screen, x2, y2 + 1, 4);
    surface_line(screen, x1, y1, x2, y2, 7);

    x1 = 310; y1 = 100; x2 = 310; y2 = 10;
    surface_pset(screen, x1, y1 + 1, 4);
    surface_pset(screen, x2, y2 - 1, 4);
    surface_line(screen, x1, y1, x2, y2, 8);


    x1 = 30; y1 = 10; x2 = 40; y2 = 20;
    surface_pset(screen, x1 - 1, y1, 4);
    surface_pset(screen, x1, y1 - 1, 4);
    surface_pset(screen, x2 + 1, y2, 4);
    surface_pset(screen, x2, y2 + 1, 4);
    surface_line_f(screen, x1, y1, x2, y2, 1);

    x1 = 30; y1 = 100; x2 = 40; y2 = 150;
    surface_pset(screen, x1 - 1, y1, 4);
    surface_pset(screen, x1, y1 - 1, 4);
    surface_pset(screen, x2 + 1, y2, 4);
    surface_pset(screen, x2, y2 + 1, 4);
    surface_line_f(screen, x1, y1, x2, y2, 2);

    x1 = 50; y1 = 20; x2 = 100; y2 = 20;
    surface_pset(screen, x1 - 1, y1, 4);
    surface_pset(screen, x2 + 1, y1, 4);
    surface_line_f(screen, x1, y1, x2, y2, 5);

    x1 = 300; y1 = 10; x2 = 300; y2 = 100;
    surface_pset(screen, x1, y1 - 1, 4);
    surface_pset(screen, x2, y2 + 1, 4);
    surface_line_f(screen, x1, y1, x2, y2, 7);

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

    surface_line(screen, 10, -30, 100, -30, 1);
    surface_line(screen, 70, 250, 170, 250, 2);
    surface_line(screen, -100, 120, -100, 199, 3);
    surface_line(screen, 320, 99, 320, 199, 5);

    for (angle = 0, color = 32; angle <= 360; angle += 10, ++color) {
        point_on_circle(400, DEG_TO_RAD(angle), &x, &y);
        surface_line(screen, 160, 100, (int)x + 160, (int)y + 100, color);
    }

    getch();
}

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

// blits a 5-color image in a few different sizes. first two test full
// image blits and partial image blits. remaining 4 blits test the shorthand
// blit function that simply blits the entire source image.
// a duplicate set of blits is done using the "fast" variants.
// the image sizes are chosen to ensure each different internal blitter is run.
void test_blit(void) {
    int x, y;

    SURFACE *bmp16 = get_image(16, 16);
    SURFACE *bmp12 = get_image(12, 12);
    SURFACE *bmp21 = get_image(21, 21);
    SURFACE *bmp3 = get_image(3, 3);

    surface_clear(screen, 0);

    x = 0; y = 0;

    surface_blit_region(bmp16, screen, 0, 0, 16, 16, x + 16, y + 16);
    surface_blit_region(bmp16, screen, 8, 8, 8, 8, x + 48, y + 16);

    surface_blit(bmp16, screen, x + 16, y + 48);
    surface_blit(bmp12, screen, x + 48, y + 48);
    surface_blit(bmp21, screen, x + 80, y + 48);
    surface_blit(bmp3, screen, x + 112, y + 48);

    x = 160; y = 0;

    surface_blit_region_f(bmp16, screen, 0, 0, 16, 16, x + 16, y + 16);
    surface_blit_region_f(bmp16, screen, 8, 8, 8, 8, x + 48, y + 16);

    surface_blit_f(bmp16, screen, x + 16, y + 48);
    surface_blit_f(bmp12, screen, x + 48, y + 48);
    surface_blit_f(bmp21, screen, x + 80, y + 48);
    surface_blit_f(bmp3, screen, x + 112, y + 48);

    surface_free(bmp16);
    surface_free(bmp12);
    surface_free(bmp21);
    surface_free(bmp3);
    getch();
}

// performs 7 blits along each edge of the screen, clipping the image by
// varying amounts. the 7th blit on each edge is completely out of bounds and
// should not be visible at all.
void test_blit_clipping(void) {
    SURFACE *bmp = get_image(16, 16);

    surface_clear(screen, 0);

    surface_blit(bmp, screen, -3, 16);
    surface_blit(bmp, screen, -4, 36);
    surface_blit(bmp, screen, -8, 56);
    surface_blit(bmp, screen, -12, 76);
    surface_blit(bmp, screen, -13, 96);
    surface_blit(bmp, screen, -14, 116);
    surface_blit(bmp, screen, -16, 136);

    surface_blit(bmp, screen, 16, -3);
    surface_blit(bmp, screen, 36, -4);
    surface_blit(bmp, screen, 56, -8);
    surface_blit(bmp, screen, 76, -12);
    surface_blit(bmp, screen, 96, -13);
    surface_blit(bmp, screen, 116, -14);
    surface_blit(bmp, screen, 136, -16);

    surface_blit(bmp, screen, 307, 16);
    surface_blit(bmp, screen, 308, 36);
    surface_blit(bmp, screen, 312, 56);
    surface_blit(bmp, screen, 316, 76);
    surface_blit(bmp, screen, 317, 96);
    surface_blit(bmp, screen, 318, 116);
    surface_blit(bmp, screen, 320, 136);

    surface_blit(bmp, screen, 16, 187);
    surface_blit(bmp, screen, 36, 188);
    surface_blit(bmp, screen, 56, 192);
    surface_blit(bmp, screen, 76, 196);
    surface_blit(bmp, screen, 96, 197);
    surface_blit(bmp, screen, 116, 198);
    surface_blit(bmp, screen, 136, 200);

    surface_free(bmp);
    getch();
}

// basically the exact same test as test_blit(), but uses sprite blitting
// instead and uses a grey background. no part of the screen buffer should be
// black as that colour is not used anywhere in the test sprite.
void test_sprite(void) {
    int x, y;
    SURFACE *bmp16 = get_sprite(16, 16);
    SURFACE *bmp12 = get_sprite(12, 12);
    SURFACE *bmp21 = get_sprite(21, 21);
    SURFACE *bmp3 = get_sprite(3, 3);

    surface_clear(screen, 8);

    x = 0; y = 0;

    surface_blit_sprite_region(bmp16, screen, 0, 0, 16, 16, x + 16, y + 16);
    surface_blit_sprite_region(bmp16, screen, 8, 8, 8, 8, x + 48, y + 16);

    surface_blit_sprite(bmp16, screen, x + 16, y + 48);
    surface_blit_sprite(bmp12, screen, x + 48, y + 48);
    surface_blit_sprite(bmp21, screen, x + 80, y + 48);
    surface_blit_sprite(bmp3, screen, x + 112, y + 48);

    x = 160; y = 0;

    surface_blit_sprite_region_f(bmp16, screen, 0, 0, 16, 16, x + 16, y + 16);
    surface_blit_sprite_region_f(bmp16, screen, 8, 8, 8, 8, x + 48, y + 16);

    surface_blit_sprite_f(bmp16, screen, x + 16, y + 48);
    surface_blit_sprite_f(bmp12, screen, x + 48, y + 48);
    surface_blit_sprite_f(bmp21, screen, x + 80, y + 48);
    surface_blit_sprite_f(bmp3, screen, x + 112, y + 48);

    surface_free(bmp16);
    surface_free(bmp12);
    surface_free(bmp21);
    surface_free(bmp3);
    getch();
}

// basically the exact same test as test_blit_clipping(), but uses sprite
// blitting instead and uses a grey background. no part of the screen buffer
// should be black as that colour is not used anywhere in the test sprite.
void test_sprite_clipping(void) {
    SURFACE *bmp = get_sprite(16, 16);

    surface_clear(screen, 8);

    surface_blit_sprite(bmp, screen, -3, 16);
    surface_blit_sprite(bmp, screen, -4, 36);
    surface_blit_sprite(bmp, screen, -8, 56);
    surface_blit_sprite(bmp, screen, -12, 76);
    surface_blit_sprite(bmp, screen, -13, 96);
    surface_blit_sprite(bmp, screen, -14, 116);
    surface_blit_sprite(bmp, screen, -16, 136);

    surface_blit_sprite(bmp, screen, 16, -3);
    surface_blit_sprite(bmp, screen, 36, -4);
    surface_blit_sprite(bmp, screen, 56, -8);
    surface_blit_sprite(bmp, screen, 76, -12);
    surface_blit_sprite(bmp, screen, 96, -13);
    surface_blit_sprite(bmp, screen, 116, -14);
    surface_blit_sprite(bmp, screen, 136, -16);

    surface_blit_sprite(bmp, screen, 307, 16);
    surface_blit_sprite(bmp, screen, 308, 36);
    surface_blit_sprite(bmp, screen, 312, 56);
    surface_blit_sprite(bmp, screen, 316, 76);
    surface_blit_sprite(bmp, screen, 317, 96);
    surface_blit_sprite(bmp, screen, 318, 116);
    surface_blit_sprite(bmp, screen, 320, 136);

    surface_blit_sprite(bmp, screen, 16, 187);
    surface_blit_sprite(bmp, screen, 36, 188);
    surface_blit_sprite(bmp, screen, 56, 192);
    surface_blit_sprite(bmp, screen, 76, 196);
    surface_blit_sprite(bmp, screen, 96, 197);
    surface_blit_sprite(bmp, screen, 116, 198);
    surface_blit_sprite(bmp, screen, 136, 200);

    surface_free(bmp);
    getch();
}

// prints out some simple text messages, including ones with newlines in them.
// also prints out the entire 1-127 ASCII character set in a 16x8 grid.
// a duplicate set of text is drawn using the "fast" variants.
void test_text(void) {
    int i, x, y;
    char string[255];
    char *p;

    surface_clear(screen, 0);
    memset(string, 0, 255);

    surface_text(screen, 10, 10, 15, "Hello, world!");
    surface_text_f(screen, 170, 10, 15, "Hello, world!");

    surface_filled_rect(screen, 8, 28, 114, 40, 7);
    surface_text(screen, 10, 30, 15, "transparency!");
    surface_filled_rect_f(screen, 168, 28, 274, 40, 7);
    surface_text_f(screen, 170, 30, 15, "transparency!");

    surface_text(screen, 10, 50, 15, "line 1\nline 2");
    surface_text_f(screen, 170, 50, 15, "line 1\nline 2");

    surface_printf(screen, 10, 70, 15, "printf pi %.5f", PI);
    surface_printf_f(screen, 170, 70, 15, "printf pi %.5f", PI);

    p = &string[0];
    for (i = 1; i <= 127; ++i, ++p) {
        if (i % 16 == 0) {
            *p = '\n';
            ++p;
        }
        if (i == 10)
            *p = ' ';   // will be interpreted as a \n (and hence, not shown)
        else
            *p = (char)i;
    }

    surface_text(screen, 10, 100, 15, string);
    surface_text_f(screen, 170, 100, 15, string);

    getch();
}

// text is drawn along each edge of the screen. two messages are drawn at each
// edge, but one is completely out of bounds so the second should not be
// visible. red rects are drawn marking the extents of the text.
void test_text_clipping(void) {
    int x, y;
    char message[] = "Hello, world!";
    int len = strlen(message);
    int width = len * 8;
    int height = 8;

    surface_clear(screen, 0);

    x = -32; y = 10;
    surface_rect(screen, x - 1, y - 1, x + width, y + height, 4);
    surface_text(screen, x, y, 9, message);

    x = 80; y = -4;
    surface_rect(screen, x - 1, y - 1, x + width, y + height, 4);
    surface_text(screen, x, y, 10, message);

    x = 288; y = 120;
    surface_rect(screen, x - 1, y - 1, x + width, y + height, 4);
    surface_text(screen, x, y, 11, message);

    x = 200; y = 196;
    surface_rect(screen, x - 1, y - 1, x + width, y + height, 4);
    surface_text(screen, x, y, 12, message);

    x = -232; y = 10;
    surface_rect(screen, x - 1, y - 1, x + width, y + height, 4);
    surface_text(screen, x, y, 5, message);

    x = 80; y = -24;
    surface_rect(screen, x - 1, y - 1, x + width, y + height, 4);
    surface_text(screen, x, y, 6, message);

    x = 360; y = 120;
    surface_rect(screen, x - 1, y - 1, x + width, y + height, 4);
    surface_text(screen, x, y, 7, message);

    x = 200; y = 240;
    surface_rect(screen, x - 1, y - 1, x + width, y + height, 4);
    surface_text(screen, x, y, 8, message);

    getch();
}

// displays the entire palette in a grid, waits for a keypress, then changes
// color 15's RGB values.
void test_palette(void) {
    int i, x, y;
    byte r, g, b;

    surface_clear(screen, 0);

    i = 0;
    for (y = 0; y < 16; ++y) {
        for (x = 0; x < 16; ++x) {
            surface_filled_rect(screen, x * 8, y * 8, x * 8 + 7, y * 8 + 7, i);
            ++i;
        }
    }

    getch();

    pal_get_color(15, &r, &g, &b);
    ASSERT(r == 63 && g == 63 && b == 63);
    pal_set_color(15, r / 2, g / 2, b / 2);

    getch();
    pal_set_color(15, r, g, b);
}

void test_palette_fading(void) {
    int i, x, y;
    byte palette[768];
    surface_clear(screen, 0);

    i = 0;
    for (y = 0; y < 16; ++y) {
        for (x = 0; x < 16; ++x) {
            surface_filled_rect(screen, x * 8, y * 8, x * 8 + 7, y * 8 + 7, i);
            ++i;
        }
    }
    pal_get(palette);

    getch();

    pal_fade_range_to_color(16, 31, 0, 0, 0, 1);

    getch();

    pal_fade_range_to_palette(16, 31, palette, 1);

    getch();

    pal_fade_to_color(0, 0, 0, 1);

    getch();

    pal_fade_to_palette(palette, 1);

    getch();
}

void test_pcx(void) {
    SURFACE *pcx;
    DGL_ERROR err;
    byte pcx_palette[768];
    byte original_palette[768];

    surface_clear(screen, 0);
    pal_get(original_palette);

    pcx = pcx_load("notreal.pcx", NULL);
    err = dgl_last_error();
    ASSERT(pcx == NULL);
    ASSERT(err != DGL_NONE);

    pcx = pcx_load("test.pcx", NULL);
    ASSERT(pcx != NULL);

    surface_blit(pcx, screen, 0, 0);
    getch();

    surface_clear(screen, 0);
    surface_free(pcx);
    pcx = pcx_load("test.pcx", pcx_palette);
    ASSERT(pcx != NULL);

    pal_set(pcx_palette);
    surface_blit(pcx, screen, 0, 0);

    getch();
    pal_set(original_palette);
}

// ---------------------------------------------------------------------------

int main(void) {
    signal(SIGINT, break_handler);

    dgl_init();

    ASSERT(screen == NULL);

    if (!video_init()) {
        printf("Error initializing video: %s\n", dgl_last_error_message());
        return 1;
    }

    ASSERT(screen != NULL);
    ASSERT(screen->width == 320 && screen->height == 200);

    backbuffer = surface_create(320, 200);
    ASSERT(backbuffer != NULL);
    ASSERT(backbuffer->width == 320 && backbuffer->height == 200);

    test_fixed();
    test_vector2i();
    test_vector2f();
    test_surface_clear();
    test_surface_copy();
    test_pixels_1();
    test_pixels_2();
    test_pixels_clipping();
    test_hline();
    test_hline_clipping();
    test_vline();
    test_vline_clipping();
    test_line();
    test_line_clipping();
    test_rect();
    test_rect_clipping();
    test_filled_rect();
    test_filled_rect_clipping();
    test_blit();
    test_blit_clipping();
    test_sprite();
    test_sprite_clipping();
    test_text();
    test_text_clipping();
    test_palette();
    test_palette_fading();
    test_pcx();

    surface_free(backbuffer);


    return 0;
}

