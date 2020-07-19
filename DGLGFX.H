#ifndef LIBDGL_DGLGFX_H
#define LIBDGL_DGLGFX_H

#include "dglcmn.h"
#include "dglrect.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int    width;
    int    height;
    uint8  *pixels;
    RECT   clip_region;
    uint32 flags;
} SURFACE;

#define SURFACE_FLAGS_ALIASED      BIT_0

extern SURFACE *screen;

bool gfx_init(void);
bool gfx_shutdown(void);
bool gfx_is_initialized(void);

void wait_vsync(void);

SURFACE* surface_create(int width, int height);
void surface_free(SURFACE *surface);
void surface_clear(SURFACE *surface, uint8 color);
void surface_copy(const SURFACE *src, SURFACE *dest);

static int32 surface_offset(const SURFACE *surface, int x, int y);
static uint8* surface_pointer(const SURFACE *surface, int x, int y);

// --------------------------------------------------------------------------

static int32 surface_offset(const SURFACE *surface, int x, int y) {
    return (surface->width * y) + x;
}

static uint8* surface_pointer(const SURFACE *surface, int x, int y) {
    return surface->pixels + surface_offset(surface, x, y);
}

#ifdef __cplusplus
}
#endif

#endif

