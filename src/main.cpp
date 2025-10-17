#include "glad/glad.h"
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

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
    glClearColor(0.1f, 0.0f, 0.1f, 1.0f);

    // See the LearnOpenGL textbook
    // Store the rectangle's vertices
    float vertices[] = {
        // 1st 3 = positions, 2nd 3 = colors
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
        0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // top right
        -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f // top left
    };

    // Store which vertices correspond to which shape
    unsigned int indices[] = {
        0, 1, 3, // triangle 1
        1, 2, 3 // triangle 2
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

    Shader ourShader("/vertexShader.glsl", "/fragmentShader.glsl");

    // Link the vertex attributes
    // Note that the previous VBO is still bound, so this will apply to that
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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

        // Update colors by time
        float timeValue = sin(static_cast<float>(clock.getElapsedTime().asSeconds()) / 2.0f) + 0.5f;

        // Prepare to draw
        ourShader.use(); // must use shader program before updating uniform values
        ourShader.setFloat("time", timeValue); // Update shader color
        glBindVertexArray(VAO); // Remembers which buffers are bound already automatically
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Draw
        static int xPos = 0;

        // End the frame (internally swaps front and back buffers)
        window.display();
    }

    // Clean up & release resources
}