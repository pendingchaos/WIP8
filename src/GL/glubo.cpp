#include "GL/glubo.h"

#include <GL/gl.h>

#include "GL/glvertexbuffer.h"

GLUBO::GLUBO(VertexBuffer *buffer) : UBO(buffer)
{
    static int nextBindingPoint = 0;

    mBindingPoint = nextBindingPoint;

    ++nextBindingPoint;

    GLuint lastBuffer;
    glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, (GLint *)&lastBuffer);

    glBindBufferRange(GL_UNIFORM_BUFFER, mBindingPoint, ((GLVertexBuffer *)mBuffer)->mBuffer, 0, mBuffer->getSize());

    glBindBuffer(GL_UNIFORM_BUFFER, lastBuffer);
}

GLUBO::~GLUBO() {}
