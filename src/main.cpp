#include <cmath>
#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>

#include "camera.hpp"
#include "helpers.hpp"
#include "config.hpp"
#include "eventhandler.hpp"

int main()
{
    // Creating window
    auto window = sf::RenderWindow(sf::VideoMode(config::windowSize), "Fractal SFML", (config::isFullscreen) ? sf::State::Fullscreen : sf::State::Windowed);
    window.setFramerateLimit(config::maxFrameRate);
    window.setMouseCursorVisible(false);
    sf::Mouse::setPosition(config::windowCenter, window);

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> jitterDist(-0.5f, 0.5f);

    // Fullscreen-Quad that is the "canvas" for our renderer
    sf::RectangleShape fullScreenQuad {config::windowSizeF};
    fullScreenQuad.setFillColor(sf::Color::Red);

    // Accumulation texture
    // Accumulation textures (ping-pong)
    sf::RenderTexture accumulation[2]
    {
        {config::windowSize},
        {config::windowSize}
    };
    accumulation[0].clear();
    accumulation[1].clear();

    int pingpong = 0;


    // Ray-marching shader
    sf::Shader shader;
    if (!shader.loadFromFile("shaders/main.frag", sf::Shader::Type::Fragment))
    {
        std::cerr << "Failed to load fragment shader" << std::endl;
        return 1;
    }
    shader.setUniform("iResolution", config::windowSizeF);
    shader.setUniform("maxDistance", 10000.0f);
    shader.setUniform("epsilon", 0.00001f);
    shader.setUniform("iterations", 1000);
    shader.setUniform("power", 8.0f);
    shader.setUniform("iTime", 0.0f);
    shader.setUniform("lastFrame", accumulation[pingpong].getTexture());
    shader.setUniform("blendFactor", 0.95f);
    shader.setUniform("accumulate", true);

    // Camera
    constexpr sf::Vector3f cameraPosition {0.001, 0, -4};
    constexpr sf::Vector3f cameraTarget {0, 0, 2};
    constexpr float fov = 90;
    raymarch::Camera camera { config::windowSizeF, cameraPosition, cameraTarget, fov, 1.0f };


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

        // Ping-pong buffers
        int readIndex = pingpong;
        int writeIndex = 1 - pingpong;

        // Set last frame for temporal blending
        shader.setUniform("lastFrame", accumulation[readIndex].getTexture());

        sf::Vector2f jitter{
            jitterDist(rng) / config::windowSizeF.x,
            jitterDist(rng) / config::windowSizeF.y
        };

        shader.setUniform("jitter", jitter);
        // Optional: Reset accumulation if camera moved
        bool camMoved = camera.isMoving(); // Implement this in your Camera class
        shader.setUniform("accumulate", !camMoved);
        if (camMoved) {
            accumulation[writeIndex].clear(sf::Color::Black);
        }

        // Render to write buffer
        accumulation[writeIndex].clear();
        accumulation[writeIndex].draw(fullScreenQuad, &shader);
        accumulation[writeIndex].display();

        // Display result
        sf::Sprite displaySprite(accumulation[writeIndex].getTexture());
        window.clear();
        window.draw(displaySprite);
        window.display();

        // Swap
        pingpong = writeIndex;

        ++frameId;
    }
}
