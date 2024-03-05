#include "Math.hpp"

#include <cmath>

float Vec3::distance(const Vec3& other) const
{
    return sqrtf((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y) + (z - other.z) * (z - other.z));
}