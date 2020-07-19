#include "blit.h"
#include "dglblit.h"
#include "dgldraw.h"
#include <stdio.h>
#include "helpers.h"

SURFACE* create_sprite(int width, int height) {
    int x_third, y_third;
    SURFACE *sprite = surface_create(width, height);
    ASSERT(sprite != NULL);
    ASSERT(sprite->width == width);
    ASSERT(sprite->height == height);

    x_third = width / 3;
    y_third = height / 3;

    draw_filled_rect(sprite, 0, 0, x_third, y_third, 1);
    draw_filled_rect(sprite, x_third * 2, y_third * 2, width - 1, height - 1, 2);
    draw_filled_rect(sprite, 0, (y_third * 2), x_third, height - 1, 3);
    draw_filled_rect(sprite, x_third * 2, 0, width - 1, y_third, 4);
    draw_filled_rect(sprite, x_third, y_third, x_third * 2, y_third * 2, 5);
    draw_rect(sprite, 0, 0, width - 1, height - 1, 6);

    return sprite;
}

SURFACE* create_image(int width, int height) {
    int x, y;
    SURFACE *image = surface_create(width, height);
    ASSERT(image != NULL);
    ASSERT(image->width == width);
    ASSERT(image->height == height);

    draw_filled_rect(image, 0, 0, width / 2, height / 2, 1);
    draw_filled_rect(image, width / 2, 0, width - 1, height / 2, 2);
    draw_filled_rect(image, 0, height / 2, width / 2, height - 1, 3);
    draw_filled_rect(image, width / 2, height / 2, width - 1, height - 1, 4);
    draw_rect(image, 0, 0, width - 1, height - 1, 5);

    return image;
}

// blits a 5-color image in a few different sizes. first two test full
// image blits and partial image blits. remaining 4 blits test the shorthand
// blit function that simply blits the entire source image.
// a duplicate set of blits is done using the "fast" variants.
// the image sizes are chosen to ensure each different internal blitter is run.
void test_blit(void) {
    int x, y;

    SURFACE *bmp16 = create_image(16, 16);
    SURFACE *bmp12 = create_image(12, 12);
    SURFACE *bmp21 = create_image(21, 21);
    SURFACE *bmp3 = create_image(3, 3);

    surface_clear(screen, 0);

    x = 0; y = 0;

    blit_region(bmp16, screen, 0, 0, 16, 16, x + 16, y + 16);
    blit_region(bmp16, screen, 8, 8, 8, 8, x + 48, y + 16);

    blit(bmp16, screen, x + 16, y + 48);
    blit(bmp12, screen, x + 48, y + 48);
    blit(bmp21, screen, x + 80, y + 48);
    blit(bmp3, screen, x + 112, y + 48);

    x = 160; y = 0;

    blit_region_f(bmp16, screen, 0, 0, 16, 16, x + 16, y + 16);
    blit_region_f(bmp16, screen, 8, 8, 8, 8, x + 48, y + 16);

    blit_f(bmp16, screen, x + 16, y + 48);
    blit_f(bmp12, screen, x + 48, y + 48);
    blit_f(bmp21, screen, x + 80, y + 48);
    blit_f(bmp3, screen, x + 112, y + 48);

    surface_free(bmp16);
    surface_free(bmp12);
    surface_free(bmp21);
    surface_free(bmp3);
    getch();
}

// performs 7 blits along each edge of the screen, clipping the image by
// varying amounts. the 7th blit on each edge is completely out of bounds and
// should not be visible at all.
void test_blit_clipping(void) {
    SURFACE *bmp = create_image(16, 16);

    surface_clear(screen, 0);

    blit(bmp, screen, -3, 16);
    blit(bmp, screen, -4, 36);
    blit(bmp, screen, -8, 56);
    blit(bmp, screen, -12, 76);
    blit(bmp, screen, -13, 96);
    blit(bmp, screen, -14, 116);
    blit(bmp, screen, -16, 136);

    blit(bmp, screen, 16, -3);
    blit(bmp, screen, 36, -4);
    blit(bmp, screen, 56, -8);
    blit(bmp, screen, 76, -12);
    blit(bmp, screen, 96, -13);
    blit(bmp, screen, 116, -14);
    blit(bmp, screen, 136, -16);

    blit(bmp, screen, 307, 16);
    blit(bmp, screen, 308, 36);
    blit(bmp, screen, 312, 56);
    blit(bmp, screen, 316, 76);
    blit(bmp, screen, 317, 96);
    blit(bmp, screen, 318, 116);
    blit(bmp, screen, 320, 136);

    blit(bmp, screen, 16, 187);
    blit(bmp, screen, 36, 188);
    blit(bmp, screen, 56, 192);
    blit(bmp, screen, 76, 196);
    blit(bmp, screen, 96, 197);
    blit(bmp, screen, 116, 198);
    blit(bmp, screen, 136, 200);

    surface_free(bmp);
    getch();
}

