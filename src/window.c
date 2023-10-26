#include "../include/gfx/gfx.h"
#include "../include/gfx/window.h"
#include "../include/params.h"
#include <cglm.h>

static Window window;
static Window *w = &window;

void InitWindow()
{
    w->skeleton = &w->s;
    glOrtho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
    mat4 proj;
    glm_perspective(glm_rad(45.0f), WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f, proj);

}