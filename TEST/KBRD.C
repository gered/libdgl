#include "kbrd.h"
#include "dglkbrd.h"
#include <dos.h>
#include <stdio.h>
#include "helpers.h"

void display_key_states(void) {
    int x, y;

    gotoxy(0, 2);
    for (y = 0; y < 8; ++y) {
        for (x = 0; x < 16; ++x) {
            unsigned char pressed;
            pressed = keys[y * 16 + x] > 0;
            printf("%d", pressed);
        }
        printf("\n");
    }
}

void test_keyboard(void) {
    bool result;
    KEY k;

    clrscr(0);

    ASSERT(keyboard_is_initialized() == false);

    result = keyboard_init();
    ASSERT(result == true);
    ASSERT(keyboard_is_initialized() == true);

    printf("Keyboard state\n");
    display_key_states();

    while (!keys[1]) {
        display_key_states();
    }

    // delay to give enough time for key to be released before next ...
    delay(500);

    gotoxy(0, 18);
    printf("Press any key ...\n");

    k = keyboard_read_key();
    printf("Pressed key %d\n", k);

    // same thing, giving enough time for key to be released ...
    delay(500);
    printf("Press that same key again ...\n");
    keyboard_wait_for_key(k);

    result = keyboard_shutdown();
    ASSERT(result == true);
    ASSERT(keyboard_is_initialized() == false);

    gotoxy(0, 23);
    printf("Press a key to continue...\n");

    getch();
}

