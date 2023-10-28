#include "../include/chunk.h"
#include "cube.c"

Chunk* CreateChunk(vec3 position, float* worldNoise)
{
    Chunk* chunk = malloc(sizeof(Chunk));
    glm_vec3_copy(position, chunk->position);

    chunk->indicesCount = 0;
    chunk->verticeCount = 0;
    float* heightNoise = worldNoise;

    chunk->vertices = malloc(sizeof(GLfloat) * CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT * 16);
    chunk->indices = malloc(sizeof(GLuint) * CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT * 25);

    glGenVertexArrays(1, &chunk->vao);
    glGenBuffers(1, &chunk->vbo);
    glGenBuffers(1, &chunk->ebo);

    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int z = 0; z < CHUNK_SIZE; z++)
        {
            float scaledNoise = (heightNoise[z * CHUNK_SIZE + x] + 1) / 2.0; // map to 0-1
            float targetHeight = 2 * CHUNK_HEIGHT / 3 + scaledNoise * CHUNK_HEIGHT / 3;
            for (int y = 0; y < CHUNK_HEIGHT; y++)
            {
                if(y > targetHeight) 
                {
                    chunk->blocks[x][y][z].type = AIR;
                } else 
                {
                    chunk->blocks[x][y][z].type = DIRT;
                }
                
            }
        }
    }
    CreateChunkMesh(chunk);
    return chunk;
}

void CreateChunkMesh(Chunk* chunk)
{
    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int y = 0; y < CHUNK_HEIGHT; y++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                BlockType currentBlock = chunk->blocks[x][y][z].type;

                if (currentBlock != AIR)
                {
                    if (x == CHUNK_SIZE - 1 || chunk->blocks[x + 1][y][z].type == AIR)
                        AddFace(chunk, x + 1, y, z, DIRECTION_X_POS);
                    if (x == 0 || chunk->blocks[x - 1][y][z].type == AIR)
                        AddFace(chunk, x, y, z, DIRECTION_X_NEG);
                    if (y == CHUNK_HEIGHT - 1 || chunk->blocks[x][y + 1][z].type == AIR)
                        AddFace(chunk, x, y + 1, z, DIRECTION_Y_POS);
                    if (y == 0 || chunk->blocks[x][y - 1][z].type == AIR)
                        AddFace(chunk, x, y, z, DIRECTION_Y_NEG);
                    if (z == CHUNK_SIZE - 1 || chunk->blocks[x][y][z + 1].type == AIR)
                        AddFace(chunk, x, y, z + 1, DIRECTION_Z_POS);
                    if (z == 0 || chunk->blocks[x][y][z - 1].type == AIR)
                        AddFace(chunk, x, y, z, DIRECTION_Z_NEG);
                }
            }
        }
    }
}

void AddFace(Chunk* chunk, int x, int y, int z, FaceDirection faceDirection)
{
    GLfloat faceVertices[12];
    GLuint faceIndices[] = {0, 1, 2, 2, 3, 0};

    switch (faceDirection)
    {
    case DIRECTION_X_POS:
        faceVertices[0] = x;   faceVertices[1] = y;     faceVertices[2] = z;
        faceVertices[3] = x;   faceVertices[4] = y + 1; faceVertices[5] = z;
        faceVertices[6] = x;   faceVertices[7] = y + 1; faceVertices[8] = z + 1;
        faceVertices[9] = x;   faceVertices[10] = y;    faceVertices[11] = z + 1;
        break;

    case DIRECTION_X_NEG:
        faceVertices[0] = x - 0; faceVertices[1] = y;     faceVertices[2] = z;
        faceVertices[3] = x - 0; faceVertices[4] = y + 1; faceVertices[5] = z;
        faceVertices[6] = x - 0; faceVertices[7] = y + 1; faceVertices[8] = z + 1;
        faceVertices[9] = x - 0; faceVertices[10] = y;    faceVertices[11] = z + 1;
        break;

    case DIRECTION_Y_POS:
        faceVertices[0] = x;     faceVertices[1] = y; faceVertices[2] = z;
        faceVertices[3] = x + 1; faceVertices[4] = y; faceVertices[5] = z;
        faceVertices[6] = x + 1; faceVertices[7] = y; faceVertices[8] = z + 1;
        faceVertices[9] = x;     faceVertices[10] = y; faceVertices[11] = z + 1;
        break;

    case DIRECTION_Y_NEG:
        faceVertices[0] = x;     faceVertices[1] = y; faceVertices[2] = z;
        faceVertices[3] = x + 1; faceVertices[4] = y; faceVertices[5] = z;
        faceVertices[6] = x + 1; faceVertices[7] = y; faceVertices[8] = z + 1;
        faceVertices[9] = x;     faceVertices[10] = y; faceVertices[11] = z + 1;
        break;

    case DIRECTION_Z_POS:
        faceVertices[0] = x;     faceVertices[1] = y;     faceVertices[2] = z;
        faceVertices[3] = x;     faceVertices[4] = y + 1; faceVertices[5] = z;
        faceVertices[6] = x + 1; faceVertices[7] = y + 1; faceVertices[8] = z;
        faceVertices[9] = x + 1; faceVertices[10] = y;    faceVertices[11] = z;
        break;

    case DIRECTION_Z_NEG:
        faceVertices[0] = x;     faceVertices[1] = y;     faceVertices[2] = z;
        faceVertices[3] = x;     faceVertices[4] = y + 1; faceVertices[5] = z;
        faceVertices[6] = x + 1; faceVertices[7] = y + 1; faceVertices[8] = z;
        faceVertices[9] = x + 1; faceVertices[10] = y;    faceVertices[11] = z;
        break;
    }

    for (int i = 0; i < 12; i++)
    {
        chunk->vertices[chunk->verticeCount + i] = faceVertices[i];
    }

    for (int i = 0; i < 6; i++)
    {
        faceIndices[i] += chunk->verticeCount / 3 - 4;
    }
    
    chunk->verticeCount += 12;

    for (int i = 0; i < 6; i++)
    {
        chunk->indices[chunk->indicesCount + i] = faceIndices[i];
    }
    chunk->indicesCount += 6;

    glBindVertexArray(chunk->vao);
    glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo);
    glBufferData(GL_ARRAY_BUFFER, chunk->verticeCount * sizeof(GLfloat), chunk->vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, chunk->indicesCount * sizeof(GLuint), chunk->indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void DrawChunk(Chunk* chunk, GLuint shader_program)
{
    if (!chunk || !chunk->vao || chunk->indicesCount == 0)
        return;

    glUseProgram(shader_program);

    mat4 matrix;
    glm_mat4_identity(matrix);
    glm_translate(matrix, chunk->position);

    GLint modelLocation = glGetUniformLocation(shader_program, "model");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, matrix[0]);

    glBindVertexArray(chunk->vao);
    glDrawElements(GL_TRIANGLES, chunk->indicesCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void FreeChunk(Chunk* chunk)
{
    free(chunk->vertices);
    free(chunk->indices);
    glDeleteBuffers(1, &chunk->vao);
    glDeleteBuffers(1, &chunk->vbo);
    glDeleteBuffers(1, &chunk->ebo);
}

Model* CreateCubeModel()
{
    return CreateModel(cubeVerts, sizeof(cubeVerts) / sizeof(float), cubeIndices, sizeof(cubeIndices) / sizeof(unsigned int));
}
