#include "pal.h"
#include "dglgfx.h"
#include "dgldraw.h"
#include "dglpal.h"
#include <stdio.h>
#include "helpers.h"

// displays the entire palette in a grid, waits for a keypress, then changes
// color 15's RGB values.
void test_palette(void) {
    int i, x, y;
    uint8 r, g, b;

    surface_clear(screen, 0);

    i = 0;
    for (y = 0; y < 16; ++y) {
        for (x = 0; x < 16; ++x) {
            draw_filled_rect(screen, x * 8, y * 8, x * 8 + 7, y * 8 + 7, i);
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
    uint8 palette[768];
    surface_clear(screen, 0);

    i = 0;
    for (y = 0; y < 16; ++y) {
        for (x = 0; x < 16; ++x) {
            draw_filled_rect(screen, x * 8, y * 8, x * 8 + 7, y * 8 + 7, i);
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

