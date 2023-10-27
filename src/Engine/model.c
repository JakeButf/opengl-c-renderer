#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../include/Engine/gfx/model.h"

Model* CreateModel(const float* vertices, size_t vertexCount, const unsigned int* indices, size_t indexCount) 
{
    globalModelID++;
    Model* model = (Model*)malloc(sizeof(Model));
    if (!model) return NULL;
    model->modelID = globalModelID;
    model->vertices = (float*)malloc(vertexCount * sizeof(float));
    model->indices = (unsigned int*)malloc(indexCount * sizeof(unsigned int));

    if (!model->vertices || !model->indices) {
        free(model->vertices);
        free(model->indices);
        free(model);
        printf("Model with id %u was not created.", model->modelID);
        return NULL;
    }

    memcpy(model->vertices, vertices, vertexCount * sizeof(float));
    memcpy(model->indices, indices, indexCount * sizeof(unsigned int));

    model->vertexCount = vertexCount;
    model->indexCount = indexCount;

    // OpenGL Buffer Initialization
    glGenVertexArrays(1, &model->VAO);
    glGenBuffers(1, &model->VBO);
    glGenBuffers(1, &model->EBO);

    glBindVertexArray(model->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), model->vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), model->indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glm_mat4_identity(model->translateMatrix);
    glm_mat4_identity(model->rotateMatrix);
    glm_mat4_identity(model->scaleMatrix);

    return model;
}

void DrawModel(const Model* model, GLuint shader_program)
{
    GetModelMatrix(model);
    unsigned int modelLoc = glGetUniformLocation(shader_program, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model->modelMatrix);
    glBindVertexArray(model->VAO);
    glDrawElements(GL_TRIANGLES, model->indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void FreeModel(Model* model)
{
    glDeleteVertexArrays(1, &model->VAO);
    glDeleteBuffers(1, &model->VBO);
    glDeleteBuffers(1, &model->EBO);

    free(model->vertices);
    free(model->indices);
    free(model);
}

void MoveModel(Model* model, vec3 offset)
{
    glm_translate_make(model->translateMatrix, offset);
}

void RotateModel(Model* model, float angle, vec3 axis)
{
    glm_rotate_make(model->rotateMatrix, glm_rad(angle), axis);
}

void ScaleModel(Model* model, vec3 scale)
{
    glm_scale_make(model->scaleMatrix, scale);
}

void GetModelMatrix(Model* model)
{
    glm_mat4_identity(model->modelMatrix);
    glm_mat4_mul(model->modelMatrix, model->translateMatrix, model->modelMatrix);
    glm_mat4_mul(model->modelMatrix, model->rotateMatrix, model->modelMatrix);
    glm_mat4_mul(model->modelMatrix, model->scaleMatrix, model->modelMatrix);
}