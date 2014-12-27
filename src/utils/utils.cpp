#include "utils/utils.h"

#include <execinfo.h>
#include <cxxabi.h>
#include <dlfcn.h>
#include <cstdlib>
#include <endian.h>
#include <cstring>
#include <iostream>

#include "utils/memory.h"

char *theBacktrace[256];

char **getBacktrace(unsigned int& depth)
{
    void *trace[256];

    depth = backtrace(trace, 256);

    for (unsigned int i=0; i<depth; ++i)
    {
        Dl_info info;

        if (dladdr(trace[i], &info) == 0)
        {
            theBacktrace[i] = (char *)std::malloc(6);
            std::memcpy(theBacktrace[i], "error", 6);
            continue;
        }

        const char *name = info.dli_sname;

        if (name == NULL)
        {
            theBacktrace[i] = (char *)std::malloc(6);
            std::memcpy(theBacktrace[i], "error", 6);
            continue;
        }

        int status;

        char *demangled = abi::__cxa_demangle(name, NULL, 0, &status);

        if (status == 0 and demangled != NULL)
        {
            theBacktrace[i] = demangled;
        } else
        {
            unsigned int len = std::strlen(name);
            theBacktrace[i] = (char *)std::malloc(len+1);
            std::memcpy(theBacktrace[i], name, len+1);
        }
    }

    return theBacktrace;
}

void freeBacktrace(char **theBacktrace, unsigned int depth)
{
    for (unsigned int i=0; i<depth; ++i)
    {
        std::free(theBacktrace[i]);
    }
}

void writeInt8(FILE *file, char i)
{
    writeUInt8(file, *(unsigned char *)&i);
}

void writeUInt8(FILE *file, unsigned char i)
{
    std::fwrite(&i, 1, 1, file);
}

void writeInt16(FILE *file, short i)
{
    writeUInt16(file, *(unsigned short *)&i);
}

void writeUInt16(FILE *file, unsigned short i)
{
    i = htole16(i);

    std::fwrite(&i, 2, 1, file);
}

void writeInt32(FILE *file, int i)
{
    writeUInt32(file, *(unsigned int *)&i);
}

void writeUInt32(FILE *file, unsigned int i)
{
    i = htole32(i);

    std::fwrite(&i, 4, 1, file);
}

void writeInt64(FILE *file, long long i)
{
    writeUInt64(file, *(unsigned long long *)&i);
}

void writeUInt64(FILE *file, unsigned long long i)
{
    i = htole64(i);

    std::fwrite(&i, 8, 1, file);
}

void writeSizet(FILE *file, std::size_t i)
{
    #if __WORDSIZE == 64
    writeUInt64(file, i);
    #elif __WORDSIZE == 32
    writeUInt32(file, i);
    #endif
}

void writeCString(FILE *file, const char *str)
{
    std::fwrite(str, std::strlen(str)+1, 1, file);
}

char readInt8(FILE *file)
{
    unsigned char i = readUInt8(file);
    return *(char *)&i;
}

unsigned char readUInt8(FILE *file)
{
    unsigned char i;
    std::fread(&i, 1, 1, file);
    return i;
}

short readInt16(FILE *file)
{
    unsigned short i = readUInt16(file);
    return *(short *)&i;
}

unsigned short readUInt16(FILE *file)
{
    unsigned short i;
    std::fread(&i, 2, 1, file);
    return le16toh(i);
}

int readInt32(FILE *file)
{
    unsigned int i = readUInt32(file);
    return *(int *)&i;
}

unsigned int readUInt32(FILE *file)
{
    unsigned int i;
    std::fread(&i, 4, 1, file);
    return le32toh(i);
}

long long readInt64(FILE *file)
{
    unsigned long long i = readUInt64(file);
    return *(long long *)&i;
}

unsigned long long readUInt64(FILE *file)
{
    unsigned long long i;
    std::fread(&i, 8, 1, file);
    return le64toh(i);
}

std::string trim(std::string str)
{
    std::string::iterator begin = str.begin();

    for (; begin != str.end(); ++begin)
    {
        if (*begin != ' ')
        {
            break;
        }
    }

    std::string::iterator end = str.end() - 1;

    for (; end != begin; --end)
    {
        if (*end != ' ')
        {
            break;
        }
    }

    return std::string(begin, end+1);
}

std::string readFile(std::string filename)
{
    FILE *file = std::fopen(filename.c_str(), "r");

    if (file == NULL)
    {
        std::cout << "Unable to find \"" << filename << "\"" << std::endl;
        return "";
    }

    std::fseek(file, 0, SEEK_END);

    std::size_t size = std::ftell(file);

    std::fseek(file, 0, SEEK_SET);

    char *cData = (char *)ALLOCATE(size+1);
    std::memset(cData, 0, size+1);

    std::fread(cData, 1, size, file);

    std::fclose(file);

    std::string data(cData);

    DEALLOCATE(cData);

    return data;
}
