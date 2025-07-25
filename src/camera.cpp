#include <cmath>

#include "camera.hpp"

#include <iostream>

constexpr float PI = 3.1415927f;

sf::Vector3f lerp(const sf::Vector3f& v1, const sf::Vector3f& v2, const float t)
{
    return v1 + (v2 - v1) * t;
}

float lerp(const float a, const float b, const float t)
{
    return a + (b - a) * t;
}

raymarch::Camera::Camera(const sf::Vector2f &resolution, const sf::Vector3f &position, const sf::Vector3f &lookAt, const float fov, const float zoom) :
    _resolution(resolution),
    _position(position),
    _quaternion(lookAtQuaternion(position, lookAt, {0, 1, 0})),
    _fov(fov),
    _aspectRatio(resolution.x / resolution.y),
    _zoom(zoom)
{
}

void raymarch::Camera::translate(const sf::Vector3f &delta)
{
    if (delta.lengthSquared() == 0) return;
    this->_position += delta;
}

void raymarch::Camera::move(const sf::Vector3f &movementVector, const float deltaTime)
{
    // Apply smoothing
    sf::Vector3f globalMovement = right * movementVector.x + up * movementVector.y + forward * movementVector.z;
    globalMovement *= _movementSpeed;

    _movementDelta = lerp(_movementDelta, globalMovement, _acceleration);

    _position += _movementDelta * deltaTime;
}


sf::Glsl::Mat3 raymarch::Camera::lookAtMatrix(const sf::Vector3f &eye, const sf::Vector3f &target, const sf::Vector3f &up)
{
    const sf::Vector3f forward = (target - eye).normalized();
    const sf::Vector3f right = up.cross(forward).normalized();
    const sf::Vector3f upward = forward.cross(right).normalized();

    float values[9] = {
        right.x, right.y, right.z,
        upward.x, upward.y, upward.z,
        forward.x, forward.y, forward.z,
    };

    return sf::Glsl::Mat3(values);
}

void raymarch::Camera::lookAt(const sf::Vector3f &target)
{
    constexpr sf::Vector3f up {0, 1, 0};
    const sf::Glsl::Mat3 lookAtMatrix = Camera::lookAtMatrix(_position, target, up);
    _quaternion = Quaternion::fromRotationMatrix(lookAtMatrix).normalize();
    updateDirectionVectors();
}

raymarch::Quaternion raymarch::Camera::lookAtQuaternion(const sf::Vector3f &eye, const sf::Vector3f &target, const sf::Vector3f &up)
{
    const sf::Glsl::Mat3 lookAtMatrix = Camera::lookAtMatrix(eye, target, up);
    return Quaternion::fromRotationMatrix(lookAtMatrix).normalize();
}

void raymarch::Camera::updateDirectionVectors()
{
    // Rotate global direction vectors to obtain local directions
    up = _quaternion.rotate(UP);
    right = _quaternion.rotate(RIGHT);
    forward = _quaternion.rotate(FORWARD);
}


void raymarch::Camera::rotate(const sf::Vector3f& deltaEuler)
{
    // if (deltaEuler.lengthSquared() == 0) return;
    _rotationDelta = lerp(_rotationDelta, deltaEuler * _rotationSpeed, _rotationAcceleration);

    // Creating 3 Quaternions for each axis of rotation
    const Quaternion yawQuat = Quaternion::fromAxisAngle(up, _rotationDelta.x);
    const Quaternion pitchQuat = Quaternion::fromAxisAngle(right, _rotationDelta.y);
    const Quaternion rollQuat = Quaternion::fromAxisAngle(forward, _rotationDelta.z);

    // Rotating the camera
    _quaternion = (yawQuat * pitchQuat * rollQuat * _quaternion).normalize();

    // Updating camera-relative direction vectors
    updateDirectionVectors();
}

sf::Glsl::Vec3 raymarch::Camera::getPosition() const
{
    return {this->_position.x, this->_position.y, this->_position.z};
}

sf::Glsl::Mat3 raymarch::Camera::getRotationMatrix() const
{
    return this->_quaternion.toMatrix();
}

float raymarch::Camera::getFOV() const
{
    return _fov * PI / (180.0f * _zoom);
}

float raymarch::Camera::getAperture() const
{
    return _aperture;
}

float raymarch::Camera::getFocusDistance() const
{
    return _focusDistance;
}


bool raymarch::Camera::isMoving() const
{
    return (
        _movementDelta.lengthSquared() > 1e-4 ||
        _rotationDelta.lengthSquared() > 1e-8 ||
        abs(_zoomDelta) > 1e-4
        );
}

void raymarch::Camera::zoom(const float delta)
{
    _zoomDelta = lerp(_zoomDelta, delta * _zoomSpeed, _zoomAcceleration);
    _zoom = std::max(1.f, _zoom + _zoomDelta);
}

void raymarch::Camera::adjustAperture(const float delta)
{
    _aperture += delta;
    _aperture = std::max(0.f, _aperture);
}

void raymarch::Camera::adjustFocus(const float delta)
{
    _focusDistance += delta;
    _focusDistance = std::max(0.1f, _focusDistance);
}
