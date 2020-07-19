#ifndef LIBDGL_DGLPAL_H
#define LIBDGL_DGLPAL_H

#include "dglcmn.h"

#ifdef __cplusplus
extern "C" {
#endif

void pal_set_color(uint8 color, uint8 r, uint8 g, uint8 b);
void pal_get_color(uint8 color, uint8 *r, uint8 *g, uint8 *b);
void pal_set(const uint8 *palette);
void pal_get(uint8 *palette);

void pal_fade_range_to_color(
    uint8 start,
    uint8 end,
    uint8 r,
    uint8 g,
    uint8 b,
    int step
);

void pal_fade_range_to_palette(
    uint8 start,
    uint8 end,
    const uint8 *palette,
    int step
);

static void pal_fade_to_color(uint8 r, uint8 g, uint8 b, int step);
static void pal_fade_to_palette(const uint8 *palette, int step);

// --------------------------------------------------------------------------

static void pal_fade_to_color(uint8 r, uint8 g, uint8 b, int step) {
    pal_fade_range_to_color(0, 255, r, g, b, step);
}

static void pal_fade_to_palette(const uint8 *palette, int step) {
    pal_fade_range_to_palette(0, 255, palette, step);
}

#ifdef __cplusplus
}
#endif

#endif

