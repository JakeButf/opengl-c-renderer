#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../include/params.h"
#include "../include/gfx/gfx.h"

static Skeleton skeleton;
static Skeleton *s = &skeleton;

//Entry Point
int main()
{
    if(!glfwInit()) //Create GLFW
    {
        printf("Failed to init GLFW");
        return -1;
    }

    //Create Main OpenGL GLFW Window
    createWindow();

    if(!s->window)
    {
        printf("Failed to create window.");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(s->window);

    //Start GL Wrapper
    GLenum error = glewInit();
    if(GLEW_OK != error)
    {
        printf("Erm... OpenGL did not init");
    } else {
        printf("glew is up and running B)");
    }
    
    //Buffer Loop
    while(!glfwWindowShouldClose(s->window))
    {
        glfwSwapBuffers(s->window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void createWindow()
{
    int window_width = WINDOW_WIDTH;
    int window_height = WINDOW_HEIGHT;
    char window_title[] = WINDOW_TITLE;

    s->window = glfwCreateWindow(window_width, window_height, window_title, NULL, NULL);
}