#pragma once

#include <SFML/Graphics.hpp>

namespace raymarch
{
    class InputHandler
    {
    public:
        static sf::Vector3f getNormalizedMovement();
        static sf::Vector3f getNormalizedRotation(const sf::Window& window);
    };
}
