#ifndef MEMORY_H
#define MEMORY_H

#include <cstdlib>

#define ALLOCATE(amount) \
__track_allocate(std::malloc((amount)), amount, __FILE__, __LINE__, __PRETTY_FUNCTION__)

#define DEALLOCATE(pointer)\
{void *&pointer2 = (void *&)(pointer);\
std::free(pointer2);\
__track_deallocate(pointer2, __FILE__, __LINE__, __PRETTY_FUNCTION__);\
pointer2 = NULL;}


#define NEW(type, ...)\
((type *)__track_new(new (type)(__VA_ARGS__),\
#type, sizeof(type), __FILE__, __LINE__, __PRETTY_FUNCTION__))

#define DELETE(type, pointer)\
{type *&pointer2 = (pointer);\
delete pointer2;\
__track_delete(pointer2, #type, sizeof(type), __FILE__, __LINE__,\
__PRETTY_FUNCTION__);\
pointer2 = NULL;}


#define NEW_ARRAY(type, amount)\
((type *)__track_new_array(new type[amount], #type, sizeof(type), amount,\
__FILE__, __LINE__, __PRETTY_FUNCTION__))

#define DELETE_ARRAY(type, pointer)\
{type *&pointer2 = (pointer);\
delete [] pointer2;\
__track_delete_array(pointer2, #type, sizeof(type), __FILE__, __LINE__,\
__PRETTY_FUNCTION__);\
pointer2 = NULL;}

void initMemorySystem();
void deinitMemorySystem();

#ifdef TRACK_MEMORY
void *__track_allocate(void *pointer,
                       std::size_t amount,
                       const char *file,
                       unsigned int line,
                       const char *function);

void __track_deallocate(void *pointer,
                        const char *file,
                        unsigned int line,
                        const char *function);

void *__track_new(void *pointer,
                  const char *type,
                  std::size_t typeSize,
                  const char *file,
                  unsigned int line,
                  const char *function);

void __track_delete(void *pointer,
                    const char *type,
                    std::size_t typeSize,
                    const char *file,
                    unsigned int line,
                    const char *function);

void *__track_new_array(void *pointer,
                        const char *type,
                        std::size_t typeSize,
                        unsigned int amount,
                        const char *file,
                        unsigned int line,
                        const char *function);

void __track_delete_array(void *pointer,
                          const char *type,
                          std::size_t typeSize,
                          const char *file,
                          unsigned int line,
                          const char *function);
#else
#define __track_allocate(pointer, amount, file, line, function) (pointer)
#define __track_deallocate(pointer, file, line, function)

#define __track_new(pointer, type, typeSize, file, line, function) (pointer)
#define __track_delete(pointer, type, typeSize, file, line, function)

#define __track_new_array(pointer, type, typeSize, amount, file, line, function) (pointer)
#define __track_delete_array(pointer, type, typeSize, file, line, function)
#endif

#endif // MEMORY_H
