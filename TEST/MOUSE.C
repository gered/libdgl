#include "mouse.h"
#include "dglmouse.h"
#include <dos.h>
#include <stdio.h>
#include "helpers.h"

void test_mouse(void) {
    bool result;

    clrscr(0);

    ASSERT(mouse_is_initialized() == false);

    result = mouse_init();
    ASSERT(result == true);
    ASSERT(mouse_is_initialized() == true);

    // this may seem a little weird to check for a mouse AFTER init, but
    // it's the init call that determines if a mouse is present :)
    if (mouse_is_present()) {
        printf("Mouse detected. Showing mouse state.\n");

        mouse_show();

        while (!kbhit()) {
            gotoxy(0, 2);
            printf("X: %03d\n", mouse_x);
            printf("Y: %03d\n", mouse_y);
            printf("Buttons: %03d\n", mouse_buttons);
        }

        mouse_hide();

    } else {
        printf("Mouse not detected. Skipping.\n");
    }

    // need to close down the mouse subsystem even if one is not present
    result = mouse_shutdown();
    ASSERT(result == true);
    ASSERT(mouse_is_initialized() == false);

    gotoxy(0, 23);
    printf("Press a key to continue...\n");

    getch();
}

