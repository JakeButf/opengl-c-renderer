#ifndef WORLD_H
#define WORLD_H
#define WORLD_SIZE 15
#define NOISE_OCTAVES 4
#define PERSISTENCE 0.5f
#define FNL_IMPL
#include "FastNoiseLite.h"

#include "chunk.h"

typedef struct
{
    Chunk* chunks[WORLD_SIZE][WORLD_SIZE];
} World;


World* CreateWorld();
Chunk* GetChunkAt(World* world, int x, int z);
void DrawWorld(World* world, GLuint shader_program, GLuint texture_atlas);
void FreeWorld(World* world);
float GetCombinedNoise(fnl_state* noise, float x, float y);
float* GenerateWorldNoise(int seed);

#endif