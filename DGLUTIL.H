#ifndef LIBDGL_DGLUTIL_H
#define LIBDGL_DGLUTIL_H

#include "dglcmn.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SWAP(type, a, b) \
    do {                 \
        type __tmp = a;  \
        a = b;           \
        b = __tmp;       \
    } while (0)

#define SIGN(x) (((x) < 0) ? -1 : (((x) > 0) ? 1 : 0))

uint32 sys_clock();
float clock_ticks_to_seconds(uint32 clocks);

int rnd_int(int low, int high);
float rnd_float(float low, float high);

void int_enable(void);
#pragma aux int_enable = "sti"

void int_disable(void);
#pragma aux int_disable = "cli"

int32 fill32(uint8 value);
#pragma aux fill32 = \
    "mov ah, al"     \
    "shl eax, 8"     \
    "mov al, ah"     \
    "shl eax, 8"     \
    "mov al, ah"     \
    parm [al]        \
    value [eax];

void REP_MOVSD(const void *src, void *dest, uint32 num_dwords);
#pragma aux REP_MOVSD =    \
    "cld"                  \
    "rep movsd"            \
    parm [esi] [edi] [ecx];

void REP_MOVSB(const void *src, void *dest, uint32 num_bytes);
#pragma aux REP_MOVSB =    \
    "cld"                  \
    "rep movsb"            \
    parm [esi] [edi] [ecx];

void REP_STOSD(uint32 value, void *dest, uint32 num_dwords);
#pragma aux REP_STOSD =    \
    "cld"                  \
    "rep stosd"            \
    parm [eax] [edi] [ecx];

void REP_STOSB(uint32 value, void *dest, uint32 num_bytes);
#pragma aux REP_STOSB =    \
    "cld"                  \
    "rep stosb"            \
    parm [eax] [edi] [ecx];

void mem_fill(void *dest, uint8 value, uint32 num_bytes);
#pragma aux mem_fill =       \
    "mov ah, al"             \
    "mov bx, ax"             \
    "shl eax, 16"            \
    "mov ax, bx"             \
    "mov ebx, ecx"           \
    "shr ecx, 2"             \
    "and ebx, 3"             \
    "rep stosd"              \
    "mov ecx, ebx"           \
    "rep stosb"              \
    parm [edi] [al] [ecx]    \
    modify [eax ebx ecx edi];

void mem_fill32(void *dest, uint32 value, uint32 num_bytes);
#pragma aux mem_fill32 =     \
    "mov ebx, ecx"           \
    "shr ecx, 2"             \
    "and ebx, 3"             \
    "rep stosd"              \
    "mov ecx, ebx"           \
    "rep stosb"              \
    parm [edi] [eax] [ecx]   \
    modify [eax ebx ecx edi];

void mem_copy(void *dest, const void *src, uint32 num_bytes);
#pragma aux mem_copy =     \
    "mov ebx, ecx"         \
    "shr ecx, 2"           \
    "and ebx, 3"           \
    "rep movsd"            \
    "mov ecx, ebx"         \
    "rep movsb"            \
    parm [edi] [esi] [ecx] \
    modify [eax ebx ecx];

#ifdef __cplusplus
}
#endif

#endif

