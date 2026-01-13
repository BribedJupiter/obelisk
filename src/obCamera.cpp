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
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    return view;
}

glm::mat4 Camera::getProjection() {
    return projection;
}

void Camera::applyMovement(MOVEMENT direction, float deltaTime) {
    float speed = cameraSpeed * deltaTime;
    switch (direction) {
        case VELOCITY:
            break;
        case FORWARD:
            cameraVelocity += cameraAccel * cameraFront;
            cameraPos += speed * cameraFront;
            break;
        case BACKWARD:
            cameraVelocity -= cameraAccel * cameraFront;
            cameraPos -= speed * cameraFront;
            break;
        case LEFT:
            cameraVelocity -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraAccel;
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
            break;
        default:
        case RIGHT:
            cameraVelocity += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraAccel;
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
            break;
    }

    // Add velocity
    cameraPos += cameraVelocity;

    // Decay camera speed by friction amount
    if (cameraVelocity.x <= 0) {
        cameraVelocity.x += cameraFriction;
    } else {
        cameraVelocity.x -= cameraFriction;
    }
    if (cameraVelocity.y <= 0) {
        cameraVelocity.y += cameraFriction;
    } else {
        cameraVelocity.y -= cameraFriction;
    }
    if (cameraVelocity.z <= 0) {
        cameraVelocity.z += cameraFriction;
    } else {
        cameraVelocity.z -= cameraFriction;
    }

    // Ensure we hit zero velocity
    if (glm::abs(cameraVelocity.x) <= velocityThreshold) {
        cameraVelocity.x = 0;
    }
    if (glm::abs(cameraVelocity.y) <= velocityThreshold) {
        cameraVelocity.y = 0;
    }
    if (glm::abs(cameraVelocity.z) <= velocityThreshold) {
        cameraVelocity.z = 0;
    }

    // Ensure we don't go beyond max velocity
    if (glm::abs(cameraVelocity.x) >= maxVelocity) {
        cameraVelocity.x = maxVelocity;
    }
    if (glm::abs(cameraVelocity.y) >= maxVelocity) {
        cameraVelocity.y = maxVelocity;
    }
    if (glm::abs(cameraVelocity.z) >= maxVelocity) {
        cameraVelocity.z = maxVelocity;
    }
}

void Camera::applyZoom(float delta) {
    fov += delta;
    if (fov < 1.0f) fov = 1.0f;
    if (fov > 90.0f) fov = 90.0f;
    projection = glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);
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