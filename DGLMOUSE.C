#include "dglmouse.h"
#include "dglerror.h"
#include <string.h>
#include <dos.h>

static boolean _installed = FALSE;
static boolean _has_mouse = FALSE;

volatile int mouse_x;
volatile int mouse_y;
volatile int mouse_buttons;
volatile int mouse_delta_x;
volatile int mouse_delta_y;

static void reset_mouse_state(void) {
    mouse_x = 0;
    mouse_y = 0;
    mouse_buttons = 0;
    mouse_delta_x = 0;
    mouse_delta_y = 0;
}

static boolean init_mouse_driver(void) {
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
    mouse_buttons = regs.w.bx;
    mouse_delta_x = 0;
    mouse_delta_y = 0;
}

#pragma off (check_stack)
void __loadds far mouse_int_handler(int eax, int ebx, int ecx, int edx) {
    #pragma aux mouse_int_handler parm [eax] [ebx] [ecx] [edx]
    mouse_delta_x = (ecx / 2) - mouse_x;
    mouse_delta_y = edx - mouse_y;
    mouse_x = (ecx / 2);
    mouse_y = edx;
    mouse_buttons = ebx;
}
#pragma on (check_stack)

boolean mouse_init(void) {
    union REGS regs;
    struct SREGS sregs;

    if (_installed) {
        dgl_set_error(DGL_MOUSE_ALREADY_INITIALIZED);
        return FALSE;
    }

    reset_mouse_state();

    _has_mouse = init_mouse_driver();
    if (!_has_mouse) {
        _installed = TRUE;
        return TRUE;
    }

    update_mouse_state();

    memset(&regs, 0, sizeof(regs));
    memset(&sregs, 0, sizeof(sregs));
    regs.w.ax = 0x0c;
    regs.w.cx = 31;
    regs.x.edx = FP_OFF(mouse_int_handler);
    sregs.es = FP_SEG(mouse_int_handler);
    int386x(0x33, &regs, &regs, &sregs);

    _installed = TRUE;
    return TRUE;
}

boolean mouse_shutdown(void) {
    union REGS regs;

    if (!_installed)
        return TRUE;   // don't care

    if (!_has_mouse) {
        _installed = FALSE;
        return TRUE;
    }

    memset(&regs, 0, sizeof(regs));
    regs.w.ax = 0x0c;
    regs.w.cx = 0;
    int386(0x33, &regs, &regs);

    reset_mouse_state();
    init_mouse_driver();

    _installed = FALSE;
    return TRUE;
}

boolean mouse_is_initialized(void) {
    return _installed;
}

boolean mouse_is_present(void) {
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

