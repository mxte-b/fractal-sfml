#pragma once

namespace raymarch
{
    class Quaternion
    {
    public:
        Quaternion(float x, float y, float z, float w);
        static Quaternion identity();
    private:
        float x, y, z, w;
    };
}
