#ifndef LIBDGL_DGLEVENT_H
#define LIBDGL_DGLEVENT_H

#include "dglcmn.h"
#include "dglkbrd.h"
#include "dglmouse.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8 EVENT_TYPE;

#define EVENT_TYPE_KEYBOARD        1
#define EVENT_TYPE_MOUSE_MOTION    2
#define EVENT_TYPE_MOUSE_BUTTON    3

typedef uint8 EVENT_ACTION;

#define EVENT_ACTION_PRESSED       1
#define EVENT_ACTION_RELEASED      2
#define EVENT_ACTION_HELD          3

typedef struct {
    KEY          key;
    EVENT_ACTION action;
    uint16       mod;
} INPUTEVENT_KEYBOARD;

typedef struct {
    int          x;
    int          y;
    int          x_delta;
    int          y_delta;
    MOUSE_BUTTON buttons;
} INPUTEVENT_MOUSE_MOTION;

typedef struct {
    int          x;
    int          y;
    MOUSE_BUTTON button;
    EVENT_ACTION action;
} INPUTEVENT_MOUSE_BUTTON;

typedef struct {
    EVENT_TYPE type;
    union {
        INPUTEVENT_KEYBOARD     keyboard;
        INPUTEVENT_MOUSE_MOTION mouse_motion;
        INPUTEVENT_MOUSE_BUTTON mouse_button;
    };
} INPUTEVENT;

extern volatile bool _events_enabled;

#define EVENTS_BUFFER_SIZE 32

extern volatile INPUTEVENT _events_buffer[EVENTS_BUFFER_SIZE];
extern volatile int _events_buffer_start;
extern volatile int _events_buffer_end;

bool events_init(void);
bool events_shutdown(void);
static bool events_is_initialized(void);

static bool events_is_empty(void);

bool events_poll(volatile INPUTEVENT **event);
bool events_peek(volatile INPUTEVENT **event);

void events_clear(void);

static bool events_key_pressed(INPUTEVENT *event, KEY key);
static bool events_key_released(INPUTEVENT *event, KEY key);
static bool events_key_held(INPUTEVENT *event, KEY key);
static bool events_mouse_pressed(INPUTEVENT *event, MOUSE_BUTTON button);
static bool events_mouse_released(INPUTEVENT *event, MOUSE_BUTTON button);
static bool events_mouse_held(INPUTEVENT *event, MOUSE_BUTTON button);

static void _events_push(INPUTEVENT **out_event);

// ---------------------------------------------------------------------------

static bool events_is_initialized(void) {
    return _events_enabled;
}

static bool events_is_empty(void) {
    return (_events_buffer_start == _events_buffer_end);
}

static bool events_key_pressed(INPUTEVENT *event, KEY key) {
    return event->type == EVENT_TYPE_KEYBOARD &&
           event->keyboard.action == EVENT_ACTION_PRESSED &&
           event->keyboard.key == key;
}

static bool events_key_released(INPUTEVENT *event, KEY key) {
    return event->type == EVENT_TYPE_KEYBOARD &&
           event->keyboard.action == EVENT_ACTION_RELEASED &&
           event->keyboard.key == key;
}

static bool events_key_held(INPUTEVENT *event, KEY key) {
    return event->type == EVENT_TYPE_KEYBOARD &&
           event->keyboard.action == EVENT_ACTION_HELD &&
           event->keyboard.key == key;
}

static bool events_mouse_pressed(INPUTEVENT *event, MOUSE_BUTTON button) {
    return event->type == EVENT_TYPE_MOUSE_BUTTON &&
           event->mouse_button.action == EVENT_ACTION_PRESSED &&
           event->mouse_button.button == button;
}

static bool events_mouse_released(INPUTEVENT *event, MOUSE_BUTTON button) {
    return event->type == EVENT_TYPE_MOUSE_BUTTON &&
           event->mouse_button.action == EVENT_ACTION_RELEASED &&
           event->mouse_button.button == button;
}

static bool events_mouse_held(INPUTEVENT *event, MOUSE_BUTTON button) {
    return event->type == EVENT_TYPE_MOUSE_BUTTON &&
           event->mouse_button.action == EVENT_ACTION_HELD &&
           event->mouse_button.button == button;
}

// only intended to be called from input device interrupt handler (the
// usage is a little weird as a result)
static void _events_push(INPUTEVENT **out_event) {
    *out_event = (INPUTEVENT*)&_events_buffer[_events_buffer_end];

    ++_events_buffer_end;

    // wrap around
    if (_events_buffer_end >= EVENTS_BUFFER_SIZE)
        _events_buffer_end = 0;

    // is the events buffer full? (if the end meets up to the start, yes)
    if (_events_buffer_end == _events_buffer_start) {
        // move the start up. this ensures start always points to the oldest
        // event in the buffer
        ++_events_buffer_start;
        if (_events_buffer_start >= EVENTS_BUFFER_SIZE)
            _events_buffer_start = 0;
    }
}

#ifdef __cplusplus
}
#endif

#endif

