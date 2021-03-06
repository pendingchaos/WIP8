#include "utils/memory.h"

#ifdef TRACK_MEMORY
#include <cstdio>

#include "utils/utils.h"

FILE *logFile = NULL;
FILE *binaryLogFile = NULL;
#endif

void initMemorySystem()
{
    deinitMemorySystem();

    #ifdef TRACK_MEMORY
    logFile = std::fopen("memory.log", "w");
    binaryLogFile = std::fopen("memory.blog", "wb");

    writeUInt32(binaryLogFile, __WORDSIZE/8);
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
                char **backtrace,
                unsigned int backtraceDepth)
{
    writeUInt32(binaryLogFile, action);
    writeCString(binaryLogFile, file);
    writeUInt32(binaryLogFile, line);
    writeCString(binaryLogFile, function);
    writeSizet(binaryLogFile, (std::size_t)pointer);
    writeSizet(binaryLogFile, amount);
    writeCString(binaryLogFile, type);
    writeSizet(binaryLogFile, typeSize);
    writeUInt32(binaryLogFile, backtraceDepth);

    for (unsigned int i=0; i<backtraceDepth; ++i)
    {
        writeCString(binaryLogFile, backtrace[i]);
    }
}

void *__track_allocate(void *pointer,
                       std::size_t amount,
                       const char *file,
                       unsigned int line,
                       const char *function)
{
    unsigned int depth;
    char **backtrace = getBacktrace(depth);

    if (logFile != NULL)
    {
        std::fprintf(logFile, "%s:%u:%s Allocated %zu bytes at %p\n", file, line, function, amount, pointer);

        for (unsigned int i=0; i<depth; ++i)
        {
            std::fprintf(logFile, "    %s\n", backtrace[i]);
        }

        std::fputc('\n', logFile);
    }

    if (binaryLogFile != NULL)
    {
        writeEntry(AllocateAction, file, line, function, pointer, amount, "", 0, backtrace, depth);
    }

    freeBacktrace(backtrace, depth);

    return pointer;
}

void __track_deallocate(void *pointer,
                        const char *file,
                        unsigned int line,
                        const char *function)
{
    unsigned int depth;
    /*std::vector<std::string>*/char **backtrace = getBacktrace(depth);

    if (pointer != NULL and logFile != NULL)
    {
        std::fprintf(logFile, "%s:%u:%s Deallocated %p\n", file, line, function, pointer);

        for (unsigned int i=0; i<depth; ++i)
        {
            std::fprintf(logFile, "    %s\n", backtrace[i]);
        }

        std::fputc('\n', logFile);
    }

    if (pointer != NULL and binaryLogFile != NULL)
    {
        writeEntry(DeallocateAction, file, line, function, pointer, 0, "", 0, backtrace, depth);
    }

    freeBacktrace(backtrace, depth);
}

void *__track_new(void *pointer,
                  const char *type,
                  std::size_t typeSize,
                  const char *file,
                  unsigned int line,
                  const char *function)
{
    unsigned int depth;
    char **backtrace = getBacktrace(depth);

    if (logFile != NULL)
    {
        std::fprintf(logFile, "%s:%u:%s Allocated a %s (%zu bytes) at %p\n", file, line, function, type, typeSize, pointer);

        for (unsigned int i=0; i<depth; ++i)
        {
            std::fprintf(logFile, "    %s\n", /*it->c_str()*/backtrace[i]);
        }

        std::fputc('\n', logFile);
    }

    if (binaryLogFile != NULL)
    {
        writeEntry(NewAction, file, line, function, pointer, 1, type, typeSize, backtrace, depth);
    }

    freeBacktrace(backtrace, depth);

    return pointer;
}

void __track_delete(void *pointer,
                    const char *type,
                    std::size_t typeSize,
                    const char *file,
                    unsigned int line,
                    const char *function)
{
    unsigned int depth;
    char **backtrace = getBacktrace(depth);

    if (pointer != NULL and logFile != NULL)
    {
        std::fprintf(logFile, "%s:%u:%s Deallocated a %s (%zu bytes) at %p\n", file, line, function, type, typeSize, pointer);

        for (unsigned int i=0; i<depth; ++i)
        {
            std::fprintf(logFile, "    %s\n", backtrace[i]);
        }

        std::fputc('\n', logFile);
    }

    if (pointer != NULL and binaryLogFile != NULL)
    {
        writeEntry(DeleteAction, file, line, function, pointer, 1, type, typeSize, backtrace, depth);
    }

    freeBacktrace(backtrace, depth);
}

void *__track_new_array(void *pointer,
                        const char *type,
                        std::size_t typeSize,
                        unsigned int amount,
                        const char *file,
                        unsigned int line,
                        const char *function)
{
    unsigned int depth;
    char **backtrace = getBacktrace(depth);

    if (logFile != NULL)
    {
        std::fprintf(logFile, "%s:%u:%s Allocated %u %s(s) (%zu bytes per object) at %p\n", file, line, function, amount, type, typeSize, pointer);

        for (unsigned int i=0; i<depth; ++i)
        {
            std::fprintf(logFile, "    %s\n", backtrace[i]);
        }

        std::fputc('\n', logFile);
    }

    if (binaryLogFile != NULL)
    {
        writeEntry(NewArrayAction, file, line, function, pointer, amount, type, typeSize, backtrace, depth);
    }

    freeBacktrace(backtrace, depth);

    return pointer;
}

void __track_delete_array(void *pointer,
                          const char *type,
                          std::size_t typeSize,
                          const char *file,
                          unsigned int line,
                          const char *function)
{
    unsigned int depth;
    char **backtrace = getBacktrace(depth);

    if (pointer != NULL and pointer != NULL)
    {
        std::fprintf(logFile, "%s:%u:%s Deallocated an array of %s(s) (%zu bytes per object) at %p\n", file, line, function, type, typeSize, pointer);

        for (unsigned int i=0; i<depth; ++i)
        {
            std::fprintf(logFile, "    %s\n", backtrace[i]);
        }

        std::fputc('\n', logFile);
    }

    if (binaryLogFile != NULL)
    {
        writeEntry(DeleteArrayAction, file, line, function, pointer, 0, type, typeSize, backtrace, depth);
    }

    freeBacktrace(backtrace, depth);
}
#endif
