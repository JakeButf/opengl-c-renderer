#ifndef GFX_WINDOW_H
#define GFX_WINDOW_H

#include "gfx.h"
#include <cglm.h>

typedef struct {
    Skeleton s;
    Skeleton* skeleton;
    mat4 view;
    mat4 proj;
    bool focused;
}Window;

void InitWindow();

#endif