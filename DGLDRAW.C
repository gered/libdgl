#include "dgldraw.h"
#include "dglgfx.h"
#include "dglmath.h"
#include "dglutil.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

static char printf_buffer[1024];

void surface_hline_f(SURFACE *surface, int x1, int x2, int y, int color) {
    byte *p = surface_pointer(surface, x1, y);
    mem_fill(p, color, (x2 - x1 + 1));
}

void surface_vline_f(SURFACE *surface, int x, int y1, int y2, int color) {
    byte *p = surface_pointer(surface, x, y1);
    int line_inc = surface->width;
    int lines_left = y2 - y1 + 1;

    extern void draw_line(byte *dest, int color, int line_inc, int lines_left);
    #pragma aux draw_line =          \
        "    test ecx, ecx"          \
        "    jz done"                \
        "draw:"                      \
        "    mov [edx], al"          \
        "    add edx, ebx"           \
        "    dec ecx"                \
        "    jnz draw"               \
        "done:"                      \
        parm [edx] [eax] [ebx] [ecx];
    draw_line(p, color, line_inc, lines_left);
}

void surface_line(SURFACE *surface, int x1, int y1, int x2, int y2, int color) {
    int delta_x, delta_y;
    int delta_x_abs, delta_y_abs;
    int delta_x_sign, delta_y_sign;
    int x, y;
    byte *p;
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
        *p = (byte)color;

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
                *p = (byte)color;
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
                *p = (byte)color;
        }
    }
}

void surface_line_f(SURFACE *surface, int x1, int y1, int x2, int y2, int color) {
    int delta_x, delta_y;
    int delta_x_abs, delta_y_abs;
    int delta_x_sign, delta_y_sign;
    int x, y;
    byte *p;
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

    *p = (byte)color;

    if (delta_x_abs >= delta_y_abs) {
        for (i = 0; i < delta_x_abs; ++i) {
            y += delta_y_abs;

            if (y >= delta_x_abs) {
                y -= delta_x_abs;
                p += p_y_inc;
            }

            p += p_x_inc;
            *p = (byte)color;
        }
    } else {
        for (i = 0; i < delta_y_abs; ++i) {
            x += delta_x_abs;

            if (x >= delta_y_abs) {
                x -= delta_y_abs;
                p += p_x_inc;
            }

            p += p_y_inc;
            *p = (byte)color;
        }
    }
}

extern void draw_both_vert_lines(byte *left, byte *right, int color, int y_inc, int height);
#pragma aux draw_both_vert_lines =      \
    "   inc ecx"                        \
    "draw:"                             \
    "   mov [edi], al"                  \
    "   mov [esi], al"                  \
    "   add edi, edx"                   \
    "   add esi, edx"                   \
    "   dec ecx"                        \
    "   jnz draw"                       \
    "done:"                             \
    parm [edi] [esi] [eax] [edx] [ecx];

extern void draw_vert_line(byte *dest, int color, int y_inc, int height);
#pragma aux draw_vert_line =      \
    "   inc ecx"                  \
    "draw:"                       \
    "   mov [edi], al"            \
    "   add edi, edx"             \
    "   dec ecx"                  \
    "   jnz draw"                 \
    "done:"                       \
    parm [edi] [eax] [edx] [ecx];

void surface_rect(SURFACE *surface, int x1, int y1, int x2, int y2, int color) {
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
    color = fill32(color);

    // top line, only if y1 was within bounds
    if (y1 == clipped.y) {
        byte *p = surface_pointer(surface, clipped.x, clipped.y);
        mem_fill32(p, color, clipped.width);
    }

    // bottom line, only if y2 was within bounds
    if (y2 == clipped_y2) {
        byte *p = surface_pointer(surface, clipped.x, clipped_y2);
        mem_fill32(p, color, clipped.width);
    }

    // draw both left and right lines if neither x1 nor x2 were clipped
    if (x1 == clipped.x && x2 == clipped_x2) {
        byte *p1 = surface_pointer(surface, clipped.x, clipped.y);
        byte *p2 = surface_pointer(surface, clipped_x2, clipped.y);
        draw_both_vert_lines(p1, p2, color, surface->width, clipped.height - 1);

    // draw left line if x1 was not clipped
    } else if (x1 == clipped.x) {
        byte *p = surface_pointer(surface, clipped.x, clipped.y);
        draw_vert_line(p, color, surface->width, clipped.height - 1);

    // draw right line if x2 was not clipped
    } else if (x2 == clipped_x2) {
        byte *p = surface_pointer(surface, clipped_x2, clipped.y);
        draw_vert_line(p, color, surface->width, clipped.height - 1);
    }
}

