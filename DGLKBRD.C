#include "dglkbrd.h"
#include "dglevent.h"
#include "dglutil.h"
#include "dglerror.h"
#include <string.h>
#include <dos.h>

#define PIC_CTRL_PORT          0x20
#define KEYBRD_DATA_PORT       0x60
#define KEYBRD_CTRL_PORT       0x61
#define KEYBRD_STATUS_PORT     0x64

#define KEYBRD_CMD_SET_LED     0xED

#define KEYBRD_FLAGS_ADDR      0x417

#define KEYBRD_FLAGS_SCROLLOCK 0x10
#define KEYBRD_FLAGS_NUMLOCK   0x20
#define KEYBRD_FLAGS_CAPSLOCK  0x40

#define KEYBRD_LED_SCROLLOCK   0x1
#define KEYBRD_LED_NUMLOCK     0x2
#define KEYBRD_LED_CAPSLOCK    0x4

static boolean _installed = FALSE;
static INPUTEVENT *keyboard_event;

volatile ubyte keys[128];

volatile KEY _key_last_scan;
volatile KEY _key_scan;

uword _old_flags;

void (interrupt far *_old_handler)();

static void reset_key_states() {
    memset((void*)keys, 0, 128);
}

// waits until the keyboard status port indicates the data port
// can be read from once again
static void wait_kb_data_read() {
    while ((inp(KEYBRD_STATUS_PORT) & BIT_0) == 0) {
    }
}

// waits until the keyboard status port indicates the data port
// can be written to once again
static void wait_kb_data_write() {
    while ((inp(KEYBRD_STATUS_PORT) & BIT_1) != 0) {
    }
}

// sends data to the keyboard data port. checks for success
// and returns TRUE if the data write succeeded
static boolean send_kb_data(ubyte data) {
    ubyte result;

    wait_kb_data_write();
    outp(KEYBRD_DATA_PORT, data);

    wait_kb_data_read();
    result = inp(KEYBRD_DATA_PORT);
    return (result == 0xFA);
}

static void push_keyboard_event(KEY key, EVENT_ACTION action) {
    if (_events_enabled) {
        // HACK: skipping extended (?) key extra keyscan code.
        //       we only actually care about the subsequent key code...
        //       (this is a terrible way to do this, doesn't handle all cases)
        if (key != 0x60) {
            _events_push(&keyboard_event);
            keyboard_event->type = EVENT_TYPE_KEYBOARD;
            keyboard_event->keyboard.key = key;
            keyboard_event->keyboard.action = action;
        }
    }
}

// keyboard interrupt handler
void interrupt far kb_int_handler(void) {
    // read scan code of key that was just pressed
    _key_scan = inp(KEYBRD_DATA_PORT);
    if (_key_scan & 0x80) {
        // high bit set indicates key was released, clear high bit to get
        // the actual key scan code
        _key_scan &= 0x7f;
        keys[(int)_key_scan] = 0;
        push_keyboard_event(_key_scan, EVENT_ACTION_RELEASED);
    } else {
        if (keys[(int)_key_scan])
            push_keyboard_event(_key_scan, EVENT_ACTION_HELD);
        else {
            keys[(int)_key_scan] = 1;
            push_keyboard_event(_key_scan, EVENT_ACTION_PRESSED);
        }
    }

    _key_last_scan = _key_scan;

    // indicate key event was processed to keyboard controller
    _key_scan = inp(KEYBRD_CTRL_PORT) | 0x82;
    outp(KEYBRD_CTRL_PORT, _key_scan);
    outp(KEYBRD_CTRL_PORT, _key_scan & 0x7f);
    outp(PIC_CTRL_PORT, 0x20);
}

static uword get_kb_flags(void) {
    return *((uword*)KEYBRD_FLAGS_ADDR);
}

static void set_kb_flags(uword flags) {
    *((uword*)KEYBRD_FLAGS_ADDR) = flags;
}

// updates the keyboard indicator LEDs from the num/caps/scroll lock flags
// set in the passed keyboard flags. returns FALSE if the LEDs could not
// be updated (if keyboard data write did not succeed)
static boolean update_kb_led(byte flags) {
    ubyte led = 0;

    if (flags & KEYBRD_FLAGS_SCROLLOCK)
        led |= KEYBRD_LED_SCROLLOCK;
    if (flags & KEYBRD_FLAGS_NUMLOCK)
        led |= KEYBRD_LED_NUMLOCK;
    if (flags & KEYBRD_FLAGS_CAPSLOCK)
        led |= KEYBRD_LED_CAPSLOCK;

    if (!send_kb_data(KEYBRD_CMD_SET_LED)) {
        dgl_set_error(DGL_KEYBOARD_UPDATE_LED_FAILURE);
        return FALSE;
    }
    if (!send_kb_data(led)) {
        dgl_set_error(DGL_KEYBOARD_UPDATE_LED_FAILURE);
        return FALSE;
    }

    return TRUE;
}

boolean keyboard_init(void) {
    if (_installed) {
        dgl_set_error(DGL_KEYBOARD_ALREADY_INITIALIZED);
        return FALSE;
    }

    // preserve old flags
    _old_flags = get_kb_flags();

    reset_key_states();
    _old_handler = _dos_getvect(9);
    _dos_setvect(9, kb_int_handler);

    // turn off keyboard LEDs since our interrupt handler does not currently
    // respect the num/caps/scroll lock statuses
    int_disable();
    update_kb_led(0);
    int_enable();

    _installed = TRUE;
    return TRUE;
}

boolean keyboard_shutdown(void) {
    if (!_installed)
        return TRUE;  // don't care

    // reset keyboard LEDs to previous state
    int_disable();
    update_kb_led(_old_flags);
    int_enable();

    _dos_setvect(9, _old_handler);
    reset_key_states();

    // restore keyboard flags to previous state
    set_kb_flags(_old_flags);

    _installed = FALSE;
    return TRUE;
}

boolean keyboard_is_initialized(void) {
    return _installed;
}

KEY keyboard_read_key(void) {
    _key_last_scan = 0;
    while (_key_last_scan == 0) {
    }
    return _key_last_scan;
}

void keyboard_wait_for_key(KEY key) {
    _key_last_scan = 0;
    while (_key_last_scan != key) {
    }
}

