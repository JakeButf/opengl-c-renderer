#ifndef CHUNK_H
#define CHUNK_H
#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 256
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm.h>
#include "Engine/gfx/model.h"
#include "Engine/gfx/gfx.h"


typedef enum
{
    AIR,
    DIRT,
    GRASS
}BlockType;

typedef struct 
{
    BlockType type;
    bool isVisible;
    Model* model;
    vec3 position;
}Block;

typedef struct
{
    Block blocks[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE];
    vec3 position;
    int verticeCount;
    int indicesCount;
    GLfloat* vertices;
    GLuint* indices;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
}Chunk;

typedef enum
{
    DIRECTION_X_POS,
    DIRECTION_X_NEG,
    DIRECTION_Y_POS,
    DIRECTION_Y_NEG,
    DIRECTION_Z_POS,
    DIRECTION_Z_NEG,
}FaceDirection;

Chunk* CreateChunk(vec3 position, float* noise);
void CreateChunkMesh(Chunk* chunk);
void DrawChunk(Chunk* chunk, GLuint shader_program, GLuint texture_atlas);
void FreeChunk(Chunk* chunk);
Model* CreateCubeModel();
void AddFace(Chunk* chunk, int x, int y, int z, FaceDirection faceDirection);

#endif