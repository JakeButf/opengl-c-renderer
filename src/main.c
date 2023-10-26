#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../include/params.h"
#include "../include/gfx/gfx.h"
#include "../include/gfx/window.h"
#include "window.c";

#include <cglm.h>


//TEMP
float vertices[] = {
0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
-0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f
};

unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
};
//

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
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

    //TODO: Relocate window size variables 
    glViewport(0, 0, w->skeleton->width, w->skeleton->height);
    //Bind buffer callback so viewport resizes with window
    glfwSetFramebufferSizeCallback(w->skeleton->window, framebuffer_size_callback);
    
    //Vertex Buffer Object
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    //Vertex Array Object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindBuffer(GL_ARRAY_BUFFER, &VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3* sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //Shaders
    GLuint vertex_shader = compile_vertex_shader("shaders/vertex_shader.glsl");
    GLuint fragment_shader = compile_fragment_shader("shaders/fragment_shader.glsl");

    GLuint shader_program = create_shader_program(vertex_shader, fragment_shader);
    glUseProgram(shader_program);
    
    //Delete shaders now that they've been linked to the program
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    

    //Buffer Loop
    while(!glfwWindowShouldClose(w->skeleton->window))
    {
        processInput(w->skeleton->window);
        //Render Commands
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0, 0, 0, 0);

        float time = glfwGetTime();
        float green = sin(time) / 2.0f + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shader_program, "vertexColor");
        glUniform4f(vertexColorLocation, 0.0f, green, 0.0f, 1.0f);

        //Matrix Testing
        mat4 trans;
        glm_mat4_identity(trans);
        vec3 axis = {0.0f, 0.0f, 1.0f};
        glm_rotate(trans, (float)glfwGetTime(), axis);
        vec3 scale = {0.5, 0.5, 0.5};
        glm_scale(trans, scale);

        unsigned int transformLoc = glGetUniformLocation(shader_program, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, trans[0]);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //
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