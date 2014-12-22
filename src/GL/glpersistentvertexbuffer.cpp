#include "GL/glpersistentvertexbuffer.h"

GLPersistentVertexBuffer::GLPersistentVertexBuffer() : mBufferStorageMap(NULL) {}

GLPersistentVertexBuffer::~GLPersistentVertexBuffer()
{
    if (mBufferStorageMap)
    {
        GLuint lastBuffer;
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint *)&lastBuffer);

        glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
        glUnmapBuffer(GL_ARRAY_BUFFER);

        glBindBuffer(GL_ARRAY_BUFFER, lastBuffer);
    }
}


void *GLPersistentVertexBuffer::map(bool read, bool write)
{
    return mBufferStorageMap;
}

void *GLPersistentVertexBuffer::map(unsigned int start,
                        unsigned int size,
                        bool read,
                        bool write,
                        bool invalidateRange,
                        bool invalidateBuffer,
                        bool unsynchronized)
{
    return (char *)mBufferStorageMap+start;
}

void GLPersistentVertexBuffer::unmap() {}

void GLPersistentVertexBuffer::alloc(unsigned int size)
{
    GLuint lastBuffer;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint *)&lastBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, mBuffer);

    if (mBufferStorageMap)
    {
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    glBufferStorage(GL_ARRAY_BUFFER, size, NULL, GL_MAP_READ_BIT|GL_MAP_WRITE_BIT|GL_MAP_PERSISTENT_BIT|GL_MAP_COHERENT_BIT);
    mBufferStorageMap = glMapBufferRange(GL_ARRAY_BUFFER, 0, size, GL_MAP_READ_BIT|GL_MAP_WRITE_BIT|GL_MAP_PERSISTENT_BIT|GL_MAP_COHERENT_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, lastBuffer);
}
