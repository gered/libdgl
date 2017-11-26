#ifndef DGL_BLIT_H_INCLUDED
#define DGL_BLIT_H_INCLUDED

#include "gfx.h"

void surface_blit_region(const SURFACE *src,
                         SURFACE *dest,
                         int src_x,
                         int src_y,
                         int src_width,
                         int src_height,
                         int dest_x,
                         int dest_y);

void surface_blit_region_f(const SURFACE *src,
                           SURFACE *dest,
                           int src_x,
                           int src_y,
                           int src_width,
                           int src_height,
                           int dest_x,
                           int dest_y);

static inline void surface_blit(const SURFACE *src, SURFACE *dest, int x, int y);
static inline void surface_blit_f(const SURFACE *src, SURFACE *dest, int x, int y);

void surface_blit_sprite_region(const SURFACE *src,
                                SURFACE *dest,
                                int src_x,
                                int src_y,
                                int src_width,
                                int src_height,
                                int dest_x,
                                int dest_y);

void surface_blit_sprite_region_f(const SURFACE *src,
                                  SURFACE *dest,
                                  int src_x,
                                  int src_y,
                                  int src_width,
                                  int src_height,
                                  int dest_x,
                                  int dest_y);

static inline void surface_blit_sprite(const SURFACE *src, SURFACE *dest, int x, int y);
static inline void surface_blit_sprite_f(const SURFACE *src, SURFACE *dest, int x, int y);

// --------------------------------------------------------------------------

static inline void surface_blit(const SURFACE *src, SURFACE *dest, int x, int y) {
    surface_blit_region(src, dest, 0, 0, src->width, src->height, x, y);
}

static inline void surface_blit_f(const SURFACE *src, SURFACE *dest, int x, int y) {
    surface_blit_region_f(src, dest, 0, 0, src->width, src->height, x, y);
}

static inline void surface_blit_sprite(const SURFACE *src, SURFACE *dest, int x, int y) {
    surface_blit_sprite_region(src, dest, 0, 0, src->width, src->height, x, y);
}

static inline void surface_blit_sprite_f(const SURFACE *src, SURFACE *dest, int x, int y) {
    surface_blit_sprite_region_f(src, dest, 0, 0, src->width, src->height, x, y);
}

#endif

