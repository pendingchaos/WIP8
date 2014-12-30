#ifndef C_API_H
#define C_API_H

typedef void * CRenderer;
typedef void * CBackend;

typedef void * CScene;
typedef void * CEntity;
typedef void * CTransform;

typedef void * CResourceManager;
typedef void * CResource;
typedef void * CMaterial;

typedef void * CMeshComponent;
typedef void * CMesh;

typedef void * CLOD;
typedef void * CModel;

typedef void * CTexture;

typedef void * CShader;
typedef void * CShaderDefines;
typedef void * CShaderUniforms;

typedef void * CVertexBuffer;
typedef void * CRenderTarget;
typedef void * CFramebuffer;

typedef struct CVec2
{
    float x;
    float y;
} CVec2;

typedef struct CVec3
{
    float x;
    float y;
    float z;
} CVec3;

typedef struct CVec4
{
    float x;
    float y;
    float z;
    float w;
} CVec4;

typedef struct CIVec2
{
    float x;
    float y;
} CIVec2;

typedef struct CIVec3
{
    float x;
    float y;
    float z;
} CIVec3;

typedef struct CIVec4
{
    float x;
    float y;
    float z;
    float w;
} CIVec4;

typedef struct CMatrix4x4
{
    float data[4][4];
} CMatrix4x4;

typedef struct CAABB
{
    CVec3 center;
    CVec3 extents;
} CAABB;

typedef enum CTextureType
{
    CTexture1D,
    CTexture2D,
    CTexture3D,
    CTextureCubeMap
} CTextureType;

typedef enum CShaderType
{
    CVertexShader,
    CFragmentShader,
    CGeometryShader
} CShaderType;

typedef enum CMeshPrimitive
{
    CPointsPrimitive,
    CLineStripPrimitive,
    CLineLoopPrimitive,
    CTriangleStripPrimitive,
    CTriangleFanPrimitive,
    CTrianglesPrimitive
} CMeshPrimitve;

typedef enum CResourceType
{
    CMaterialResType,
    CMeshResType,
    CModelResType,
    CTextureResType,
    CShaderResType,
    CUnknownResType
} CResourceType;

typedef enum CDepthFunc
{
    CNever,
    CLess,
    CEqual,
    CLessEqual,
    CGreater,
    CNotEqual,
    CGreaterEqual,
    CAlways
} CDepthFunc;

typedef enum CBlendEquation
{
    CAdd,
    CSubtract,
    CReverseSubtract,
    CMin,
    CMax
} CBlendEquation;

typedef enum CBlendFactor
{
    CZero,
    COne,
    CSrcColor,
    COneMinusSrcColor,
    CDstColor,
    COneMinusDstColor,
    CSrcAlpha,
    COneMinusSrcAlpha,
    CDstAlpha,
    COneMinusDstAlpha
} CBlendFactor;

typedef enum CMeshComponentType
{
    CSInt8,
    CUInt8,

    CSInt16,
    CUInt16,

    CSInt32,
    CUInt32,

    CFloat16,
    CFloat32,
    CFloat64
} CMeshComponentType;

typedef enum CMeshWinding
{
    CClockwise,
    CCounterClockwise
} CMeshWinding;

typedef enum CMeshCullFace
{
    CNone,
    CFront,
    CBack
} CMeshCullFace;

