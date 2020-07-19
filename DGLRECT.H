#ifndef LIBDGL_DGLRECT_H
#define LIBDGL_DGLRECT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int x;
    int y;
    int width;
    int height;
} RECT;

static RECT rect(int x, int y, int width, int height);
static int rect_right(const RECT *r);
static int rect_bottom(const RECT *r);

// --------------------------------------------------------------------------

static RECT rect(int x, int y, int width, int height) {
    RECT result;
    result.x = x;
    result.y = y;
    result.width = width;
    result.height = height;
    return result;
}

static int rect_right(const RECT *r) {
    if (r->width)
        return r->x + r->width - 1;
    else
        return r->x;
}

static int rect_bottom(const RECT *r) {
    if (r->height)
        return r->y + r->height - 1;
    else
        return r->y;
}

#ifdef __cplusplus
}
#endif

#endif

