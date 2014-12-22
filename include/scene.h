#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include <glm/glm.hpp>

#include "resource.h"
#include "model.h"
#include "transform.h"
#include "texture.h"

class Renderer;
class Entity;
class UniformAccess;
class VertexBuffer;
class UBO;

class Scene
{
    public:
        Scene(Renderer *renderer);
        virtual ~Scene();

        Entity *createEntity(ResPtr<Model> model);

        void destroyEntity(Entity *entity);
        void destroyEntity(std::vector<Entity *>::iterator pos);

        inline const std::vector<Entity *>& getEntities() const {return mEntities;}

        inline Renderer *getRenderer() const {return mRenderer;}

        Transform mViewTransform;
        Transform mProjectionTransform;

        ResPtr<Texture> mSkyboxTexture;
    private:
        Renderer *mRenderer;

        std::vector<Entity *> mEntities;
};

#endif // SCENE_H
