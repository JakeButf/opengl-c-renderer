#ifndef INPUT_H
#define INPUT_H
#include <cglm.h>

typedef struct
{
    vec3 position;
    vec3 front;
    vec3 up;
    vec3 right;
    vec3 worldUp;

    float yaw;
    float pitch;
    float speed;
    float sensitivity;
} Camera;

void InitCamera(Camera* camera, vec3 position);
void UpdateCameraVectors(Camera* camera);
void GetViewMatrix(Camera camera, mat4 dest);

#endif