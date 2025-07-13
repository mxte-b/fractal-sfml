#include "events.hpp"

#include <iostream>

#include "config.hpp"
#include "SFML/Graphics/View.hpp"

void processEvents(sf::RenderWindow &window, sf::RectangleShape &fullScreenQuad, sf::Shader &shader) {
    while (const std::optional event = window.pollEvent())
    {
        // Handle window closing
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }
        // Handling window resize
        else if (event->is<sf::Event::Resized>())
        {
            config::windowSize = window.getSize();
            config::windowSizeF = static_cast<sf::Vector2f>(config::windowSize);

            // Updating FSC size
            fullScreenQuad.setSize(config::windowSizeF);

            // Updating viewport size
            sf::View view = window.getView();
            view.setSize(config::windowSizeF);
            view.setCenter(sf::Vector2f(config::windowSizeF.x / 2.f, config::windowSizeF.y / 2.f));
            window.setView(view);

            // Updating shader uniform
            shader.setUniform("iResolution", config::windowSizeF);
        }
        // Handle keypresses
        else if (event->is<sf::Event::KeyPressed>())
        {
            if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape)
            {
                window.close();
            }
        }
    }
}

void updateShader(sf::Shader &shader, const raymarch::Camera &camera)
{
    // auto campos = camera.getPosition();
    // auto r = camera.getRotationMatrix();

    // auto look = raymarch::Camera::lookAtMatrix({0, 0, 0}, {0, 0, 1}, {0, 1, 0});
    // auto look2 = raymarch::Quaternion::fromRotationMatrix(look).toMatrix();
    shader.setUniform("camPosition", camera.getPosition());
    shader.setUniform("camRotationMatrix", camera.getRotationMatrix());
}
