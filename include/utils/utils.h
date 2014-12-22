#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

#include <glm/glm.hpp>

template <typename T>
T safe_normalize(T v)
{
    float length = glm::length(v);

    if (length == 0.0f)
    {
        return v;
    }

    return v / length;
}

inline bool operator < (const glm::vec3& v1, const glm::vec3& v2)
{
    return v1.x < v2.x and v1.y < v2.y and v1.z < v2.z;
}

inline bool operator > (const glm::vec3& v1, const glm::vec3& v2)
{
    return v1.x > v2.x and v1.y > v2.y and v1.z > v2.z;
}

std::vector<std::string> getBacktrace();

#endif // UTILS_H
