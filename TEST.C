#include <stdio.h>
#include <time.h>
#include "dgl.h"

int main(void) {
    SURFACE *backbuffer;

    if (!dgl_init()) {
        printf("Failed to initialize DGL: %s\n", dgl_last_error_message());
        return 1;
    }

    backbuffer = surface_create(320, 200);


    while (!keys[KEY_ESC]) {
        surface_clear(backbuffer, 0);

        surface_text(backbuffer, 100, 100, 15, "Hello, world!");


        video_wait_vsync();
        surface_copy(backbuffer, screen);
    }

    surface_free(backbuffer);

    if (!dgl_shutdown()) {
        printf("Failed to close DGL: %s\n", dgl_last_error_message());
        return 1;
    }

    return 0;
}

