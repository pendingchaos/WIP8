#include "utils/memory.h"

#ifdef TRACK_MEMORY
#include <cstdio>

FILE *logFile = NULL;
FILE *binaryLogFile = NULL;
#endif

void initMemorySystem()
{
    deinitMemorySystem();

    #ifdef TRACK_MEMORY
    logFile = std::fopen("memory.log", "w");
    binaryLogFile = std::fopen("memory.blog", "wb");

    unsigned int pointerSize = htole32(__WORDSIZE/8);
    std::fwrite(&pointerSize, 4, 1, binaryLogFile);
    #endif
}

void deinitMemorySystem()
{
    #ifdef TRACK_MEMORY
    if (logFile != NULL)
    {
        std::fclose(logFile);
    }

    if (binaryLogFile != NULL)
    {
        std::fclose(binaryLogFile);
    }
    #endif
}

#ifdef TRACK_MEMORY
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <endian.h>

#include "utils/utils.h"

typedef enum Action
{
    AllocateAction,
    DeallocateAction,
    NewAction,
    DeleteAction,
    NewArrayAction,
    DeleteArrayAction
} Action;

void writeEntry(Action action,
                const char *file,
                unsigned int line,
                const char *function,
                void *pointer,
                std::size_t amount,
                const char *type,
                std::size_t typeSize,
                const std::vector<std::string>& backtrace)
{
    action = htole32(action);
    line = htole32(line);

    #if __WORDSIZE == 64
    pointer = htole64(pointer);
    amount = htole64(amount);
    typeSize = htole64(typeSize);
    #elif __WORDSIZE == 32
    pointer = htole32(pointer);
    amount = htole32(amount);
    typeSize = htole32(typeSize);
    #endif

    std::fwrite(&action, 4, 1, binaryLogFile);

    std::fwrite(file, std::strlen(file)+1, 1, binaryLogFile);

    std::fwrite(&line, 4, 1, binaryLogFile);

    std::fwrite(function, std::strlen(function)+1, 1, binaryLogFile);

    std::fwrite(&pointer, __WORDSIZE/8, 1, binaryLogFile);

    std::fwrite(&amount, __WORDSIZE/8, 1, binaryLogFile);

    std::fwrite(type, std::strlen(type)+1, 1, binaryLogFile);

    std::fwrite(&typeSize, __WORDSIZE/8, 1, binaryLogFile);

    unsigned int backtraceSize = htole32(backtrace.size());
    std::fwrite(&backtraceSize, 4, 1, binaryLogFile);

    for (std::vector<std::string>::const_iterator it = backtrace.begin();
         it != backtrace.end(); ++it)
    {
        std::fwrite(it->c_str(), it->size()+1, 1, binaryLogFile);
    }
}

void *__track_allocate(void *pointer,
                       std::size_t amount,
                       const char *file,
                       unsigned int line,
                       const char *function)
{
    std::vector<std::string> backtrace = getBacktrace();

    if (logFile != NULL)
    {
        std::fprintf(logFile, "%s:%u:%s Allocated %zu bytes at %p\n", file, line, function, amount, pointer);

        for (std::vector<std::string>::iterator it = backtrace.begin(); it != backtrace.end(); ++it)
        {
            std::fprintf(logFile, "    %s\n", it->c_str());
        }

        std::fputc('\n', logFile);
    }

    if (binaryLogFile != NULL)
    {
        writeEntry(AllocateAction, file, line, function, pointer, amount, "", 0, backtrace);
    }

    return pointer;
}

void __track_deallocate(void *pointer,
                        const char *file,
                        unsigned int line,
                        const char *function)
{
    std::vector<std::string> backtrace = getBacktrace();

    if (pointer != NULL and logFile != NULL)
    {
        std::fprintf(logFile, "%s:%u:%s Deallocated %p\n", file, line, function, pointer);

        for (std::vector<std::string>::iterator it = backtrace.begin(); it != backtrace.end(); ++it)
        {
            std::fprintf(logFile, "    %s\n", it->c_str());
        }

        std::fputc('\n', logFile);
    }

    if (pointer != NULL and binaryLogFile != NULL)
    {
        writeEntry(DeallocateAction, file, line, function, pointer, 0, "", 0, backtrace);
    }
}

void *__track_new(void *pointer,
                  const char *type,
                  std::size_t typeSize,
                  const char *file,
                  unsigned int line,
                  const char *function)
{
    std::vector<std::string> backtrace = getBacktrace();

    if (logFile != NULL)
    {
        std::fprintf(logFile, "%s:%u:%s Allocated a %s (%zu bytes) at %p\n", file, line, function, type, typeSize, pointer);

        for (std::vector<std::string>::iterator it = backtrace.begin(); it != backtrace.end(); ++it)
        {
            std::fprintf(logFile, "    %s\n", it->c_str());
        }

        std::fputc('\n', logFile);
    }

    if (binaryLogFile != NULL)
    {
        writeEntry(NewAction, file, line, function, pointer, 1, type, typeSize, backtrace);
    }

    return pointer;
}

void __track_delete(void *pointer,
                    const char *type,
                    std::size_t typeSize,
                    const char *file,
                    unsigned int line,
                    const char *function)
{
    std::vector<std::string> backtrace = getBacktrace();

    if (pointer != NULL and logFile != NULL)
    {
        std::fprintf(logFile, "%s:%u:%s Deallocated a %s (%zu bytes) at %p\n", file, line, function, type, typeSize, pointer);

        for (std::vector<std::string>::iterator it = backtrace.begin(); it != backtrace.end(); ++it)
        {
            std::fprintf(logFile, "    %s\n", it->c_str());
        }

        std::fputc('\n', logFile);
    }

    if (pointer != NULL and binaryLogFile != NULL)
    {
        writeEntry(DeleteAction, file, line, function, pointer, 1, type, typeSize, backtrace);
    }
}

void *__track_new_array(void *pointer,
                        const char *type,
                        std::size_t typeSize,
                        unsigned int amount,
                        const char *file,
                        unsigned int line,
                        const char *function)
{
    std::vector<std::string> backtrace = getBacktrace();

    if (logFile != NULL)
    {
        std::fprintf(logFile, "%s:%u:%s Allocated %u %s(s) (%zu bytes per object) at %p\n", file, line, function, amount, type, typeSize, pointer);

        for (std::vector<std::string>::iterator it = backtrace.begin(); it != backtrace.end(); ++it)
        {
            std::fprintf(logFile, "    %s\n", it->c_str());
        }

        std::fputc('\n', logFile);
    }

    if (binaryLogFile != NULL)
    {
        writeEntry(NewArrayAction, file, line, function, pointer, amount, type, typeSize, backtrace);
    }

    return pointer;
}

void __track_delete_array(void *pointer,
                          const char *type,
                          std::size_t typeSize,
                          const char *file,
                          unsigned int line,
                          const char *function)
{
    std::vector<std::string> backtrace = getBacktrace();

    if (pointer != NULL and pointer != NULL)
    {
        std::fprintf(logFile, "%s:%u:%s Deallocated an array of %s(s) (%zu bytes per object) at %p\n", file, line, function, type, typeSize, pointer);

        for (std::vector<std::string>::iterator it = backtrace.begin(); it != backtrace.end(); ++it)
        {
            std::fprintf(logFile, "    %s\n", it->c_str());
        }

        std::fputc('\n', logFile);
    }

    if (binaryLogFile != NULL)
    {
        writeEntry(DeleteArrayAction, file, line, function, pointer, 0, type, typeSize, backtrace);
    }
}
#endif
