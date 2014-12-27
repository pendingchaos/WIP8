#ifndef MESH_H
#define MESH_H

#include <string>

#include "resource.h"
#include "shader.h"
#include "geometry/aabb.h"

class Renderer;
class Mesh;
class VertexBuffer;

class MeshComponent
{
    friend Mesh;

    public:
        typedef enum Type
        {
            SInt8,
            UInt8,

            SInt16,
            UInt16,

            SInt32,
            UInt32,

            Float16,
            Float32,
            Float64
        } Type;

        MeshComponent(unsigned int numValues,
                      Type type,
                      bool normalized=false,
                      unsigned int stride=0,
                      unsigned int offset=0);

        inline VertexBuffer *getVertexBuffer() const {return mBuffer;}
        inline unsigned int getNumValues() const {return mNumValues;}
        inline Type getType() const {return mType;}
        inline bool getNormalized() const {return mNormalized;}
        inline unsigned int getStride() const {return mStride;}
        inline unsigned int getOffset() const {return mOffset;}
    private:
        VertexBuffer *mBuffer;
        unsigned int mNumValues;
        Type mType;
        bool mNormalized;
        unsigned int mStride;
        unsigned int mOffset;
};

class Mesh : public Resource
{
    public:
        typedef enum Primitive
        {
            Points,
            LineStrip,
            LineLoop,
            TriangleStrip,
            TriangleFan,
            Triangles
        } Primitive;

        typedef enum Winding
        {
            Clockwise,
            CounterClockwise
        } Winding;

        typedef enum CullFace
        {
            None,
            Front,
            Back
        } CullFace;

        Mesh(ResPtr<Shader> shader,
             Primitive primitive,
             unsigned int numVertices,
             unsigned int numIndices=0);
        virtual ~Mesh();
        
        //TODO: These should not need the Renderer.
        MeshComponent *addPositions(Renderer *renderer, const MeshComponent& positionComponent);
        void removePositions();
        inline bool hasPositions() {return mPositions != NULL;}
        MeshComponent *getPositions() const {return mPositions;}

        MeshComponent *addNormals(Renderer *renderer, const MeshComponent& normalComponent);
        void removeNormals();
        inline bool hasNormals() {return mNormals != NULL;}
        MeshComponent *getNormals() const {return mNormals;}

        MeshComponent *addUVs(Renderer *renderer, const MeshComponent& uvComponent);
        void removeUVs();
        inline bool hasUVs() {return mUVs != NULL;}
        MeshComponent *getUVs() const {return mUVs;}

        MeshComponent *addColors(Renderer *renderer, const MeshComponent& colorComponent);
        void removeColors();
        inline bool hasColors() {return mColors != NULL;}
        MeshComponent *getColors() const {return mColors;}

        MeshComponent *addTangents(Renderer *renderer, const MeshComponent& tangentComponent);
        void removeTangents();
        inline bool hasTangents() {return mTangents != NULL;}
        MeshComponent *getTangents() const {return mTangents;}

        MeshComponent *addIndices(Renderer *renderer, const MeshComponent& indexComponent);
        void removeIndices();
        inline bool hasIndices() {return mIndices != NULL;}
        MeshComponent *getIndices() const {return mIndices;}

        void calculateAABB();

        inline const AABB& getAABB() const {return mAABB;}

        ResPtr<Shader> mVertexShader;
        ResPtr<Shader> mGeometryShader;

        Primitive mPrimitive;
        unsigned int mNumVertices;
        unsigned int mNumIndices;

        Winding mFrontFaceWinding;
        CullFace mCullFace;

        std::map<std::string, std::string> mDefines;
        std::map<std::string, UniformValue> mUniforms;
    private:
        MeshComponent *mPositions;
        MeshComponent *mNormals;
        MeshComponent *mUVs;
        MeshComponent *mColors;
        MeshComponent *mTangents;
        MeshComponent *mIndices;

        AABB mAABB;
};

#endif // MESH_H
