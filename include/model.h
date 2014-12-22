#ifndef MODEL_H
#define MODEL_H

#include <vector>

#include "mesh.h"
#include "material.h"
#include "resource.h"

class LOD
{
    public:
        LOD(ResPtr<Mesh> mesh,
            ResPtr<Material> material,
            float distance);

        inline ResPtr<Mesh> getMesh() const {return mMesh;}
        inline ResPtr<Material> getMaterial() const {return mMaterial;}

        float mDistance;
    private:
        ResPtr<Mesh> mMesh;
        ResPtr<Material> mMaterial;
};


class Model : public Resource
{
    public:
        Model();
        virtual ~Model();

        LOD *choose1stLOD(float distance, float *weight);
        LOD *choose2ndLOD(float distance, float *weight);

        void sortLODs();

        std::vector<LOD> mLODs;
        bool mStippledLODs;
        float mMaxDrawDistance;
};

#endif // MODEL_H
