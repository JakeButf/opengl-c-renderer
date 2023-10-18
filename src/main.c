#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//Entry Point
int main()
{
    if(!glfwInit()) //Create GLFW
    {
        printf("Failed to init GLFW");
        return -1;
    }

    //Create Main OpenGL GLFW Window
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World!", NULL, NULL);

    if(!window)
    {
        printf("Failed to create window.");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    //Start GL Wrapper
    GLenum error = glewInit();
    if(GLEW_OK != error)
    {
        printf("Erm... OpenGL did not init");
    } else {
        printf("glew is up and running B)");
    }
    
    //Buffer Loop
    while(!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}