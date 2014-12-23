#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <unordered_map>
#include <string>

#include <jsoncpp/json/reader.h>

#include "resource.h"
#include "texture.h"
#include "model.h"
#include "shader.h"
#include "mesh.h"
#include "material.h"

class Renderer;

class ResourceManager
{
    public:
        ResourceManager(Renderer *renderer);
        virtual ~ResourceManager();

        inline const std::unordered_map<std::string, Resource *> getResources() const
        {
            return mResources;
        }

        void deleteUnusedResources();

        ResPtr<Resource> load(std::string filename);

        ResPtr<Texture> createTexture(Texture::Type type);

        ResPtr<Model> createModel();

        ResPtr<Shader> createShader(CompiledShader::Type type, std::string source);

        ResPtr<Mesh> createMesh(ResPtr<Shader> shader,
                                Mesh::Primitive primitive,
                                unsigned int numVertices,
                                unsigned int numIndices=0);

        ResPtr<Material> createMaterial(ResPtr<Shader> shader);
    private:
        Renderer *mRenderer;

        std::unordered_map<std::string, Resource *> mResources;

        Resource *_load(std::string filename);

        Texture *loadTexture(const Json::Value& json,
                             const std::unordered_map<std::string, ResPtr<Resource> >& externalResources);

        Model *loadModel(const Json::Value& json,
                         const std::unordered_map<std::string, ResPtr<Resource> >& externalResources);

        Shader *loadShader(const Json::Value& json,
                           const std::unordered_map<std::string, ResPtr<Resource> >& externalResources);

        Mesh *loadMesh(const Json::Value& json,
                       const std::unordered_map<std::string, ResPtr<Resource> >& externalResources);

        Material *loadMaterial(const Json::Value& json,
                               const std::unordered_map<std::string, ResPtr<Resource> >& externalResources);
};

#endif // RESOURCEMANAGER_H
