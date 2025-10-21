#ifndef OBCAMERA_H
#define OBCAMERA_H

#include <glm/glm.hpp>

class Camera {
    public:
        enum MOVE {
            FORWARD,
            BACKWARD,
            RIGHT,
            LEFT
        };

        // Setup the projection matrix and initalize camera values
        Camera();

        // Update view matrix in response to player movement
        void applyMovement(MOVE direction, float deltaTime);

        // Update view matrix in response to player mouse scrolling
        void applyZoom(float delta);

        // Update view matrix in response to player mouse movement
        void applyRotation(glm::vec2 delta);

        // Return an updated lookAt matrix for use in shaders
        void getUpdatedLookAt();

    private:
        // Owned matrices
        glm::mat4 view;
        glm::mat4 projection;

        // Camera vectors
        glm::vec3 cameraPos;
        glm::vec3 cameraFront;
        glm::vec3 cameraUp;

        // Camera settings
        float fov = 45.0f;
        float aspectRatio = 1920.0f / 1080.0f;
        float zNear = 0.1f;
        float zFar = 100.0f;

        // Speed settings
        float cameraSpeed = 0.5f;
        float cameraSensitivity = 0.1f;

        // Camera rotation values
        float pitch = 0.0f;
        float yaw = -90.0f;
};

#endif