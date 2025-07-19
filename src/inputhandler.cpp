#include "inputhandler.hpp"
#include "config.hpp"

sf::Vector3f raymarch::InputHandler::getNormalizedMovement()
{
    sf::Vector3f movement;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) movement.z = 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) movement.z = -1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) movement.x = -1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) movement.x = 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) movement.y = 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) movement.y = -1.f;

    if (movement.lengthSquared() > 0)
    {
        return movement.normalized();
    }

    return movement;
}

sf::Vector3f raymarch::InputHandler::getNormalizedRotation(const sf::Window& window)
{
    // Getting mouse delta
    const sf::Vector2f mouseDelta2D = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window) - config::windowCenter).componentWiseDiv(config::windowSizeF) * 2.0f;

    // Resetting mouse position
    sf::Mouse::setPosition(config::windowCenter, window);

    // Getting rotation on the Z-axis (roll)
    sf::Vector3f rotation = sf::Vector3f(mouseDelta2D.x, mouseDelta2D.y, 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) rotation.z = 0.01f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) rotation.z = -0.01f;

    return rotation;
}
