#ifndef CHUNK_H
#define CHUNK_H
#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 256
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm.h>
#include "model.h"

typedef struct 
{
    int blockID;
    bool isVisible;
    Model* model;
    vec3 position;
}Block;

typedef struct
{
    Block blocks[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE];
    vec3 position;
    GLuint VAO;
}Chunk;

Chunk* CreateChunk(vec3 position);
void DrawChunk(Chunk* chunk, GLuint shader_program);
void FreeChunk(Chunk* chunk);
Model* CreateCubeModel();
float* CreateNoise();

#endif