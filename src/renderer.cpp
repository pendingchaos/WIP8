#include "renderer.h"

#include <sstream>
#include <iostream>

#include <SOIL/SOIL.h>

#include "utils/memory.h"
#include "utils/utils.h"
#include "GL/glutils.h"
#include "scene.h"
#include "resourcemanager.h"
#include "entity.h"
#include "vertexbuffer.h"

Renderer::Renderer(Backend *backend) : mBackend(backend)
{
    mResMgr = NEW(ResourceManager, this);

    createSkybox();
}

Renderer::~Renderer()
{
    mSkyboxMesh = nullRes<Mesh>();
    mSkyboxMaterial = nullRes<Material>();

    DELETE(Backend, mBackend); //TODO: The Renderer should not manage the backend.
    DELETE(ResourceManager, mResMgr);
}

void Renderer::render(RenderTarget *target, Scene *scene)
{
    if (scene->mSkyboxTexture != nullRes<Texture>())
    {
        mSkyboxMaterial->mUniforms["skyBoxTexture"] = scene->mSkyboxTexture;

        mBackend->submitDrawCall(DrawCall(scene, mSkyboxMesh, mSkyboxMaterial, glm::mat4(1.0f), 1.0f));
    }

    for (std::vector<Entity *>::const_iterator it = scene->getEntities().begin();
         it != scene->getEntities().end(); ++it)
    {
        renderEntity(*it, scene);
    }

    mBackend->executeDrawCalls(target);
}

CompiledShader *Renderer::createShader(std::string filename,
                                       CompiledShader::Type type,
                                       std::string source,
                                       std::map<std::string, std::string> defines)
{
    unsigned int glslMajor;
    unsigned int glslMinor;

    getGLSLVersion(glslMajor, glslMinor);

    std::stringstream ss;
    ss << "#version ";
    ss << glslMajor * 100 + glslMinor * 10;
    ss << "\n#define GLSLMAJOR ";
    ss << glslMajor;
    ss << "\n#define GLSLMINOR ";
    ss << glslMinor;
    ss << "\n";

    if (glslMajor == 1 and glslMinor < 4)
    {
        ss << "#define CORE 0\n";
    } else
    {
        ss << "#define CORE 1\n";
    }

    switch (type)
    {
        case CompiledShader::Vertex:
        {
            ss << "#define VERTEX_SHADER 1\n";
            break;
        }
        case CompiledShader::Fragment:
        {
            ss << "#define FRAGMENT_SHADER 1\n";
            break;
        }
        case CompiledShader::Geometry:
        {
            ss << "#define GEOMETRY_SHADER 1\n";
            break;
        }
        case CompiledShader::TessControl:
        {
            ss << "#define TESS_CONTROL_SHADER 1\n";
            break;
        }
        case CompiledShader::TessEval:
        {
            ss << "#define TESS_EVAL_SHADER 1\n";
            break;
        }
    }

    for (std::map<std::string, std::string>::iterator it = defines.begin();
         it != defines.end(); ++it)
    {
        ss << "#define " << it->first << ' ' << it->second << std::endl;
    }

    ss << handleIncludes(source);

    std::string str = ss.str();

    const char *c_str = str.c_str();

    CompiledShader *shader = createShader(filename, type, 1, &c_str);

    str.clear();

    return shader;
}

unsigned char *loadImage(const char *filename,
                         Texture::InternalFormat *internalFormat,
                         Texture::Format *format,
                         int *width,
                         int *height)
{
    int channels;

    unsigned char *data = SOIL_load_image(filename, width, height, &channels, SOIL_LOAD_AUTO);

    if (data == NULL)
    {
        std::cout << "Error loading file: \"" << filename << "\": " << SOIL_last_result() << std::endl;
    }

    switch (channels)
    {
        case 1:
        {
            *internalFormat = Texture::RedU8_Norm_InternalFormat;
            *format = Texture::RedU8_Format;
            break;
        }
        case 2:
        {
            *internalFormat = Texture::RedGreenU8_Norm_InternalFormat;
            *format = Texture::RedGreenU8_Format;
            break;
        }
        case 3:
        {
            *internalFormat = Texture::RGBU8_Norm_InternalFormat;
            *format = Texture::RGBU8_Format;
            break;
        }
        case 4:
        {
            *internalFormat = Texture::RGBAU8_Norm_InternalFormat;
            *format = Texture::RGBAU8_Format;
            break;
        }
    }

    return data;
}

