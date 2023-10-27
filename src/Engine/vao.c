#include "../include/Engine/gfx/vao.h"


void vao_create(VAO* vao)
{
    glGenVertexArrays(1, &vao->vao);
}

void vao_free(VAO* vao)
{
    glDeleteVertexArrays(1, &vao->vao);
    free(&vao->vao);
}