typedef enum CTextureInternalFormat
{
    CRedU8_Norm_InternalFormat,
    CRedS8_Norm_InternalFormat,

    CRedU16_Norm_InternalFormat,
    CRedS16_Norm_InternalFormat,

    CRedU8_InternalFormat,
    CRedS8_InternalFormat,

    CRedU16_InternalFormat,
    CRedS16_InternalFormat,

    CRedU32_InternalFormat,
    CRedS32_InternalFormat,

    CRed16F_InternalFormat,
    CRed32F_InternalFormat,

    CRedGreenU8_Norm_InternalFormat,
    CRedGreenS8_Norm_InternalFormat,

    CRedGreenU16_Norm_InternalFormat,
    CRedGreenS16_Norm_InternalFormat,

    CRedGreenU8_InternalFormat,
    CRedGreenS8_InternalFormat,

    CRedGreenU16_InternalFormat,
    CRedGreenS16_InternalFormat,

    CRedGreenU32_InternalFormat,
    CRedGreenS32_InternalFormat,

    CRedGreen16F_InternalFormat,
    CRedGreen32F_InternalFormat,

    CRGBU8_Norm_InternalFormat,
    CRGBS8_Norm_InternalFormat,

    CRGBU16_Norm_InternalFormat,
    CRGBS16_Norm_InternalFormat,

    CRGBU8_InternalFormat,
    CRGBS8_InternalFormat,

    CRGBU16_InternalFormat,
    CRGBS16_InternalFormat,

    CRGBU32_InternalFormat,
    CRGBS32_InternalFormat,

    CRGB16F_InternalFormat,
    CRGB32F_InternalFormat,

    CRGBAU8_Norm_InternalFormat,
    CRGBAS8_Norm_InternalFormat,

    CRGBAU16_Norm_InternalFormat,
    CRGBAS16_Norm_InternalFormat,

    CRGBAU8_InternalFormat,
    CRGBAS8_InternalFormat,

    CRGBAU16_InternalFormat,
    CRGBAS16_InternalFormat,

    CRGBAU32_InternalFormat,
    CRGBAS32_InternalFormat,

    CRGBA16F_InternalFormat,
    CRGBA32F_InternalFormat,

    CRGB_Norm_DXT1_InternalFormat,
    CRGBA_Norm_DXT1_InternalFormat,

    CRGBA_Norm_DXT3_InternalFormat,

    CRGBA_Norm_DXT5_InternalFormat,

    CDepthI16_Norm_InternalFormat,
    CDepthI24_Norm_InternalFormat,
    CDepthI32_Norm_InternalFormat,
    CDepth32F_InternalFormat
} CTextureInternalFormat;

typedef enum CTextureFormat
{
    CRedU8_Format,
    CRedS8_Format,

    CRedU16_Format,
    CRedS16_Format,

    CRedU32_Format,
    CRedS32_Format,

    CRed32F_Format,

    CRedGreenU8_Format,
    CRedGreenS8_Format,

    CRedGreenU16_Format,
    CRedGreenS16_Format,

    CRedGreenU32_Format,
    CRedGreenS32_Format,

    CRedGreen32F_Format,

    CRGBU8_Format,
    CRGBS8_Format,

    CRGBU16_Format,
    CRGBS16_Format,

    CRGBU32_Format,
    CRGBS32_Format,

    CRGB32F_Format,

    CRGBAU8_Format,
    CRGBAS8_Format,

    CRGBAU16_Format,
    CRGBAS16_Format,

    CRGBAU32_Format,
    CRGBAS32_Format,

    CRGBA32F_Format,

    CDepth32F_Format
} CTextureFormat;

typedef enum CTextureMinFilter
{
    CLinearMinFilter,
    CNearestMinFilter,

    CLinearWithNearestMipmapMinFilter,
    CNearestWithNearestMipmapMinFilter,

    CLinearWithLinearMipmapMinFilter,
    CNearestWithLinearMipmapMinFilter
} CTextureMinFilter;

typedef enum CTextureMagFilter
{
    CLinearMagFilter,
    CNearestMagFilter
} CTextureMagFilter;

typedef enum CTextureWrap
{
    CRepeat,
    CMirroredRepeat,
    CClamp
} CTextureWrap;

typedef enum CUniformType
{
    CNothingUType,
    CFloatUType,
    CVec2UType,
    CVec3UType,
    CVec4UType,
    CIntUType,
    CIVec2UType,
    CIVec3UType,
    CIVec4UType,
    CTextureUType
} CUniformType;

CRenderer createRenderer(CBackend backend);
void deleteRenderer(CRenderer renderer);
void render(CRenderer renderer, CRenderTarget target, CScene scene);
CResourceManager getResourceManager(CRenderer renderer);
CBackend getBackend(CRenderer renderer);

CBackend createGLBackend();
CVertexBuffer createVertexBuffer(CBackend backend);
CFramebuffer createFramebuffer(CBackend backend);
float getMaxAnisotropy(CBackend backend);
void submitDrawCall(CBackend backend,
                    CScene scene,
                    CMesh mesh,
                    CMaterial material,
                    CMatrix4x4 matrix,
                    float lodStipple);
void executeDrawCalls(CBackend backend, CRenderTarget renderTarget);

