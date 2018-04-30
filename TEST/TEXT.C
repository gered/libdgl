#include "text.h"
#include "dgl.h"
#include <stdio.h>
#include <string.h>

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

