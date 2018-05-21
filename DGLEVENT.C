#include "dglevent.h"
#include "dglerror.h"
#include "dglutil.h"
#include <string.h>

volatile boolean _events_enabled;

volatile INPUTEVENT _events_buffer[EVENTS_BUFFER_SIZE];
volatile int _events_buffer_start = 0;
volatile int _events_buffer_end = 0;

boolean events_init(void) {
    if (_events_enabled) {
        dgl_set_error(DGL_EVENTS_ALREADY_INITIALIZED);
        return FALSE;
    }

    events_clear();
    _events_enabled = TRUE;

    return TRUE;
}

boolean events_shutdown(void) {
    if (!_events_enabled)
        return TRUE;  // don't care

    _events_enabled = FALSE;
    events_clear();

    return TRUE;
}

boolean events_poll(INPUTEVENT **event) {
    if (events_is_empty())
        return FALSE;

    int_disable();

    *event = &_events_buffer[_events_buffer_start];

    ++_events_buffer_start;
    if (_events_buffer_start >= EVENTS_BUFFER_SIZE)
        _events_buffer_start = 0;

    int_enable();

    return TRUE;
}

boolean events_peek(INPUTEVENT **event) {
    if (events_is_empty())
        return FALSE;

    *event = &_events_buffer[_events_buffer_start];

    return TRUE;
}

void events_clear(void) {
    int_disable();

    memset(_events_buffer, 0, sizeof(_events_buffer));
    _events_buffer_start = 0;
    _events_buffer_end = 0;

    int_enable();
}

