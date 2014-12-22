#include "scene.h"

#include <algorithm>

#include "utils/memory.h"
#include "renderer.h"
#include "entity.h"

Scene::Scene(Renderer *renderer) : mSkyboxTexture(NULL),
                                   mRenderer(renderer) {}

Scene::~Scene()
{
    for (std::vector<Entity *>::iterator it = mEntities.begin(); it != mEntities.end(); ++it)
    {
        DELETE(Entity, *it);
    }
}

Entity *Scene::createEntity(ResPtr<Model> model)
{
    Entity *entity = NEW(Entity, model);

    mEntities.push_back(entity);

    return entity;
}

void Scene::destroyEntity(Entity *entity)
{
    destroyEntity(std::find(mEntities.begin(), mEntities.end(), entity));
}

void Scene::destroyEntity(std::vector<Entity *>::iterator pos)
{
    if (pos == mEntities.end())
    {
        throw std::out_of_range("Iterator pointing to entity to destroy is not valid.");
    }

    DELETE(Entity, *pos);

    mEntities.erase(pos);
}
