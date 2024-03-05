#pragma once

struct Vec2
{
    union {
        struct
        {
            float x, y;
        };
        float v[2];
    };
};

struct Vec3
{
    union {
        struct
        {
            float x, y, z;
        };
        struct
        {
            float r, g, b;
        };
        float v[3];
    };

    float distance(const Vec3& other) const;
};

struct Vec4
{
    union {
        struct
        {
            float x, y, z, w;
        };
        struct
        {
            float r, g, b, a;
        };
        float v[4];
    };
};