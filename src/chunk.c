#include "../include/chunk.h"
#include "cube.c"

extern float GenerateLakeNoise(fnl_state* noise, float x, float y);

Chunk* CreateChunk(vec3 position, float* worldNoise)
{
    Chunk* chunk = malloc(sizeof(Chunk));
    glm_vec3_copy(position, chunk->position);
    chunk->biome = GetNewBiome();

    chunk->indicesCount = 0;
    chunk->verticeCount = 0;
    float* heightNoise = worldNoise;

    chunk->vertices = malloc(sizeof(GLfloat) * CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT * 20 * 15);
    chunk->indices = malloc(sizeof(GLuint) * CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT * 6 * 8);

    glGenVertexArrays(1, &chunk->vao);
    glGenBuffers(1, &chunk->vbo);
    glGenBuffers(1, &chunk->ebo);

    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int z = 0; z < CHUNK_SIZE; z++)
        {
            float scaledNoise = (heightNoise[z * CHUNK_SIZE + x] + 1) / 2.0; // map to 0-1
            float targetHeight = CHUNK_HEIGHT / 3 + scaledNoise * CHUNK_HEIGHT / 2;

            for (int y = 0; y < CHUNK_HEIGHT; y++)
            {
                if(y == (int)targetHeight)
                {
                    chunk->blocks[x][y][z].type = chunk->biome->topBlock;
                }
                else if(y > targetHeight) 
                {
                    chunk->blocks[x][y][z].type = AIR;
                } else
                {
                    chunk->blocks[x][y][z].type = DIRT;
                }
                
            }
        }
    }
    //Lake Gen
    if(chunk->biome->genLakes)
    {
        for (int x = 0; x < CHUNK_SIZE; x++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                for (int y = 0; y <= SEALEVEL; y++)
                {
                    if (chunk->blocks[x][y][z].type == AIR && (y == CHUNK_HEIGHT - 1 || chunk->blocks[x][y+1][z].type == AIR || chunk->blocks[x][y+1][z].type == WATER))
                    {
                        chunk->blocks[x][y][z].type = WATER;
                    }
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
                        AddFace(chunk, x, y, z, DIRECTION_X_POS);
                    if (x == 0 || chunk->blocks[x - 1][y][z].type == AIR)
                        AddFace(chunk, x, y, z, DIRECTION_X_NEG);
                    if (y == CHUNK_HEIGHT - 1 || chunk->blocks[x][y + 1][z].type == AIR)
                        AddFace(chunk, x, y, z, DIRECTION_Y_POS);
                    if (y == 0 || chunk->blocks[x][y - 1][z].type == AIR)
                        AddFace(chunk, x, y, z, DIRECTION_Y_NEG);
                    if (z == CHUNK_SIZE - 1 || chunk->blocks[x][y][z + 1].type == AIR)
                        AddFace(chunk, x, y, z, DIRECTION_Z_POS);
                    if (z == 0 || chunk->blocks[x][y][z - 1].type == AIR)
                        AddFace(chunk, x, y, z, DIRECTION_Z_NEG);
                }
            }
        }
    }
    glBindVertexArray(chunk->vao);
    glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo);
    glBufferData(GL_ARRAY_BUFFER, chunk->verticeCount * sizeof(GLfloat), chunk->vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, chunk->indicesCount * sizeof(GLuint), chunk->indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void AddFace(Chunk* chunk, int x, int y, int z, FaceDirection faceDirection) {
    GLfloat faceVertices[20]; // Vert positions + texture coords
    GLuint faceIndices[] = {0, 1, 2, 2, 3, 0};
    
    
    int tileX = 0;
    int tileY = 0;

    //Check for index oob
    if (x >= 0 && x < CHUNK_SIZE && y >= 0 && y < CHUNK_HEIGHT && z >= 0 && z < CHUNK_SIZE) 
    {
        int *textCoords = GetTextureFromAtlas(chunk->blocks[x][y][z].type);
        switch(faceDirection)
        {
            case DIRECTION_X_POS:
            case DIRECTION_X_NEG:
            case DIRECTION_Z_NEG:
            case DIRECTION_Z_POS:
                tileX = *textCoords;
                tileY = *(textCoords + 1);
                break;
            case DIRECTION_Y_NEG:
            case DIRECTION_Y_POS:
                tileX = *(textCoords + 2);
                tileY = *(textCoords + 3);
                break;
        }
    }
    

    GLfloat tileWidth = 1.0 / 16.0;
    GLfloat u = tileX * tileWidth;
    GLfloat v = tileY * tileWidth;

    switch (faceDirection) {
    case DIRECTION_X_POS:
        // Positive X face (Counter-clockwise)
        faceVertices[0] = x + 1; faceVertices[1] = y; faceVertices[2] = z;
        faceVertices[3] = u; faceVertices[4] = v + tileWidth;

        faceVertices[5] = x + 1; faceVertices[6] = y + 1; faceVertices[7] = z;
        faceVertices[8] = u; faceVertices[9] = v;

        faceVertices[10] = x + 1; faceVertices[11] = y + 1; faceVertices[12] = z + 1;
        faceVertices[13] = u + tileWidth; faceVertices[14] = v;

        faceVertices[15] = x + 1; faceVertices[16] = y; faceVertices[17] = z + 1;
        faceVertices[18] = u + tileWidth; faceVertices[19] = v + tileWidth;
        break;

    case DIRECTION_X_NEG:
        // Negative X face (Counter-clockwise)
        faceVertices[0] = x; faceVertices[1] = y + 1; faceVertices[2] = z;
        faceVertices[3] = u; faceVertices[4] = v;

        faceVertices[5] = x; faceVertices[6] = y; faceVertices[7] = z;
        faceVertices[8] = u; faceVertices[9] = v + tileWidth;

        faceVertices[10] = x; faceVertices[11] = y; faceVertices[12] = z + 1;
        faceVertices[13] = u + tileWidth; faceVertices[14] = v + tileWidth;

        faceVertices[15] = x; faceVertices[16] = y + 1; faceVertices[17] = z + 1;
        faceVertices[18] = u + tileWidth; faceVertices[19] = v;
        break;

    case DIRECTION_Y_POS:
        // Top face (Counter-clockwise)
        faceVertices[0] = x + 1; faceVertices[1] = y + 1; faceVertices[2] = z;
        faceVertices[3] = u; faceVertices[4] = v  + tileWidth;

        faceVertices[5] = x; faceVertices[6] = y + 1; faceVertices[7] = z;
        faceVertices[8] = u; faceVertices[9] = v ;

        faceVertices[10] = x; faceVertices[11] = y + 1; faceVertices[12] = z + 1;
        faceVertices[13] = u + tileWidth; faceVertices[14] = v;

        faceVertices[15] = x + 1; faceVertices[16] = y + 1; faceVertices[17] = z + 1;
        faceVertices[18] = u + tileWidth; faceVertices[19] = v  + tileWidth;
        break;

    case DIRECTION_Y_NEG:
        // Bottom face (Counter-clockwise)
        faceVertices[0] = x; faceVertices[1] = y - 0; faceVertices[2] = z;
        faceVertices[3] = u; faceVertices[4] = v + tileWidth;

        faceVertices[5] = x + 1; faceVertices[6] = y - 0; faceVertices[7] = z;
        faceVertices[8] = u; faceVertices[9] = v;

        faceVertices[10] = x + 1; faceVertices[11] = y - 0; faceVertices[12] = z + 1;
        faceVertices[13] = u + tileWidth; faceVertices[14] = v;

        faceVertices[15] = x; faceVertices[16] = y - 0; faceVertices[17] = z + 1;
        faceVertices[18] = u + tileWidth; faceVertices[19] = v  + tileWidth;
        break;

    case DIRECTION_Z_NEG:
        // Positive Z face (Counter-clockwise)
        faceVertices[0] = x + 1; faceVertices[1] = y; faceVertices[2] = z + 0;
        faceVertices[3] = u; faceVertices[4] = v;

        faceVertices[5] = x; faceVertices[6] = y; faceVertices[7] = z + 0;
        faceVertices[8] = u; faceVertices[9] = v + tileWidth;

        faceVertices[10] = x; faceVertices[11] = y + 1; faceVertices[12] = z + 0;
        faceVertices[13] = u + tileWidth; faceVertices[14] = v + tileWidth;

        faceVertices[15] = x + 1; faceVertices[16] = y + 1; faceVertices[17] = z + 0;
        faceVertices[18] = u + tileWidth; faceVertices[19] = v;
        break;

    case DIRECTION_Z_POS:
        // Negative Z face (Counter-clockwise)
        faceVertices[0] = x; faceVertices[1] = y; faceVertices[2] = z + 1;
        faceVertices[3] = u; faceVertices[4] = v;

        faceVertices[5] = x + 1; faceVertices[6] = y; faceVertices[7] = z + 1;
        faceVertices[8] = u; faceVertices[9] = v + tileWidth;

        faceVertices[10] = x + 1; faceVertices[11] = y + 1; faceVertices[12] = z + 1;
        faceVertices[13] = u + tileWidth; faceVertices[14] = v + tileWidth;

        faceVertices[15] = x; faceVertices[16] = y + 1; faceVertices[17] = z + 1;
        faceVertices[18] = u + tileWidth; faceVertices[19] = v;
        break;
}

    for (int i = 0; i < 20; i++) {
        chunk->vertices[chunk->verticeCount + i] = faceVertices[i];
    }

    GLuint baseIndex = chunk->verticeCount / 5;  // Since we have 3 positions + 2 UVs
    for (int i = 0; i < 6; i++) {
        faceIndices[i] += baseIndex;
    }
    
    chunk->verticeCount += 20;

    for (int i = 0; i < 6; i++) {
        chunk->indices[chunk->indicesCount + i] = faceIndices[i];
    }
    chunk->indicesCount += 6;

    
}


void DrawChunk(Chunk* chunk, GLuint shader_program, GLuint texture_atlas)
{
    if (!chunk || !chunk->vao || chunk->indicesCount == 0)
        return;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_atlas);

    GLint textureLocation = glGetUniformLocation(shader_program, "textureAtlas");
    glUniform1i(textureLocation, 0);

    mat4 matrix;
    glm_mat4_identity(matrix);
    glm_translate(matrix, chunk->position);

    GLint modelLocation = glGetUniformLocation(shader_program, "model");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, matrix[0]);

    glBindVertexArray(chunk->vao);
    glDrawElements(GL_TRIANGLES, chunk->indicesCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("OpenGL error: %d\n", err);
}
}

