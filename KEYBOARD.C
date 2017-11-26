#include "keyboard.h"
#include "internal.h"
#include "util.h"
#include "error.h"
#include <string.h>
#include <dpmi.h>
#include <pc.h>
#include <sys/nearptr.h>

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

volatile ubyte keys[128];

volatile KEY _key_last_scan;
volatile KEY _key_scan;

uword _old_flags;

_go32_dpmi_seginfo _old_handler;
_go32_dpmi_seginfo _new_handler;

static inline void reset_key_states() {
    memset((void*)keys, 0, 128);
}

// waits until the keyboard status port indicates the data port
// can be read from once again
static inline void wait_kb_data_read() {
    while ((inportb(KEYBRD_STATUS_PORT) & BIT_0) == 0) {
    }
}

// waits until the keyboard status port indicates the data port
// can be written to once again
static inline void wait_kb_data_write() {
    while ((inportb(KEYBRD_STATUS_PORT) & BIT_1) != 0) {
    }
}

// sends data to the keyboard data port. checks for success
// and returns TRUE if the data write succeeded
static inline boolean send_kb_data(ubyte data) {
    ubyte result;

    wait_kb_data_write();
    outportb(KEYBRD_DATA_PORT, data);

    wait_kb_data_read();
    result = inportb(KEYBRD_DATA_PORT);
    return (result == 0xFA);
}

// keyboard interrupt handler
void kb_int_handler(void) {
    // read scan code of key that was just pressed
    _key_scan = inportb(KEYBRD_DATA_PORT);
    if (_key_scan & 0x80) {
        // high bit set indicates key was released, clear high bit to get
        // the actual key scan code
        _key_scan &= 0x7f;
        keys[(int)_key_scan] = 0;
    } else {
        keys[(int)_key_scan] = 1;
    }

    _key_last_scan = _key_scan;

    // indicate key event was processed to keyboard controller
    _key_scan = inportb(KEYBRD_CTRL_PORT) | 0x82;
    outportb(KEYBRD_CTRL_PORT, _key_scan);
    outportb(KEYBRD_CTRL_PORT, _key_scan & 0x7f);
    outportb(PIC_CTRL_PORT, 0x20);
}
END_OF_FUNCTION(kb_int_handler)

static uword get_kb_flags(void) {
    return *((uword*)(map_dos_memory(KEYBRD_FLAGS_ADDR)));
}

static void set_kb_flags(uword flags) {
    *((uword*)(map_dos_memory(KEYBRD_FLAGS_ADDR))) = flags;
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

    LOCK_MEMORY(keys, 128);
    LOCK_VARIABLE(_key_scan);
    LOCK_VARIABLE(_key_last_scan);
    LOCK_FUNCTION(kb_int_handler);

    // preserve old flags
    _old_flags = get_kb_flags();

    reset_key_states();
    if (!_install_irq(9, kb_int_handler, &_new_handler, &_old_handler)) {
        dgl_set_error(DGL_KEYBOARD_IRQ_INSTALL_FAILURE);
        return FALSE;
    }

    // turn off keyboard LEDs since our interrupt handler does not currently
    // respect the num/caps/scroll lock statuses
    _disable_interrupts();
    update_kb_led(0);
    _enable_interrupts();

    _installed = TRUE;
    return TRUE;
}

boolean keyboard_shutdown(void) {
    if (!_installed)
        return TRUE;  // don't care

    // reset keyboard LEDs to previous state
    _disable_interrupts();
    update_kb_led(_old_flags);
    _enable_interrupts();

    if (!_restore_irq(9, &_new_handler, &_old_handler)) {
        dgl_set_error(DGL_KEYBOARD_IRQ_RESTORE_FAILURE);
        return FALSE;
    }
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

