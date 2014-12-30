#include "c api.h"

#include "utils/memory.h"
#include "GL/glbackend.h"
#include "renderer.h"
#include "backend.h"
#include "scene.h"
#include "entity.h"
#include "transform.h"
#include "resourcemanager.h"
#include "resource.h"
#include "material.h"
#include "mesh.h"
#include "model.h"
#include "texture.h"
#include "shader.h"
#include "vertexbuffer.h"
#include "rendertarget.h"

CRenderer createRenderer(CBackend backend)
{
    return reinterpret_cast<CRenderer>(NEW(Renderer, reinterpret_cast<Backend *>(backend)));
}

void deleteRenderer(CRenderer renderer)
{
    Renderer *renderer2 = reinterpret_cast<Renderer *>(renderer);

    DELETE(Renderer, renderer2);
}

void render(CRenderer renderer, CRenderTarget target, CScene scene)
{
    reinterpret_cast<Renderer *>(renderer)->render(reinterpret_cast<RenderTarget *>(target),
                                                   reinterpret_cast<Scene *>(scene));
}

CResourceManager getResourceManager(CRenderer renderer)
{
    return reinterpret_cast<CResourceManager>(reinterpret_cast<Renderer *>(renderer)->getResourceManager());
}

CBackend getBackend(CRenderer renderer)
{
    return reinterpret_cast<CBackend>(reinterpret_cast<Renderer *>(renderer)->getBackend());
}

CBackend createGLBackend()
{
    return reinterpret_cast<CBackend>(NEW(GLBackend));
}

CVertexBuffer createVertexBuffer(CBackend backend)
{
    return reinterpret_cast<CVertexBuffer>(reinterpret_cast<Backend *>(backend)->createVertexBuffer());
}

CFramebuffer createFramebuffer(CBackend backend)
{
    return reinterpret_cast<CFramebuffer>(reinterpret_cast<Backend *>(backend)->createFramebuffer());
}

float getMaxAnisotropy(CBackend backend)
{
    return reinterpret_cast<Backend *>(backend)->getMaxAnisotropy();
}

void submitDrawCall(CBackend backend,
                    CScene scene,
                    CMesh mesh,
                    CMaterial material,
                    CMatrix4x4 matrix,
                    float lodStipple)
{
    reinterpret_cast<Backend *>(backend)->submitDrawCall(DrawCall(reinterpret_cast<Scene *>(scene),
                                                                  reinterpret_cast<Mesh *>(mesh),
                                                                  reinterpret_cast<Material *>(material),
                                                                  *(glm::mat4 *)&matrix,
                                                                  lodStipple));
}

void executeDrawCalls(CBackend backend, CRenderTarget target)
{
    reinterpret_cast<Backend *>(backend)->executeDrawCalls(reinterpret_cast<RenderTarget *>(target));
}

CScene createScene(CRenderer renderer)
{
    return reinterpret_cast<CScene>(NEW(Scene, reinterpret_cast<Renderer *>(renderer)));
}

CEntity createEntity(CScene scene, CModel model)
{
    return reinterpret_cast<CEntity>(reinterpret_cast<Scene *>(scene)->createEntity(reinterpret_cast<Model *>(model)));
}

void destroyEntity(CScene scene, CEntity entity)
{
    reinterpret_cast<Scene *>(scene)->destroyEntity(reinterpret_cast<Entity *>(entity));
}

unsigned int getNumEntities(CScene scene)
{
    return reinterpret_cast<Scene *>(scene)->getEntities().size();
}

CEntity *getEntities(CScene scene)
{
    return (CEntity *)reinterpret_cast<Scene *>(scene)->getEntities().data();
}

CTransform getViewTransform(CScene scene)
{
    return reinterpret_cast<CTransform>(&reinterpret_cast<Scene *>(scene)->mViewTransform);
}

CTransform getProjectionTransform(CScene scene)
{
    return reinterpret_cast<CTransform>(&reinterpret_cast<Scene *>(scene)->mProjectionTransform);
}

CTexture getSkyboxTexture(CScene scene)
{
    return reinterpret_cast<CTexture>(reinterpret_cast<Scene *>(scene)->mSkyboxTexture.getPointer());
}

void setSkyboxTexture(CScene scene, CTexture texture)
{
    reinterpret_cast<Scene *>(scene)->mSkyboxTexture = reinterpret_cast<Texture *>(texture);
}

CTransform getTransform(CEntity entity)
{
    return reinterpret_cast<CTransform>(&reinterpret_cast<Entity *>(entity)->mTransform);
}

CModel getModel(CEntity entity)
{
    return reinterpret_cast<CModel>(&reinterpret_cast<Entity *>(entity)->mModel);
}

void resetTransform(CTransform transform)
{
    reinterpret_cast<Transform *>(transform)->reset();
}

void translate(CTransform transform, CVec3 translation)
{
    reinterpret_cast<Transform *>(transform)->translate(*(glm::vec3 *)&translation);
}

void rotate(CTransform transform, float angle, CVec3 axis)
{
    reinterpret_cast<Transform *>(transform)->rotate(angle, *(glm::vec3 *)&axis);
}

void scale(CTransform transform, CVec3 by)
{
    reinterpret_cast<Transform *>(transform)->scale(*(glm::vec3 *)&by);
}

void lookAt(CTransform transform, CVec3 eye, CVec3 center, CVec3 up)
{
    reinterpret_cast<Transform *>(transform)->lookAt(*(glm::vec3 *)&eye,
                                                     *(glm::vec3 *)&center,
                                                     *(glm::vec3 *)&up);
}

void perspective(CTransform transform, float fovy, float aspect, float near, float far)
{
    reinterpret_cast<Transform *>(transform)->perspective(fovy, aspect, near, far);
}

void setMatrix(CTransform transform, CMatrix4x4 matrix)
{
    reinterpret_cast<Transform *>(transform)->setMatrix(*(glm::mat4 *)&matrix);
}

CMatrix4x4 getMatrix(CTransform transform)
{
    glm::mat4 matrix = reinterpret_cast<Transform *>(transform)->getMatrix();

    return *(CMatrix4x4 *)&matrix;
}

void multiplyMatrix(CTransform transform, CMatrix4x4 matrix)
{
    reinterpret_cast<Transform *>(transform)->multiplyMatrix(*(glm::mat4 *)&matrix);
}

CTransform getParent(CTransform transform)
{
    return reinterpret_cast<CTransform>(reinterpret_cast<Transform *>(transform)->getParent());
}

void setParent(CTransform transform, CTransform parent)
{
    reinterpret_cast<Transform *>(transform)->setParent(reinterpret_cast<Transform *>(parent));
}

unsigned int getNumChildren(CTransform transform)
{
    return reinterpret_cast<Transform *>(transform)->getChildren().size();
}

CTransform *getChildren(CTransform transform)
{
    return (CTransform *)reinterpret_cast<Transform *>(transform)->getChildren().data();
}

void removeChild(CTransform transform, unsigned int index)
{
    Transform *transform2 = reinterpret_cast<Transform *>(transform);

    transform2->removeChild(index);
}

void addChild(CTransform transform, CTransform child)
{
    reinterpret_cast<Transform *>(transform)->addChild(reinterpret_cast<Transform *>(child));
}

unsigned int getNumResources(CResourceManager resMgr);
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
