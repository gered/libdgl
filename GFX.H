#ifndef DGL_GFX_H_INCLUDED
#define DGL_GFX_H_INCLUDED

#include "common.h"
#include "rect.h"

typedef struct {
    int     width;
    int     height;
    byte    *pixels;
    boolean aliased;
    RECT    clip_region;
} SURFACE;

extern SURFACE *screen;

boolean video_init(void);
boolean video_shutdown(void);
boolean video_is_initialized(void);

void video_wait_vsync(void);

void video_set_color(ubyte color, ubyte r, ubyte g, ubyte b);
void video_get_color(ubyte color, ubyte *r, ubyte *g, ubyte *b);
void video_set_palette(const byte *palette);
void video_get_palette(byte *palette);

SURFACE* surface_create(int width, int height);
void surface_free(SURFACE *surface);
void surface_clear(SURFACE *surface, int color);
void surface_copy(const SURFACE *src, SURFACE *dest);

static inline int surface_offset(const SURFACE *surface, int x, int y);
static inline byte* surface_pointer(const SURFACE *surface, int x, int y);

// --------------------------------------------------------------------------

static inline int surface_offset(const SURFACE *surface, int x, int y) {
    return surface->width * y + x;
}

static inline byte* surface_pointer(const SURFACE *surface, int x, int y) {
    return surface->pixels + surface_offset(surface, x, y);
}

#endif

