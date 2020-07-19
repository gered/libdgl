#include "dglpal.h"
#include "dglgfx.h"
#include <conio.h>
#include <dos.h>
#include <stdlib.h>

void pal_set_color(uint8 color, uint8 r, uint8 g, uint8 b) {
    outp(0x3c6, 0xff);
    outp(0x3c8, color);
    outp(0x3c9, r);
    outp(0x3c9, g);
    outp(0x3c9, b);
}

void pal_get_color(uint8 color, uint8 *r, uint8 *g, uint8 *b) {
    outp(0x3c6, 0xff);
    outp(0x3c7, color);
    *r = inp(0x3c9);
    *g = inp(0x3c9);
    *b = inp(0x3c9);
}

void pal_set(const uint8 *palette) {
    int i = 0;
    for (i = 0; i < 256; ++i) {
        pal_set_color(i, palette[0], palette[1], palette[2]);
        palette += 3;
    }
}

void pal_get(uint8 *palette) {
    int i = 0;
    for (i = 0; i < 256; ++i) {
        pal_get_color(i, palette, palette + 1, palette + 2);
        palette += 3;
    }
}

static bool fade_color(uint8 color, uint8 r, uint8 g, uint8 b, int step) {
    uint8 red, green, blue;
    uint8 diff_r, diff_g, diff_b;
    bool color_diff = false;

    pal_get_color(color, &red, &green, &blue);

    if (red != r) {
        color_diff = true;
        diff_r = abs(red - r);
        if (red > r)
            red -= MIN(step, diff_r);
        else
            red += MIN(step, diff_r);
    }

    if (green != g) {
        color_diff = true;
        diff_g = abs(green - g);
        if (green > g)
            green -= MIN(step, diff_g);
        else
            green += MIN(step, diff_g);
    }

    if (blue != b) {
        color_diff = true;
        diff_b = abs(blue - b);
        if (blue > b)
            blue -= MIN(step, diff_b);
        else
            blue += MIN(step, diff_b);
    }

    if (color_diff)
        pal_set_color(color, red, green, blue);

    return (red == r && green == g && blue == b);
}

void pal_fade_range_to_color(
    uint8 start,
    uint8 end,
    uint8 r,
    uint8 g,
    uint8 b,
    int step
) {
    int i;
    bool done = false;

    step = abs(step);

    while (!done) {
        done = true;
        wait_vsync();
        for (i = start; i <= end; ++i) {
            if (!fade_color(i, r, g, b, step))
                done = false;
        }
    }
}

void pal_fade_range_to_palette(
    uint8 start,
    uint8 end,
    const uint8 *palette,
    int step
) {
    int color, i;
    bool done = false;

    step = abs(step);

    while (!done) {
        done = true;
        wait_vsync();
        for (i = (start * 3), color = start; color <= end; ++color, i += 3) {
            if (!fade_color(color, palette[i], palette[i + 1], palette[i + 2], step))
                done = false;
        }
    }
}

