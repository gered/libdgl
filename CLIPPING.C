#include "clipping.h"
#include "common.h"
#include "mathext.h"

static inline boolean is_in_bounds(int clip_x,
                                   int clip_y,
                                   int clip_right,
                                   int clip_bottom,
                                   int x1,
                                   int y1,
                                   int x2,
                                   int y2) {
	if (y1 < clip_y && y2 < clip_y)
        return FALSE;
    if (y1 > clip_bottom && y2 > clip_bottom)
        return FALSE;
    if (x1 < clip_x && x2 < clip_x)
        return FALSE;
    if (x1 > clip_right && x2 > clip_right)
        return FALSE;

    return TRUE;
}

boolean is_point_in_bounds(const RECT *clip_region, int x, int y) {
    return (
        x >= clip_region->x &&
        y >= clip_region->y &&
        x <= rect_right(clip_region) &&
        y <= rect_bottom(clip_region)
    );
}

boolean clamp_to_region(const RECT *clip_region,
                        int *x1,
                        int *y1,
                        int *x2,
                        int *y2) {
    int clip_x = clip_region->x;
    int clip_y = clip_region->y;
    int clip_right = rect_right(clip_region);
    int clip_bottom = rect_bottom(clip_region);

    if (!is_in_bounds(clip_x, clip_y, clip_right, clip_bottom, *x1, *y1, *x2, *y2))
        return FALSE;

    // at least partially within bounds

    if (*y1 < clip_y)      *y1 = clip_y;
    if (*y1 > clip_bottom) *y1 = clip_bottom;
    if (*y2 < clip_y)      *y2 = clip_y;
    if (*y2 > clip_bottom) *y2 = clip_bottom;
    if (*x1 < clip_x)      *x1 = clip_x;
    if (*x1 > clip_right)  *x1 = clip_right;
    if (*x2 < clip_x)      *x2 = clip_x;
    if (*x2 > clip_right)  *x2 = clip_right;

    return TRUE;
}

boolean clip_to_region(const RECT *clip_region, RECT *r) {
    int clip_right = rect_right(clip_region);
    int clip_bottom = rect_bottom(clip_region);
    int offset;

    // off the left edge?
    if (r->x < clip_region->x) {
        // completely off the left edge?
        if (rect_right(r) < clip_region->x)
            return FALSE;

        offset = clip_region->x - r->x;
        r->x += offset;
        r->width -= offset;
    }

    // off the right edge?
    if (r->x > (clip_region->width - r->width)) {
        // completely off the right edge?
        if (r->x > clip_right)
            return FALSE;

        offset = r->x + r->width - clip_region->width;
        r->width -= offset;
    }

    // off the top edge?
    if (r->y < clip_region->y) {
        // completely off the top edge?
        if (rect_bottom(r) < clip_region->y)
            return FALSE;

        offset = clip_region->y - r->y;
        r->y += offset;
        r->height -= offset;
    }

    // off the bottom edge?
    if (r->y > (clip_region->height - r->height)) {
        // completely off the bottom edge?
        if (r->y > clip_bottom)
            return FALSE;

        offset = r->y + r->height - clip_region->height;
        r->height -= offset;
    }

    return TRUE;
}


