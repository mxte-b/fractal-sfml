#include "eventhandler.hpp"

#include "config.hpp"
#include "inputhandler.hpp"


raymarch::EventHandler::EventHandler(sf::RenderWindow &window, sf::RectangleShape &fullScreenQuad, sf::Shader &shader, Camera &camera):
_window(window),
_fullScreenQuad(fullScreenQuad),
_shader(shader),
_camera(camera)
{}

void raymarch::EventHandler::handleEvents(const float deltaTime) const
{
    // Handling camera rotation
    sf::Vector3f movementVector = InputHandler::getNormalizedMovement();
    sf::Vector3f rotationVector = InputHandler::getNormalizedRotation(_window);
    float zoomDelta = 0;

    // Handling generic window events
    _window.handleEvents(
        [&](const sf::Event::Closed&)
        {
            _window.close();
        },
        [&](const sf::Event::Resized&)
        {
            // Getting new size of window and updating config variables
            config::windowSize = _window.getSize();
            config::windowSizeF = static_cast<sf::Vector2f>(config::windowSize);
            config::windowCenter = {static_cast<int>(config::windowSize.x / 2), static_cast<int>(config::windowSize.y / 2)};

            // Updating FSQ size
            _fullScreenQuad.setSize(config::windowSizeF);

            // Updating viewport size
            sf::View view = _window.getView();
            view.setSize(config::windowSizeF);
            view.setCenter(sf::Vector2f(config::windowSizeF.x / 2.f, config::windowSizeF.y / 2.f));
            _window.setView(view);

            // Updating shader uniform
            _shader.setUniform("iResolution", config::windowSizeF);
        },
        [&](const sf::Event::KeyPressed& event)
        {
            switch (event.code)
            {
                case sf::Keyboard::Key::Escape:
                    _window.close();
                    break;
                case sf::Keyboard::Key::H:
                    // TODO - Return Home
                    break;
                default:
                    break;
            }
        },
        [&](const sf::Event::MouseWheelScrolled& event)
        {
            if (event.wheel == sf::Mouse::Wheel::Vertical)
            {
                zoomDelta = event.delta;
            }
        });

    _camera.zoom(zoomDelta);
    _camera.move(movementVector, deltaTime);
    _camera.rotate(rotationVector);
}