// basically the exact same test as test_blit(), but uses sprite blitting
// instead and uses a grey background. no part of the screen buffer should be
// black as that colour is not used anywhere in the test sprite.
void test_sprite(void) {
    int x, y;
    SURFACE *bmp16 = create_sprite(16, 16);
    SURFACE *bmp12 = create_sprite(12, 12);
    SURFACE *bmp21 = create_sprite(21, 21);
    SURFACE *bmp3 = create_sprite(3, 3);

    surface_clear(screen, 8);

    x = 0; y = 0;

    blit_sprite_region(bmp16, screen, 0, 0, 16, 16, x + 16, y + 16);
    blit_sprite_region(bmp16, screen, 8, 8, 8, 8, x + 48, y + 16);

    blit_sprite(bmp16, screen, x + 16, y + 48);
    blit_sprite(bmp12, screen, x + 48, y + 48);
    blit_sprite(bmp21, screen, x + 80, y + 48);
    blit_sprite(bmp3, screen, x + 112, y + 48);

    x = 160; y = 0;

    blit_sprite_region_f(bmp16, screen, 0, 0, 16, 16, x + 16, y + 16);
    blit_sprite_region_f(bmp16, screen, 8, 8, 8, 8, x + 48, y + 16);

    blit_sprite_f(bmp16, screen, x + 16, y + 48);
    blit_sprite_f(bmp12, screen, x + 48, y + 48);
    blit_sprite_f(bmp21, screen, x + 80, y + 48);
    blit_sprite_f(bmp3, screen, x + 112, y + 48);

    surface_free(bmp16);
    surface_free(bmp12);
    surface_free(bmp21);
    surface_free(bmp3);
    getch();
}

// basically the exact same test as test_blit_clipping(), but uses sprite
// blitting instead and uses a grey background. no part of the screen buffer
// should be black as that colour is not used anywhere in the test sprite.
void test_sprite_clipping(void) {
    SURFACE *bmp = create_sprite(16, 16);

    surface_clear(screen, 8);

    blit_sprite(bmp, screen, -3, 16);
    blit_sprite(bmp, screen, -4, 36);
    blit_sprite(bmp, screen, -8, 56);
    blit_sprite(bmp, screen, -12, 76);
    blit_sprite(bmp, screen, -13, 96);
    blit_sprite(bmp, screen, -14, 116);
    blit_sprite(bmp, screen, -16, 136);

    blit_sprite(bmp, screen, 16, -3);
    blit_sprite(bmp, screen, 36, -4);
    blit_sprite(bmp, screen, 56, -8);
    blit_sprite(bmp, screen, 76, -12);
    blit_sprite(bmp, screen, 96, -13);
    blit_sprite(bmp, screen, 116, -14);
    blit_sprite(bmp, screen, 136, -16);

    blit_sprite(bmp, screen, 307, 16);
    blit_sprite(bmp, screen, 308, 36);
    blit_sprite(bmp, screen, 312, 56);
    blit_sprite(bmp, screen, 316, 76);
    blit_sprite(bmp, screen, 317, 96);
    blit_sprite(bmp, screen, 318, 116);
    blit_sprite(bmp, screen, 320, 136);

    blit_sprite(bmp, screen, 16, 187);
    blit_sprite(bmp, screen, 36, 188);
    blit_sprite(bmp, screen, 56, 192);
    blit_sprite(bmp, screen, 76, 196);
    blit_sprite(bmp, screen, 96, 197);
    blit_sprite(bmp, screen, 116, 198);
    blit_sprite(bmp, screen, 136, 200);

    surface_free(bmp);
    getch();
}

