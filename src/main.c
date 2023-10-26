#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../include/params.h"
#include "../include/gfx/gfx.h"
#include "../include/gfx/window.h"
#include "../include/gfx/model.h"
#include "window.c";
#include "cube.c";
#include "model.c"

#include <cglm.h>

mat4 globalProjectionMatrix;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    glm_perspective(glm_rad(45.0f), (float)width / (float)height, 0.1f, 100.0f, globalProjectionMatrix);
}

//Entry Point
int main()
{
    if(!glfwInit()) //Create GLFW
    {
        printf("Failed to init GLFW");
        return -1;
    }

    //Create Main OpenGL GLFW Window
    InitWindow();
    create_window(w->skeleton);
    

    if(!w->skeleton->window)
    {
        printf("Failed to create window.");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(w->skeleton->window);

    //Start GL Wrapper
    GLenum error = glewInit();
    if(GLEW_OK != error)
    {
        printf("ERR: OpenGL did not initialize.");
    } else {
        printf("GLEW is running.\n");
        const GLubyte* version = glGetString(GL_VERSION);
        printf("OpenGL Version: %s\n", version);
    }
    glEnable(GL_DEPTH_TEST);
    //TODO: Relocate window size variables 
    glViewport(0, 0, w->skeleton->width, w->skeleton->height);
    //Bind buffer callback so viewport resizes with window
    glfwSetFramebufferSizeCallback(w->skeleton->window, framebuffer_size_callback);
    glm_perspective(glm_rad(45.0f), WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f, globalProjectionMatrix);
    
    Model* testModel = CreateModel(cubeVerts, sizeof(cubeVerts) / sizeof(float), cubeIndices, sizeof(cubeIndices) / sizeof(unsigned int));

    //Shaders
    GLuint vertex_shader = compile_vertex_shader("shaders/vertex_shader.glsl");
    GLuint fragment_shader = compile_fragment_shader("shaders/fragment_shader.glsl");

    GLuint shader_program = create_shader_program(vertex_shader, fragment_shader);
    glUseProgram(shader_program);
    
    //Delete shaders now that they've been linked to the program
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    if(WIREFRAME)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //Buffer Loop
    while(!glfwWindowShouldClose(w->skeleton->window))
    {
        processInput(w->skeleton->window);
        //Render Commands
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(SKYBOX_COLOR_R / 255.0f, SKYBOX_COLOR_G / 255.0f, SKYBOX_COLOR_B / 255.0f, SKYBOX_COLOR_A / 255.0f);

        float time = glfwGetTime();

        vec3 axis = {0.5f, 1.0f, 0.0f};
        RotateModel(testModel, (float)glfwGetTime() * glm_rad(50.0f) * 100, axis);

        mat4 modelMatrix;
        GetModelMatrix(testModel, modelMatrix);

        unsigned int modelLoc = glGetUniformLocation(shader_program, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix);

        unsigned int viewLoc = glGetUniformLocation(shader_program, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, w->view[0]);

        unsigned int projLoc = glGetUniformLocation(shader_program, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, globalProjectionMatrix);
        
        float green = sin(time) / 2.0f + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shader_program, "vertexColor");
        glUniform4f(vertexColorLocation, 0.0f, green, 0.0f, 1.0f);

        DrawModel(testModel);

        glfwSwapBuffers(w->skeleton->window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, 1); //Close window
    }
}