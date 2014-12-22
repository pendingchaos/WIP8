#include "backend.h"

DrawCall::DrawCall(Scene *scene,
                   ResPtr<Mesh> mesh,
                   ResPtr<Material> material,
                   const glm::mat4& modelMatrix,
                   float lodStipple) : mScene(scene),
                                       mMesh(mesh),
                                       mMaterial(material),
                                       mModelMatrix(modelMatrix),
                                       mLodStipple(lodStipple) {}


Backend::Backend() {}

Backend::~Backend() {}
