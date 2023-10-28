#include "../include/world.h"

#include "chunk.c"


World* CreateWorld(int seed)
{
    World* world = malloc(sizeof(World));
    float* worldNoise = GenerateWorldNoise(seed);
    for(int x = 0; x < WORLD_SIZE; x++)
    {
        for(int z = 0; z < WORLD_SIZE; z++)
        {
            vec3 chunkLocation = {x * CHUNK_SIZE, 0, z * CHUNK_SIZE};
            float* noise = malloc(CHUNK_SIZE * CHUNK_SIZE * sizeof(float));
            for(int cz = 0; cz < CHUNK_SIZE; cz++)
            {
                for(int cx = 0; cx < CHUNK_SIZE; cx++)
                {
                    int worldIndex = ((z * CHUNK_SIZE + cz) * WORLD_SIZE * CHUNK_SIZE) + (x * CHUNK_SIZE + cx);
                    noise[cz * CHUNK_SIZE + cx] = worldNoise[worldIndex];
                }
            }
            world->chunks[x][z] = CreateChunk(chunkLocation, noise);
            free(noise);
        }
    }
    free(worldNoise);

    return world;
}

Chunk* GetChunkAt(World* world, int x, int z)
{
    if(x < 0 || x >= WORLD_SIZE || z < 0 || z >= WORLD_SIZE)
    {
        return NULL;
    }
    return world->chunks[x][z];
}

void DrawWorld(World* world, GLuint shader_program)
{
    glUseProgram(shader_program);
    for (int x = 0; x < WORLD_SIZE; x++)
    {
        for(int z = 0; z < WORLD_SIZE; z++)
        {
            DrawChunk(world->chunks[x][z], shader_program);
        }
    }
}

void FreeWorld(World* world)
{
    for(int x = 0; x < WORLD_SIZE; x++)
    {
        for(int y = 0; y < WORLD_SIZE; y++)
        {
            FreeChunk(world->chunks[x][y]);
        }
    }

    free(world);
}

float GetCombinedNoise(fnl_state* noise, float x, float y)
{
    float totalNoise = 0;
    float amp = 0.5f;
    float frequency = 1.0f;
    float maxAmplitude = 0.0f;

    for(int i = 0; i < NOISE_OCTAVES; i++)
    {
        totalNoise += fnlGetNoise2D(noise, x * frequency, y * frequency) * amp;
        maxAmplitude += amp;

        amp *= PERSISTENCE;
        frequency *= 2;
    }

    return totalNoise / maxAmplitude;
}

float* GenerateWorldNoise(int seed)
{
    float* noiseData = malloc(WORLD_SIZE * CHUNK_SIZE * WORLD_SIZE * CHUNK_SIZE * sizeof(float));
    fnl_state noise = fnlCreateState();
    noise.seed = seed;
    noise.noise_type = FNL_NOISE_PERLIN;

    int index = 0;
    for(int y = 0; y < WORLD_SIZE * CHUNK_SIZE; y++)
    {
        for(int x = 0; x < WORLD_SIZE * CHUNK_SIZE; x++)
        {
            noiseData[index++] = GetCombinedNoise(&noise, x, y);
        }
    }
    return noiseData;
}