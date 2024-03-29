#pragma once

#include <glm/fwd.hpp>

namespace Math
{
    using Vec2 = glm::vec2;

    using Vec3 = glm::vec3;
    using DVec3 = glm::dvec3;
    using BVec3 = glm::bvec3;

    using Point = Vec3;

    using Vec4 = glm::vec4;

    using Mat3 = glm::mat3;
    using Mat4 = glm::mat4;
    using Quat = glm::quat;

    struct AABB
    {
        Vec3 min;
        Vec3 max;

        AABB() = default;
        AABB(const Vec3& min, const Vec3& max) : min(min), max(max)
        {
        }
    };

} // namespace Math