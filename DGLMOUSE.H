#ifndef DGL_DGLMOUSE_H_INCLUDED
#define DGL_DGLMOUSE_H_INCLUDED

#include "dglcmn.h"

typedef byte MOUSE_BUTTON;

#define MOUSE_LEFTBUTTON   0x01
#define MOUSE_RIGHTBUTTON  0x02
#define MOUSE_CENTERBUTTON 0x04

/*
 * Current mouse cursor X position.
 */
extern volatile int mouse_x;

/*
 * Current mouse cursor Y position.
 */
extern volatile int mouse_y;

/*
 * Current state of mouse buttons.
 */
extern volatile int mouse_buttons;

/*
 * Amount the cursor moved along the X-axis since the last update.
 */
extern volatile int mouse_delta_x;

/*
 * Amount the cursor moved along the Y-axis since the last update.
 */
extern volatile int mouse_delta_y;

/*
 * Installs a custom mouse handler.
 * @return TRUE on success
 */
boolean mouse_init(void);

/*
 * Removes a previously installed mouse handler.
 * @return TRUE on success
 */
boolean mouse_shutdown(void);

/*
 * @return TRUE if the custom mouse handler is installed.
 */
boolean mouse_is_initialized(void);

/*
 * @return TRUE if the user's computer has a (recognized) mouse connected.
 */
boolean mouse_is_present(void);

/*
 * Shows the mouse cursor. If the mouse cursor is currently shown, this does
 * nothing.
 */
void mouse_show(void);

/*
 * Hides the mouse cursor. If the mouse cursor is not currently shown, this
 * does nothing.
 */
void mouse_hide(void);

/*
 * Sets the pixel boundaries for the mouse cursor.
 * @param min_x left coordinate
 * @param max_x right coordinate
 * @param min_y top coordinate
 * @param max_y bottom coordinate
 */
void mouse_set_bounds(int min_x, int min_y, int max_x, int max_y);

/*
 * Returns the current status of the specified button.
 * @param button The button to check the status of.
 * @return TRUE if the button is pressed.
 */
static boolean mouse_button(int button);

// --------------------------------------------------------------------------

static boolean mouse_button(int button) {
    return (mouse_buttons & button) != 0;
}

#endif

