#ifndef DGL_DGLPCX_H_INCLUDED
#define DGL_DGLPCX_H_INCLUDED

#include "dglcmn.h"
#include "dglgfx.h"

SURFACE* pcx_load(const char *filename, byte *pcx_palette);
boolean pcx_save(const char *filename, const SURFACE *src, const byte *palette);

#endif

