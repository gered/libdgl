#include "dgldraw.h"
#include "dglgfx.h"
#include "dglrect.h"
#include "dglutil.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

static char printf_buffer[1024];

void draw_hline_f(SURFACE *surface, int x1, int x2, int y, uint8 color) {
    uint8 *p = surface_pointer(surface, x1, y);
    mem_fill(p, color, (x2 - x1 + 1));
}

extern void _draw_vline(uint8 *dest, uint8 color, int line_inc, int lines_left);
#pragma aux _draw_vline =          \
    "    test ecx, ecx"            \
    "    jz done"                  \
    "draw:"                        \
    "    mov [edx], al"            \
    "    add edx, ebx"             \
    "    dec ecx"                  \
    "    jnz draw"                 \
    "done:"                        \
    parm [edx] [al] [ebx] [ecx];

void draw_vline_f(SURFACE *surface, int x, int y1, int y2, uint8 color) {
    uint8 *p = surface_pointer(surface, x, y1);
    int line_inc = surface->width;
    int lines_left = y2 - y1 + 1;

    _draw_vline(p, color, line_inc, lines_left);
}

void draw_line(SURFACE *surface, int x1, int y1, int x2, int y2, uint8 color) {
    int delta_x, delta_y;
    int delta_x_abs, delta_y_abs;
    int delta_x_sign, delta_y_sign;
    int x, y;
    uint8 *p;
    int p_x_inc;
    int p_y_inc;
    int i;
    int dx = x1;
    int dy = y1;
    int clip_x1 = surface->clip_region.x;
    int clip_y1 = surface->clip_region.y;
    int clip_x2 = rect_right(&surface->clip_region);
    int clip_y2 = rect_bottom(&surface->clip_region);

    delta_x = x2 - x1;
    delta_y = y2 - y1;
    delta_x_abs = abs(delta_x);
    delta_y_abs = abs(delta_y);
    delta_x_sign = SIGN(delta_x);
    delta_y_sign = SIGN(delta_y);
    x = delta_y_abs / 2;
    y = delta_x_abs / 2;

    p = surface_pointer(surface, x1, y1);
    p_x_inc = delta_x_sign;
    p_y_inc = delta_y_sign * surface->width;

    if (dx >= clip_x1 &&
        dy >= clip_y1 &&
        dx <= clip_x2 &&
        dy <= clip_y2)
        *p = color;

    if (delta_x_abs >= delta_y_abs) {
        for (i = 0; i < delta_x_abs; ++i) {
            y += delta_y_abs;

            if (y >= delta_x_abs) {
                y -= delta_x_abs;
                p += p_y_inc;
                dy += delta_y_sign;
            }

            p += p_x_inc;
            dx += delta_x_sign;

            if (dx >= clip_x1 &&
                dy >= clip_y1 &&
                dx <= clip_x2 &&
                dy <= clip_y2)
                *p = color;
        }
    } else {
        for (i = 0; i < delta_y_abs; ++i) {
            x += delta_x_abs;

            if (x >= delta_y_abs) {
                x -= delta_y_abs;
                p += p_x_inc;
                dx += delta_x_sign;
            }

            p += p_y_inc;
            dy += delta_y_sign;

            if (dx >= clip_x1 &&
                dy >= clip_y1 &&
                dx <= clip_x2 &&
                dy <= clip_y2)
                *p = color;
        }
    }
}

void draw_line_f(SURFACE *surface, int x1, int y1, int x2, int y2, uint8 color) {
    int delta_x, delta_y;
    int delta_x_abs, delta_y_abs;
    int delta_x_sign, delta_y_sign;
    int x, y;
    uint8 *p;
    int p_x_inc;
    int p_y_inc;
    int i;

    delta_x = x2 - x1;
    delta_y = y2 - y1;
    delta_x_abs = abs(delta_x);
    delta_y_abs = abs(delta_y);
    delta_x_sign = SIGN(delta_x);
    delta_y_sign = SIGN(delta_y);
    x = delta_y_abs / 2;
    y = delta_x_abs / 2;

    p = surface_pointer(surface, x1, y1);
    p_x_inc = delta_x_sign;
    p_y_inc = delta_y_sign * surface->width;

    *p = color;

    if (delta_x_abs >= delta_y_abs) {
        for (i = 0; i < delta_x_abs; ++i) {
            y += delta_y_abs;

            if (y >= delta_x_abs) {
                y -= delta_x_abs;
                p += p_y_inc;
            }

            p += p_x_inc;
            *p = color;
        }
    } else {
        for (i = 0; i < delta_y_abs; ++i) {
            x += delta_x_abs;

            if (x >= delta_y_abs) {
                x -= delta_y_abs;
                p += p_x_inc;
            }

            p += p_y_inc;
            *p = color;
        }
    }
}

