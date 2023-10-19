#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../include/params.h"
#include "../include/gfx/gfx.h"

static Skeleton skeleton;
static Skeleton *s = &skeleton;
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

void createWindow()
{
    int window_width = WINDOW_WIDTH;
    int window_height = WINDOW_HEIGHT;
    char window_title[] = WINDOW_TITLE;

    s->window = glfwCreateWindow(window_width, window_height, window_title, NULL, NULL);

    //These variables may not always need to be the window size, this may be changed.
    s->width = window_width;
    s->height = window_height;

    if(WIREFRAME)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
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
        printf("ERR: OpenGL did not initialize.");
    } else {
        printf("GLEW is running.\n");
        const GLubyte* version = glGetString(GL_VERSION);
        printf("OpenGL Version: %s\n", version);
    }

    //TODO: Relocate window size variables 
    glViewport(0, 0, s->width, s->height);
    //Bind buffer callback so viewport resizes with window
    glfwSetFramebufferSizeCallback(s->window, framebuffer_size_callback);
    
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
    while(!glfwWindowShouldClose(s->window))
    {
        processInput(s->window);
        //Render Commands
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0, 0, 0, 0);

        float time = glfwGetTime();
        float green = sin(time) / 2.0f + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shader_program, "vertexColor");
        glUniform4f(vertexColorLocation, 0.0f, green, 0.0f, 1.0f);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //
        glfwSwapBuffers(s->window);
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