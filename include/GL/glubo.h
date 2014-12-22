#ifndef GLUBO_H
#define GLUBO_H

#include "ubo.h"

class VertexBuffer;

class GLUBO : public UBO
{
    public:
        GLUBO(VertexBuffer *buffer);
        virtual ~GLUBO();

        unsigned int mBindingPoint;
};

#endif // GLUBO_H
