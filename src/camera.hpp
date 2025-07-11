#pragma once
#include <SFML/Graphics.hpp>

#include "quaternion.hpp"

namespace raymarch
{
    class Camera
    {
    public:
        Camera(const sf::Vector2f &resolution, const sf::Vector3f &position, const sf::Vector3f &lookAt, float fov,
               float aspectRatio, float zoom);

        // void rotate(const sf::Vector3f &delta);
        void translate(const sf::Vector3f &delta);

        // sf::Glsl::Mat3 getRotationMatrix() const;
        // sf::Glsl::Vec3 getPosition() const;
    private:
        sf::Vector2f _resolution;
        sf::Vector3f _position;
        Quaternion _quaternion;
        sf::Vector3f _up = sf::Vector3f(0, 1, 0);
        float _fov;
        float _aspectRatio;
        float _zoom;
    };
}
