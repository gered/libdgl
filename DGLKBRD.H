#ifndef DGL_DGLKYBRD_H_INCLUDED
#define DGL_DGLKYBRD_H_INCLUDED

#include "dglcmn.h"
#include "dglkeys.h"

typedef byte KEY;

/*
 * Current state of the keyboard.
 */
extern volatile ubyte keys[128];

/*
 * Installs a custom keyboard interrupt handler.
 * @return TRUE on success
 */
boolean keyboard_init(void);

/*
 * Removes a previously installed keyboard interrupt handler.
 * @return TRUE on success
 */
boolean keyboard_shutdown(void);

/*
 * @return TRUE if the custom keyboard interrupt handler is installed.
 */
boolean keyboard_is_initialized(void);

/*
 * Waits indefinitely until any key is pressed.
 * @return The key that was pressed.
 */
KEY keyboard_read_key(void);

/*
 * Waits indefinitely until the specified key is pressed.
 */
void keyboard_wait_for_key(KEY key);

#endif
