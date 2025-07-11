#include <iostream>
#include <SFML/Graphics.hpp>

#include "camera.hpp"
#include "events.hpp"
#include "config.hpp"

int main()
{
    // Creating window
    auto window = sf::RenderWindow(sf::VideoMode(config::windowSize), "Fractal SFML", (config::isFullscreen) ? sf::State::Fullscreen : sf::State::Windowed);
    window.setFramerateLimit(config::maxFrameRate);

    // Fullscreen-Quad that is the "canvas" for our renderer
    sf::RectangleShape fullScreenQuad {config::windowSizeF};
    fullScreenQuad.setFillColor(sf::Color::Red);

    // UV shader
    sf::Shader shader;
    if (!shader.loadFromFile("shaders/uv.frag", sf::Shader::Type::Fragment))
    {
        std::cerr << "Failed to load fragment shader" << std::endl;
        return 1;
    }
    shader.setUniform("iResolution", config::windowSizeF);

    // Camera
    constexpr sf::Vector3f cameraPosition {0, 0, 0};
    constexpr sf::Vector3f cameraTarget {0, 0, 0};
    constexpr float fov = 60;
    const float aspectRatio = config::windowSizeF.x / config::windowSizeF.y;
    raymarch::Camera camera { config::windowSizeF, cameraPosition, cameraTarget, fov, aspectRatio, 1.0f };

    // Loop
    while (window.isOpen())
    {
        // Processing window events
        processEvents(window, fullScreenQuad, shader);

        // Clearing the window content
        window.clear();

        // Fullscreen quad
        window.draw(fullScreenQuad, &shader);

        // Displaying the frame
        window.display();
    }
}
