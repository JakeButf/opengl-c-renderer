#include "../include/input.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm.h>

Camera camera;

void InitCamera(Camera* camera, vec3 position)
{
    glm_vec3_copy(position, camera->position);
    glm_vec3_copy((vec3){0.0f, 0.0f, -1.0f}, camera->front);
    glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, camera->up);
    glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, camera->worldUp);

    camera->yaw = -90.0f;
    camera->pitch = 0.0f;
    camera->speed = 2.5f;
    camera->sensitivity = 0.2f;
}

void UpdateCameraVectors(Camera* camera)
{
    vec3 front;
    front[0] = cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
    front[1] = sin(glm_rad(camera->pitch));
    front[2] = sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
    glm_normalize(front);
    glm_vec3_copy(front, camera->front);

    glm_cross(camera->front, camera->worldUp, camera->right);
    glm_normalize(camera->right);
    glm_cross(camera->right, camera->front, camera->up);
    glm_normalize(camera->up);
}

void GetViewMatrix(Camera camera, mat4 dest)
{
    vec3 center;
    glm_vec3_add(camera.position, camera.front, center);
    glm_lookat(camera.position, center, camera.up, dest);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(!w->focused)
        return;
    static float lastX = 400;
    static float lastY = 300;
    static bool firstMouse = true;

    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    xoffset *= camera.sensitivity;
    yoffset *= camera.sensitivity;

    camera.yaw += xoffset;
    camera.pitch += yoffset;

    if(camera.pitch > 89.0f)
        camera.pitch = 89.0f;
    if(camera.pitch < -89.0f)
        camera.pitch = -89.0f;

    UpdateCameraVectors(&camera);
}

void processInput(GLFWwindow* window, float deltaTime)
{
    float cameraSpeed = camera.speed * deltaTime; // deltaTime should be time between current frame and last frame
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        if(w->focused)
        {
            glfwSetInputMode(w->skeleton->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            w->focused = false;
        }
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
    {
        //TODO check if click is over window
        if(!w->focused)
        {
            w->focused = true;
            glfwSetInputMode(w->skeleton->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        vec3 scaledFront;
        glm_vec3_scale(camera.front, cameraSpeed, scaledFront);
        glm_vec3_add(camera.position, scaledFront, camera.position);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        vec3 scaledFront;
        glm_vec3_scale(camera.front, cameraSpeed, scaledFront);
        glm_vec3_sub(camera.position, scaledFront, camera.position);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        vec3 scaledRight;
        glm_vec3_scale(camera.right, cameraSpeed, scaledRight);
        glm_vec3_sub(camera.position, scaledRight, camera.position);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        vec3 scaledRight;
        glm_vec3_scale(camera.right, cameraSpeed, scaledRight);
        glm_vec3_add(camera.position, scaledRight, camera.position);
    }
}