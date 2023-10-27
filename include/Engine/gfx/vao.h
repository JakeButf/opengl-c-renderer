#ifndef VAO_H
#define VAO_H
#include "gfx.h"

/*struct VAO
{
    GLuint vao;
};*/

typedef struct
{
    GLuint vao;
}VAO;

void vao_create(VAO* vao);
void vao_free(VAO* vao);

#endif