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
        1.0f - 2.0f * (y * y + z * z), 2.0f * (x * y + w * z), 2.0f * (x * z - y * w),
        2.0f * (x * y - w * z), 1.0f - 2.0f * (x * x + z * z), 2.0f * (y * z + x * w),
        2.0f * (x * z + y * w), 2.0f * (y * z - x * w), 1.0f - 2.0f * (x * x + y * y)
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

raymarch::Quaternion raymarch::Quaternion::fromAxisAngle(const sf::Vector3f &axis, const float angleRad)
{
    const sf::Vector3f axisNorm = axis.normalized();

    const float sinAngleHalf = sinf(angleRad * 0.5f);
    const float cosAngleHalf = cosf(angleRad * 0.5f);

    return { axisNorm.x * sinAngleHalf, axisNorm.y * sinAngleHalf, axisNorm.z * sinAngleHalf, cosAngleHalf };
}

/**
 * @brief Creates a quaternion representing a rotation from Euler angles (in radians).
 *
 * The Euler angles are applied in YXZ order:
 *   1. Yaw (rotation around Y axis)
 *   2. Pitch (rotation around X axis)
 *   3. Roll (rotation around Z axis)
 *
 * @param euler A sf::Vector3f containing Euler angles (in radians) as (pitch, yaw, roll),
 *              where:
 *              - euler.x is pitch angle (rotation about X axis)
 *              - euler.y is yaw angle (rotation about Y axis)
 *              - euler.z is roll angle (rotation about Z axis)
 *
 * @return raymarch::Quaternion The resulting quaternion representing the combined rotation.
 */
raymarch::Quaternion raymarch::Quaternion::fromEuler(const sf::Vector3f &euler)
{
    const sf::Vector3f halfAngles = euler * 0.5f;

    const float yawSin = sinf(halfAngles.y);
    const float yawCos = cosf(halfAngles.y);
    const float pitchSin = sinf(halfAngles.x);
    const float pitchCos = cosf(halfAngles.x);
    const float rollSin = sinf(halfAngles.z);
    const float rollCos = cosf(halfAngles.z);

    return {
        yawSin * pitchCos * rollCos - yawCos * pitchSin * rollSin,
        yawCos * pitchSin * rollCos + yawSin * pitchCos * rollSin,
        yawCos * pitchCos * rollSin - yawSin * pitchSin * rollCos,
        yawCos * pitchCos * rollCos + yawSin * pitchSin * rollSin,
    };
}

raymarch::Quaternion raymarch::Quaternion::fromRotationMatrix(const sf::Glsl::Mat3 &rotation)
{
    const float m00 = rotation.array[0];
    const float m01 = rotation.array[3];
    const float m02 = rotation.array[6];
    const float m10 = rotation.array[1];
    const float m11 = rotation.array[4];
    const float m12 = rotation.array[7];
    const float m20 = rotation.array[2];
    const float m21 = rotation.array[5];
    const float m22 = rotation.array[8];

    const float trace = m00 + m11 + m22;
    float x, y, z, w;

    if (trace > 0)
    {
        const float s = sqrtf(trace + 1.0f) * 2;
        w = 0.25f * s;
        x = (m21 - m12) / s;
        y = (m02 - m20) / s;
        z = (m10 - m01) / s;
    }
    else if ((m00 > m11) && (m00 > m22))
    {
        const float s = sqrtf(1.0f + m00 - m11 - m22) * 2;
        w = (m21 - m12) / s;
        x = 0.25f * s;
        y = (m01 + m10) / s;
        z = (m02 + m20) / s;
    }
    else if (m11 > m22)
    {
        const float s = sqrtf(1.0f + m11 - m00 - m22) * 2;
        w = (m02 - m20) / s;
        x = (m01 + m10) / s;
        y = 0.25f * s;
        z = (m12 + m21) / s;
    }
    else
    {
        const float s = sqrtf(1.0f + m22 - m00 - m11) * 2;
        w = (m10 - m01) / s;
        x = (m02 + m20) / s;
        y = (m12 + m21) / s;
        z = 0.25f * s;
    }

    return {x, y, z, w};
}
