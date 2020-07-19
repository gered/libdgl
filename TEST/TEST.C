#include <stdio.h>
#include <signal.h>
#include "dgl.h"
#include "dglgfx.h"
#include "helpers.h"

#include "blit.h"
#include "events.h"
#include "fixed.h"
#include "kbrd.h"
#include "line.h"
#include "mouse.h"
#include "pal.h"
#include "pcx.h"
#include "pset.h"
#include "rect.h"
#include "surface.h"
#include "text.h"
#include "vector2.h"

SURFACE *backbuffer = NULL;

void break_handler(int sig) {
    exit(0);
}

int main(void) {
    signal(SIGINT, break_handler);

    dgl_init();

    ASSERT(screen == NULL);

    if (!gfx_init()) {
        printf("Error initializing video: %s\n", dgl_last_error_message());
        return 1;
    }

    ASSERT(screen != NULL);
    ASSERT(screen->width == 320 && screen->height == 200);

    backbuffer = surface_create(320, 200);
    ASSERT(backbuffer != NULL);
    ASSERT(backbuffer->width == 320 && backbuffer->height == 200);

    test_keyboard();
    test_mouse();
    test_events();
    test_fixed();
    test_vec2i();
    test_vec2();
    test_surface_clear();
    test_surface_copy();
    test_pixels_1();
    test_pixels_2();
    test_pixels_clipping();
    test_hline();
    test_hline_clipping();
    test_vline();
    test_vline_clipping();
    test_line();
    test_line_clipping();
    test_rect();
    test_rect_clipping();
    test_filled_rect();
    test_filled_rect_clipping();
    test_blit();
    test_blit_clipping();
    test_sprite();
    test_sprite_clipping();
    test_text();
    test_text_clipping();
    test_palette();
    test_palette_fading();
    test_pcx();

    surface_free(backbuffer);

    return 0;
}

