#include "../include/Engine/gfx/gfx.h"
#include "../include/params.h"
#include "../include/Engine/gfx/window.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint compile_vertex_shader(const char* shaderSource)
{
    const char* loadedShader = loadShaderSource(shaderSource);

    if (loadedShader == NULL) {
        fprintf(stderr, "Failed to load shader from: %s\n", shaderSource);
        return 0;
    }

    GLuint shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader, 1, &loadedShader, NULL);
    glCompileShader(shader);

    free((void*)loadedShader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        fprintf(stderr, "Vertex Shader compile error: %s at %s\n", infoLog, shaderSource);
    } else {
        printf("Vertex Shader Compiled.\n");
    }

    return shader;
}

GLuint compile_fragment_shader(const char* shaderSource)
{
    const char* loadedShader = loadShaderSource(shaderSource);

    if (loadedShader == NULL) {
        fprintf(stderr, "Failed to load shader from: %s\n", shaderSource);
        return 0;
    }
    
    GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader, 1, &loadedShader, NULL);
    glCompileShader(shader);

    free((void*)loadedShader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        fprintf(stderr, "Fragment Shader compile error: %s at %s\n", infoLog, shaderSource);
    } else 
    {
        printf("Fragment Shader Compiled.\n");
    }

    return shader;
}

GLuint create_shader_program(GLuint vertexShader, GLuint fragmentShader)
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    //Check for errors
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    
    if(!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        fprintf(stderr, "Shader program linking error: %s\n", infoLog);
    } else 
    {
        printf("Shader Program Success.\n");
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

char* loadShaderSource(const char* filepath) 
{
    FILE* file = fopen(filepath, "rb");
    if(!file) 
    {
        perror("Failed to open shader file.");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* source = (char*) malloc(filesize + 1);

    if(!source)
    {
        perror("Cannot allocate memory for shader source");
        fclose(file);
        return NULL;
    }

    fread(source, 1, filesize, file);
    source[filesize] = '\0';

    fclose(file);
    return source;
}

GLuint load_texture_atlas(const char* imageSource)
{
    int width, height, channels;
    unsigned char* data = stbi_load(imageSource, &width, &height, &channels, 0);
    if(!data)
    {
        fprintf(stderr, "Couldn't load texture atlas\n");
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(data);
    return textureID;
}

void create_window(Skeleton* s)
{
    int window_width = WINDOW_WIDTH;
    int window_height = WINDOW_HEIGHT;
    char window_title[] = WINDOW_TITLE;

    s->window = glfwCreateWindow(window_width, window_height, window_title, NULL, NULL);

    //These variables may not always need to be the window size, this may be changed.
    s->width = window_width;
    s->height = window_height;

    //glDisable(GL_CULL_FACE);


    if(WIREFRAME)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //glDisable(GL_CULL_FACE);
    }
}