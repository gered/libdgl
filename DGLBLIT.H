#ifndef DGL_DGLBLIT_H_INCLUDED
#define DGL_DGLBLIT_H_INCLUDED

#include "dglgfx.h"
#include "dglutil.h"

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

static void surface_blit(const SURFACE *src, SURFACE *dest, int x, int y);

static void surface_blit_f(const SURFACE *src, SURFACE *dest, int x, int y);

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

void direct_blit_4(int width4, int lines, byte *dest, const byte *src, int dest_y_inc, int src_y_inc);
void direct_blit_4r(int width4, int lines, int remainder, byte *dest, const byte *src, int dest_y_inc, int src_y_inc);
void direct_blit_r(int width, int lines, byte *dest, const byte *src, int dest_y_inc, int src_y_inc);
void direct_blit_sprite_4(int width4, int lines, byte *dest, const byte *src, int dest_y_inc, int src_y_inc);
void direct_blit_sprite_4r(int width4, int lines, byte *dest, const byte *src, int remainder, int dest_y_inc, int src_y_inc);
void direct_blit_sprite_r(int width, int lines, byte *dest, const byte *src, int dest_y_inc, int src_y_inc);
void direct_blit_sprite_8(int width8, int lines, byte *dest, const byte *src, int dest_y_inc, int src_y_inc);
void direct_blit_sprite_8r(int width8, int lines, byte *dest, const byte *src, int remainder, int dest_y_inc, int src_y_inc);

static void surface_blit_sprite(const SURFACE *src, SURFACE *dest, int x, int y);
static void surface_blit_sprite_f(const SURFACE *src, SURFACE *dest, int x, int y);

// --------------------------------------------------------------------------

static void surface_blit(const SURFACE *src, SURFACE *dest, int x, int y) {
    surface_blit_region(src, dest, 0, 0, src->width, src->height, x, y);
}


static void surface_blit_f(const SURFACE *src, SURFACE *dest, int x, int y) {
    surface_blit_region_f(src, dest, 0, 0, src->width, src->height, x, y);
}

static void surface_blit_sprite(const SURFACE *src, SURFACE *dest, int x, int y) {
    surface_blit_sprite_region(src, dest, 0, 0, src->width, src->height, x, y);
}

static void surface_blit_sprite_f(const SURFACE *src, SURFACE *dest, int x, int y) {
    surface_blit_sprite_region_f(src, dest, 0, 0, src->width, src->height, x, y);
}

#endif

