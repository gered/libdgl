#include "dglpal.h"
#include "dglgfx.h"
#include <stdlib.h>
#include <dos.h>

void pal_set_color(byte color, byte r, byte g, byte b) {
    outp(0x3c6, 0xff);
    outp(0x3c8, color);
    outp(0x3c9, r);
    outp(0x3c9, g);
    outp(0x3c9, b);
}

void pal_get_color(byte color, byte *r, byte *g, byte *b) {
    outp(0x3c6, 0xff);
    outp(0x3c7, color);
    *r = inp(0x3c9);
    *g = inp(0x3c9);
    *b = inp(0x3c9);
}

void pal_set(const byte *palette) {
    int i = 0;
    for (i = 0; i < 256; ++i) {
        pal_set_color(i, palette[0], palette[1], palette[2]);
        palette += 3;
    }
}

void pal_get(byte *palette) {
    int i = 0;
    for (i = 0; i < 256; ++i) {
        pal_get_color(i, palette, palette + 1, palette + 2);
        palette += 3;
    }
}

static boolean fade_color(int color, byte r, byte g, byte b, int step) {
    byte red, green, blue;
    byte diff_r, diff_g, diff_b;
    boolean color_diff = FALSE;

    pal_get_color(color, &red, &green, &blue);

    if (red != r) {
        color_diff = TRUE;
        diff_r = abs(red - r);
        if (red > r)
            red -= min(step, diff_r);
        else
            red += min(step, diff_r);
    }

    if (green != g) {
        color_diff = TRUE;
        diff_g = abs(green - g);
        if (green > g)
            green -= min(step, diff_g);
        else
            green += min(step, diff_g);
    }

    if (blue != b) {
        color_diff = TRUE;
        diff_b = abs(blue - b);
        if (blue > b)
            blue -= min(step, diff_b);
        else
            blue += min(step, diff_b);
    }

    if (color_diff)
        pal_set_color(color, red, green, blue);

    return (red == r && green == g && blue == b);
}

void pal_fade_range_to_color(int start, int end, byte r, byte g, byte b, int step) {
    int i;
    boolean done = FALSE;

    step = abs(step);

    while (!done) {
        done = TRUE;
        video_wait_vsync();
        for (i = start; i <= end; ++i) {
            if (!fade_color(i, r, g, b, step))
                done = FALSE;
        }
    }
}

void pal_fade_range_to_palette(int start, int end, const byte *palette, int step) {
    int color, i;
    boolean done = FALSE;

    step = abs(step);

    while (!done) {
        done = TRUE;
        video_wait_vsync();
        for (i = (start * 3), color = start; color <= end; ++color, i += 3) {
            if (!fade_color(color, palette[i], palette[i + 1], palette[i + 2], step))
                done = FALSE;
        }
    }
}

