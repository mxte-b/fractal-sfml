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
        void move(const sf::Vector3f &movementVector, float deltaTime);
        void translate(const sf::Vector3f &delta);
        void zoom(float delta);
        void adjustAperture(float delta);
        void adjustFocus(float delta);
        void lookAt(const sf::Vector3f &target);
        void updateDirectionVectors();
        [[nodiscard]] bool isMoving() const;

        [[nodiscard]] sf::Glsl::Mat3 getRotationMatrix() const;
        [[nodiscard]] sf::Glsl::Vec3 getPosition() const;
        [[nodiscard]] float getFOV() const;
        [[nodiscard]] float getAperture() const;
        [[nodiscard]] float getFocusDistance() const;
        [[nodiscard]] static sf::Glsl::Mat3 lookAtMatrix(const sf::Vector3f& eye, const sf::Vector3f& target, const sf::Vector3f& up);
    private:
        sf::Vector2f _resolution;
        sf::Vector3f _position;
        sf::Vector3f _movementDelta = sf::Vector3f(0, 0, 0);
        sf::Vector3f _rotationDelta = sf::Vector3f(0, 0, 0);
        Quaternion _quaternion;
        float _fov;
        float _aperture = 0.01f;
        float _focusDistance = 1.0f;
        float _aspectRatio;
        float _zoom;
        float _zoomDelta;

        float _movementSpeed = 1;
        float _rotationSpeed = 1;
        float _zoomSpeed = 0.2;

        float _acceleration = 0.03;
        float _rotationAcceleration = 0.1;
        float _zoomAcceleration = 0.05;

        // Global direction vectors
        static constexpr auto UP = sf::Vector3f(0, 1, 0);
        static constexpr auto FORWARD = sf::Vector3f(0, 0, 1);
        static constexpr auto RIGHT = sf::Vector3f(1, 0, 0);

        [[nodiscard]] static Quaternion lookAtQuaternion(const sf::Vector3f& eye, const sf::Vector3f& target, const sf::Vector3f& up);
    };
}
