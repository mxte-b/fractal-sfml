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
    constexpr sf::Vector3f cameraPosition {0.01, 0, -4};
    constexpr sf::Vector3f cameraTarget {0, 0, 2};
    constexpr float fov = 60;
    raymarch::Camera camera { config::windowSizeF, cameraPosition, cameraTarget, fov, 1.0f };

    // Event handler
    raymarch::EventHandler eventHandler {window, fullScreenQuad, shader, camera};

    unsigned int frameId = 0;
    constexpr float sensitivity = 2.0f;

    sf::Clock clock;
    sf::Time previousTime = sf::Time::Zero;

    // Loop
    while (window.isOpen())
    {
        // Updating the time
        sf::Time elapsedTime = clock.getElapsedTime();
        previousTime = elapsedTime;
        float deltaTime = (elapsedTime - previousTime).asSeconds();
        float iTime = elapsedTime.asSeconds();

        // Getting mouse delta
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f delta = static_cast<sf::Vector2f>(mousePosition - config::windowCenter).componentWiseDiv(config::windowSizeF) * sensitivity;

        // Resetting mouse position
        sf::Mouse::setPosition(config::windowCenter, window);

        camera.rotate({delta.x, delta.y, 0});

        // Processing window events
        // processEvents(window, fullScreenQuad, shader);
        eventHandler.handleEvents();

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
