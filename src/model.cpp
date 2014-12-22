#include "model.h"

#include <algorithm>

LOD::LOD(ResPtr<Mesh> mesh,
         ResPtr<Material> material,
         float distance) : mDistance(distance),
                           mMesh(mesh),
                           mMaterial(material) {}


Model::Model() : Resource(typeid(Model)),
                 mStippledLODs(true),
                 mMaxDrawDistance(-1.0f) {}

Model::~Model() {}

LOD *Model::choose1stLOD(float distance, float *weight)
{
    for (std::vector<LOD>::iterator it = mLODs.begin()+1; it != mLODs.end(); ++it)
    {
        if (distance < it->mDistance and distance > (it-1)->mDistance)
        {
            if (weight != NULL)
            {
                *weight = std::abs(distance-it->mDistance) / std::abs(it->mDistance - (it-1)->mDistance);
            }

            return &*(it-1);
        }
    }

    if (weight != NULL)
    {
        *weight = 1.0f;
    }

    if (distance < mLODs.begin()->mDistance)
    {
        return &*(mLODs.begin());
    } else
    {
        return &*(mLODs.end()-1);
    }
}

LOD *Model::choose2ndLOD(float distance, float *weight)
{
    for (std::vector<LOD>::iterator it = mLODs.begin()+1; it != mLODs.end(); ++it)
    {
        if (distance < it->mDistance and distance > (it-1)->mDistance)
        {
            if (weight != NULL)
            {
                *weight = std::abs(distance-(it-1)->mDistance) / std::abs(it->mDistance - (it-1)->mDistance);
            }

            return &*it;
        }
    }

    if (weight != NULL)
    {
        *weight = 0.0f;
    }

    return NULL;
}

bool compareLODs(const LOD& first, const LOD& second)
{
    return first.mDistance < second.mDistance;
}

void Model::sortLODs()
{
    std::sort(mLODs.begin(), mLODs.end(), compareLODs);
}
