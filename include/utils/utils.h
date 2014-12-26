#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
#include <cstdio>
#include <cstdint>

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

char **getBacktrace(unsigned int& depth);
void freeBacktrace(char **theBacktrace, unsigned int depth);

void writeInt8(FILE *file, char i);
void writeUInt8(FILE *file, unsigned char i);

void writeInt16(FILE *file, short i);
void writeUInt16(FILE *file, unsigned short i);

void writeInt32(FILE *file, int i);
void writeUInt32(FILE *file, unsigned int i);

void writeInt64(FILE *file, long long i);
void writeUInt64(FILE *file, unsigned long long i);

void writeSizet(FILE *file, std::size_t i);

void writeCString(FILE *file, const char *str);

char readInt8(FILE *file);
unsigned char readUInt8(FILE *file);

short readInt16(FILE *file);
unsigned short readUInt16(FILE *file);

int readInt32(FILE *file);
unsigned int readUInt32(FILE *file);

long long readInt64(FILE *file);
unsigned long long readUInt64(FILE *file);

#endif // UTILS_H
