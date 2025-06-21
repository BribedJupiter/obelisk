#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

int main() {
    sf::Window window(sf::VideoMode({1920u, 1080u}), "Obelisk", sf::Style::Default, sf::State::Windowed, sf::ContextSettings());
    window.setFramerateLimit(144);
    window.setVerticalSyncEnabled(true);
    window.setActive(true);

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
        }

        // Clear buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw

        // End the frame (internally swaps front and back buffers)
        window.display();
    }

    // Clean up & release resources
}