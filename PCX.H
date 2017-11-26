#ifndef DGL_PCX_H_INCLUDED
#define DGL_PCX_H_INCLUDED

#include "common.h"
#include "gfx.h"

SURFACE* pcx_load(const char *filename, byte *pcx_palette);
boolean pcx_save(const char *filename, const SURFACE *src, const byte *palette);

#endif

