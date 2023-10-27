#include "../include/gfx/gfx.h"
#include "../include/gfx/window.h"
#include "../include/params.h"
#include <cglm.h>

static Window window;
static Window *w = &window;

void InitWindow()
{

    w->skeleton = &w->s;
    w->focused = true;
    glm_mat4_identity(w->view);

    //vec3 translation = {0.0f, 0.0f, -3.0f};
    //glm_translate_to(w->view, translation, w->view);

    //glOrtho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
}