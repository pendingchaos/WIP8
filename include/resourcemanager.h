#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <unordered_map>

#include <jsoncpp/json/reader.h>

#include "resource.h"

class Renderer;
class Texture;
class Model;
class Shader;
class Mesh;
class Material;

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
