#include "dglkbrd.h"
#include "dglcmn.h"
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

#define KEYBRD_LED_SCROLLOCK   0x1
#define KEYBRD_LED_NUMLOCK     0x2
#define KEYBRD_LED_CAPSLOCK    0x4

#define KEY_EXTENDED           ((KEY)0xe0)

static boolean _installed = FALSE;
static INPUTEVENT *keyboard_event;

volatile ubyte keys[128];

volatile KEY _key_last_scan;
volatile KEY _key_scan;
volatile uword key_flags;
volatile uword key_mod;

static char lookup_key_to_char[128] = {
    0,    0,    '1',  '2',  '3',  '4',  '5',  '6',  // 00 - 07
    '7',  '8',  '9',  '0',  '-',  '=',  0,    '\t', // 08 - 0F
    'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',  // 10 - 17
    'o',  'p',  '[',  ']',  0,    0,    'a',  's',  // 18 - 1F
    'd',  'f',  'g',  'h',  'j',  'k',  'l',  ';',  // 20 - 27
    '\'', '`',  0,    '\\', 'z',  'x',  'c',  'v',  // 28 - 2F
    'b',  'n',  'm',  ',',  '.',  '/',  0,    '*',  // 30 - 37
    0,    ' ',  0,    0,    0,    0,    0,    0,    // 38 - 3F
    0,    0,    0,    0,    0,    0,    0,    0,    // 40 - 47
    0,    0,    0,    0,    0,    0,    0,    0,    // 48 - 4F
    0,    0,    0,    0,    0,    0,    0,    0,    // 50 - 57
    0,    0,    0,    0,    0,    0,    0,    0,    // 58 - 5F
    0,    0,    0,    0,    0,    0,    0,    0,    // 60 - 67
    0,    0,    0,    0,    0,    0,    0,    0,    // 68 - 6F
    0,    0,    0,    0,    0,    0,    0,    0,    // 70 - 77
    0,    0,    0,    0,    0,    0,    0,    0     // 78 - 7F
};

static char lookup_key_to_char_shift[128] = {
    0,    0,    '!',  '@',  '#',  '$',  '%',  '^',  // 00 - 07
    '&',  '*',  '(',  ')',  '_',  '+',  0,    '\t', // 08 - 0F
    'Q',  'W',  'E',  'R',  'T',  'Y',  'U',  'I',  // 10 - 17
    'O',  'P',  '{',  '}',  0,    0,    'A',  'S',  // 18 - 1F
    'D',  'F',  'G',  'H',  'J',  'K',  'L',  ':',  // 20 - 27
    '"',  '~',  0,    '|',  'Z',  'X',  'C',  'V',  // 28 - 2F
    'B',  'N',  'M',  '<',  '>',  '?',  0,    '*',  // 30 - 37
    0,    ' ',  0,    0,    0,    0,    0,    0,    // 38 - 3F
    0,    0,    0,    0,    0,    0,    0,    0,    // 40 - 47
    0,    0,    0,    0,    0,    0,    0,    0,    // 48 - 4F
    0,    0,    0,    0,    0,    0,    0,    0,    // 50 - 57
    0,    0,    0,    0,    0,    0,    0,    0,    // 58 - 5F
    0,    0,    0,    0,    0,    0,    0,    0,    // 60 - 67
    0,    0,    0,    0,    0,    0,    0,    0,    // 68 - 6F
    0,    0,    0,    0,    0,    0,    0,    0,    // 70 - 77
    0,    0,    0,    0,    0,    0,    0,    0     // 78 - 7F
};

