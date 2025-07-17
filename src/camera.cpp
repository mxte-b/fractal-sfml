#include <cmath>

#include "camera.hpp"

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
    this->_position += delta;
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
    const sf::Glsl::Mat3 lookAtMatrix = Camera::lookAtMatrix(this->_position, target, up);
    this->_quaternion = Quaternion::fromRotationMatrix(lookAtMatrix).normalize();
}

raymarch::Quaternion raymarch::Camera::lookAtQuaternion(const sf::Vector3f &eye, const sf::Vector3f &target, const sf::Vector3f &up)
{
    const sf::Glsl::Mat3 lookAtMatrix = Camera::lookAtMatrix(eye, target, up);
    return Quaternion::fromRotationMatrix(lookAtMatrix).normalize();
}

void raymarch::Camera::updateDirectionVectors()
{
    // Rotate global direction vectors to obtain local directions
    _up = _quaternion.rotate(UP);
    _right = _quaternion.rotate(RIGHT);
    _forward = _quaternion.rotate(FORWARD);
}


void raymarch::Camera::rotate(const sf::Vector3f& deltaEuler)
{
    // Creating 3 Quaternions for each axis of rotation
    const Quaternion yawQuat = Quaternion::fromAxisAngle(UP, deltaEuler.x);
    const Quaternion pitchQuat = Quaternion::fromAxisAngle(_right, deltaEuler.y);
    const Quaternion rollQuat = Quaternion::fromAxisAngle(_forward, deltaEuler.z);

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

