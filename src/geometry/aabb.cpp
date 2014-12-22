#include "geometry/aabb.h"

#include <algorithm>

#include "utils/utils.h"

AABB::AABB() :  mCenter(0.0f),
                mExtents(0.0f) {}

AABB::AABB(const glm::vec3& center, const glm::vec3& extents) : mCenter(center),
                                                                mExtents(extents) {}

AABB AABB::operator * (const glm::mat4& matrix) const
{
    glm::vec3 minA = mCenter - mExtents;
    glm::vec3 minB;

    glm::vec3 maxA = mCenter + mExtents;
    glm::vec3 maxB;

    for (unsigned int i=0; i<3; ++i)
    {
        minB[i] = matrix[3][i];
        maxB[i] = matrix[3][i];

        for (unsigned int j=0; j<3; ++j)
        {
            float x = minA[j] * matrix[j][i];
            float y = maxA[j] * matrix[j][i];

            minB[i] += std::min(x, y);
            maxB[i] += std::max(x, y);
        }
    }

    glm::vec3 center = maxB - minB / 2.0f;

    return AABB(center, maxB - center);
}

bool collides(const AABB& aabb1, const AABB& aabb2)
{
    return glm::abs(aabb1.mCenter - aabb2.mCenter) < aabb1.mExtents + aabb2.mExtents;
}