Texture *Renderer::createTexture(const char *filename)
{
    Texture *texture = createTexture(Texture::Texture2D);

    Texture::InternalFormat internalFormat;
    Texture::Format format;
    int width;
    int height;

    unsigned char *data = loadImage(filename, &internalFormat, &format, &width, &height);

    texture->allocData2D(width, height, internalFormat, format, data);

    SOIL_free_image_data(data);

    return texture;
}

Texture *Renderer::createCubemap(const char *posX, const char *negX,
                                 const char *posY, const char *negY,
                                 const char *posZ, const char *negZ)
{
    Texture *texture = createTexture(Texture::TextureCubeMap);

    const char *filenames[6] = {posX, negX,
                                posY, negY,
                                posZ, negZ};

    void *datas[6];

    Texture::InternalFormat internalFormat;
    Texture::Format format;
    int width;
    int height;

    datas[0] = loadImage(filenames[0], &internalFormat, &format, &width, &height);

    for (unsigned int i=1; i<6; ++i)
    {
        Texture::InternalFormat nextInternalFormat;
        Texture::Format nextFormat;
        int nextWidth;
        int nextHeight;

        datas[i] = loadImage(filenames[i],
                             &nextInternalFormat,
                             &nextFormat,
                             &nextWidth,
                             &nextHeight);

        if (nextInternalFormat != internalFormat)
        {
            std::cout << "Inconsistent cube map formats" << std::endl;
            return NULL;
        }

        if (nextFormat != format)
        {
            std::cout << "Inconsistent cube map formats" << std::endl;
            return NULL;
        }

        if (nextWidth != width)
        {
            std::cout << "Inconsistent cube map widths" << std::endl;
            return NULL;
        }

        if (nextHeight != height)
        {
            std::cout << "Inconsistent cube map heights" << std::endl;
            return NULL;
        }
    }

    texture->allocFaceData(width, height, internalFormat, format, (const void **)datas);

    for (unsigned int i=0; i<6; ++i)
    {
        SOIL_free_image_data((unsigned char *)datas[i]);
    }

    return texture;
}

void Renderer::createSkybox()
{
    mSkyboxMesh = mResMgr->createMesh(mResMgr->load("res/shaders/skybox vertex.json").cast<Shader>(), Mesh::Triangles, 36);
    VertexBuffer *buffer = mSkyboxMesh->addPositions(this, MeshComponent(3, MeshComponent::Float32))->getVertexBuffer();

    buffer->alloc(sizeof(glm::vec3)*36);
    glm::vec3 *positions = (glm::vec3 *)buffer->map(false, true);

    positions[0] = glm::vec3(-1.0f,  1.0f, -1.0f);
    positions[1] = glm::vec3(-1.0f, -1.0f, -1.0f);
    positions[2] = glm::vec3(1.0f, -1.0f, -1.0f);
    positions[3] = glm::vec3(1.0f, -1.0f, -1.0f);
    positions[4] = glm::vec3(1.0f,  1.0f, -1.0f);
    positions[5] = glm::vec3(-1.0f,  1.0f, -1.0f);

    positions[6] = glm::vec3(-1.0f, -1.0f,  1.0f);
    positions[7] = glm::vec3(-1.0f, -1.0f, -1.0f);
    positions[8] = glm::vec3(-1.0f,  1.0f, -1.0f);
    positions[9] = glm::vec3(-1.0f,  1.0f, -1.0f);
    positions[10] = glm::vec3(-1.0f,  1.0f,  1.0f);
    positions[11] = glm::vec3(-1.0f, -1.0f,  1.0f);

    positions[12] = glm::vec3(1.0f, -1.0f, -1.0f);
    positions[13] = glm::vec3(1.0f, -1.0f,  1.0f);
    positions[14] = glm::vec3(1.0f,  1.0f,  1.0f);
    positions[15] = glm::vec3(1.0f,  1.0f,  1.0f);
    positions[16] = glm::vec3(1.0f,  1.0f, -1.0f);
    positions[17] = glm::vec3(1.0f, -1.0f, -1.0f);

    positions[18] = glm::vec3(-1.0f, -1.0f,  1.0f);
    positions[19] = glm::vec3(-1.0f,  1.0f,  1.0f);
    positions[20] = glm::vec3(1.0f,  1.0f,  1.0f);
    positions[21] = glm::vec3(1.0f,  1.0f,  1.0f);
    positions[22] = glm::vec3(1.0f, -1.0f,  1.0f);
    positions[23] = glm::vec3(-1.0f, -1.0f,  1.0f);

    positions[24] = glm::vec3(-1.0f,  1.0f, -1.0f);
    positions[25] = glm::vec3(1.0f,  1.0f, -1.0f);
    positions[26] = glm::vec3(1.0f,  1.0f,  1.0f);
    positions[27] = glm::vec3(1.0f,  1.0f,  1.0f);
    positions[28] = glm::vec3(-1.0f,  1.0f,  1.0f);
    positions[29] = glm::vec3(-1.0f,  1.0f, -1.0f);

    positions[30] = glm::vec3(-1.0f, -1.0f, -1.0f);
    positions[31] = glm::vec3(-1.0f, -1.0f,  1.0f);
    positions[32] = glm::vec3(1.0f, -1.0f, -1.0f);
    positions[33] = glm::vec3(1.0f, -1.0f, -1.0f);
    positions[34] = glm::vec3(-1.0f, -1.0f,  1.0f);
    positions[35] = glm::vec3(1.0f, -1.0f,  1.0f);

    buffer->unmap();

    mSkyboxMaterial = mResMgr->createMaterial(mResMgr->load("res/shaders/skybox fragment.json").cast<Shader>());
    mSkyboxMaterial->mWriteDepth = false;
}