static char lookup_key_to_char_caps[128] = {
    0,    0,    '1',  '2',  '3',  '4',  '5',  '6',  // 00 - 07
    '7',  '8',  '9',  '0',  '-',  '=',  0,    '\t', // 08 - 0F
    'Q',  'W',  'E',  'R',  'T',  'Y',  'U',  'I',  // 10 - 17
    'O',  'P',  '[',  ']',  0,    0,    'A',  'S',  // 18 - 1F
    'D',  'F',  'G',  'H',  'J',  'K',  'L',  ';',  // 20 - 27
    '\'', '`',  0,    '\\', 'Z',  'X',  'C',  'V',  // 28 - 2F
    'B',  'N',  'M',  ',',  '.',  '/',  0,    '*',  // 30 - 37
    0,    ' ',  0,    0,    0,    0,    0,    0,    // 38 - 3F
    0,    0,    0,    0,    0,    0,    0,    0,    // 40 - 47
    0,    0,    0,    0,    0,    0,    0,    0,    // 48 - 4F
    0,    0,    0,    0,    0,    0,    0,    0,    // 50 - 57
    0,    0,    0,    0,    0,    0,    0,    0,    // 58 - 5F
    0,    0,    0,    0,    0,    0,    0,    0,    // 60 - 67
    0,    0,    0,    0,    0,    0,    0,    0,    // 68 - 6F
    0,    0,    0,    0,    0,    0,    0,    0,    // 70 - 77
    0,    0,    0,    0,    0,    0,    0,    0     // 78 - 7F
};

static char lookup_key_to_char_caps_shift[128] = {
    0,    0,    '!',  '@',  '#',  '$',  '%',  '^',  // 00 - 07
    '&',  '*',  '(',  ')',  '_',  '+',  0,    '\t', // 08 - 0F
    'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',  // 10 - 17
    'o',  'p',  '{',  '}',  0,    0,    'a',  's',  // 18 - 1F
    'd',  'f',  'g',  'h',  'j',  'k',  'l',  ':',  // 20 - 27
    '"',  '~',  0,    '|',  'z',  'x',  'c',  'v',  // 28 - 2F
    'b',  'n',  'm',  '<',  '>',  '?',  0,    '*',  // 30 - 37
    0,    ' ',  0,    0,    0,    0,    0,    0,    // 38 - 3F
    0,    0,    0,    0,    0,    0,    0,    0,    // 40 - 47
    0,    0,    0,    0,    0,    0,    0,    0,    // 48 - 4F
    0,    0,    0,    0,    0,    0,    0,    0,    // 50 - 57
    0,    0,    0,    0,    0,    0,    0,    0,    // 58 - 5F
    0,    0,    0,    0,    0,    0,    0,    0,    // 60 - 67
    0,    0,    0,    0,    0,    0,    0,    0,    // 68 - 6F
    0,    0,    0,    0,    0,    0,    0,    0,    // 70 - 77
    0,    0,    0,    0,    0,    0,    0,    0     // 78 - 7F
};

static char lookup_key_to_char_numpad[128] = {
    0,    0,    0,    0,    0,    0,    0,    0,    // 00 - 07
    0,    0,    0,    0,    0,    0,    0,    0,    // 08 - 0F
    0,    0,    0,    0,    0,    0,    0,    0,    // 10 - 17
    0,    0,    0,    0,    0,    0,    0,    0,    // 18 - 1F
    0,    0,    0,    0,    0,    0,    0,    0,    // 20 - 27
    0,    0,    0,    0,    0,    0,    0,    0,    // 28 - 2F
    0,    0,    0,    0,    0,    0,    0,    0,    // 30 - 37
    0,    0,    0,    0,    0,    0,    0,    0,    // 38 - 3F
    0,    0,    0,    0,    0,    0,    0,    '7',  // 40 - 47
    '8',  '9',  '-',  '4',  '5',  '6',  '+',  '1',  // 48 - 4F
    '2',  '3',  '0',  '.',  0,    0,    0,    0,    // 50 - 57
    0,    0,    0,    0,    0,    0,    0,    0,    // 58 - 5F
    0,    0,    0,    0,    0,    0,    0,    0,    // 60 - 67
    0,    0,    0,    0,    0,    0,    0,    0,    // 68 - 6F
    0,    0,    0,    0,    0,    0,    0,    0,    // 70 - 77
    0,    0,    0,    0,    0,    0,    0,    0     // 78 - 7F
};

