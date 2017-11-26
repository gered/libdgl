#ifndef DGL_INTERNAL_H_INCLUDED
#define DGL_INTERNAL_H_INCLUDED

#include "common.h"
#include <dpmi.h>

#define END_OF_FUNCTION(x)     void x##_end() {}
#define LOCK_VARIABLE(x)       _go32_dpmi_lock_data((void*)&x, sizeof(x))
#define LOCK_MEMORY(ptr, len)  _go32_dpmi_lock_data((void*)(ptr), (len))
#define LOCK_FUNCTION(x)       _go32_dpmi_lock_code(x, (long)x##_end - (long)x)

static inline void _enable_interrupts(void) {
	asm volatile ("sti");
}
static inline void _disable_interrupts(void) {
	asm volatile ("cli");
}

boolean _install_irq(int irq,
                     void* irq_handler,
                     _go32_dpmi_seginfo* new_handler,
                     _go32_dpmi_seginfo* old_handler);
boolean _restore_irq(int irq,
                     _go32_dpmi_seginfo* new_handler,
                     _go32_dpmi_seginfo* old_handler);

#define REP_MOVSL(src, dest, num_dwords) \
	__asm__ __volatile__ ( \
		"cld\n\t" \
		"rep\n\t" \
 		"movsl" \
		: : "S" (src), "D" (dest), "c" (num_dwords) \
		: "%ecx", "%esi", "%edi" )

#define REP_MOVSB(src, dest, num_bytes) \
	__asm__ __volatile__ ( \
		"cld\n\t" \
		"rep\n\t" \
 		"movsb" \
		: : "S" (src), "D" (dest), "c" (num_bytes) \
		: "%ecx", "%esi", "%edi" )

#define REP_STOSL(value, dest, num_dwords) \
	__asm__ __volatile__ ( \
		"cld\n\t" \
		"rep\n\t" \
 		"stosl" \
		: : "a" (value), "D" (dest), "c" (num_dwords) \
		: "%ecx", "%edi" )

#define REP_STOSB(value, dest, num_bytes) \
	__asm__ __volatile__ ( \
		"cld\n\t" \
		"rep\n\t" \
 		"stosb" \
		: : "a" (value), "D" (dest), "c" (num_bytes) \
		: "%ecx", "%edi" )

#endif

