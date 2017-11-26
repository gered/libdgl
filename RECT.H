#ifndef DGL_RECT_H_INCLUDED
#define DGL_RECT_H_INCLUDED

typedef struct {
    int x;
    int y;
    int width;
    int height;
} RECT;

static inline RECT rect(int x, int y, int width, int height);
static inline int rect_right(const RECT *r);
static inline int rect_bottom(const RECT *r);

// --------------------------------------------------------------------------

static inline RECT rect(int x, int y, int width, int height) {
    RECT result;
    result.x = x;
    result.y = y;
    result.width = width;
    result.height = height;
    return result;
}

static inline int rect_right(const RECT *r) {
    if (r->width)
        return r->x + r->width - 1;
    else
        return r->x;
}

static inline int rect_bottom(const RECT *r) {
    if (r->height)
        return r->y + r->height - 1;
    else
        return r->y;
}

#endif

