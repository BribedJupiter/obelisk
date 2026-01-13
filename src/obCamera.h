#ifndef OBCAMERA_H
#define OBCAMERA_H

#include <glm/glm.hpp>

class Camera {
    public:
        enum MOVEMENT {
            FORWARD,
            BACKWARD,
            RIGHT,
            LEFT,
            VELOCITY
        };

        // Setup the projection matrix and initalize camera values
        Camera(glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f));

        // Return an updated view matrix for use in shaders
        glm::mat4 getView();

        // Return the projection matrix
        glm::mat4 getProjection();

        // Return the camera's position
        glm::vec3 getPosition() {
            return cameraPos;
        }

        // Update view matrix in response to player movement
        void applyMovement(MOVEMENT direction, float deltaTime);

        // Update view matrix in response to player mouse scrolling
        void applyZoom(float delta);

        // Update view matrix in response to player mouse movement
        void applyRotation(glm::vec2 delta);

    private:
        // Camera vectors
        glm::vec3 cameraPos;
        glm::vec3 cameraFront;
        glm::vec3 cameraUp;

        // Owned matrices
        glm::mat4 view;
        glm::mat4 projection;

        // Camera settings
        float fov = 45.0f;
        float aspectRatio = 1920.0f / 1080.0f;
        float zNear = 0.1f;
        float zFar = 100.0f;

        // Speed settings
        float cameraSpeed = 0.005f;
        float cameraSensitivity = 0.1f;
        float cameraFriction = 0.005f;
        float cameraAccel = 0.1f;
        float velocityThreshold = 0.01f; // Needs to be greater than cameraFriction
        float maxVelocity = 1.0f;
        glm::vec3 cameraVelocity;

        // Camera rotation values
        float pitch = 0.0f;
        float yaw = -90.0f;
};

#endif