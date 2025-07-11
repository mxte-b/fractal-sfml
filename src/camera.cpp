#include "camera.hpp"

raymarch::Camera::Camera(const sf::Vector2f &resolution, const sf::Vector3f &position, const sf::Vector3f &lookAt, const float fov, const float aspectRatio, const float zoom) :
    _resolution(resolution),
    _position(position),
    _quaternion(Quaternion::identity()),
    _fov(fov),
    _aspectRatio(aspectRatio),
    _zoom(zoom)
{
}

void raymarch::Camera::translate(const sf::Vector3f &delta)
{
    this->_position += delta;
}

// void raymarch::Camera::rotate(const sf::Vector3f &delta)
// {
//
// }
//
// sf::Glsl::Vec3 raymarch::Camera::getPosition() const
// {
//
// }
//
// sf::Glsl::Mat3 raymarch::Camera::getRotationMatrix() const
// {
//
// }

