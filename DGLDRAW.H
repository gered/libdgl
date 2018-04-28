#ifndef DGL_DGLDRAW_H_INCLUDED
#define DGL_DGLDRAW_H_INCLUDED

#include "dglgfx.h"
#include "dglclip.h"
#include "dglutil.h"

static void surface_pset(SURFACE *surface, int x, int y, int color);
static void surface_pset_f(SURFACE *surface, int x, int y, int color);
static int surface_point(const SURFACE *surface, int x, int y);
static int surface_point_f(const SURFACE *surface, int x, int y);

static void surface_hline(SURFACE *surface, int x1, int x2, int y, int color);
void surface_hline_f(SURFACE *surface, int x1, int x2, int y, int color);
static void surface_vline(SURFACE *surface, int x, int y1, int y2, int color);
void surface_vline_f(SURFACE *surface, int x, int y1, int y2, int color);
void surface_line(SURFACE *surface, int x1, int y1, int x2, int y2, int color);
void surface_line_f(SURFACE *surface, int x1, int y1, int x2, int y2, int color);

void surface_rect(SURFACE *surface, int x1, int y1, int x2, int y2, int color);
void surface_rect_f(SURFACE *surface, int x1, int y1, int x2, int y2, int color);
void surface_filled_rect(SURFACE *surface, int x1, int y1, int x2, int y2, int color);
void surface_filled_rect_f(SURFACE *surface, int x1, int y1, int x2, int y2, int color);

void surface_text(SURFACE *surface, int x, int y, int color, const char *text);
void surface_text_f(SURFACE *surface, int x, int y, int color, const char *text);
void surface_printf(SURFACE *surface, int x, int y, int color, const char *format, ...);
void surface_printf_f(SURFACE *surface, int x, int y, int color, const char *format, ...);

void direct_rect_f(int color, int width, int y_inc, int lines, byte *p1, byte *p2);
void direct_filled_rect_f(int color, int y_inc, int width, int lines, byte *dest);

// --------------------------------------------------------------------------

static void surface_pset(SURFACE *surface, int x, int y, int color) {
    if (is_point_in_bounds(&surface->clip_region, x, y))
        surface_pset_f(surface, x, y, color);
}

static void surface_pset_f(SURFACE *surface, int x, int y, int color) {
    int offset = surface_offset(surface, x, y);
    surface->pixels[offset] = color;
}

static int surface_point(const SURFACE *surface, int x, int y) {
    if (is_point_in_bounds(&surface->clip_region, x, y))
        return surface_point_f(surface, x, y);
    else
        return 0;
}

static int surface_point_f(const SURFACE *surface, int x, int y) {
    int offset = surface_offset(surface, x, y);
    return surface->pixels[offset];
}

static void surface_hline(SURFACE *surface, int x1, int x2, int y, int color) {
    if (x2 < x1)
        SWAP(int, x1, x2);
    if (clamp_to_region(&surface->clip_region, &x1, &y, &x2, &y))
        surface_hline_f(surface, x1, x2, y, color);
}

static void surface_vline(SURFACE *surface, int x, int y1, int y2, int color) {
    if (y2 < y1)
        SWAP(int, y1, y2);
    if (clamp_to_region(&surface->clip_region, &x, &y1, &x, &y2))
        surface_vline_f(surface, x, y1, y2, color);
}

#endif

