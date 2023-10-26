#ifndef MODEL_H
#define MODEL_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm.h>

static unsigned int globalModelID = 0;

typedef struct
{
    float* vertices;
    unsigned int* indices;
    unsigned int vertexCount;
    unsigned int indexCount;
    unsigned int modelID;

    mat4 translateMatrix;
    mat4 rotateMatrix;
    mat4 scaleMatrix;

    unsigned int VAO, VBO, EBO;
}Model;

Model* CreateModel(const float* vertices, size_t vertexCount, const unsigned int* indices, size_t indexCount);
void DrawModel(const Model* model);
void FreeModel(Model* model);

void MoveModel(Model* model, vec3 offset);
void RotateModel(Model* model, float angle, vec3 axis);
void ScaleModel(Model* model, vec3 scale);
void GetModelMatrix(Model* model, mat4 dest);

#endif