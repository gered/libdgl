#include "gfx.h"
#include "blit.h"
#include "util.h"
#include "internal.h"
#include "error.h"
#include <stdlib.h>
#include <string.h>
#include <dpmi.h>
#include <pc.h>

static boolean _initialized = FALSE;

SURFACE *screen = NULL;

static SURFACE* surface_create_internal(int width, int height, byte *pixels) {
    SURFACE *surface = malloc(sizeof(SURFACE));

    surface->width = width;
    surface->height = height;
    surface->clip_region = rect(0, 0, width, height);

    if (pixels != NULL) {
        surface->aliased = TRUE;
        surface->pixels = pixels;
    } else {
        surface->aliased = FALSE;
        surface->pixels = malloc(width * height);
        memset(surface->pixels, 0, width * height);
    }

    return surface;
}

boolean video_init(void) {
    __dpmi_regs regs;
    void *framebuffer;

    if (_initialized) {
        dgl_set_error(DGL_VIDEO_ALREADY_INITIALIZED);
        return FALSE;
    }

    memset(&regs, 0, sizeof(__dpmi_regs));
    regs.h.ah = 0x00;
    regs.h.al = 0x13;
    if (__dpmi_int(0x10, &regs)) {
        dgl_set_error(DGL_VIDEO_MODE_13H_INIT_FAILURE);
        return FALSE;
    }

    framebuffer = map_dos_memory(0xa0000);
    screen = surface_create_internal(320, 200, framebuffer);
    surface_clear(screen, 0);

    _initialized = TRUE;
    return TRUE;
}

boolean video_shutdown(void) {
    __dpmi_regs regs;

    if (!_initialized)
        return TRUE;   // don't care

    memset(&regs, 0, sizeof(__dpmi_regs));
    regs.h.ah = 0x00;
    regs.h.al = 0x03;
    if (__dpmi_int(0x10, &regs)) {
        dgl_set_error(DGL_VIDEO_TEXT_MODE_INIT_FAILURE);
        return FALSE;
    }

    surface_free(screen);
    screen = NULL;

    _initialized = FALSE;
    return TRUE;
}

boolean video_is_initialized(void) {
    return _initialized;
}

void video_wait_vsync(void) {
    do {} while (inportb(0x3da) & 0x8);
    do {} while (!(inportb(0x3da) & 0x8));
}

void video_set_color(ubyte color, ubyte r, ubyte g, ubyte b) {
    outportb(0x3c6, 0xff);
    outportb(0x3c8, color);
    outportb(0x3c9, r);
    outportb(0x3c9, g);
    outportb(0x3c9, b);
}

void video_get_color(ubyte color, ubyte *r, ubyte *g, ubyte *b) {
    outportb(0x3c6, 0xff);
    outportb(0x3c7, color);
    *r = inportb(0x3c9);
    *g = inportb(0x3c9);
    *b = inportb(0x3c9);
}

void video_set_palette(const byte *palette) {
    int i = 0;
    for (i = 0; i < 256; ++i) {
        video_set_color(i, palette[0], palette[1], palette[2]);
        palette += 3;
    }
}

void video_get_palette(byte *palette) {
    int i = 0;
    for (i = 0; i < 256; ++i) {
        video_get_color(i, palette, palette + 1, palette + 2);
        palette += 3;
    }
}

SURFACE* surface_create(int width, int height) {
    return surface_create_internal(width, height, NULL);
}

void surface_free(SURFACE *surface) {
    if (!surface)
        return;

    if (!surface->aliased)
        free(surface->pixels);
    free(surface);
}

void surface_clear(SURFACE *surface, int color) {
    int length = surface->width * surface->height;
    if (length % 4 == 0) {
        color *= 0x01010101;
        REP_STOSL(color, surface->pixels, length / 4);
    } else {
        memset(surface->pixels, color, surface->width * surface->height);
    }
}

void surface_copy(const SURFACE *src, SURFACE *dest) {
    if (src->width == dest->width && src->height == dest->height) {
        memcpy(dest->pixels, src->pixels, src->width * src->height);
    } else {
        surface_blit(src, dest, 0, 0);
    }
}

