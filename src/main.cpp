#include "glad/glad.h"
#include <SFML/System/Clock.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <filesystem>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "obShader.h"
#include "obCamera.h"

#include <iostream>
#include <string>

#ifndef IS_MACOS
// Khronos debug function converted to C++ (see https://www.khronos.org/opengl/wiki/OpenGL_Error)
void GLAPIENTRY MessageCallback(GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam)
{
    std::cerr << "GL CALLBACK: " << message << std::endl;
}
#endif

int main() {
    // ---------------------
    // Initialization
    // ---------------------

    // window size
    constexpr int windowWidth = 1920;
    constexpr int windowHeight = 1080;

    // Movement flags
    char MOVE_FORWARD = 1 << 0; // 1
    char MOVE_BACKWARD = 1 << 1; // 2
    char MOVE_LEFT = 1 << 2; // 4
    char MOVE_RIGHT = 1 << 3; // 8

    // OpenGL Context Setup
    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 24;
    contextSettings.stencilBits = 8;
    contextSettings.antiAliasingLevel = 4;
    contextSettings.majorVersion = 4;
    contextSettings.minorVersion = 1;
    contextSettings.attributeFlags = contextSettings.Core;
    contextSettings.sRgbCapable = true;

    // SFML Window Setup
    sf::Window window(sf::VideoMode({windowWidth, windowHeight}), "Obelisk", sf::Style::Default, sf::State::Windowed, contextSettings);
    window.setFramerateLimit(144);
    window.setVerticalSyncEnabled(true);
    window.setMouseCursorVisible(true); // Needed until mouse movement is fixed
    window.setMouseCursorGrabbed(true);
    if (!window.setActive(true)) {
        return -1;
    };

    // Initialize OpenGL extension loader
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(sf::Context::getFunction))) {
        return -1;
    }

    // Start the SFML clock
    sf::Clock clock;

#ifndef IS_MACOS
    // Enable debug output (see https://www.khronos.org/opengl/wiki/OpenGL_Error)
    glEnable( GL_DEBUG_OUTPUT );
    glDebugMessageCallback( MessageCallback, 0 );
