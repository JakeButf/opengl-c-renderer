#include "../include/Engine/gfx/vbo.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm.h>


void vbo_create(VBO* vbo)
{
    glBindBuffer(GL_ARRAY_BUFFER, &vbo->vbo);
}

void vbo_free(VBO* vbo)
{
    glDeleteBuffers(1, &vbo->vbo);
    free(&vbo->vbo);
}