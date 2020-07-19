#include "dglcmn.h"
#include "dglrect.h"
#include "dglblit.h"
#include "dglclip.h"

static bool clip_blit(
    const RECT *dest_clip_region,
    RECT *src_blit_region,
    int *dest_x,
    int *dest_y
) {
    int dest_clip_right = rect_right(dest_clip_region);
    int dest_clip_bottom = rect_bottom(dest_clip_region);
    int offset;

    // off the left edge?
    if (*dest_x < dest_clip_region->x) {
        // completely off the left edge?
        if ((*dest_x + src_blit_region->width - 1) < dest_clip_region->x)
            return false;

        offset = src_blit_region->x - *dest_x;
        src_blit_region->x += offset;
        src_blit_region->width -= offset;
        *dest_x = dest_clip_region->x;
    }

    // off the right edge?
    if (*dest_x > (dest_clip_region->width - src_blit_region->width)) {
        // completely off the right edge?
        if (*dest_x > dest_clip_right)
            return false;

        offset = *dest_x + src_blit_region->width - dest_clip_region->width;
        src_blit_region->width -= offset;
    }

    // off the top edge?
    if (*dest_y < dest_clip_region->y) {
        // completely off the top edge?
        if ((*dest_y + src_blit_region->height - 1) < dest_clip_region->y)
            return false;

        offset = dest_clip_region->y - *dest_y;
        src_blit_region->y += offset;
        src_blit_region->height -= offset;
        *dest_y = dest_clip_region->y;
    }

    // off the bottom edge?
    if (*dest_y > (dest_clip_region->height - src_blit_region->height)) {
        // completely off the bottom edge?
        if (*dest_y > dest_clip_bottom)
            return false;

        offset = *dest_y + src_blit_region->height - dest_clip_region->height;
        src_blit_region->height -= offset;
    }

    return true;
}

void blit_region(
    const SURFACE *src,
    SURFACE *dest,
    int src_x,
    int src_y,
    int src_width,
    int src_height,
    int dest_x,
    int dest_y
) {
    RECT src_region = rect(src_x, src_y, src_width, src_height);
    bool on_screen = clip_blit(&dest->clip_region, &src_region, &dest_x, &dest_y);

    if (!on_screen)
        return;

    blit_region_f(
        src, dest,
        src_region.x, src_region.y,
        src_region.width, src_region.height,
        dest_x, dest_y
    );
}

void blit_region_f(
    const SURFACE *src,
    SURFACE *dest,
    int src_x,
    int src_y,
    int src_width,
    int src_height,
    int dest_x,
    int dest_y
) {
    const uint8 *psrc;
    uint8 *pdest;
    int lines;
    int src_y_inc = src->width - src_width;
    int dest_y_inc = dest->width - src_width;
    int width_4, width_remainder;

    psrc = (const uint8*)surface_pointer(src, src_x, src_y);
    pdest = (uint8*)surface_pointer(dest, dest_x, dest_y);
    lines = src_height;

    width_4 = src_width / 4;
    width_remainder = src_width & 3;

    if (width_4 && !width_remainder) {
        // width is a multiple of 4 (no remainder)
        lowlevel_blit_4(width_4, lines, pdest, psrc, dest_y_inc, src_y_inc);

    } else if (width_4 && width_remainder) {
        // width is >= 4 and there is a remainder ( <= 3 )
        lowlevel_blit_4r(width_4, lines, width_remainder, pdest, psrc, dest_y_inc, src_y_inc);

    } else {
        // width is <= 3
        lowlevel_blit_r(width_remainder, lines, pdest, psrc, dest_y_inc, src_y_inc);
    }
}

void blit_sprite_region(
    const SURFACE *src,
    SURFACE *dest,
    int src_x,
    int src_y,
    int src_width,
    int src_height,
    int dest_x,
    int dest_y
) {
    RECT src_region = rect(src_x, src_y, src_width, src_height);
    bool on_screen = clip_blit(&dest->clip_region, &src_region, &dest_x, &dest_y);

    if (!on_screen)
        return;

    blit_sprite_region_f(
        src, dest,
        src_region.x, src_region.y,
        src_region.width, src_region.height,
        dest_x, dest_y
    );
}

void blit_sprite_region_f(
    const SURFACE *src,
    SURFACE *dest,
    int src_x,
    int src_y,
    int src_width,
    int src_height,
    int dest_x,
    int dest_y
) {
    const uint8 *psrc;
    uint8 *pdest;
    uint8 pixel;
    int src_y_inc, dest_y_inc;
    int width, width_4, width_8, width_remainder;
    int lines_left;
    int x;

    psrc = (const uint8*)surface_pointer(src, src_x, src_y);
    src_y_inc = src->width;
    pdest = (uint8*)surface_pointer(dest, dest_x, dest_y);
    dest_y_inc = dest->width;
    width = src_width;
    lines_left = src_height;

    src_y_inc -= width;
    dest_y_inc -= width;

    width_4 = width / 4;
    width_remainder = width & 3;

    if (width_4 && !width_remainder) {
        if ((width_4 & 1) == 0) {
            // width is actually an even multiple of 8!
            lowlevel_blit_sprite_8(width_4 / 2, lines_left, pdest, psrc, dest_y_inc, src_y_inc);
        } else {
            // width is a multiple of 4 (no remainder)
            lowlevel_blit_sprite_4(width_4, lines_left, pdest, psrc, dest_y_inc, src_y_inc);
        }

    } else if (width_4 && width_remainder) {
        if ((width_4 & 1) == 0) {
            // width is _mostly_ made up of an even multiple of 8,
            // plus a small remainder
            lowlevel_blit_sprite_8r(width_4 / 2, lines_left, pdest, psrc, width_remainder, dest_y_inc, src_y_inc);
        } else {
            // width is >= 4 and there is a remainder
            lowlevel_blit_sprite_4r(width_4, lines_left, pdest, psrc, width_remainder, dest_y_inc, src_y_inc);
        }

    } else {
        // width is <= 3
        lowlevel_blit_sprite_r(width_remainder, lines_left, pdest, psrc, dest_y_inc, src_y_inc);
    }
}

