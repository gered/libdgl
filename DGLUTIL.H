#ifndef DGL_DGLUTIL_H_INCLUDED
#define DGL_DGLUTIL_H_INCLUDED

#include "dglcmn.h"

#define SWAP(type, a, b) \
    do {                 \
        type __tmp = a;  \
        a = b;           \
        b = __tmp;       \
    } while (0)

#define SIGN(x) (((x) < 0) ? -1 : (((x) > 0) ? 1 : 0))

int sys_clock();
float clock_ticks_to_seconds(int clocks);

int rnd_int(int low, int high);
float rnd_float(float low, float high);

void int_enable(void);
#pragma aux int_enable = "sti"

void int_disable(void);
#pragma aux int_disable = "cli"

int fill32(byte value);
#pragma aux fill32 = \
    "mov ah, al"     \
    "shl eax, 8"     \
    "mov al, ah"     \
    "shl eax, 8"     \
    "mov al, ah"     \
    parm [eax]       \
    value [eax];

void REP_MOVSD(const void *src, void *dest, int num_dwords);
#pragma aux REP_MOVSD =    \
    "cld"                  \
    "rep movsd"            \
    parm [esi] [edi] [ecx];

void REP_MOVSB(const void *src, void *dest, int num_bytes);
#pragma aux REP_MOVSB =    \
    "cld"                  \
    "rep movsb"            \
    parm [esi] [edi] [ecx];

void REP_STOSD(int value, void *dest, int num_dwords);
#pragma aux REP_STOSD =    \
    "cld"                  \
    "rep stosd"            \
    parm [eax] [edi] [ecx];

void REP_STOSB(int value, void *dest, int num_bytes);
#pragma aux REP_STOSB =    \
    "cld"                  \
    "rep stosb"            \
    parm [eax] [edi] [ecx];

void mem_fill(void *dest, byte value, int num_bytes);
#pragma aux mem_fill =       \
    "mov ah, al"             \
    "shl eax, 8"             \
    "mov al, ah"             \
    "shl eax, 8"             \
    "mov al, ah"             \
    "mov ebx, ecx"           \
    "shr ecx, 2"             \
    "and ebx, 3"             \
    "rep stosd"              \
    "mov ecx, ebx"           \
    "rep stosb"              \
    parm [edi] [eax] [ecx]   \
    modify [eax ebx ecx edi];

void mem_fill32(void *dest, int value, int num_bytes);
#pragma aux mem_fill32 =     \
    "mov ebx, ecx"           \
    "shr ecx, 2"             \
    "and ebx, 3"             \
    "rep stosd"              \
    "mov ecx, ebx"           \
    "rep stosb"              \
    parm [edi] [eax] [ecx]   \
    modify [eax ebx ecx edi];

void mem_copy(void *dest, const void *src, int num_bytes);
#pragma aux mem_copy =     \
    "mov ebx, ecx"         \
    "shr ecx, 2"           \
    "and ebx, 3"           \
    "rep movsd"            \
    "mov ecx, ebx"         \
    "rep movsb"            \
    parm [edi] [esi] [ecx] \
    modify [eax ebx ecx];

#endif

