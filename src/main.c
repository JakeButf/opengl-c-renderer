#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../include/params.h"
#include "../include/Engine/gfx/gfx.h"
#include "../include/Engine/gfx/window.h"
#include "../include/Engine/gfx/model.h"
#include "../include/chunk.h"
#include "Engine/window.c"
#include "Engine/model.c"
#include "Engine/input.c"


#include "world.c"

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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on Mac


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
    glfwSetCursorPosCallback(w->skeleton->window, mouse_callback);
    glfwSetInputMode(w->skeleton->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
    glm_perspective(glm_rad(45.0f), WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f, globalProjectionMatrix);
    //Shaders
    #ifdef _WIN32
    GLuint vertex_shader = compile_vertex_shader("shaders/vertex_shader_win64.glsl");
    GLuint fragment_shader = compile_fragment_shader("shaders/fragment_shader_win64.glsl");
    #endif

    #if defined(__APPLE__) && defined(__MACH__)
    GLuint vertex_shader = compile_vertex_shader("shaders/vertex_shader_osx.glsl");
    GLuint fragment_shader = compile_fragment_shader("shaders/fragment_shader_osx.glsl");
    #endif
    GLuint shader_program = create_shader_program(vertex_shader, fragment_shader);
    
    glUseProgram(shader_program);
    
    //Delete shaders now that they've been linked to the program
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    

    InitCamera(&camera, (vec3){2.0f, CHUNK_HEIGHT, 10.0f});
    UpdateCameraVectors(&camera);

    GLuint textureAtlas = load_texture_atlas("block-atlas.png");

    printf("Creating World..");

    World* world = CreateWorld(10000);

    printf("World Created. Begun Drawing.");
    //Depth Testing
    //This has to be here bc idk, it was in window create in gfx.c before but it just wasnt working lol
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    
    //Buffer Loop
    float lastFrame = 0.0f;
    while(!glfwWindowShouldClose(w->skeleton->window))
    {
        glClearColor(SKYBOX_COLOR_R / 255.0f, SKYBOX_COLOR_G / 255.0f, SKYBOX_COLOR_B / 255.0f, SKYBOX_COLOR_A / 255.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(w->skeleton->window, deltaTime);
        //Render Commands
        mat4 viewMatrix;
        GetViewMatrix(camera, viewMatrix);

        //CalculateFrameRate();

        unsigned int viewLoc = glGetUniformLocation(shader_program, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, viewMatrix[0]);

        unsigned int projLoc = glGetUniformLocation(shader_program, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, globalProjectionMatrix);
        
        float green = sin(deltaTime) / 2.0f + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shader_program, "vertexColor");
        glUniform4f(vertexColorLocation, 0.0f, green, 0.0f, 1.0f);
        //Draw Code Here
        DrawWorld(world, shader_program, textureAtlas);
        //

        glfwSwapBuffers(w->skeleton->window);
        glfwPollEvents();
    }
    FreeWorld(world);
    glfwTerminate();
    return 0;
}



void CalculateFrameRate()
{
    /*static float framesPerSecond = 0.0f;
    static int fps;
    static float lastTime = 0.0f;
    float currentTime = GetTickCount() * 0.001f;
    ++framesPerSecond;
    printf("Current Frames Per Second: %d\n\n", fps);
    if (currentTime - lastTime > 1.0f)
    {
        lastTime = currentTime;
        fps = (int)framesPerSecond;
        framesPerSecond = 0;
    }*/
}

