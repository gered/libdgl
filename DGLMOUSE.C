#include "dglmouse.h"
#include "dgl.h"
#include "dglevent.h"
#include <string.h>
#include <dos.h>

static bool _installed = false;
static bool _has_mouse = false;

static INPUTEVENT *mouse_event;

volatile int mouse_x;
volatile int mouse_y;
volatile int mouse_buttons;
volatile int mouse_delta_x;
volatile int mouse_delta_y;
volatile int mouse_prev_buttons;

static void reset_mouse_state(void) {
    mouse_x = 0;
    mouse_y = 0;
    mouse_buttons = 0;
    mouse_delta_x = 0;
    mouse_delta_y = 0;
    mouse_prev_buttons = 0;
}

static bool init_mouse_driver(void) {
    union REGS regs;

    memset(&regs, 0, sizeof(regs));
    regs.w.ax = 0x00;
    int386(0x33, &regs, &regs);

    return (regs.w.ax != 0);
}

static void update_mouse_state(void) {
    union REGS regs;

    memset(&regs, 0, sizeof(regs));
    regs.w.ax = 0x03;
    int386(0x33, &regs, &regs);
    mouse_x = (regs.w.cx / 2);
    mouse_y = regs.w.dx;
    mouse_prev_buttons = mouse_buttons;
    mouse_buttons = regs.w.bx;
    mouse_delta_x = 0;
    mouse_delta_y = 0;
}

static void push_motion_event(void) {
    _events_push(&mouse_event);
    mouse_event->type = EVENT_TYPE_MOUSE_MOTION;
    mouse_event->mouse_motion.x = mouse_x;
    mouse_event->mouse_motion.y = mouse_y;
    mouse_event->mouse_motion.x_delta = mouse_delta_x;
    mouse_event->mouse_motion.y_delta = mouse_delta_y;
    mouse_event->mouse_motion.buttons = mouse_buttons;
}

static void push_button_event(EVENT_ACTION action, MOUSE_BUTTON button) {
    _events_push(&mouse_event);
    mouse_event->type = EVENT_TYPE_MOUSE_BUTTON;
    mouse_event->mouse_button.x = mouse_x;
    mouse_event->mouse_button.y = mouse_y;
    mouse_event->mouse_button.action = action;
    mouse_event->mouse_button.button = button;
}

#pragma off (check_stack)
void __loadds far mouse_int_handler(int eax, int ebx, int ecx, int edx) {
    #pragma aux mouse_int_handler parm [eax] [ebx] [ecx] [edx]
    mouse_delta_x = (ecx / 2) - mouse_x;
    mouse_delta_y = edx - mouse_y;
    mouse_x = (ecx / 2);
    mouse_y = edx;
    mouse_prev_buttons = mouse_buttons;
    mouse_buttons = ebx;

    if (_events_enabled) {
        if (mouse_delta_x || mouse_delta_y) {
            push_motion_event();
        }

        if (mouse_buttons != mouse_prev_buttons) {
            if ((mouse_buttons & MOUSE_LEFTBUTTON) !=
                (mouse_prev_buttons & MOUSE_LEFTBUTTON)) {
                if (mouse_buttons & MOUSE_LEFTBUTTON)
                    push_button_event(EVENT_ACTION_PRESSED, MOUSE_LEFTBUTTON);
                else
                    push_button_event(EVENT_ACTION_RELEASED, MOUSE_LEFTBUTTON);
            }

            if ((mouse_buttons & MOUSE_RIGHTBUTTON) !=
                (mouse_prev_buttons & MOUSE_RIGHTBUTTON)) {
                if (mouse_buttons & MOUSE_RIGHTBUTTON)
                    push_button_event(EVENT_ACTION_PRESSED, MOUSE_RIGHTBUTTON);
                else
                    push_button_event(EVENT_ACTION_RELEASED, MOUSE_RIGHTBUTTON);
            }

            if ((mouse_buttons & MOUSE_CENTERBUTTON) !=
                (mouse_prev_buttons & MOUSE_CENTERBUTTON)) {
                if (mouse_buttons & MOUSE_CENTERBUTTON)
                    push_button_event(EVENT_ACTION_PRESSED, MOUSE_CENTERBUTTON);
                else
                    push_button_event(EVENT_ACTION_RELEASED, MOUSE_CENTERBUTTON);
            }
        }
    }
}
#pragma on (check_stack)

bool mouse_init(void) {
    union REGS regs;
    struct SREGS sregs;

    if (_installed) {
        dgl_set_error(DGL_MOUSE_ALREADY_INITIALIZED);
        return false;
    }

    reset_mouse_state();

    _has_mouse = init_mouse_driver();
    if (!_has_mouse) {
        _installed = true;
        return true;
    }

    update_mouse_state();

    memset(&regs, 0, sizeof(regs));
    memset(&sregs, 0, sizeof(sregs));
    regs.w.ax = 0x0c;
    regs.w.cx = 31;
    regs.x.edx = FP_OFF(mouse_int_handler);
    sregs.es = FP_SEG(mouse_int_handler);
    int386x(0x33, &regs, &regs, &sregs);

    _installed = true;
    return true;
}

bool mouse_shutdown(void) {
    union REGS regs;

    if (!_installed)
        return true;   // don't care

    if (!_has_mouse) {
        _installed = false;
        return true;
    }

    memset(&regs, 0, sizeof(regs));
    regs.w.ax = 0x0c;
    regs.w.cx = 0;
    int386(0x33, &regs, &regs);

    reset_mouse_state();
    init_mouse_driver();

    _installed = false;
    return true;
}

bool mouse_is_initialized(void) {
    return _installed;
}

bool mouse_is_present(void) {
    return _has_mouse;
}

void mouse_show(void) {
    union REGS regs;

    if (!_has_mouse)
        return;

    memset(&regs, 0, sizeof(regs));
    regs.w.ax = 0x01;
    int386(0x33, &regs, &regs);
}

void mouse_hide(void) {
    union REGS regs;

    if (!_has_mouse)
        return;

    memset(&regs, 0, sizeof(regs));
    regs.w.ax = 0x02;
    int386(0x33, &regs, &regs);
}

void mouse_set_bounds(int min_x, int min_y, int max_x, int max_y) {
    union REGS regs;

    if (!_has_mouse)
        return;

    memset(&regs, 0, sizeof(regs));

    regs.w.ax = 0x07;
    regs.w.cx = min_x;
    regs.w.dx = max_x;
    int386(0x33, &regs, &regs);

    regs.w.ax = 0x08;
    regs.w.cx = min_y;
    regs.w.dx = max_y;
    int386(0x33, &regs, &regs);
}

