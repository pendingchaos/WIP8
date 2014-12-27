#include "geometry/aabb.h"

#include <algorithm>

#include "utils/utils.h"

AABB::AABB() :  mCenter(0.0f),
                mExtents(0.0f) {}

AABB::AABB(const glm::vec3& center, const glm::vec3& extents) : mCenter(center),
                                                                mExtents(extents) {}

bool collides(const AABB& aabb1, const AABB& aabb2)
{
    return glm::abs(aabb1.mCenter - aabb2.mCenter) < aabb1.mExtents + aabb2.mExtents;
}
