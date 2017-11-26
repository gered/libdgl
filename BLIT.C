#include "blit.h"
#include "clipping.h"
#include "internal.h"

static inline boolean clip_blit(const RECT *dest_clip_region,
                                RECT *src_blit_region,
                                int *dest_x,
                                int *dest_y) {
    int dest_clip_right = rect_right(dest_clip_region);
    int dest_clip_bottom = rect_bottom(dest_clip_region);
    int offset;

    // off the left edge?
    if (*dest_x < dest_clip_region->x) {
        // completely off the left edge?
        if ((*dest_x + src_blit_region->width) < dest_clip_region->x)
            return FALSE;

        offset = src_blit_region->x - *dest_x;
        src_blit_region->x += offset;
        src_blit_region->width -= offset;
        *dest_x = dest_clip_region->x;
    }

    // off the right edge?
    if (*dest_x > (dest_clip_region->width - src_blit_region->width)) {
        // completely off the right edge?
        if (*dest_x > dest_clip_right)
            return FALSE;

        offset = *dest_x + src_blit_region->width - dest_clip_region->width;
        src_blit_region->width -= offset;
    }

    // off the top edge?
    if (*dest_y < dest_clip_region->y) {
        // completely off the top edge?
        if ((*dest_y + src_blit_region->height) < dest_clip_region->y)
            return FALSE;

        offset = dest_clip_region->y - *dest_y;
        src_blit_region->y += offset;
        src_blit_region->height -= offset;
        *dest_y = dest_clip_region->y;
    }

    // off the bottom edge?
    if (*dest_y > (dest_clip_region->height - src_blit_region->height)) {
        // completely off the bottom edge?
        if (*dest_y > dest_clip_bottom)
            return FALSE;

        offset = *dest_y + src_blit_region->height - dest_clip_region->height;
        src_blit_region->height -= offset;
    }

    return TRUE;
}

void surface_blit_region(const SURFACE *src,
                         SURFACE *dest,
                         int src_x,
                         int src_y,
                         int src_width,
                         int src_height,
                         int dest_x,
                         int dest_y) {
    RECT src_region = rect(src_x, src_y, src_width, src_height);
    boolean on_screen = clip_blit(&dest->clip_region, &src_region, &dest_x, &dest_y);

    if (!on_screen)
        return;

    surface_blit_region_f(src, dest,
                          src_region.x, src_region.y,
                          src_region.width, src_region.height,
                          dest_x, dest_y);
}

void surface_blit_region_f(const SURFACE *src,
                           SURFACE *dest,
                           int src_x,
                           int src_y,
                           int src_width,
                           int src_height,
                           int dest_x,
                           int dest_y) {
    byte *psrc, *pdest;
    int src_y_inc, dest_y_inc;
    int width;
    int lines_left;
    int num_dwords;

    psrc = surface_pointer(src, src_x, src_y);
    src_y_inc = src->width;
    pdest = surface_pointer(dest, dest_x, dest_y);
    dest_y_inc = dest->width;
    width = src_width;
    lines_left = src_height;

    if (width % 4 == 0) {
        num_dwords = width / 4;
        while (lines_left) {
            REP_MOVSL(psrc, pdest, num_dwords);
            psrc += src_y_inc;
            pdest += dest_y_inc;
            --lines_left;
        }
    } else {
        while (lines_left) {
            REP_MOVSB(psrc, pdest, width);
            psrc += src_y_inc;
            pdest += dest_y_inc;
            --lines_left;
        }
    }
}

void surface_blit_sprite_region(const SURFACE *src,
                                SURFACE *dest,
                                int src_x,
                                int src_y,
                                int src_width,
                                int src_height,
                                int dest_x,
                                int dest_y) {
    RECT src_region = rect(src_x, src_y, src_width, src_height);
    boolean on_screen = clip_blit(&dest->clip_region, &src_region, &dest_x, &dest_y);

    if (!on_screen)
        return;

    surface_blit_sprite_region_f(src, dest,
                                 src_region.x, src_region.y,
                                 src_region.width, src_region.height,
                                 dest_x, dest_y);
}

void surface_blit_sprite_region_f(const SURFACE *src,
                                  SURFACE *dest,
                                  int src_x,
                                  int src_y,
                                  int src_width,
                                  int src_height,
                                  int dest_x,
                                  int dest_y) {
    byte *psrc, *pdest;
    byte pixel;
    int src_y_inc, dest_y_inc;
    int width;
    int lines_left;
    int x;

    psrc = surface_pointer(src, src_x, src_y);
    src_y_inc = src->width;
    pdest = surface_pointer(dest, dest_x, dest_y);
    dest_y_inc = dest->width;
    width = src_width;
    lines_left = src_height;

    // based on benchmarking on a DX2-66, there is VERY significant
    // diminishing returns for loop unrolling beyond these sizes
    // (in fact, even the one for 8 is a very small gain over 4)
    if (width % 8 == 0) {
        while (lines_left) {
            for (x = 0; x < width; x += 8) {
                if ((pixel = psrc[x + 0])) pdest[x + 0] = pixel;
                if ((pixel = psrc[x + 1])) pdest[x + 1] = pixel;
                if ((pixel = psrc[x + 2])) pdest[x + 2] = pixel;
                if ((pixel = psrc[x + 3])) pdest[x + 3] = pixel;
                if ((pixel = psrc[x + 4])) pdest[x + 4] = pixel;
                if ((pixel = psrc[x + 5])) pdest[x + 5] = pixel;
                if ((pixel = psrc[x + 6])) pdest[x + 6] = pixel;
                if ((pixel = psrc[x + 7])) pdest[x + 7] = pixel;
            }
            psrc += src_y_inc;
            pdest += dest_y_inc;
            --lines_left;
        }
    } else if (width % 4 == 0) {
        while (lines_left) {
            for (x = 0; x < width; x += 4) {
                if ((pixel = psrc[x + 0])) pdest[x + 0] = pixel;
                if ((pixel = psrc[x + 1])) pdest[x + 1] = pixel;
                if ((pixel = psrc[x + 2])) pdest[x + 2] = pixel;
                if ((pixel = psrc[x + 3])) pdest[x + 3] = pixel;
            }
            psrc += src_y_inc;
            pdest += dest_y_inc;
            --lines_left;
        }
    } else {
        while (lines_left) {
            for (x = 0; x < width; ++x) {
                if ((pixel = psrc[x]))
                    pdest[x] = pixel;
            }
            psrc += src_y_inc;
            pdest += dest_y_inc;
            --lines_left;
        }
    }
}