extern void _draw_both_vert_lines(
    uint8 *left,
    uint8 *right,
    uint8 color,
    int y_inc,
    int height
);
#pragma aux _draw_both_vert_lines =      \
    "   inc ecx"                         \
    "draw:"                              \
    "   mov [edi], al"                   \
    "   mov [esi], al"                   \
    "   add edi, edx"                    \
    "   add esi, edx"                    \
    "   dec ecx"                         \
    "   jnz draw"                        \
    "done:"                              \
    parm [edi] [esi] [al] [edx] [ecx];

extern void _draw_vert_line(
    uint8 *dest,
    uint8 color,
    int y_inc,
    int height
);
#pragma aux _draw_vert_line =      \
    "   inc ecx"                   \
    "draw:"                        \
    "   mov [edi], al"             \
    "   add edi, edx"              \
    "   dec ecx"                   \
    "   jnz draw"                  \
    "done:"                        \
    parm [edi] [al] [edx] [ecx];

void draw_rect(SURFACE *surface, int x1, int y1, int x2, int y2, uint8 color) {
    RECT clipped;
    int clipped_x2, clipped_y2;

    if (x2 < x1)
        SWAP(int, x1, x2);
    if (y2 < y1)
        SWAP(int, y1, y2);

    clipped.x = x1;
    clipped.y = y1;
    clipped.width = x2 - x1 + 1;
    clipped.height = y2 - y1 + 1;

    if (!clip_to_region(&surface->clip_region, &clipped))
        return;

    clipped_x2 = clipped.x + clipped.width - 1;
    clipped_y2 = clipped.y + clipped.height - 1;

    // top line, only if y1 was within bounds
    if (y1 == clipped.y) {
        uint8 *p = surface_pointer(surface, clipped.x, clipped.y);
        mem_fill(p, color, clipped.width);
    }

    // bottom line, only if y2 was within bounds
    if (y2 == clipped_y2) {
        uint8 *p = surface_pointer(surface, clipped.x, clipped_y2);
        mem_fill(p, color, clipped.width);
    }

    // draw both left and right lines if neither x1 nor x2 were clipped
    if (x1 == clipped.x && x2 == clipped_x2) {
        uint8 *p1 = surface_pointer(surface, clipped.x, clipped.y);
        uint8 *p2 = surface_pointer(surface, clipped_x2, clipped.y);
        _draw_both_vert_lines(p1, p2, color, surface->width, clipped.height - 1);

    // draw left line if x1 was not clipped
    } else if (x1 == clipped.x) {
        uint8 *p = surface_pointer(surface, clipped.x, clipped.y);
        _draw_vert_line(p, color, surface->width, clipped.height - 1);

    // draw right line if x2 was not clipped
    } else if (x2 == clipped_x2) {
        uint8 *p = surface_pointer(surface, clipped_x2, clipped.y);
        _draw_vert_line(p, color, surface->width, clipped.height - 1);
    }
}

void draw_rect_f(SURFACE *surface, int x1, int y1, int x2, int y2, uint8 color) {
    uint8 *p;
    uint8 *p1;
    uint8 *p2;
    int width = x2 - x1 + 1;
    int lines_left = y2 - y1;
    int y_inc = surface->width;

    p = surface_pointer(surface, x1, y1);

    p1 = p;
    p2 = p + width - 1;

    lowlevel_rect_f(color, width, y_inc, lines_left, p1, p2);
}

