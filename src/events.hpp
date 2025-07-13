#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "camera.hpp"


void processEvents(sf::RenderWindow& window, sf::RectangleShape& fullScreenQuad, sf::Shader &shader);
void updateShader(sf::Shader& shader, const raymarch::Camera& camera);