#endif

    // GL Setup
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // ---------------------
    // Vertex Data
    // ---------------------

    // See the LearnOpenGL textbook
    // Store a cube's vertices
    // Cube vertices + normal vectors (normally will calculate with cross product)
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    // Create a vertex array object (VAO) to store vertex attribute states
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // Create a vertex buffer object to store the vertex data
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Link the vertex attributes
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // ---------------------
    // Lighting
    // ---------------------
    glm::vec3 lightPos(1.2f, 1.0f, -5.0f);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // We can reuse the previous buffer 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Create  OpenGL textures
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // ---------------------
    // Textures
    // ---------------------

    // Setup texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Flip stbi images
    stbi_set_flip_vertically_on_load(true);

    // Load image texture
    int width, height, nrChannels;
    std::string containerPath = std::filesystem::path(TEXTURE_PATH).string() + "/container.jpg";
    unsigned char *data = stbi_load(containerPath.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        // Generate actual texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "TEXTURE1::FAILED_TO_LOAD" << std::endl;
    }

    // Now free the image data since it has been loaded into OpenGL
    stbi_image_free(data);

    // Texture 2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // Load second image texture
    std::string facePath = std::filesystem::path(TEXTURE_PATH).string() + "/awesomeface.png";
    data = stbi_load(facePath.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        // Generate actual texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "TEXTURE2::FAILED_TO_LOAD" << std::endl;
    }

    // Now free the image data since it has been loaded into OpenGL
    stbi_image_free(data);

    // ---------------------
    // Camera
    // ---------------------

    // Camera
    Camera cam = Camera();

    // Timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // Model matrix that we'll modify for each cube
    glm::mat4 model = glm::mat4(1.0f);

    // ---------------------
    // Shaders
    // ---------------------

    // Load and compile our shaders
    Shader litShader("/basic.vert", "/litObject.frag");
    glm::vec3 lightColor = {1.0f, 0.5f, 0.31f};
    glm::vec3 objectColor = {1.0f, 1.0f, 1.0f};
    litShader.use();
    litShader.setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
    litShader.setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
    litShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    litShader.setFloat("material.shininess", 32.0f);
    litShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    // For the light source
    Shader sourceShader("/basic.vert", "/light.frag");

    bool running = true;
    bool focused = true;
    while (running) {
        float currentFrame = static_cast<float>(clock.getElapsedTime().asMilliseconds());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Reset the moust to the center every frame
        if (focused) {
            sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2));
        }

        char movement = 0; 
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                running = false;
            } 
            else if (const auto* resized = event->getIf<sf::Event::Resized>())
            {
                glViewport(0, 0, resized->size.x, resized->size.y);
            }

            // if (event->is<sf::Event::FocusLost>()) {
            //     window.setMouseCursorVisible(true);
            //     window.setMouseCursorGrabbed(false);
            //     focused = false;
            // } 
            // else if (event->is<sf::Event::FocusGained>()) {
            //     window.setMouseCursorVisible(false);
            //     window.setMouseCursorGrabbed(true);
            //     focused = true;
            // }

            if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->scancode == sf::Keyboard::Scancode::Escape) {
                    running = false;
                }

                if (key->scancode == sf::Keyboard::Scancode::T) {
                    // Toggle wireframe draw
                    static bool showWires = true; 
                    if (showWires) {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    } else {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    }
                    showWires = !showWires;
                }

                if (key->scancode == sf::Keyboard::Scancode::W) {
                    std::cout << "Pressed W" << std::endl;
                    movement |= MOVE_FORWARD;
                }
                if (key->scancode == sf::Keyboard::Scancode::S) {
                    movement |= MOVE_BACKWARD;
                }
                if (key->scancode == sf::Keyboard::Scancode::A) {
                    movement |= MOVE_LEFT;
                }
                if (key->scancode == sf::Keyboard::Scancode::D) {
                    movement |= MOVE_RIGHT;
                }
            }

            if (const auto* key = event->getIf<sf::Event::KeyReleased>()) {
                if (key->scancode == sf::Keyboard::Scancode::W) {
                    std::cout << "Released W" << std::endl;
                    movement &= ~MOVE_FORWARD;
                }
                if (key->scancode == sf::Keyboard::Scancode::S) {
                    movement &= ~MOVE_BACKWARD;
                }
                if (key->scancode == sf::Keyboard::Scancode::A) {
                    movement &= ~MOVE_LEFT;
                }
                if (key->scancode == sf::Keyboard::Scancode::D) {
                    movement &= ~MOVE_RIGHT;
                }
            }

            if (const auto* moved = event->getIf<sf::Event::MouseMoved>()) {       
                sf::Vector2i center = static_cast<sf::Vector2i>(window.getSize() / 2u);
                sf::Vector2i mousePos = moved->position;
                sf::Vector2i delta = mousePos - center;

                cam.applyRotation(glm::vec2(delta.x, delta.y));

                sf::Mouse::setPosition(center, window);
            }

            if (const auto* scrolled = event->getIf<sf::Event::MouseWheelScrolled>()) {
                cam.applyZoom(static_cast<float>(scrolled->delta));
            }
        }

        // Apply player movement
        if ((movement & MOVE_FORWARD) == MOVE_FORWARD) {
            std::cout << "Moving forward" << std::endl;
            cam.applyMovement(Camera::MOVEMENT::FORWARD, deltaTime);
        }
        if ((movement & MOVE_BACKWARD) == MOVE_BACKWARD) {
            cam.applyMovement(Camera::MOVEMENT::BACKWARD, deltaTime);
        }
        if ((movement & MOVE_LEFT) == MOVE_LEFT) {
            cam.applyMovement(Camera::MOVEMENT::LEFT, deltaTime);
        }
        if ((movement & MOVE_RIGHT) == MOVE_RIGHT) {
            cam.applyMovement(Camera::MOVEMENT::RIGHT, deltaTime);
        }

        // Clear buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Prepare to draw
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // Light color
        glm::vec3 lightColor;
        lightColor.x = sin(clock.getElapsedTime().asSeconds() * 2.0f);
        lightColor.y = sin(clock.getElapsedTime().asSeconds() * 0.7f);
        lightColor.z = sin(clock.getElapsedTime().asSeconds() * 1.3f);
        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

        // Cube 1 - light source
        glBindVertexArray(lightVAO);
        model = glm::mat4(1.0f); // reset
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        sourceShader.use();
        sourceShader.setMat4("model", model);
        sourceShader.setMat4("view", cam.getView());
        sourceShader.setMat4("projection", cam.getProjection());
        // sourceShader.setVec3("lightColor", diffuseColor); // This doesn't work as intended
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Cube 2
        glBindVertexArray(VAO); // Remembers which buffers are bound already automatically
        model = glm::mat4(1.0f); // reset
        model = glm::translate(model, glm::vec3(0, -1, -3));
        float angle = 20.0f * 2 * static_cast<float>(clock.getElapsedTime().asSeconds());
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        litShader.use();
        litShader.setMat4("model", model);
        litShader.setMat4("view", cam.getView());
        litShader.setMat4("projection", cam.getProjection());
        litShader.setVec3("light.position", lightPos);
        litShader.setVec3("light.ambient", ambientColor);
        litShader.setVec3("light.diffuse", diffuseColor);
        litShader.setVec3("viewPos", cam.getPosition());
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Unbind current VAO
        glBindVertexArray(0);

        // End the frame (internally swaps front and back buffers)
        window.display();
    }
}