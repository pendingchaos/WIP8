#include "utils/utils.h"

#include <execinfo.h>
#include <cxxabi.h>
#include <dlfcn.h>
#include <cstdlib>
#include <endian.h>
#include <cstring>

//TODO: res.push_back seems to sometimes crash.
std::vector<std::string> getBacktrace()
{
    std::vector<std::string> res;

    void *trace[256];

    unsigned int depth = backtrace(trace, 256);

    for (unsigned int i=0; i<depth; ++i)
    {
        Dl_info info;

        if (dladdr(trace[i], &info) == 0)
        {
            continue;
        }

        const char *name = info.dli_sname;

        if (name == NULL)
        {
            res.push_back("error");
        }

        int status;

        char *demangled = abi::__cxa_demangle(name, NULL, 0, &status);

        if (status == 0 and demangled != NULL)
        {
            res.push_back(demangled);
        } else if (name != NULL)
        {
            res.push_back(name);
        }

        if (status == 0)
        {
            std::free(demangled);
        }
    }

    return res;
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
