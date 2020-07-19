#ifndef LIBDGL_DGLCLIP_H
#define LIBDGL_DGLCLIP_H

#include "dglcmn.h"
#include "dglrect.h"

#ifdef __cplusplus
extern "C" {
#endif

bool is_in_bounds(
    int clip_x,
    int clip_y,
    int clip_right,
    int clip_bottom,
    int x1,
    int y1,
    int x2,
    int y2
);

bool is_rect_in_bounds(const RECT *clip_region, const RECT *r);
bool is_point_in_bounds(const RECT *clip_region, int x, int y);
bool clamp_to_region(const RECT *clip_region, int *x1, int *y1, int *x2, int *y2);
bool clip_to_region(const RECT *clip_region, RECT *r);

#ifdef __cplusplus
}
#endif

#endif

