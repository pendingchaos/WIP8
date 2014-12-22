#ifndef AABB_H
#define AABB_H

#include <glm/glm.hpp>

class AABB
{
    public:
        AABB();
        AABB(const glm::vec3& center, const glm::vec3& extents);

        AABB operator * (const glm::mat4& matrix) const;

        glm::vec3 mCenter;
        glm::vec3 mExtents;
};

bool collides(const AABB& aabb1, const AABB& aabb2);

#endif // AABB_H
