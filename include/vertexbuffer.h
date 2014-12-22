#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

class VertexBuffer
{
    public:
        VertexBuffer();
        virtual ~VertexBuffer();

        virtual void *map(bool read, bool write)=0;
        virtual void *map(unsigned int start,
                          unsigned int size,
                          bool read,
                          bool write,
                          bool invalidateRange=false,
                          bool invalidateBuffer=false,
                          bool unsynchronized=false)=0;
        virtual void unmap()=0;

        virtual void alloc(unsigned int size)=0;

        virtual unsigned int getSize() const =0;
};

#endif // VERTEXBUFFER_H
