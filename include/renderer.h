#ifndef RENDERER_H
#define RENDERER_H

#include <map>
#include <string>

#include "resource.h"
#include "shader.h"
#include "texture.h"
#include "material.h"
#include "backend.h"

class Scene;
class ResourceManager;
class VertexBuffer;
class Framebuffer;
class UBO;
class Mesh;
class RenderTarget;
class Entity;
class LOD;

class Renderer
{
    public:
        Renderer(Backend *backend);
        virtual ~Renderer();

        void render(RenderTarget *target, Scene *scene);

        inline VertexBuffer *createVertexBuffer() {return mBackend->createVertexBuffer();}

        inline CompiledShader *createShader(CompiledShader::Type type,
                                             unsigned int numSources,
                                             const char **sources) {return mBackend->createShader(type, numSources, sources);}

        CompiledShader *createShader(CompiledShader::Type type,
                                     std::string source,
                                     std::map<std::string, std::string> defines
                                     =std::map<std::string, std::string>());

        inline Texture *createTexture(Texture::Type type) {return mBackend->createTexture(type);}
        Texture *createTexture(const char *filename);
        Texture *createCubemap(const char *posX, const char *negX,
                               const char *posY, const char *negY,
                               const char *posZ, const char *negZ);

        inline Framebuffer *createFramebuffer() {return mBackend->createFramebuffer();}

        inline UBO *createUBO(VertexBuffer *buffer) {return mBackend->createUBO(buffer);}

        inline float getMaxAnisotropy() {return mBackend->getMaxAnisotropy();}

        inline ResourceManager *getResourceManager() const {return mResMgr;}

        inline Backend *getBackend() const {return mBackend;}
    protected:
        Backend *mBackend;
        ResourceManager *mResMgr;

        void createSkybox();

        void renderEntity(const Entity *entity, Scene *scene);
        void renderLOD(const Entity *entity, Scene *scene, LOD *lod, float weight);

        Mesh *mSkyboxMesh;
        Material *mSkyboxMaterial;
};

#endif // RENDERER_H
