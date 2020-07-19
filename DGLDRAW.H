#ifndef LIBDGL_DGLDRAW_H
#define LIBDGL_DGLDRAW_H

#include "dglgfx.h"
#include "dglclip.h"
#include "dglutil.h"

#ifdef __cplusplus
extern "C" {
#endif

static void pset(SURFACE *surface, int x, int y, uint8 color);
static void pset_f(SURFACE *surface, int x, int y, uint8 color);
static uint8 pget(const SURFACE *surface, int x, int y);
static uint8 pget_f(const SURFACE *surface, int x, int y);

static void draw_hline(SURFACE *surface, int x1, int x2, int y, uint8 color);
void draw_hline_f(SURFACE *surface, int x1, int x2, int y, uint8 color);
static void draw_vline(SURFACE *surface, int x, int y1, int y2, uint8 color);
void draw_vline_f(SURFACE *surface, int x, int y1, int y2, uint8 color);
void draw_line(SURFACE *surface, int x1, int y1, int x2, int y2, uint8 color);
void draw_line_f(SURFACE *surface, int x1, int y1, int x2, int y2, uint8 color);

void draw_rect(SURFACE *surface, int x1, int y1, int x2, int y2, uint8 color);
void draw_rect_f(SURFACE *surface, int x1, int y1, int x2, int y2, uint8 color);
void draw_filled_rect(SURFACE *surface, int x1, int y1, int x2, int y2, uint8 color);
void draw_filled_rect_f(SURFACE *surface, int x1, int y1, int x2, int y2, uint8 color);

void draw_text(SURFACE *surface, int x, int y, uint8 color, const char *text);
void draw_text_f(SURFACE *surface, int x, int y, uint8 color, const char *text);
void draw_printf(SURFACE *surface, int x, int y, uint8 color, const char *format, ...);
void draw_printf_f(SURFACE *surface, int x, int y, uint8 color, const char *format, ...);

void lowlevel_rect_f(
    uint8 color,
    int width,
    int y_inc,
    int lines,
    uint8 *p1,
    uint8 *p2
);

void lowlevel_filled_rect_f(
    uint8 color,
    int y_inc,
    int width,
    int lines,
    uint8 *dest
);

// --------------------------------------------------------------------------

static void pset(SURFACE *surface, int x, int y, uint8 color) {
    if (is_point_in_bounds(&surface->clip_region, x, y))
        pset_f(surface, x, y, color);
}

static void pset_f(SURFACE *surface, int x, int y, uint8 color) {
    uint32 offset = surface_offset(surface, x, y);
    surface->pixels[offset] = color;
}

static uint8 pget(const SURFACE *surface, int x, int y) {
    if (is_point_in_bounds(&surface->clip_region, x, y))
        return pget_f(surface, x, y);
    else
        return 0;
}

static uint8 pget_f(const SURFACE *surface, int x, int y) {
    uint32 offset = surface_offset(surface, x, y);
    return surface->pixels[offset];
}

static void draw_hline(SURFACE *surface, int x1, int x2, int y, uint8 color) {
    if (x2 < x1)
        SWAP(int, x1, x2);
    if (clamp_to_region(&surface->clip_region, &x1, &y, &x2, &y))
        draw_hline_f(surface, x1, x2, y, color);
}

static void draw_vline(SURFACE *surface, int x, int y1, int y2, uint8 color) {
    if (y2 < y1)
        SWAP(int, y1, y2);
    if (clamp_to_region(&surface->clip_region, &x, &y1, &x, &y2))
        draw_vline_f(surface, x, y1, y2, color);
}

#ifdef __cplusplus
}
#endif

#endif

