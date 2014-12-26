#include "GL/glvertexbuffer.h"

GLVertexBuffer::GLVertexBuffer()
{
    glGenBuffers(1, &mBuffer);
}

GLVertexBuffer::~GLVertexBuffer()
{
    glDeleteBuffers(1, &mBuffer);
}

void *GLVertexBuffer::map(bool read, bool write)
{
    GLuint lastBuffer;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint *)&lastBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
    void *data = glMapBuffer(GL_ARRAY_BUFFER, read and write ? GL_READ_WRITE : (read ? GL_READ_ONLY : (write ? GL_WRITE_ONLY : 0)));

    glBindBuffer(GL_ARRAY_BUFFER, lastBuffer);

    return data;
}

void *GLVertexBuffer::map(unsigned int start,
                        unsigned int size,
                        bool read,
                        bool write,
                        bool invalidateRange,
                        bool invalidateBuffer,
                        bool unsynchronized)
{
    GLuint lastBuffer;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint *)&lastBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
    void *data = glMapBufferRange(GL_ARRAY_BUFFER, start, size,   read * GL_MAP_READ_BIT
                                                                | write * GL_MAP_WRITE_BIT
                                                                | invalidateRange * GL_MAP_INVALIDATE_RANGE_BIT
                                                                | invalidateBuffer * GL_MAP_INVALIDATE_BUFFER_BIT
                                                                | unsynchronized * GL_MAP_UNSYNCHRONIZED_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, lastBuffer);

    return data;
}

void GLVertexBuffer::unmap()
{
    GLuint lastBuffer;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint *)&lastBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
    glUnmapBuffer(GL_ARRAY_BUFFER);

    glBindBuffer(GL_ARRAY_BUFFER, lastBuffer);
}

void GLVertexBuffer::alloc(unsigned int size)
{
    GLuint lastBuffer;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint *)&lastBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, mBuffer);

    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, lastBuffer);
}

unsigned int GLVertexBuffer::getSize() const
{
    GLint size;

    GLuint lastBuffer;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint *)&lastBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

    glBindBuffer(GL_ARRAY_BUFFER, lastBuffer);

    return size;
}
