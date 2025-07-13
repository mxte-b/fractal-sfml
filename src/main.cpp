#include <cmath>
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
    shader.setUniform("maxDistance", 100.0f);
    shader.setUniform("epsilon", 0.001f);
    shader.setUniform("iterations", 100);
    shader.setUniform("power", 8.0f);

    // Camera
    constexpr sf::Vector3f cameraPosition {0.01, 0, -5};
    constexpr sf::Vector3f cameraTarget {0, 0, 2};
    constexpr float fov = 60;
    const float aspectRatio = config::windowSizeF.x / config::windowSizeF.y;
    raymarch::Camera camera { config::windowSizeF, cameraPosition, cameraTarget, fov, aspectRatio, 1.0f };

    unsigned int frameId = 0;

    // Loop
    while (window.isOpen())
    {
        // Processing window events
        processEvents(window, fullScreenQuad, shader);

        // Gradual upwards look
        camera.translate({sinf(frameId / 1000.f) * 0.01f, 0, frameId / 100000.f});
        camera.lookAt({0, 0, 0});

        // Updating shader uniforms related to the camera
        updateShader(shader, camera);

        // Clearing the window content
        window.clear();

        // Fullscreen quad
        window.draw(fullScreenQuad, &shader);

        // Displaying the frame
        window.display();

        ++frameId;
    }
}
