#ifndef ENTITY_H
#define ENTITY_H

#include "transform.h"
#include "model.h"
#include "resource.h"

class Scene;

class Entity
{
    friend Scene;

    public:
        virtual ~Entity();

        Transform mTransform;

        ResPtr<Model> mModel;
    private:
        Entity(ResPtr<Model> model);
};

#endif // ENTITY_H
