#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "camera.hpp"
#include "events.hpp"
#include "config.hpp"
#include "eventhandler.hpp"

int main()
{
    // Creating window
    auto window = sf::RenderWindow(sf::VideoMode(config::windowSize), "Fractal SFML", (config::isFullscreen) ? sf::State::Fullscreen : sf::State::Windowed);
    window.setFramerateLimit(config::maxFrameRate);
    window.setMouseCursorVisible(false);
    sf::Mouse::setPosition(config::windowCenter, window);

    // Fullscreen-Quad that is the "canvas" for our renderer
    sf::RectangleShape fullScreenQuad {config::windowSizeF};
    fullScreenQuad.setFillColor(sf::Color::Red);

    // Ray-marching shader
    sf::Shader shader;
    if (!shader.loadFromFile("shaders/main.frag", sf::Shader::Type::Fragment))
    {
        std::cerr << "Failed to load fragment shader" << std::endl;
        return 1;
    }
    shader.setUniform("iResolution", config::windowSizeF);
    shader.setUniform("maxDistance", 100.0f);
    shader.setUniform("epsilon", 0.001f);
    shader.setUniform("iterations", 100);
    shader.setUniform("power", 8.0f);
    shader.setUniform("iTime", 0.0f);

    // Camera
    constexpr sf::Vector3f cameraPosition {0, 0, -4};
    constexpr sf::Vector3f cameraTarget {0, 0, 2};
    constexpr float fov = 90;
    raymarch::Camera camera { config::windowSizeF, cameraPosition, cameraTarget, fov, 1.0f };
    shader.setUniform("fov", camera.getFOV());


    // Event handler
    raymarch::EventHandler eventHandler {window, fullScreenQuad, shader, camera};

    unsigned int frameId = 0;

    sf::Clock clock;
    sf::Time previousTime = sf::Time::Zero;

    // Loop
    while (window.isOpen())
    {
        // Updating the time
        sf::Time elapsedTime = clock.getElapsedTime();
        float deltaTime = (elapsedTime - previousTime).asSeconds();
        float iTime = elapsedTime.asSeconds();
        previousTime = elapsedTime;

        // Processing window events
        // processEvents(window, fullScreenQuad, shader);
        eventHandler.handleEvents(deltaTime);

        // Updating shader uniforms related to the camera
        updateShader(shader, camera, iTime);

        // Clearing the window content
        window.clear();

        // Fullscreen quad
        window.draw(fullScreenQuad, &shader);

        // Displaying the frame
        window.display();
        ++frameId;
    }
}
