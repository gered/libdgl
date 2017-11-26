#ifndef DGL_DGL_H_INCLUDED
#define DGL_DGL_H_INCLUDED

#include "common.h"
#include "error.h"

#include "keyboard.h"
#include "mouse.h"
#include "gfx.h"
#include "clipping.h"
#include "draw.h"
#include "blit.h"
#include "mathext.h"
#include "rect.h"
#include "vector2.h"
#include "matrix33.h"
#include "util.h"
#include "pcx.h"

boolean dgl_init(void);
boolean dgl_shutdown(void);

#endif

