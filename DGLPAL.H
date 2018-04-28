#ifndef DGL_DGLPAL_H_INCLUDED
#define DGL_DGLPAL_H_INCLUDED

#include "dglcmn.h"

void pal_set_color(byte color, byte r, byte g, byte b);
void pal_get_color(byte color, byte *r, byte *g, byte *b);
void pal_set(const byte *palette);
void pal_get(byte *palette);

void pal_fade_range_to_color(int start, int end, byte r, byte g, byte b, int step);
void pal_fade_range_to_palette(int start, int end, const byte *palette, int step);
static void pal_fade_to_color(byte r, byte g, byte b, int step);
static void pal_fade_to_palette(const byte *palette, int step);

// --------------------------------------------------------------------------

static void pal_fade_to_color(byte r, byte g, byte b, int step) {
    pal_fade_range_to_color(0, 255, r, g, b, step);
}

static void pal_fade_to_palette(const byte *palette, int step) {
    pal_fade_range_to_palette(0, 255, palette, step);
}

#endif

