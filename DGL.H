#ifndef LIBDGL_DGL_H
#define LIBDGL_DGL_H

#ifdef __cplusplus
extern "C" {
#endif

void dgl_init(void);
void dgl_close(void);

typedef enum {
    DGL_NONE = 0,
    DGL_ALREADY_INIT,
    DGL_NEARPTR_ENABLE_FAILURE,
    DGL_VIDEO_ALREADY_INITIALIZED,
    DGL_VIDEO_MODE_13H_INIT_FAILURE,
    DGL_VIDEO_TEXT_MODE_INIT_FAILURE,
    DGL_KEYBOARD_ALREADY_INITIALIZED,
    DGL_KEYBOARD_IRQ_INSTALL_FAILURE,
    DGL_KEYBOARD_IRQ_RESTORE_FAILURE,
    DGL_KEYBOARD_UPDATE_LED_FAILURE,
    DGL_MOUSE_ALREADY_INITIALIZED,
    DGL_MOUSE_ALLOCATE_CALLBACK_FAILURE,
    DGL_MOUSE_FREE_CALLBACK_FAILURE,
    DGL_MOUSE_INT_CALLBACK_SET_FAILURE,
    DGL_MOUSE_INT_CALLBACK_RESTORE_FAILURE,
    DGL_EVENTS_ALREADY_INITIALIZED,
    DGL_IO_ERROR,
    DGL_PCX_BAD_FORMAT
} DGL_ERROR;

DGL_ERROR dgl_last_error(void);
const char* dgl_last_error_message(void);
void dgl_set_error(DGL_ERROR error);

#ifdef __cplusplus
}
#endif

#endif