static char lookup_key_to_char_extended[128] = {
    0,    0,    0,    0,    0,    0,    0,    0,    // 00 - 07
    0,    0,    0,    0,    0,    0,    0,    0,    // 08 - 0F
    0,    0,    0,    0,    0,    0,    0,    0,    // 10 - 17
    0,    0,    0,    0,    0,    0,    0,    0,    // 18 - 1F
    0,    0,    0,    0,    0,    0,    0,    0,    // 20 - 27
    0,    0,    0,    0,    0,    0,    0,    0,    // 28 - 2F
    0,    0,    0,    0,    0,    '/',  0,    0,    // 30 - 37
    0,    0,    0,    0,    0,    0,    0,    0,    // 38 - 3F
    0,    0,    0,    0,    0,    0,    0,    0,    // 40 - 47
    0,    0,    0,    0,    0,    0,    0,    0,    // 48 - 4F
    0,    0,    0,    0,    0,    0,    0,    0,    // 50 - 57
    0,    0,    0,    0,    0,    0,    0,    0,    // 58 - 5F
    0,    0,    0,    0,    0,    0,    0,    0,    // 60 - 67
    0,    0,    0,    0,    0,    0,    0,    0,    // 68 - 6F
    0,    0,    0,    0,    0,    0,    0,    0,    // 70 - 77
    0,    0,    0,    0,    0,    0,    0,    0     // 78 - 7F
};

void (interrupt far *_old_handler)();

static void reset_key_states() {
    _key_last_scan = 0;
    _key_scan = 0;
    key_flags = 0;
    key_mod = 0;
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
    if (!send_kb_data(KEYBRD_CMD_SET_LED)) {
        dgl_set_error(DGL_KEYBOARD_UPDATE_LED_FAILURE);
        return FALSE;
    }
    if (!send_kb_data((flags >> 4) & 3)) {
        dgl_set_error(DGL_KEYBOARD_UPDATE_LED_FAILURE);
        return FALSE;
    }

    return TRUE;
}

static void push_keyboard_event(KEY key, EVENT_ACTION action) {
    if (_events_enabled) {
        _events_push(&keyboard_event);
        keyboard_event->type = EVENT_TYPE_KEYBOARD;
        keyboard_event->keyboard.key = key;
        keyboard_event->keyboard.action = action;
        keyboard_event->keyboard.mod = key_mod;
    }
}

static boolean handler_filter_keys(void) {
    if (BIT_ISSET(KEYBRD_MOD_EXTENDED, key_mod)) {
        // extended key + leftshift comes with cursor key presses when
        // numlock is enabled
        if ((_key_scan & 0x7f) == (KEY)KEY_LEFTSHIFT)
            return TRUE;
    }
    return FALSE;
}

static void handler_update_flags_and_leds(void) {
    switch (_key_scan) {
        case (KEY)KEY_CAPSLOCK:
            BIT_TOGGLE(KEYBRD_FLAGS_CAPSLOCK, key_flags);
            update_kb_led(key_flags);
            set_kb_flags(key_flags);
            break;
        case (KEY)KEY_NUMLOCK:
            BIT_TOGGLE(KEYBRD_FLAGS_NUMLOCK, key_flags);
            update_kb_led(key_flags);
            set_kb_flags(key_flags);
            break;
        case (KEY)KEY_SCROLLLOCK:
            BIT_TOGGLE(KEYBRD_FLAGS_SCROLLOCK, key_flags);
            update_kb_led(key_flags);
            set_kb_flags(key_flags);
            break;
        default:
            break;
    }
}

