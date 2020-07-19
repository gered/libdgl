#ifndef LIBDGL_DGLBLIT_H
#define LIBDGL_DGLBLIT_H

#include "dglgfx.h"

#ifdef __cplusplus
extern "C" {
#endif

void blit_region(
    const SURFACE *src,
    SURFACE *dest,
    int src_x,
    int src_y,
    int src_width,
    int src_height,
    int dest_x,
    int dest_y
);

void blit_region_f(
    const SURFACE *src,
    SURFACE *dest,
    int src_x,
    int src_y,
    int src_width,
    int src_height,
    int dest_x,
    int dest_y
);

void blit_sprite_region(
    const SURFACE *src,
    SURFACE *dest,
    int src_x,
    int src_y,
    int src_width,
    int src_height,
    int dest_x,
    int dest_y
);

void blit_sprite_region_f(
    const SURFACE *src,
    SURFACE *dest,
    int src_x,
    int src_y,
    int src_width,
    int src_height,
    int dest_x,
    int dest_y
);

// --------------------------------------------------------------------------

void lowlevel_blit_4(
    int width4,
    int lines,
    uint8 *dest,
    const uint8 *src,
    int dest_y_inc,
    int src_y_inc
);

void lowlevel_blit_4r(
    int width4,
    int lines,
    int remainder,
    uint8 *dest,
    const uint8 *src,
    int dest_y_inc,
    int src_y_inc
);

void lowlevel_blit_r(
    int width,
    int lines,
    uint8 *dest,
    const uint8 *src,
    int dest_y_inc,
    int src_y_inc
);

void lowlevel_blit_sprite_4(
    int width4,
    int lines,
    uint8 *dest,
    const uint8 *src,
    int dest_y_inc,
    int src_y_inc
);

void lowlevel_blit_sprite_4r(
    int width4,
    int lines,
    uint8 *dest,
    const uint8 *src,
    int remainder,
    int dest_y_inc,
    int src_y_inc
);

void lowlevel_blit_sprite_r(
    int width,
    int lines,
    uint8 *dest,
    const uint8 *src,
    int dest_y_inc,
    int src_y_inc
);

void lowlevel_blit_sprite_8(
    int width8,
    int lines,
    uint8 *dest,
    const uint8 *src,
    int dest_y_inc,
    int src_y_inc
);

void lowlevel_blit_sprite_8r(
    int width8,
    int lines,
    uint8 *dest,
    const uint8 *src,
    int remainder,
    int dest_y_inc,
    int src_y_inc
);

// --------------------------------------------------------------------------


static void blit(const SURFACE *src, SURFACE *dest, int x, int y);
static void blit_f(const SURFACE *src, SURFACE *dest, int x, int y);
static void blit_sprite(const SURFACE *src, SURFACE *dest, int x, int y);
static void blit_sprite_f(const SURFACE *src, SURFACE *dest, int x, int y);


// --------------------------------------------------------------------------

static void blit(const SURFACE *src, SURFACE *dest, int x, int y) {
    blit_region(src, dest, 0, 0, src->width, src->height, x, y);
}

static void blit_f(const SURFACE *src, SURFACE *dest, int x, int y) {
    blit_region_f(src, dest, 0, 0, src->width, src->height, x, y);
}

static void blit_sprite(const SURFACE *src, SURFACE *dest, int x, int y) {
    blit_sprite_region(src, dest, 0, 0, src->width, src->height, x, y);
}

static void blit_sprite_f(const SURFACE *src, SURFACE *dest, int x, int y) {
    blit_sprite_region_f(src, dest, 0, 0, src->width, src->height, x, y);
}

#ifdef __cplusplus
}
#endif

#endif