void FreeChunk(Chunk* chunk)
{
    free(chunk->vertices);
    free(chunk->indices);
    glDeleteBuffers(1, &chunk->vao);
    glDeleteBuffers(1, &chunk->vbo);
    glDeleteBuffers(1, &chunk->ebo);
}

int* GetTextureFromAtlas(BlockType type)
{
    static int returnArr[4]; //XTileAtlasX, XTileAtlasY, YTileAtlasX, YTileAtlasY
    switch(type)
    {
        case DIRT:
            returnArr[0] = 0;
            returnArr[1] = 0;
            returnArr[2] = 0;
            returnArr[3] = 0;
            break;
        case GRASS:
            returnArr[0] = 0;
            returnArr[1] = 0;
            returnArr[2] = 1;
            returnArr[3] = 0;
            break;
        case MOLLY:
            returnArr[0] = 2;
            returnArr[1] = 0;
            returnArr[2] = 2;
            returnArr[3] = 0;
            break;
        case WATER:
            returnArr[0] = 3;
            returnArr[1] = 0;
            returnArr[2] = 3;
            returnArr[3] = 0;
            break;
        case AIR:
            break;
        default:
            fprintf(stderr, "Block of type does not have an assigned atlas location.\n");
            break;

    }
    return returnArr;
}
void DoChunkInfo(Biome* biome)
{

}
///
///BIOMES
///
static Biome** biomes;
unsigned int biomeListSize;
Biome* GetNewBiome()
{
    float rng = (float)rand() / (float) RAND_MAX;
    if(rng < .95)
    {
        return GetBiomeFromName("PLNS");
    } else 
    {
        return GetBiomeFromName("MLLY");
    }
}

void CreateBiomes()
{
    biomeListSize = 2;
    biomes = malloc(biomeListSize * sizeof(Biome*));
    biomes[0] = CreateBiome("PLNS", GRASS, true, false);
    biomes[1] = CreateBiome("MLLY", MOLLY, true, true);
}

Biome* CreateBiome(char name[4], BlockType grassBlock, bool generateLakes, bool rare)
{
    Biome *biome = malloc(sizeof(Biome));
    strncpy(biome->name, name, 4);
    biome->topBlock = grassBlock;
    biome->genLakes = generateLakes;
    biome->rare = rare;
    return biome;
}

Biome* GetBiomeFromName(char name[4])
{
    for(int i = 0; i < biomeListSize; i++)
    {
        if(strncmp(biomes[i]->name, name, 4) == 0)
        {
            return biomes[i];
        }
    }
    return NULL;
}

void FreeBiomes()
{
    free(biomes);
}

Model* CreateCubeModel()
{
    return CreateModel(cubeVerts, sizeof(cubeVerts) / sizeof(float), cubeIndices, sizeof(cubeIndices) / sizeof(unsigned int));
}
