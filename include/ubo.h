#ifndef UBO_H
#define UBO_H

#include <glm/glm.hpp>

class VertexBuffer;

class UBO
{
    public:
        virtual ~UBO();

        inline VertexBuffer *getBuffer() const {return mBuffer;}
    protected:
        UBO(VertexBuffer *buffer);

        VertexBuffer *mBuffer;
};

#endif // UBO_H
