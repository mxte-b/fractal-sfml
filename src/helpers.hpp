#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "camera.hpp"


void updateShader(sf::Shader& shader, const raymarch::Camera& camera, float iTime);
std::string getDateTimeString();