#include "obCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp) : cameraPos(cameraPos), cameraFront(cameraFront), cameraUp(cameraUp) {
    view = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);
}

glm::mat4 Camera::getView() {
    // LookAt matrix - transform any vector to the camera's coordinate space by multiplying it with this and a translation camera position vector
    // Note that we have to invert rotation and translation since the world has to move, not the camera
    // glm has a lookAt function that takes care of this.
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

glm::mat4 Camera::getProjection() {
    return projection;
}

void Camera::applyMovement(MOVEMENT direction, float deltaTime) {
    float speed = cameraSpeed * deltaTime;

    switch (direction) {
        case FORWARD:
            cameraPos += speed * cameraFront;
            break;
        case BACKWARD:
            cameraPos -= speed * cameraFront;
            break;
        case LEFT:
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
            break;
        default:
        case RIGHT:
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
            break;
    }
}

void Camera::applyZoom(float delta) {
    fov += delta;
    if (fov < 1.0f) fov = 1.0f;
    if (fov > 90.0f) fov = 90.0f;
}

void Camera::applyRotation(glm::vec2 delta) {
    float xoffset = delta.x;
    float yoffset = delta.y * -1; // invert since OpenGL has y+ as up

    xoffset *= cameraSensitivity;
    yoffset *= cameraSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    // Calculate new direction value
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}