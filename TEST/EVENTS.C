#include "events.h"
#include "dgl.h"
#include <stdio.h>
#include "helpers.h"

void test_events(void) {
    boolean result;
    INPUTEVENT *event;

    clrscr(0);
    delay(500);

    result = keyboard_init();
    ASSERT(result == TRUE);
    result = mouse_init();
    ASSERT(result == TRUE);

    ASSERT(events_is_initialized() == FALSE);

    result = events_init();
    ASSERT(result == TRUE);
    ASSERT(events_is_initialized() == TRUE);

    printf("Displaying events:\n\n");

    while (!keys[1]) {
        if (!events_poll(&event))
            continue;

        switch (event->type) {
            case EVENT_TYPE_KEYBOARD:
                printf("KEYBOARD: %2d - %d\n",
                       event->keyboard.key,
                       event->keyboard.action);
                break;

            case EVENT_TYPE_MOUSE_MOTION:
                printf("MOUSE MOTION: %3d, %3d (%3d, %3d), %d\n",
                       event->mouse_motion.x,
                       event->mouse_motion.y,
                       event->mouse_motion.x_delta,
                       event->mouse_motion.y_delta,
                       event->mouse_motion.buttons);
                break;

            case EVENT_TYPE_MOUSE_BUTTON:
                printf("MOUSE BUTTON: %3d, %3d, %d - %d\n",
                       event->mouse_button.x,
                       event->mouse_button.y,
                       event->mouse_button.button,
                       event->mouse_button.action);
                break;

            default:
                printf("** UNKNOWN: %d **\n", event->type);
                break;
        }
    }

    result = events_shutdown();
    ASSERT(result == TRUE);
    ASSERT(events_is_initialized() == FALSE);

    result = mouse_shutdown();
    ASSERT(result == TRUE);
    result = keyboard_shutdown();
    ASSERT(result == TRUE);

    printf("\nPress a key to continue...\n");

    getch();
}

