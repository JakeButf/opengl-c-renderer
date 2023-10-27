#include "../include/Engine/gfx/ebo.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm.h>


void ebo_create(EBO* ebo)
{
    glBindBuffer(GL_ARRAY_BUFFER, &ebo->ebo);
}

void ebo_free(EBO* ebo)
{
    glDeleteBuffers(1, &ebo->ebo);
    free(&ebo->ebo);
}