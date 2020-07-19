#include "dglevent.h"
#include "dgl.h"
#include "dglutil.h"
#include <string.h>

volatile bool _events_enabled = false;

volatile INPUTEVENT _events_buffer[EVENTS_BUFFER_SIZE];
volatile int _events_buffer_start = 0;
volatile int _events_buffer_end = 0;

bool events_init(void) {
    if (_events_enabled) {
        dgl_set_error(DGL_EVENTS_ALREADY_INITIALIZED);
        return false;
    }

    events_clear();
    _events_enabled = true;

    return true;
}

bool events_shutdown(void) {
    if (!_events_enabled)
        return true;  // don't care

    _events_enabled = false;
    events_clear();

    return true;
}

bool events_poll(volatile INPUTEVENT **event) {
    if (events_is_empty())
        return false;

    int_disable();

    *event = &_events_buffer[_events_buffer_start];

    ++_events_buffer_start;
    if (_events_buffer_start >= EVENTS_BUFFER_SIZE)
        _events_buffer_start = 0;

    int_enable();

    return true;
}

bool events_peek(volatile INPUTEVENT **event) {
    if (events_is_empty())
        return false;

    int_disable();

    *event = &_events_buffer[_events_buffer_start];

    int_enable();

    return true;
}

void events_clear(void) {
    int_disable();

    memset((void*)_events_buffer, 0, sizeof(_events_buffer));
    _events_buffer_start = 0;
    _events_buffer_end = 0;

    int_enable();
}

