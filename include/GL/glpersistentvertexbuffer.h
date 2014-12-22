#ifndef GLPERSISTENTVERTEXBUFFER_H
#define GLPERSISTENTVERTEXBUFFER_H

#include <GL/gl.h>

#include "GL/glvertexbuffer.h"

class GLPersistentVertexBuffer : public GLVertexBuffer
{
    public:
        GLPersistentVertexBuffer();
        virtual ~GLPersistentVertexBuffer();

        void *map(bool read, bool write);
        void *map(unsigned int start,
                  unsigned int size,
                  bool read,
                  bool write,
                  bool invalidateRange=false,
                  bool invalidateBuffer=false,
                  bool unsynchronized=false);
        void unmap();

        void alloc(unsigned int size);

        void *mBufferStorageMap;
};

#endif // GLPERSISTENTVERTEXBUFFER_H
