#ifndef DGL_DGLCLIP_H_INCLUDED
#define DGL_DGLCLIP_H_INCLUDED

#include "dglcmn.h"
#include "dglrect.h"

/*
 * Determines if the given point lies within the clipping region.
 * @param clip_region the clipping region to check against
 * @param x x coordinate of the point
 * @param y y coordinate of the point
 * @return TRUE if the point lies inside the clipping region
 */
boolean is_point_in_bounds(const RECT *clip_region, int x, int y);

/*
 * Clamps the coordinates given to the clipping region, assuming that the
 * region defined by the coordinates lies at least partially within the
 * clipping region.
 * @param clip_region the clipping region to check against and clamp to
 * @param x1 x coordinate of the top-left point of the region to clamp
 * @param y1 y coordinate of the top-left point of the region to clamp
 * @param x2 x coordinate of the bottom-right point of the region to clamp
 * @param y2 y coordinate of the bottom-right point of the region to clamp
 * @return TRUE if the given region was clamped and was at least partially
 *         within the clipping region to begin with. If the region was
 *         totally outside of the clipping region, returns FALSE and the
 *         coordinates will not be modified.
 */
boolean clamp_to_region(const RECT *clip_region,
                        int *x1,
                        int *y1,
                        int *x2,
                        int *y2);

boolean clip_to_region(const RECT *clip_region, RECT *r);

#endif

