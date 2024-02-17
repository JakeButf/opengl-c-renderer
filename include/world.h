#ifndef WORLD_H
#define WORLD_H
#define WORLD_SIZE 15
#define NOISE_OCTAVES 4
#define PERSISTENCE 0.5f
#define LAKE_FREQUENCY 0.005f
#define LAKE_THRESHOLD 0.2f
#define DEPTH_MOD 0.3f
#define MAX_LAKE_DEPTH 10
#define SEALEVEL 130
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
float GenerateLakeNoise(fnl_state* noise, float x, float y);

#endif