CScene createScene(CRenderer renderer);
CEntity createEntity(CScene scene, CModel model);
void destroyEntity(CScene scene, CEntity entity);
unsigned int getNumEntities(CScene scene);
CEntity *getEntities(CScene scene);
CTransform getViewTransform(CScene scene);
CTransform getProjectionTransform(CScene scene);
CTexture getSkyboxTexture(CScene scene);
void setSkyboxTexture(CScene scene, CTexture texture);

CTransform getTransform(CEntity entity);
CModel getModel(CEntity entity);

void resetTransform(CTransform transform);
void translate(CTransform transform, CVec3 translation);
void rotate(CTransform transform, float angle, CVec3 axis);
void scale(CTransform transform, CVec3 by);
void lookAt(CTransform transform, CVec3 eye, CVec3 center, CVec3 up);
void perspective(CTransform transform, float fovy, float aspect, float near, float far);
void setMatrix(CTransform transform, CMatrix4x4 matrix);
CMatrix4x4 getMatrix(CTransform transform);
void multiplyMatrix(CTransform transform, CMatrix4x4 matrix);
CTransform getParent(CTransform transform);
void setParent(CTransform transform, CTransform parent);
unsigned int getNumChildren(CTransform transform);
CTransform *getChildren(CTransform transform);
void removeChild(CTransform transform, unsigned int index);
void addChild(CTransform transform, CTransform child);

unsigned int getNumResources(CResourceManager resMgr);
//TODO: Resources should store their own name.
const char *getResourceName(CResourceManager resMgr, unsigned int index);
CResource getResource(CResourceManager resMgr, unsigned int index);
void deleteUnusedResources(CResourceManager resMgr);
CResource loadResource(CResourceManager resMgr, const char *filename);
CTexture createTexture(CResourceManager resMgr, CTextureType type);
CModel createModel(CResourceManager resMgr);
CShader createShader(CResourceManager resMgr, CShaderType type, const char *source);
CMesh createMesh(CResourceManager resMgr,
                 CShader shader,
                 CMeshPrimitive primitive,
                 unsigned int numVertices,
                 unsigned int numIndices);
CMaterial createMaterial(CResourceManager resMgr, CShader fragmentShader);

unsigned int getRefCount(CResource resource);
void incRefCount(CResource resource);
void decRefCount(CResource resource);
CResourceType getResourceType(CResource resource);
CMaterial asMaterial(CResource resource);
CMesh asMesh(CResource resource);
CModel asModel(CResource resource);
CTexture asTexture(CResource resource);
CShader asShader(CResource resource);

CShader getFragmentShader(CMaterial material);
CShaderDefines getMaterialDefines(CMaterial material);
CShaderUniforms getMaterialUniforms(CMaterial material);
bool *getWriteDepth(CMaterial material);
bool *getWriteRed(CMaterial material);
bool *getWriteGreen(CMaterial material);
bool *getWriteBlue(CMaterial material);
bool *getWriteAlpha(CMaterial material);
CDepthFunc *getDepthFunc(CMaterial material);
CBlendEquation *getRGBBlendEquation(CMaterial material);
CBlendEquation *getAlphaBlendEquation(CMaterial material);
CBlendFactor *getRGBSrcBlendFactor(CMaterial material);
CBlendFactor *getRGBDstBlendFactor(CMaterial material);
CBlendFactor *getAlphaSrcBlendFactor(CMaterial material);
CBlendFactor *getAlphaDstBlendFactor(CMaterial material);

CVertexBuffer getVertexBuffer(CMeshComponent meshComponent);
unsigned int getNumValues(CMeshComponent meshComponent);
CMeshComponentType getMeshComponentType(CMeshComponent meshComponent);
bool getMeshComponentNormalized(CMeshComponent meshComponent);
unsigned int getMeshComponentStride(CMeshComponent meshComponent);
unsigned int getMeshComponentOffset(CMeshComponent meshComponent);

CMeshComponent addPositions(CMesh mesh,
                            CRenderer renderer,
                            unsigned int numValues,
                            bool normalized,
                            unsigned int stride,
                            unsigned int offset);
void removePositions(CMesh mesh);
bool hasPositions(CMesh mesh);
CMeshComponent getPositions(CMesh mesh);
CMeshComponent addNormals(CMesh mesh,
                          CRenderer renderer,
                          unsigned int numValues,
                          bool normalized,
                          unsigned int stride,
                          unsigned int offset);
