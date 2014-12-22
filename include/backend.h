#ifndef BACKEND_H
#define BACKEND_H

#include <glm/glm.hpp>

#include "shader.h"
#include "mesh.h"
#include "material.h"
#include "resource.h"

class Scene;

class VertexBuffer;
class Texture;
class Framebuffer;
class UBO;
class RenderTarget;

class DrawCall
{
    public:
        DrawCall(Scene *scene,
                 ResPtr<Mesh> mesh,
                 ResPtr<Material> material,
                 const glm::mat4& modelMatrix,
                 float lodStipple);

        Scene *mScene;
        ResPtr<Mesh> mMesh;
        ResPtr<Material> mMaterial;
        glm::mat4 mModelMatrix;
        float mLodStipple;
};


class Backend
{
    public:
        Backend();
        virtual ~Backend();

        virtual VertexBuffer *createVertexBuffer()=0;

        virtual CompiledShader *createShader(CompiledShader::Type type,
                                             unsigned int numSources,
                                             const char **sources)=0;

        virtual Texture *createTexture(Texture::Type type)=0;

        virtual Framebuffer *createFramebuffer()=0;

        virtual UBO *createUBO(VertexBuffer *buffer)=0;

        virtual float getMaxAnisotropy()=0;

        virtual void submitDrawCall(const DrawCall& drawCall)=0;
        virtual void executeDrawCalls(RenderTarget *target)=0;
};

#endif // BACKEND_H
