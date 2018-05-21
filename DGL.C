#include "dgl.h"
#include "dglkbrd.h"
#include "dglmouse.h"
#include "dglgfx.h"
#include "dglutil.h"
#include <stdlib.h>

static DGL_ERROR _last_error = DGL_NONE;

DGL_ERROR dgl_last_error(void) {
    DGL_ERROR err = _last_error;
    _last_error = DGL_NONE;
    return err;
}

const char* dgl_last_error_message(void) {
    switch (_last_error) {
        case DGL_NONE:
            return "";
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
        case DGL_EVENTS_ALREADY_INITIALIZED:
            return "Input device events subsystem is already initialized.";
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

void _dgl_atexit(void) {
    events_shutdown();
    mouse_shutdown();
    keyboard_shutdown();
    video_shutdown();
}

void dgl_init(void) {
    atexit(_dgl_atexit);
}

