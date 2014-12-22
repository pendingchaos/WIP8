#include "mesh.h"

#include "renderer.h"
#include "vertexbuffer.h"
#include "utils/memory.h"

MeshComponent::MeshComponent(unsigned int numValues,
                             Type type,
                             bool normalized,
                             unsigned int stride,
                             unsigned int offset) : mBuffer(NULL),
                                                    mNumValues(numValues),
                                                    mType(type),
                                                    mNormalized(normalized),
                                                    mStride(stride),
                                                    mOffset(offset) {}


Mesh::Mesh(ResPtr<Shader> vertexShader,
           Primitive primitive,
           unsigned int numVertices,
           unsigned int numIndices) : Resource(typeid(Mesh)),
                                      mVertexShader(vertexShader),
                                      mPrimitive(primitive),
                                      mNumVertices(numVertices),
                                      mNumIndices(numIndices),
                                      mFrontFaceWinding(CounterClockwise),
                                      mCullFace(None),
                                      mPositions(NULL),
                                      mNormals(NULL),
                                      mUVs(NULL),
                                      mColors(NULL),
                                      mTangents(NULL),
                                      mIndices(NULL) {}

Mesh::~Mesh()
{
    removePositions();
    removeNormals();
    removeUVs();
    removeColors();
    removeTangents();
    removeIndices();
}

MeshComponent *Mesh::addPositions(Renderer *renderer, const MeshComponent& positionComponent)
{
    mPositions = NEW(MeshComponent, positionComponent);
    mPositions->mBuffer = renderer->createVertexBuffer();
    return mPositions;
}

void Mesh::removePositions()
{
    if (mPositions != NULL)
    {
        DELETE(VertexBuffer, mPositions->mBuffer);
    }

    DELETE(MeshComponent, mPositions);
}

MeshComponent *Mesh::addNormals(Renderer *renderer, const MeshComponent& normalComponent)
{
    mNormals = NEW(MeshComponent, normalComponent);
    mNormals->mBuffer = renderer->createVertexBuffer();
    return mNormals;
}

void Mesh::removeNormals()
{
    if (mNormals != NULL)
    {
        DELETE(VertexBuffer, mNormals->mBuffer);
    }

    DELETE(MeshComponent, mNormals);
}

MeshComponent *Mesh::addUVs(Renderer *renderer, const MeshComponent& uvComponent)
{
    mUVs = NEW(MeshComponent, uvComponent);
    mUVs->mBuffer = renderer->createVertexBuffer();
    return mUVs;
}

void Mesh::removeUVs()
{
    if (mUVs != NULL)
    {
        DELETE(VertexBuffer, mUVs->mBuffer);
    }

    DELETE(MeshComponent, mUVs);
}

MeshComponent *Mesh::addColors(Renderer *renderer, const MeshComponent& colorComponent)
{
    mColors = NEW(MeshComponent, colorComponent);
    mColors->mBuffer = renderer->createVertexBuffer();
    return mColors;
}

void Mesh::removeColors()
{
    if (mColors != NULL)
    {
        DELETE(VertexBuffer, mColors->mBuffer);
    }

    DELETE(MeshComponent, mColors);
}

MeshComponent *Mesh::addTangents(Renderer *renderer, const MeshComponent& tangentComponent)
{
    mTangents = NEW(MeshComponent, tangentComponent);
    mTangents->mBuffer = renderer->createVertexBuffer();
    return mTangents;
}

void Mesh::removeTangents()
{
    if (mTangents != NULL)
    {
        DELETE(VertexBuffer, mTangents->mBuffer);
    }

    DELETE(MeshComponent, mTangents);
}

MeshComponent *Mesh::addIndices(Renderer *renderer, const MeshComponent& indexComponent)
{
    mIndices = NEW(MeshComponent, indexComponent);
    mIndices->mBuffer = renderer->createVertexBuffer();
    return mIndices;
}

void Mesh::removeIndices()
{
    if (mIndices != NULL)
    {
        DELETE(VertexBuffer, mIndices->mBuffer);
    }

    DELETE(MeshComponent, mIndices);
}

void Mesh::calculateAABB()
{
    if (mPositions == NULL)
    {
        return;
    }

    if (mPositions->getType() != MeshComponent::Float32 or mPositions->getNumValues() != 3)
    {
        return;
    }

    int stride = mPositions->getStride();

    if (stride == 0)
    {
        stride = 12;
    }

    glm::vec3 *positions = (glm::vec3 *)mPositions->getVertexBuffer()->map(true, false);

    glm::vec3 min = *(positions+mPositions->getOffset());
    glm::vec3 max = min;

    unsigned int size = mPositions->getVertexBuffer()->getSize();

    for (unsigned int i = mPositions->getOffset()+12; i<size; i += stride)
    {
        min = glm::min(*(glm::vec3 *)((char *)positions+i), min);
        max = glm::max(*(glm::vec3 *)((char *)positions+i), max);
    }

    glm::vec3 center = max - min / 2.0f;

    mAABB = AABB(center, max - center);

    mPositions->getVertexBuffer()->unmap();
}
