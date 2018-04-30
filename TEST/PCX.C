#include "pcx.h"
#include "dgl.h"
#include <stdio.h>
#include "helpers.h"

void test_pcx(void) {
    SURFACE *pcx;
    DGL_ERROR err;
    byte pcx_palette[768];
    byte original_palette[768];

    surface_clear(screen, 0);
    pal_get(original_palette);

    pcx = pcx_load("notreal.pcx", NULL);
    err = dgl_last_error();
    ASSERT(pcx == NULL);
    ASSERT(err != DGL_NONE);

    pcx = pcx_load("test.pcx", NULL);
    ASSERT(pcx != NULL);

    surface_blit(pcx, screen, 0, 0);
    getch();

    surface_clear(screen, 0);
    surface_free(pcx);
    pcx = pcx_load("test.pcx", pcx_palette);
    ASSERT(pcx != NULL);

    pal_set(pcx_palette);
    surface_blit(pcx, screen, 0, 0);

    getch();
    pal_set(original_palette);
}

