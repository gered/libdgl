#include "dgl.h"
#include "keyboard.h"
#include "mouse.h"
#include "gfx.h"
#include "util.h"
#include <stdlib.h>
#include <go32.h>
#include <sys/nearptr.h>

static boolean _initialized = FALSE;
static DGL_ERROR _last_error = DGL_NONE;

boolean dgl_init(void) {
    if (_initialized) {
        dgl_set_error(DGL_ALREADY_INIT);
        return FALSE;
    }
    if (__djgpp_nearptr_enable() == 0) {
        dgl_set_error(DGL_NEARPTR_ENABLE_FAILURE);
        return FALSE;
    }

    srandom((int)sys_clock());

    // first call will return zero, so call here just to "init"
    sys_clock();
    sys_ticks();

    if (!video_init())
        return FALSE;
    if (!keyboard_init())
        return FALSE;
    if (!mouse_init())
        return FALSE;

    _initialized = TRUE;
    return TRUE;
}

boolean dgl_shutdown(void) {
    if (!_initialized)
        return TRUE;   // don't care

    // remove installed services
    if (!mouse_shutdown())
        return FALSE;
    if (!keyboard_shutdown())
        return FALSE;
    if (!video_shutdown())
        return FALSE;

    __djgpp_nearptr_disable();

    _initialized = FALSE;
    return TRUE;
}

DGL_ERROR dgl_last_error(void) {
    return _last_error;
}

const char* dgl_last_error_message(void) {
    switch (_last_error) {
        case DGL_NONE:
            return "No error.";
        case DGL_ALREADY_INIT:
            return "DGL is already initialized.";
        case DGL_NEARPTR_ENABLE_FAILURE:
            return "Failed to enable DJGPP near pointers.";
        case DGL_VIDEO_ALREADY_INITIALIZED:
            return "Video subsystem is already initialized.";
        case DGL_VIDEO_MODE_13H_INIT_FAILURE:
            return "Failed to set VGA mode 13h.";
        case DGL_VIDEO_TEXT_MODE_INIT_FAILURE:
            return "Failed to set text mode.";
        case DGL_KEYBOARD_ALREADY_INITIALIZED:
            return "Keyboard subsystem is already initialized.";
        case DGL_KEYBOARD_IRQ_INSTALL_FAILURE:
            return "Failed to install IRQ handler for keyboard.";
        case DGL_KEYBOARD_IRQ_RESTORE_FAILURE:
            return "Failed to restore original keyboard IRQ handler.";
        case DGL_KEYBOARD_UPDATE_LED_FAILURE:
            return "Failed to update keyboard LED state.";
        case DGL_MOUSE_ALREADY_INITIALIZED:
            return "Mouse subsystem is already initialized.";
        case DGL_MOUSE_ALLOCATE_CALLBACK_FAILURE:
            return "Failed to allocate mouse handler callback.";
        case DGL_MOUSE_FREE_CALLBACK_FAILURE:
            return "Failed to free mouse handler callback.";
        case DGL_MOUSE_INT_CALLBACK_SET_FAILURE:
            return "Failed to set mouse interrupt handler callback.";
        case DGL_MOUSE_INT_CALLBACK_RESTORE_FAILURE:
            return "Failed to restore original mouse interrupt handler callback.";
        case DGL_IO_ERROR:
            return "File IO error.";
        case DGL_PCX_BAD_FORMAT:
            return "PCX file unsupported format or bad file.";
        default:
            return "Unknown error.";
    }
}

void dgl_set_error(DGL_ERROR error) {
    _last_error = error;
}