void Renderer::renderEntity(const Entity *entity, Scene *scene)
{
    glm::mat4 matrix =   scene->mProjectionTransform.getMatrix()
                       * scene->mViewTransform.getMatrix()
                       * entity->mTransform.getMatrix();

    float distance = std::abs(glm::vec3(matrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)).z);

    if (distance > entity->mModel->mMaxDrawDistance and entity->mModel->mMaxDrawDistance >= 0.0f)
    {
        return;
    }

    if (entity->mModel->mLODs.size() == 1)
    {
        renderLOD(entity, scene, &entity->mModel->mLODs[0], 1.0f);
    } else if (entity->mModel->mStippledLODs)
    {
        float weight1;
        LOD *lod1 = entity->mModel->choose1stLOD(distance, &weight1);

        float weight2;
        LOD *lod2 = entity->mModel->choose2ndLOD(distance, &weight2);

        if (weight1 < weight2)
        {
            weight1 *= 2.0f;
            weight2 = 1.0f;
        } else
        {
            weight1 = 1.0f;
            weight2 *= 2.0f;
        }

        if (weight1 != 0.0f)
        {
            renderLOD(entity,
                      scene,
                      lod1,
                      weight1);
        }

        if (weight2 != 0.0f)
        {
            renderLOD(entity,
                      scene,
                      lod2,
                      weight2);
        }
    } else
    {
        renderLOD(entity,
                  scene,
                  entity->mModel->choose1stLOD(distance, NULL),
                  1.0);
    }
}

void Renderer::renderLOD(const Entity *entity, Scene *scene, LOD *lod, float weight)
{
    if (lod == NULL)
    {
        return;
    }

    ResPtr<Mesh> mesh = lod->getMesh();
    ResPtr<Material> material = lod->getMaterial();

    if (mesh != nullRes<Mesh>() and material != nullRes<Material>())
    {
        mBackend->submitDrawCall(DrawCall(scene, mesh, material, entity->mTransform.getMatrix(), weight));
    }
}

std::string Renderer::handleIncludes(std::string source) const
{
    std::stringstream sourceStream(source);
    std::stringstream outputStream;
    std::string line;

    unsigned int includeLen = sizeof("#include") - 1;

    while (std::getline(sourceStream, line))
    {
        if (line.size() >= includeLen)
        {
            if (std::string(line.begin(), line.begin()+includeLen) == "#include")
            {
                std::string filename = trim(std::string(line.begin()+includeLen, line.end()));

                outputStream << readFile(filename) << std::endl;
            } else
            {
                outputStream << line << std::endl;
            }
        } else
        {
            outputStream << line << std::endl;
        }
    }

    return outputStream.str();
}
