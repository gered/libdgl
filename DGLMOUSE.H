#ifndef LIBDGL_DGLMOUSE_H
#define LIBDGL_DGLMOUSE_H

#include "dglcmn.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8 MOUSE_BUTTON;

#define MOUSE_LEFTBUTTON   0x01
#define MOUSE_RIGHTBUTTON  0x02
#define MOUSE_CENTERBUTTON 0x04

extern volatile int mouse_x;
extern volatile int mouse_y;
extern volatile int mouse_buttons;
extern volatile int mouse_delta_x;
extern volatile int mouse_delta_y;

bool mouse_init(void);
bool mouse_shutdown(void);
bool mouse_is_initialized(void);
bool mouse_is_present(void);
void mouse_show(void);
void mouse_hide(void);
void mouse_set_bounds(int min_x, int min_y, int max_x, int max_y);

static bool mouse_button(int button);

// --------------------------------------------------------------------------

static bool mouse_button(int button) {
    return (mouse_buttons & button) != 0;
}

#ifdef __cplusplus
}
#endif

#endif

