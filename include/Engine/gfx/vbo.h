#ifndef VBO_H
#define VBO_H

#include "gfx.h"

typedef struct
{
    GLuint vbo;
}VBO;

void vbo_create(VBO* vbo);
void vbo_free(VBO* vbo);

#endif