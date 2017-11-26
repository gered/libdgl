#include "internal.h"
#include <dpmi.h>
#include <go32.h>
#include <string.h>

boolean _install_irq(int irq,
                     void* irq_handler,
                     _go32_dpmi_seginfo* new_handler,
                     _go32_dpmi_seginfo* old_handler) {
    memset((void*)new_handler, 0, sizeof(_go32_dpmi_seginfo));

    if (_go32_dpmi_get_real_mode_interrupt_vector(irq, old_handler) != 0)
        return FALSE;

    new_handler->pm_offset = (int)irq_handler;
    new_handler->pm_selector = _go32_my_cs();

    if (_go32_dpmi_allocate_iret_wrapper(new_handler) != 0)
        return FALSE;
    if (_go32_dpmi_set_protected_mode_interrupt_vector(irq, new_handler) != 0)
        return FALSE;

    return TRUE;
}

boolean _restore_irq(int irq,
                     _go32_dpmi_seginfo* new_handler,
                     _go32_dpmi_seginfo* old_handler) {
    if (_go32_dpmi_set_real_mode_interrupt_vector(irq, old_handler) != 0)
        return FALSE;
    if (_go32_dpmi_free_iret_wrapper(new_handler) != 0)
        return FALSE;

    return TRUE;
}

