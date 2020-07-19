#ifndef LIBDGL_DGLPCX_H
#define LIBDGL_DGLPCX_H

#include "dglcmn.h"
#include "dglgfx.h"

#ifdef __cplusplus
extern "C" {
#endif

SURFACE* pcx_load(const char *filename, uint8 *pcx_palette);
bool pcx_save(const char *filename, const SURFACE *src, const uint8 *palette);

#ifdef __cplusplus
}
#endif

#endif

