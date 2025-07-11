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
    return Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
}

