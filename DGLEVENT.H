#ifndef DGL_DGLEVENT_H_INCLUDED
#define DGL_DGLEVENT_H_INCLUDED

#include "dglcmn.h"
#include "dglkbrd.h"
#include "dglmouse.h"

typedef byte EVENT_TYPE;

#define EVENT_TYPE_KEYBOARD        1
#define EVENT_TYPE_MOUSE_MOTION    2
#define EVENT_TYPE_MOUSE_BUTTON    3

typedef byte EVENT_ACTION;

#define EVENT_ACTION_PRESSED       1
#define EVENT_ACTION_RELEASED      2
#define EVENT_ACTION_HELD          3

typedef struct {
    KEY key;
    EVENT_ACTION action;
} INPUTEVENT_KEYBOARD;

typedef struct {
    int x;
    int y;
    int x_delta;
    int y_delta;
    MOUSE_BUTTON buttons;
} INPUTEVENT_MOUSE_MOTION;

typedef struct {
    int x;
    int y;
    MOUSE_BUTTON button;
    EVENT_ACTION action;
} INPUTEVENT_MOUSE_BUTTON;

typedef struct {
    EVENT_TYPE type;
    union {
        INPUTEVENT_KEYBOARD keyboard;
        INPUTEVENT_MOUSE_MOTION mouse_motion;
        INPUTEVENT_MOUSE_BUTTON mouse_button;
    };
} INPUTEVENT;

extern volatile boolean _events_enabled;

#define EVENTS_BUFFER_SIZE 32

extern volatile INPUTEVENT _events_buffer[EVENTS_BUFFER_SIZE];
extern volatile int _events_buffer_start;
extern volatile int _events_buffer_end;

boolean events_init(void);
boolean events_shutdown(void);
static boolean events_is_initialized(void);

static boolean events_is_empty(void);

boolean events_poll(INPUTEVENT **event);
boolean events_peek(INPUTEVENT **event);

void events_clear(void);

static void _events_push(INPUTEVENT **out_event);

// ---------------------------------------------------------------------------

static boolean events_is_initialized(void) {
    return _events_enabled;
}

static boolean events_is_empty(void) {
    return (_events_buffer_start == _events_buffer_end);
}

// only intended to be called from input device interrupt handler (the
// usage is a little weird as a result)
static void _events_push(INPUTEVENT **out_event) {
    *out_event = &_events_buffer[_events_buffer_end];

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

#endif

