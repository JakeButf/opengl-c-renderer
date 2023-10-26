#ifndef GFX_WINDOW_H
#define GFX_WINDOW_H

#include "gfx.h"
typedef struct {
    Skeleton s;
    Skeleton* skeleton;
}Window;

void InitWindow();

#endif