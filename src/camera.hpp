#pragma once
#include <SFML/Graphics.hpp>

#include "quaternion.hpp"

namespace raymarch
{
    class Camera
    {
    public:
        Camera(const sf::Vector2f &resolution, const sf::Vector3f &position, const sf::Vector3f &lookAt, float fov, float zoom);

        // Camera-relative direction vectors
        sf::Vector3f up = UP;
        sf::Vector3f forward = FORWARD;
        sf::Vector3f right = RIGHT;

        void rotate(const sf::Vector3f &deltaEuler);
        void move(const sf::Vector3f &movementDirection);
        void translate(const sf::Vector3f &delta);
        void lookAt(const sf::Vector3f &target);
        void updateDirectionVectors();

        [[nodiscard]] sf::Glsl::Mat3 getRotationMatrix() const;
        [[nodiscard]] sf::Glsl::Vec3 getPosition() const;
        [[nodiscard]] static sf::Glsl::Mat3 lookAtMatrix(const sf::Vector3f& eye, const sf::Vector3f& target, const sf::Vector3f& up);
    private:
        sf::Vector2f _resolution;
        sf::Vector3f _position;
        sf::Vector3f _movementDelta;
        sf::Vector3f _rotationDelta;
        Quaternion _quaternion;
        float _fov;
        float _aspectRatio;
        float _zoom;
        float _movementSpeed;
        float _rotationSpeed;
        float _acceleration;

        // Global direction vectors
        static constexpr auto UP = sf::Vector3f(0, 1, 0);
        static constexpr auto FORWARD = sf::Vector3f(0, 0, 1);
        static constexpr auto RIGHT = sf::Vector3f(1, 0, 0);

        [[nodiscard]] static Quaternion lookAtQuaternion(const sf::Vector3f& eye, const sf::Vector3f& target, const sf::Vector3f& up);
    };
}
