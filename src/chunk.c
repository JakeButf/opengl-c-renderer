#include "../include/gfx/chunk.h"
#include "cube.c"
#include "../deps/FastNoiseLite.h"

Chunk* CreateChunk(vec3 position)
{
    Chunk* chunk = malloc(sizeof(Chunk));
    glm_vec3_copy(position, chunk->position);
    
    for(int x = 0; x < CHUNK_SIZE; x++)
    {
        for(int y = 0; y < CHUNK_HEIGHT; y++) //TODO REPLACE WITH NOISE VALUE
        {
            for(int z = 0; z < CHUNK_SIZE; z++)
            {
                if(y < CHUNK_HEIGHT / 2)
                {
                    chunk->blocks[x][y][z].blockID = 1;
                    chunk->blocks[x][y][z].isVisible = true;
                } else
                {
                    chunk->blocks[x][y][z].blockID = 0;
                    chunk->blocks[x][y][z].isVisible = false;
                }

                vec3 blockPos = 
                {
                    chunk->position[0] + x,
                    chunk->position[1] + y,
                    chunk->position[2] + z
                };
                
                chunk->blocks[x][y][z].model = CreateCubeModel();
                glm_vec3_copy(blockPos, chunk->blocks[x][y][z].position);
            }
        }
    }

    return chunk;
}

void DrawChunk(Chunk* chunk, GLuint shader_program)
{
    for(int x = 0; x < CHUNK_SIZE; x++)
    {
        for(int y = 0; y < CHUNK_HEIGHT; y++)
        {
            for(int z = 0; z < CHUNK_SIZE; z++)
            {
                Block* block = &chunk->blocks[x][y][z];
                
                mat4 modelMatrix;
                glm_mat4_identity(modelMatrix);
                glm_translate(modelMatrix, block->position);

                unsigned int modelLoc = glGetUniformLocation(shader_program, "model");
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix);

                glBindVertexArray(block->model->VAO);
                glDrawElements(GL_TRIANGLES, block->model->indexCount, GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);

                //DrawModel(block->model, shader_program);
            }
        }
    }
}

void FreeChunk(Chunk* chunk)
{
    for(int x = 0; x < CHUNK_SIZE; x++)
    {
        for(int y = 0; y < CHUNK_SIZE; y++)
        {
            for(int z = 0; z < CHUNK_SIZE; z++)
            {
                FreeModel(chunk->blocks[x][y][z].model);
            }
        }
    }
    free(chunk);
}

Model* CreateCubeModel()
{
    return CreateModel(cubeVerts, sizeof(cubeVerts) / sizeof(float), cubeIndices, sizeof(cubeIndices) / sizeof(unsigned int));
}

float* CreateNoise()
{
}