void draw_filled_rect(
    SURFACE *surface,
    int x1,
    int y1,
    int x2,
    int y2,
    uint8 color
) {
    if (!clamp_to_region(&surface->clip_region, &x1, &y1, &x2, &y2))
        return;

    if (x2 < x1)
        SWAP(int, x1, x2);
    if (y2 < y1)
        SWAP(int, y1, y2);

    draw_filled_rect_f(surface, x1, y1, x2, y2, color);
}

void draw_filled_rect_f(
    SURFACE *surface,
    int x1,
    int y1,
    int x2,
    int y2,
    uint8 color
) {
    uint8 *p;
    int width = x2 - x1 + 1;
    int y_inc = surface->width;
    int lines_left = y2 - y1 + 1;

    p = surface_pointer(surface, x1, y1);

    lowlevel_filled_rect_f(color, y_inc, width, lines_left, p);
}

#define CHAR_WIDTH               8
#define CHAR_HEIGHT              8
#define CHAR_LINE_BITMASK(x)     (1 << ((CHAR_WIDTH - 1) - (x)))
#define IS_CHAR_PIXEL(x, line)   ((line) & CHAR_LINE_BITMASK(x))
#define BIOS_FONT_ADDR           0xffa6e

// dest_x, dest_y - original unclipped render x,y coords
// dest_clipped - clipped destination render region
// TODO: rewrite this. this function is kinda sloppy...
static void print_char(
    SURFACE *surface,
    int dest_x,
    int dest_y,
    const RECT *dest_clipped,
    uint8 color,
    char c
) {
    uint8 *p;
    uint8 *rom_char;
    uint8 char_line_bits;
    int cx, cy;
    int offset_x, offset_y;
    int width, height;
    int y_inc = surface->width;

    p = surface_pointer(surface, dest_clipped->x, dest_clipped->y);
    rom_char = ((uint8*)BIOS_FONT_ADDR) + (c * CHAR_HEIGHT);

    // get offset x,y to start rendering char from (will be in range 0-7)
    offset_x = dest_clipped->x - dest_x;
    offset_y = dest_clipped->y - dest_y;
    width = dest_clipped->width + offset_x;
    height = dest_clipped->height + offset_y;

    // cx,cy are always in "char coordinate space" (that is, 0-7)
    for (cy = offset_y; cy < height; ++cy) {
        char_line_bits = rom_char[cy];

        for (cx = offset_x; cx < width; ++cx) {
            if (IS_CHAR_PIXEL(cx, char_line_bits))
                p[cx - offset_x] = color;
        }

        p += y_inc;
    }
}

static void print_text(
    SURFACE *surface,
    int x,
    int y,
    uint8 color,
    bool clip,
    const char *text
) {
    const char *c;
    RECT r;
    RECT draw_r;

    r = rect(x, y, CHAR_WIDTH, CHAR_HEIGHT);

    for (c = text; *c; ++c) {
        switch (*c) {
        case '\n':
            r.x = x;
            r.y += r.height;
            break;
        case ' ':
            r.x += r.width;
            break;
        case '\r':
            break;
        default:
            if (clip) {
                draw_r = r;
                if (clip_to_region(&surface->clip_region, &draw_r))
                    print_char(surface, r.x, r.y, &draw_r, color, *c);
            } else {
                print_char(surface, r.x, r.y, &r, color, *c);
            }
            r.x += r.width;
        }
    }
}

void draw_text(SURFACE *surface, int x, int y, uint8 color, const char *text) {
    print_text(surface, x, y, color, true, text);
}

void draw_text_f(SURFACE *surface, int x, int y, uint8 color, const char *text) {
    print_text(surface, x, y, color, false, text);
}

void draw_printf(
    SURFACE *surface,
    int x,
    int y,
    uint8 color,
    const char *format,
    ...
) {
    va_list args;
    va_start(args, format);
    vsprintf(printf_buffer, format, args);
    va_end(args);

    print_text(surface, x, y, color, true, printf_buffer);
}

void draw_printf_f(
    SURFACE *surface,
    int x,
    int y,
    uint8 color,
    const char *format,
    ...
) {
    va_list args;
    va_start(args, format);
    vsprintf(printf_buffer, format, args);
    va_end(args);

    print_text(surface, x, y, color, false, printf_buffer);
}