void removeNormals(CMesh mesh);
bool hasNormals(CMesh mesh);
CMeshComponent getNormals(CMesh mesh);
CMeshComponent addUVs(CMesh mesh,
                      CRenderer renderer,
                      unsigned int numValues,
                      bool normalized,
                      unsigned int stride,
                      unsigned int offset);
void removeUVs(CMesh mesh);
bool hasUVs(CMesh mesh);
CMeshComponent getUVs(CMesh mesh);
CMeshComponent addColors(CMesh mesh,
                         CRenderer renderer,
                         unsigned int numValues,
                         bool normalized,
                         unsigned int stride,
                         unsigned int offset);
void removeColors(CMesh mesh);
bool hasColors(CMesh mesh);
CMeshComponent getColors(CMesh mesh);
CMeshComponent addTangents(CMesh mesh,
                           CRenderer renderer,
                           unsigned int numValues,
                           bool normalized,
                           unsigned int stride,
                           unsigned int offset);
void removeTangents(CMesh mesh);
bool hasTangents(CMesh mesh);
CMeshComponent getTangents(CMesh mesh);
CMeshComponent addIndices(CMesh mesh,
                          CRenderer renderer,
                          unsigned int numValues,
                          bool normalized,
                          unsigned int stride,
                          unsigned int offset);
void removeIndices(CMesh mesh);
bool hasIndices(CMesh mesh);
CMeshComponent getIndices(CMesh mesh);
void calculateAABB(CMesh mesh);
CAABB getAABB(CMesh mesh);
CShader getVertexShader(CMesh mesh);
CShader getGeometryShader(CMesh mesh);
CMeshPrimitive getMeshPrimitive(CMesh mesh);
unsigned int getNumVertices(CMesh mesh);
unsigned int getNumIndices(CMesh mesh);
CMeshWinding getMeshWinding(CMesh mesh);
CMeshCullFace getMeshCullFace(CMesh mesh);
CShaderDefines getMeshDefines(CMesh mesh);
CShaderUniforms getMeshUniforms(CMesh mesh);

float *getDistance(CLOD lod);
CMesh getMesh(CLOD lod);
CMaterial getMaterial(CLOD lod);

void sortLODs(CModel model);
bool *getStippledLODs(CModel model);
float *getMaxDrawDistance(CModel model);
unsigned int getNumLODs(CModel model);
CLOD getLOD(CModel model, unsigned int index);
void addLOD(CModel model, CLOD lod);
void removeLOD(CModel model, unsigned int index);

CTextureType getTextureType(CTexture texture);
void allocData1D(CTexture texture,
                 unsigned int width,
                 CTextureInternalFormat internalFormat,
                 CTextureFormat format,
                 const void *data);
void allocData2D(CTexture texture,
                 unsigned int width,
                 unsigned int height,
                 CTextureInternalFormat internalFormat,
                 CTextureFormat format,
                 const void *data);
void allocFaceData(CTexture texture,
                   unsigned int width,
                   unsigned int height,
                   CTextureInternalFormat internalFormat,
                   CTextureFormat format,
                   const void **data);
void allocData3D(CTexture texture,
                 unsigned int width,
                 unsigned int height,
                 unsigned int depth,
                 CTextureInternalFormat internalFormat,
                 CTextureFormat format,
                 const void *data);
void generateMipmaps(CTexture texture);
CTextureMinFilter getMinFilter(CTexture texture);
void setMinFilter(CTexture texture, CTextureMinFilter filter);
CTextureMagFilter getMagFilter(CTexture texture);
void setMagFilter(CTexture texture, CTextureMagFilter filter);
CTextureWrap getUWrap(CTexture texture);
void setUWrap(CTexture texture, CTextureWrap wrap);
CTextureWrap getVWrap(CTexture texture);
void setVWrap(CTexture texture, CTextureWrap wrap);
CTextureWrap getWWrap(CTexture texture);
void setWWrap(CTexture texture, CTextureWrap wrap);
float getAnisotropy(float anisotropy);
void setAnisotropy(CTexture texture, float anisotropy);

CShaderType getType(CShader shader);
const char *getSource(CShader shader);

