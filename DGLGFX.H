#ifndef DGL_DGLGFX_H_INCLUDED
#define DGL_DGLGFX_H_INCLUDED

#include "dglcmn.h"
#include "dglrect.h"

typedef struct {
    int          width;
    int          height;
    byte         *pixels;
    RECT         clip_region;
    unsigned int flags;
} SURFACE;

#define SURFACE_FLAGS_ALIASED      BIT_0

extern SURFACE *screen;

boolean video_init(void);
boolean video_shutdown(void);
boolean video_is_initialized(void);

void video_wait_vsync(void);

SURFACE* surface_create(int width, int height);
void surface_free(SURFACE *surface);
void surface_clear(SURFACE *surface, int color);
void surface_copy(const SURFACE *src, SURFACE *dest);

static int surface_offset(const SURFACE *surface, int x, int y);
static byte* surface_pointer(const SURFACE *surface, int x, int y);

// --------------------------------------------------------------------------

static int surface_offset(const SURFACE *surface, int x, int y) {
    return (surface->width * y) + x;
}

static byte* surface_pointer(const SURFACE *surface, int x, int y) {
    return surface->pixels + surface_offset(surface, x, y);
}

#endif

