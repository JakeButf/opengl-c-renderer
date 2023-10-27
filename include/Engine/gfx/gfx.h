#ifndef GFX_H
#define GFX_H
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../include/chunk.h"
#include "vao.h"
#include "vbo.h"
#include "ebo.h"

typedef struct {
    GLFWwindow *window;
    int width;
    int height;
}Skeleton;

//
//SHADERS
//
GLuint compile_vertex_shader(const char* shaderSource);
GLuint compile_fragment_shader(const char* shaderSource);
GLuint create_shader_program(GLuint vertexShader, GLuint fragmentShader);
char* loadShaderSource(const char* filepath);
void create_window(Skeleton* s);

//void update_window(Window* w);
#endif