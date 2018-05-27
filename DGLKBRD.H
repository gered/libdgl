#ifndef DGL_DGLKYBRD_H_INCLUDED
#define DGL_DGLKYBRD_H_INCLUDED

#include "dglcmn.h"
#include "dglkeys.h"

typedef byte KEY;

#define KEYBRD_FLAGS_SCROLLOCK 0x10
#define KEYBRD_FLAGS_NUMLOCK   0x20
#define KEYBRD_FLAGS_CAPSLOCK  0x40

#define KEYBRD_MOD_EXTENDED    0x1
#define KEYBRD_MOD_SHIFT       0x2
#define KEYBRD_MOD_NUMLOCK     0x4
#define KEYBRD_MOD_CAPSLOCK    0x8

/*
 * Current state of the keyboard.
 */
extern volatile ubyte keys[128];

extern volatile uword key_flags;
extern volatile uword key_mod;

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

