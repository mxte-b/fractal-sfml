#pragma once
#include <SFML/Graphics.hpp>

#include "camera.hpp"

namespace raymarch
{
    class EventHandler
    {
    public:
        EventHandler(sf::RenderWindow& window, sf::RectangleShape& fullScreenQuad, sf::Shader& shader, Camera& camera);
        void handleEvents() const;
    private:
        sf::RenderWindow& _window;
        sf::RectangleShape& _fullScreenQuad;
        sf::Shader& _shader;
        Camera& _camera;
    };
}
