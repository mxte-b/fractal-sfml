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

        void rotate(const sf::Vector3f &deltaEuler);
        void translate(const sf::Vector3f &delta);
        void lookAt(const sf::Vector3f &target);

        [[nodiscard]] sf::Glsl::Mat3 getRotationMatrix() const;
        [[nodiscard]] sf::Glsl::Vec3 getPosition() const;
        [[nodiscard]] static sf::Glsl::Mat3 lookAtMatrix(const sf::Vector3f& eye, const sf::Vector3f& target, const sf::Vector3f& up);
    private:
        sf::Vector2f _resolution;
        sf::Vector3f _position;
        Quaternion _quaternion;
        sf::Vector3f _up = sf::Vector3f(0, 1, 0);
        float _fov;
        float _aspectRatio;
        float _zoom;

        [[nodiscard]] static Quaternion lookAtQuaternion(const sf::Vector3f& eye, const sf::Vector3f& target, const sf::Vector3f& up);
    };
}
