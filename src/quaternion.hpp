#pragma once

#include <SFML/Graphics.hpp>

namespace raymarch
{
    class Quaternion
    {
    public:
        Quaternion(float x, float y, float z, float w);

        [[nodiscard]] sf::Vector3f rotate(const sf::Vector3f& v) const;
        [[nodiscard]] sf::Glsl::Mat3 toMatrix() const;
        [[nodiscard]] Quaternion normalize() const;
        [[nodiscard]] Quaternion conjugate() const;
        [[nodiscard]] Quaternion negative() const;

        Quaternion operator*(const Quaternion& q) const;
        Quaternion operator*(float f) const;
        Quaternion operator+(const Quaternion& q) const;

        static float dot(const Quaternion& q1, const Quaternion& q2);
        static Quaternion identity();
        static Quaternion slerp(const Quaternion& q1, const Quaternion& q2, float t);
        static Quaternion fromAxisAngle(const sf::Vector3f &axis, float angle);
        static Quaternion fromEuler(const sf::Vector3f &euler);
    private:
        float x, y, z, w;
    };
}
