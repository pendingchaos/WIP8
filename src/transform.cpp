#include "transform.h"

#include <algorithm>

Transform::Transform() : mParent(NULL) {}

Transform::~Transform()
{
    for (std::vector<Transform *>::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
    {
        (*it)->mParent = NULL;
    }

    if (mParent != NULL)
    {
        mParent->mChildren.erase(std::find(mParent->mChildren.begin(),
                                           mParent->mChildren.end(),
                                           this));
    }
}

void Transform::setParent(Transform *parent)
{
    if (mParent != parent)
    {
        mParent = parent;

        mParent->mChildren.push_back(this);
    }
}

void Transform::removeChild(std::vector<Transform *>::iterator pos)
{
    (*pos)->mParent = NULL;

    mChildren.erase(pos);
}

void Transform::removeChild(unsigned int pos)
{
    std::vector<Transform *>::iterator it = mChildren.begin()+pos;

    (*it)->mParent = NULL;

    mChildren.erase(it);
}

glm::mat4 Transform::getMatrix() const
{
    if (mParent != NULL)
    {
        return mMatrix * mParent->getMatrix();
    } else
    {
        return mMatrix;
    }
}