static void handler_update_modifiers(void) {
    if (BIT_ISSET(KEYBRD_FLAGS_NUMLOCK, key_flags))
        BIT_SET(KEYBRD_MOD_NUMLOCK, key_mod);
    else
        BIT_CLEAR(KEYBRD_MOD_NUMLOCK, key_mod);

    if (BIT_ISSET(KEYBRD_FLAGS_CAPSLOCK, key_flags))
        BIT_SET(KEYBRD_MOD_CAPSLOCK, key_mod);
    else
        BIT_CLEAR(KEYBRD_MOD_CAPSLOCK, key_mod);

    if (keys[KEY_LEFTSHIFT] || keys[KEY_RIGHTSHIFT])
        BIT_SET(KEYBRD_MOD_SHIFT, key_mod);
    else
        BIT_CLEAR(KEYBRD_MOD_SHIFT, key_mod);
}

// keyboard interrupt handler
void interrupt far kb_int_handler(void) {
    // read scan code of key that was just pressed
    _key_scan = inp(KEYBRD_DATA_PORT);
    if (_key_scan == KEY_EXTENDED) {
        // extended key scan
        BIT_SET(KEYBRD_MOD_EXTENDED, key_mod);

    } else {
        if (!handler_filter_keys()) {
            if (_key_scan & 0x80) {
                // high bit set indicates key was released, clear high bit
                // to get the actual key scan code
                _key_scan &= 0x7f;
                keys[(int)_key_scan] = 0;
                handler_update_modifiers();
                push_keyboard_event(_key_scan, EVENT_ACTION_RELEASED);
            } else {
                if (keys[(int)_key_scan])
                    push_keyboard_event(_key_scan, EVENT_ACTION_HELD);
                else {
                    keys[(int)_key_scan] = 1;
                    handler_update_flags_and_leds();
                    handler_update_modifiers();
                    push_keyboard_event(_key_scan, EVENT_ACTION_PRESSED);
                }
            }
            _key_last_scan = _key_scan;
        }

        BIT_CLEAR(KEYBRD_MOD_EXTENDED, key_mod);
    }

    // indicate key event was processed to keyboard controller
    _key_scan = inp(KEYBRD_CTRL_PORT) | 0x82;
    outp(KEYBRD_CTRL_PORT, _key_scan);
    outp(KEYBRD_CTRL_PORT, _key_scan & 0x7f);
    outp(PIC_CTRL_PORT, 0x20);
}

boolean keyboard_init(void) {
    if (_installed) {
        dgl_set_error(DGL_KEYBOARD_ALREADY_INITIALIZED);
        return FALSE;
    }

    reset_key_states();

    key_flags = get_kb_flags();
    handler_update_modifiers();

    _old_handler = _dos_getvect(9);
    _dos_setvect(9, kb_int_handler);

    _installed = TRUE;
    return TRUE;
}

boolean keyboard_shutdown(void) {
    if (!_installed)
        return TRUE;  // don't care

    _dos_setvect(9, _old_handler);

    reset_key_states();

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

char key_to_char(KEY key, uword modifiers) {
    if (BIT_ISSET(KEYBRD_MOD_EXTENDED, modifiers)) {
        return lookup_key_to_char_extended[(int)key];
    }
    else if ((key >= (KEY)0x47) && (key <= (KEY)0x53)) {
        if (BIT_ISSET(KEYBRD_MOD_NUMLOCK, modifiers) &&
            !BIT_ISSET(KEYBRD_MOD_SHIFT, modifiers))
            return lookup_key_to_char_numpad[(int)key];
        else
            return 0;
    } else {
        if (BIT_ISSET(KEYBRD_MOD_CAPSLOCK, modifiers)) {
            if (BIT_ISSET(KEYBRD_MOD_SHIFT, modifiers))
                return lookup_key_to_char_caps_shift[(int)key];
            else
                return lookup_key_to_char_caps[(int)key];
        } else {
            if (BIT_ISSET(KEYBRD_MOD_SHIFT, modifiers))
                return lookup_key_to_char_shift[(int)key];
            else
                return lookup_key_to_char[(int)key];
        }
    }
}

