#include "eventhandler.hpp"

#include "config.hpp"


raymarch::EventHandler::EventHandler(sf::RenderWindow &window, sf::RectangleShape &fullScreenQuad, sf::Shader &shader, Camera &camera):
_window(window),
_fullScreenQuad(fullScreenQuad),
_shader(shader),
_camera(camera)
{}

void raymarch::EventHandler::handleEvents() const
{
    // Handling camera rotation
    sf::Vector2f mouseDelta2D = static_cast<sf::Vector2f>(sf::Mouse::getPosition(_window) - config::windowCenter).componentWiseDiv(config::windowSizeF) * 2.0f;
    sf::Vector3f rotationDelta = sf::Vector3f(mouseDelta2D.x, mouseDelta2D.y, 0);
    sf::Vector3f translateDelta;

    // Resetting mouse position
    sf::Mouse::setPosition(config::windowCenter, _window);

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
            sf::Vector3f movementVector {0, 0, 0};

            switch (event.code)
            {
                case sf::Keyboard::Key::Escape:
                    _window.close();
                    break;
                case sf::Keyboard::Key::W:
                    movementVector.z = 1.f;
                    break;
                case sf::Keyboard::Key::A:
                    movementVector.x = -1.f;
                    break;
                case sf::Keyboard::Key::S:
                    movementVector.z = -1.f;
                    break;
                case sf::Keyboard::Key::D:
                    movementVector.x = 1.f;
                    break;
                case sf::Keyboard::Key::Q:
                    rotationDelta.z = -0.1f;
                    break;
                case sf::Keyboard::Key::E:
                    rotationDelta.z = 0.1f;
                    break;
                default:
                    break;
            }

            // Handle camera movement (smoothed)
            translateDelta =
                _camera.right * movementVector.x +
                _camera.up * movementVector.y +
                _camera.forward * movementVector.z;

        });

    _camera.translate(translateDelta);
    _camera.rotate(rotationDelta);
}