void surface_rect_f(SURFACE *surface, int x1, int y1, int x2, int y2, int color) {
    byte *p;
    byte *p1;
    byte *p2;
    int width = x2 - x1 + 1;
    int lines_left = y2 - y1;
    int y_inc = surface->width;

    p = surface_pointer(surface, x1, y1);

    p1 = p;
    p2 = p + width - 1;

    direct_rect_f(color, width, y_inc, lines_left, p1, p2);
}

void surface_filled_rect(SURFACE *surface,
                         int x1,
                         int y1,
                         int x2,
                         int y2,
                         int color) {
    if (!clamp_to_region(&surface->clip_region, &x1, &y1, &x2, &y2))
        return;

    if (x2 < x1)
        SWAP(int, x1, x2);
    if (y2 < y1)
        SWAP(int, y1, y2);

    surface_filled_rect_f(surface, x1, y1, x2, y2, color);
}

void surface_filled_rect_f(SURFACE *surface,
                           int x1,
                           int y1,
                           int x2,
                           int y2,
                           int color) {
    byte *p;
    int width = x2 - x1 + 1;
    int y_inc = surface->width;
    int lines_left = y2 - y1 + 1;

    p = surface_pointer(surface, x1, y1);

    direct_filled_rect_f(color, y_inc, width, lines_left, p);
}

#define CHAR_WIDTH  8
#define CHAR_HEIGHT 8
#define CHAR_LINE_BITMASK(x)   (1 << ((CHAR_WIDTH - 1) - (x)))
#define IS_CHAR_PIXEL(x, line) ((line) & CHAR_LINE_BITMASK(x))

// dest_x, dest_y - original unclipped render x,y coords
// dest_clipped - clipped destination render region
static void print_char(SURFACE *surface,
                       int dest_x,
                       int dest_y,
                       const RECT *dest_clipped,
                       int color,
                       char c) {
    byte *p;
    byte *rom_char;
    char char_line_bits;
    int cx, cy;
    int offset_x, offset_y;
    int width, height;
    int y_inc = surface->width;

    p = surface_pointer(surface, dest_clipped->x, dest_clipped->y);
    rom_char = ((byte*)0xffa6e) + (c * CHAR_HEIGHT);

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
                p[cx - offset_x] = (byte)color;
        }

        p += y_inc;
    }
}

static void print_text(SURFACE *surface,
                       int x,
                       int y,
                       int color,
                       boolean clip,
                       const char *text) {
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

void surface_text(SURFACE *surface, int x, int y, int color, const char *text) {
    print_text(surface, x, y, color, TRUE, text);
}

void surface_text_f(SURFACE *surface, int x, int y, int color, const char *text) {
    print_text(surface, x, y, color, FALSE, text);
}

void surface_printf(SURFACE *surface,
                    int x,
                    int y,
                    int color,
                    const char *format, ...) {
    va_list args;
    va_start(args, format);
    vsprintf(printf_buffer, format, args);
    va_end(args);

    print_text(surface, x, y, color, TRUE, printf_buffer);
}

void surface_printf_f(SURFACE *surface,
                      int x,
                      int y,
                      int color,
                      const char *format, ...) {
    va_list args;
    va_start(args, format);
    vsprintf(printf_buffer, format, args);
    va_end(args);

    print_text(surface, x, y, color, FALSE, printf_buffer);
}

