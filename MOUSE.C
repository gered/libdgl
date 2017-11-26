#include "mouse.h"
#include "internal.h"
#include "error.h"
#include <string.h>
#include <go32.h>

static boolean _installed = FALSE;
static boolean _has_mouse = FALSE;

volatile int mouse_x;
volatile int mouse_y;
volatile int mouse_buttons;
volatile int mouse_delta_x;
volatile int mouse_delta_y;

__dpmi_regs _mouse_regs;
_go32_dpmi_seginfo _mouse_seg_info;

static void reset_mouse_state(void) {
    mouse_x = 0;
    mouse_y = 0;
    mouse_buttons = 0;
    mouse_delta_x = 0;
    mouse_delta_y = 0;
}

static boolean init_mouse_driver(void) {
    __dpmi_regs regs;

    memset(&regs, 0, sizeof(__dpmi_regs));
    regs.x.ax = 0x00;
    __dpmi_int(0x33, &regs);

    return (regs.x.ax != 0);
}

static void update_mouse_state(void) {
    __dpmi_regs regs;

    memset(&regs, 0, sizeof(__dpmi_regs));
    regs.x.ax = 0x03;
    __dpmi_int(0x33, &regs);
    mouse_x = (regs.x.cx / 2);
    mouse_y = regs.x.dx;
    mouse_buttons = regs.x.bx;
    mouse_delta_x = 0;
    mouse_delta_y = 0;
}

void mouse_int_handler(__dpmi_regs* regs) {
    mouse_delta_x = (regs->x.cx / 2) - mouse_x;
    mouse_delta_y = regs->x.dx - mouse_y;
    mouse_x = (regs->x.cx / 2);
    mouse_y = regs->x.dx;
    mouse_buttons = regs->x.bx;
}
END_OF_FUNCTION(mouse_int_handler)

boolean mouse_init(void) {
    __dpmi_regs regs;

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

    LOCK_FUNCTION(mouse_int_handler);
    LOCK_VARIABLE(_mouse_regs);
    LOCK_VARIABLE(_mouse_seg_info);
    LOCK_VARIABLE(mouse_x);
    LOCK_VARIABLE(mouse_y);
    LOCK_VARIABLE(mouse_buttons);
    LOCK_VARIABLE(mouse_delta_x);
    LOCK_VARIABLE(mouse_delta_y);

    memset(&_mouse_regs, 0, sizeof(__dpmi_regs));
    memset(&_mouse_seg_info, 0, sizeof(_go32_dpmi_seginfo));

    _mouse_seg_info.pm_offset = (int)mouse_int_handler;
    _mouse_seg_info.pm_selector = _go32_my_cs();
    if (_go32_dpmi_allocate_real_mode_callback_retf(&_mouse_seg_info, &_mouse_regs) != 0) {
        dgl_set_error(DGL_MOUSE_ALLOCATE_CALLBACK_FAILURE);
        return FALSE;
    }

    update_mouse_state();

    memset(&regs, 0, sizeof(__dpmi_regs));
    regs.x.ax = 0x0c;
    regs.x.cx = 0x7f;
    regs.x.dx = _mouse_seg_info.rm_offset;
    regs.x.es = _mouse_seg_info.rm_segment;
    if (__dpmi_int(0x33, &regs) != 0) {
        dgl_set_error(DGL_MOUSE_INT_CALLBACK_SET_FAILURE);
        return FALSE;
    }

    _installed = TRUE;
    return TRUE;
}

boolean mouse_shutdown(void) {
    __dpmi_regs regs;

    if (!_installed)
        return TRUE;   // don't care

    if (!_has_mouse) {
        _installed = FALSE;
        return TRUE;
    }

    memset(&regs, 0, sizeof(__dpmi_regs));
    regs.x.ax = 0x0c;
    regs.x.cx = 0;
    regs.x.dx = 0;
    regs.x.es = 0;
    if (__dpmi_int(0x33, &regs) != 0) {
        dgl_set_error(DGL_MOUSE_INT_CALLBACK_RESTORE_FAILURE);
        return FALSE;
    }

    if (_go32_dpmi_free_real_mode_callback(&_mouse_seg_info) != 0)
        dgl_set_error(DGL_MOUSE_FREE_CALLBACK_FAILURE);

    reset_mouse_state();

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
    __dpmi_regs regs;

    if (!_has_mouse)
        return;

    memset(&regs, 0, sizeof(__dpmi_regs));
    regs.x.ax = 0x01;
    __dpmi_int(0x33, &regs);
}

void mouse_hide(void) {
    __dpmi_regs regs;

    if (!_has_mouse)
        return;

    memset(&regs, 0, sizeof(__dpmi_regs));
    regs.x.ax = 0x02;
    __dpmi_int(0x33, &regs);
}

void mouse_set_bounds(int min_x, int min_y, int max_x, int max_y) {
    __dpmi_regs regs;

    if (!_has_mouse)
        return;

    memset(&regs, 0, sizeof(__dpmi_regs));

    regs.x.ax = 0x07;
    regs.x.cx = min_x;
    regs.x.dx = max_x;
    __dpmi_int(0x33, &regs);

    regs.x.ax = 0x08;
    regs.x.cx = min_y;
    regs.x.dx = max_y;
    __dpmi_int(0x33, &regs);
}