unsigned int getNumUniforms(CShaderUniforms uniforms);
int getUniformIndex(CShaderUniforms uniforms, const char *name);
void removeUniform(CShaderUniforms uniforms, int index);
CUniformType getUniformType(CShaderUniforms uniforms, int index);
float getUniformValueAsFloat(CShaderUniforms uniforms, int index);
CVec2 getUniformValueAsVec2(CShaderUniforms uniforms, int index);
CVec3 getUniformValueAsVec3(CShaderUniforms uniforms, int index);
CVec4 getUniformValueAsVec4(CShaderUniforms uniforms, int index);
int getUniformValueAsInt(CShaderUniforms uniforms, int index);
CIVec2 getUniformValueAsIVec2(CShaderUniforms uniforms, int index);
CIVec3 getUniformValueAsIVec3(CShaderUniforms uniforms, int index);
CIVec4 getUniformValueAsIVec4(CShaderUniforms uniforms, int index);
CTexture getUniformValueAsTexture(CShaderUniforms uniforms, int index);
void setUniformValueAsNothing(CShaderUniforms uniforms,
                            int index);
void setUniformValueAsFloat(CShaderUniforms uniforms,
                            int index,
                            float value);
void setUniformValueAsCVec2(CShaderUniforms uniforms,
                            int index,
                            CVec2 value);
void setUniformValueAsCVec3(CShaderUniforms uniforms,
                            int index,
                            CVec3 value);
void setUniformValueAsCVec4(CShaderUniforms uniforms,
                            int index,
                            CVec4 value);
void setUniformValueAsInt(CShaderUniforms uniforms,
                          int index,
                          unsigned int value);
void setUniformValueAsIVec2(CShaderUniforms uniforms,
                            int index,
                            CIVec2 value);
void setUniformValueAsIVec3(CShaderUniforms uniforms,
                            int index,
                            CIVec3 value);
void setUniformValueAsIVec4(CShaderUniforms uniforms,
                            int index,
                            CIVec4 value);
void setUniformValueAsTexture(CShaderUniforms uniforms,
                            int index,
                              CTexture value);

unsigned int getNumDefines(CShaderDefines defines);
int getDefineIndex(CShaderDefines defines, const char *name);
void removeDefine(CShaderDefines defines, int index);
const char *getDefineValue(CShaderDefines, int index);
void setDefineValue(CShaderDefines, int index, const char *value);

void *mapVertexBuffer(CVertexBuffer buffer, bool read, bool write);
void *mapVertexBufferRange(CVertexBuffer buffer,
                           unsigned int start,
                           unsigned int size,
                           bool read,
                           bool write,
                           bool invalidateRange,
                           bool invalidateBuffer,
                           bool unsynchronized);
void unmapVertexBuffer(CVertexBuffer buffer);
void allocVertexBufferData(CVertexBuffer buffer, unsigned int size);
unsigned int getVertexBufferSize(CVertexBuffer buffer);

CFramebuffer addFramebuffer(CRenderTarget renderTarget);
void removeFramebuffer(CRenderTarget renderTarget);
CFramebuffer getFramebuffer(CRenderTarget renderTarget);
bool hasFramebuffer(CRenderTarget renderTarget);
void setRTWidth(CRenderTarget renderTarget, unsigned int width);
unsigned int getRTWidth(CRenderTarget renderTarget);
void setRTHeight(CRenderTarget renderTarget, unsigned int height);
unsigned int getRTHeight(CRenderTarget renderTarget);
void setRTWidthAndHeight(CRenderTarget renderTarget,
                         unsigned int width,
                         unsigned int height);

void addDepth(CFramebuffer framebuffer,
              CTexture texture,
              CTextureInternalFormat internalFormat);
void addColor(CFramebuffer framebuffer,
              CTexture texture,
              CTextureInternalFormat internalFormat,
              CVec2 scale);
void finish(CFramebuffer framebuffer, bool depth);
void setFramebufferWidth(CFramebuffer framebuffer, unsigned int width);
unsigned int getFramebufferWidth(CFramebuffer framebuffer);
void setFramebufferHeight(CFramebuffer framebuffer, unsigned int height);
unsigned int getFramebufferHeight(CFramebuffer framebuffer);
void setFramebufferWidthAndHeight(CFramebuffer framebuffer,
                                  unsigned int width,
                                  unsigned int height);
CTexture getColorTexture(CFramebuffer framebuffer, unsigned int index);
CTexture getDepthTexture(CFramebuffer framebuffer);
bool hasDepthTexture(CFramebuffer framebuffer);

#endif // C_API_H
