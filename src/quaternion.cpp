#include <cmath>

#include "quaternion.hpp"

raymarch::Quaternion::Quaternion(const float x, const float y, const float z, const float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

raymarch::Quaternion raymarch::Quaternion::identity()
{
    return {0.0f, 0.0f, 0.0f, 1.0f};
}

float raymarch::Quaternion::dot(const Quaternion &q1, const Quaternion &q2)
{
    return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
}

sf::Vector3f cross(const sf::Vector3f &v1, const sf::Vector3f &v2)
{
    return {
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    };
}

sf::Vector3f raymarch::Quaternion::rotate(const sf::Vector3f &v) const
{
    // Vector part of the quaternion
    const sf::Vector3f quatVector{this->x, this->y, this->z};

    // Scalar part
    const float scalar = this->w;

    // Temporary variable
    const sf::Vector3f t = 2.0f * cross(quatVector, v);

    return v + scalar * t + cross(quatVector, t);
}

raymarch::Quaternion raymarch::Quaternion::normalize() const
{
    // Scale the quaternions parts by the magnitude to normalize
    const float magnitude = sqrtf(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);

    if (magnitude == 0.0f) return {0.0f, 0.0f, 0.0f, 1.0f};

    return {this->x / magnitude, this->y / magnitude, this->z / magnitude, this->w / magnitude};
}

raymarch::Quaternion raymarch::Quaternion::negative() const
{
    // Negates every part of the quaternion
    return {-this->x, -this->y, -this->z, -this->w};
}

raymarch::Quaternion raymarch::Quaternion::conjugate() const
{
    return { -this->x, -this->y, -this->z, this->w };
}

sf::Glsl::Mat3 raymarch::Quaternion::toMatrix() const
{
    // Converting the quaternion to a 3D rotation matrix
    float array[9] = {
        1.0f - 2.0f * (this->y * this->y + this->z * this->z), 2.0f * (this->x * this->y + this->w * this->z),
        2.0f * (this->x * this->z - this->y * this->w),
        2.0f * (this->x * this->y - this->w * this->z), 1.0f - 2.0f * (this->x * this->x + this->z * this->z),
        2.0f * (this->y * this->z + this->x * this->w),
        2.0f * (this->x * this->z + this->y * this->w), 2.0f * (this->y * this->z - this->x * this->w),
        1.0f - 2.0f * (this->x * this->x + this->y * this->y)
    };

    return sf::Glsl::Mat3(array);
}

raymarch::Quaternion raymarch::Quaternion::operator*(const Quaternion &q) const
{
    // Quaternion multiplication
    float w = this->w * q.w - this->x * q.x - this->y * q.y - this->z * q.z;
    float x = this->w * q.x + this->x * q.w + this->y * q.z - this->z * q.y;
    float y = this->w * q.y - this->x * q.z + this->y * q.w + this->z * q.x;
    float z = this->w * q.z + this->x * q.y - this->y * q.x + this->z * q.w;

    return {x, y, z, w};
}

raymarch::Quaternion raymarch::Quaternion::operator*(float f) const
{
    return {f * this->x, f * this->y, f * this->z, f * this->w};
}

raymarch::Quaternion raymarch::Quaternion::operator+(const Quaternion &q) const
{
    return {this->x + q.x, this->y + q.y, this->z + q.z, this->w + q.w};
}

raymarch::Quaternion raymarch::Quaternion::slerp(const Quaternion &q1, const Quaternion &q2, float t)
{
    // Normalize quaternions
    Quaternion q1n = q1.normalize();
    Quaternion q2n = q2.normalize();

    float dotProduct = dot(q1n, q2n);
    dotProduct = fmaxf(-1.0f, fminf(dotProduct, 1.0f));

    // Take the shortest path
    if (dotProduct < 0.0f)
    {
        q1n = q1n.negative();
        dotProduct = -dotProduct;
    }

    const float theta = acosf(dotProduct);
    const float sinTheta = sinf(theta);

    if (constexpr float EPSILON = 1e-6f; sinTheta < EPSILON) {
        return (q1n * (1.0f - t) + q2n * t).normalize(); // LERP fallback
    }

    // https://en.wikipedia.org/wiki/Slerp#Geometric_slerp
    const float a = sinf((1.0f - t) * theta) / sinTheta;
    const float b = sinf(t * theta) / sinTheta;

    return (q1n * a + q2n * b).normalize();
}
