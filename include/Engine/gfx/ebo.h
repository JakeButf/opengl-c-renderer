#ifndef EBO_H
#define EBO_H

#include "gfx.h"

typedef struct
{
    GLuint ebo;
}EBO;

void ebo_create(EBO* ebo);
void ebo_free(EBO* ebo);

#endif