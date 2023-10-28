#ifndef WORLD_H
#define WORLD_H
#define WORLD_SIZE 2

#include "chunk.h"

typedef struct
{
    Chunk* chunks[WORLD_SIZE][WORLD_SIZE];
} World;


World* CreateWorld();
Chunk* GetChunkAt(World* world, int x, int z);
void DrawWorld(World* world, GLuint shader_program);
void FreeWorld(World* world);
float* GenerateWorldNoise(int seed);

#endif