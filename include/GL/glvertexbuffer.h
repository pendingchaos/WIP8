#ifndef GLVERTEXBUFFER_H
#define GLVERTEXBUFFER_H

#include <GL/gl.h>

#include "vertexbuffer.h"

class GLVertexBuffer : public VertexBuffer
{
    public:
        GLVertexBuffer();
        virtual ~GLVertexBuffer();

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

        unsigned int getSize() const;

        GLuint mBuffer;
};

#endif // GLVERTEXBUFFER_H
