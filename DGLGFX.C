#include "dglgfx.h"
#include "dglblit.h"
#include "dglutil.h"
#include "dglerror.h"
#include <stdlib.h>
#include <string.h>

extern void _video_mode(int mode);
#pragma aux _video_mode = \
    "int 0x10"            \
    parm [eax];

static boolean _initialized = FALSE;

SURFACE *screen = NULL;

static SURFACE* surface_create_internal(int width, int height, byte *pixels) {
    SURFACE *surface = (SURFACE*)malloc(sizeof(SURFACE));

    surface->width = width;
    surface->height = height;
    surface->clip_region = rect(0, 0, width, height);
    surface->flags = 0;

    if (pixels != NULL) {
        surface->flags |= SURFACE_FLAGS_ALIASED;
        surface->pixels = pixels;
    } else {
        int size = width * height;
        surface->pixels = (byte*)malloc(size);
        mem_fill(surface->pixels, 0, size);
    }

    return surface;
}

boolean video_init(void) {
    byte *framebuffer;

    if (_initialized) {
        dgl_set_error(DGL_VIDEO_ALREADY_INITIALIZED);
        return FALSE;
    }

    _video_mode(0x13);

    framebuffer = (byte*)0xa0000;
    screen = surface_create_internal(320, 200, framebuffer);
    surface_clear(screen, 0);

    _initialized = TRUE;
    return TRUE;
}

boolean video_shutdown(void) {
    if (!_initialized)
        return TRUE;   // don't care

    _video_mode(0x03);

    surface_free(screen);
    screen = NULL;

    _initialized = FALSE;
    return TRUE;
}

boolean video_is_initialized(void) {
    return _initialized;
}

void video_wait_vsync(void) {
    do {} while (inp(0x3da) & 0x8);
    do {} while (!(inp(0x3da) & 0x8));
}

SURFACE* surface_create(int width, int height) {
    return surface_create_internal(width, height, NULL);
}

void surface_free(SURFACE *surface) {
    if (!surface)
        return;

    if (!BIT_ISSET(SURFACE_FLAGS_ALIASED, surface->flags))
        free(surface->pixels);
    free(surface);
}

void surface_clear(SURFACE *surface, int color) {
    int length = surface->width * surface->height;
    mem_fill(surface->pixels, (byte)color, length);
}

void surface_copy(const SURFACE *src, SURFACE *dest) {
    if (src->width == dest->width && src->height == dest->height) {
        int length = src->width * src->height;
        mem_copy(dest->pixels, src->pixels, length);
    } else {
        surface_blit(src, dest, 0, 0);
    }
}

