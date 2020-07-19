#ifndef LIBDGL_DGLKYBRD_H
#define LIBDGL_DGLKYBRD_H

#include "dglcmn.h"
#include "dglkeys.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8 KEY;

#define KEYBRD_FLAGS_SCROLLOCK 0x10
#define KEYBRD_FLAGS_NUMLOCK   0x20
#define KEYBRD_FLAGS_CAPSLOCK  0x40

#define KEYBRD_MOD_EXTENDED    0x1
#define KEYBRD_MOD_SHIFT       0x2
#define KEYBRD_MOD_NUMLOCK     0x4
#define KEYBRD_MOD_CAPSLOCK    0x8

extern volatile uint8 keys[128];

extern volatile uint16 key_flags;
extern volatile uint16 key_mod;

bool keyboard_init(void);
bool keyboard_shutdown(void);
bool keyboard_is_initialized(void);

KEY keyboard_read_key(void);
void keyboard_wait_for_key(KEY key);

char key_to_char(KEY key, uint16 modifiers);

#ifdef __cplusplus
}
#endif

#endif

