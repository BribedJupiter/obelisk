#include "glad/glad.h"
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <filesystem>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "obShader.h"

#include <iostream>
#include <string>

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

int main() {
    // OpenGL Context Setup
    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 24;
    contextSettings.stencilBits = 8;
    contextSettings.antiAliasingLevel = 4;
    contextSettings.majorVersion = 4;
    contextSettings.minorVersion = 6;
    contextSettings.attributeFlags = contextSettings.Default;
    contextSettings.sRgbCapable = true;

    // SFML Window Setup
    sf::Window window(sf::VideoMode({1920u, 1080u}), "Obelisk", sf::Style::Default, sf::State::Windowed, contextSettings);
    window.setFramerateLimit(144);
    window.setVerticalSyncEnabled(true);
    if (!window.setActive(true)) {
        return -1;
    };

    // Initialize OpenGL extension loader
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(sf::Context::getFunction))) {
        return -1;
    }

    // Start the SFML clock
    sf::Clock clock;

    // Enable debug output (see https://www.khronos.org/opengl/wiki/OpenGL_Error)
    glEnable( GL_DEBUG_OUTPUT );
    glDebugMessageCallback( MessageCallback, 0 );

    // GL Setup
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.0f, 0.1f, 1.0f);

    // See the LearnOpenGL textbook
    // Store the rectangle's vertices
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f, 0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f, 0.0f, 1.0f
    };

    // Store which vertices correspond to which shape
    unsigned int indices[] = {
        0, 1, 3, // triangle 1
        1, 2, 3 // triangle 2
    };

    // We want many cubes to be drawn
    glm::vec3 cubePositions[] {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    // Create a vertex array object (VAO) to store vertex attribute states
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create a vertex buffer object to store the vertex data
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Store which indices OpenGL should use to draw
    unsigned int EBO;
    glGenBuffers(1, &EBO); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Link the vertex attributes
    // Note that the previous VBO is still bound, so this will apply to that
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Create  OpenGL textures
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

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

    // Load and compile our shaders
    Shader ourShader("/vertexShader.vert", "/fragmentShader.frag");
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    // Set up space transformation matrices            
    // Local space -(model matrix)> world space -(view matrix)> view space -(projection matrix)> 
    //      clip space |vertex shader output|-(viewport transform)> screen space.
    // Model matrix
    float fov = 45.0f;
    float aspectRatio = 800.0f / 600.0f;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // view matrix
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); // translate opposite to desired movement direction
    // perspective projection matrix
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f);

    bool running = true;
    while (running) {
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
            }
        }

        // Clear buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Prepare to draw
        ourShader.use(); // must use shader program before updating uniform values
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(VAO); // Remembers which buffers are bound already automatically
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        for (unsigned int i = 0; i < 10; i++) {
            ourShader.setMat4("view", view);
            ourShader.setMat4("projection", projection);

            model = glm::mat4(1.0f); // reset
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i * static_cast<float>(clock.getElapsedTime().asSeconds());

            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0);

        // Draw
        static int xPos = 0;

        // End the frame (internally swaps front and back buffers)
        window.display();
    }

    // Clean up & release resources
}