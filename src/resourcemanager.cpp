#include "resourcemanager.h"

#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>
#include <vector>

#include "utils/memory.h"
#include "utils/utils.h"
#include "GL/glutils.h"
#include "texture.h"
#include "model.h"
#include "mesh.h"
#include "shader.h"
#include "material.h"
#include "awm.h"
#include "renderer.h"

ResourceManager::ResourceManager(Renderer *renderer) : mRenderer(renderer) {}

ResourceManager::~ResourceManager()
{
    for (unsigned int i=0; i<256; ++i)
    {
        deleteUnusedResources();

        if (mResources.size() == 0)
        {
            break;
        }
    }

    for (std::unordered_map<std::string, Resource *>::iterator it = mResources.begin();
         it != mResources.end(); ++it)
    {
        std::cout << "Warning: Unfreed resource: " << it->first << std::endl;
    }
}

void ResourceManager::deleteUnusedResources()
{
    std::vector<std::string> toDelete;

    for (std::unordered_map<std::string, Resource *>::iterator it = mResources.begin();
         it != mResources.end(); ++it)
    {
        if (it->second->getRefCount() <= 0)
        {
            toDelete.push_back(it->first);
        }
    }

    for (std::vector<std::string>::iterator it
         = toDelete.begin(); it != toDelete.end(); ++it)
    {
        std::unordered_map<std::string, Resource *>::iterator pos
        = mResources.find(*it);

        std::cout << "Deleting " << pos->first << std::endl;

        DELETE(Resource, pos->second);
        mResources.erase(pos);
    }
}

ResPtr<Resource> ResourceManager::load(std::string filename)
{
    std::unordered_map<std::string, Resource *>::iterator pos
    = mResources.find(filename);

    if (pos == mResources.end())
    {
        Resource *resource;

        try
        {
            std::cout << "Loading resource: " << filename << std::endl;
            resource = _load(filename);
        } catch (const std::exception& err)
        {
            std::cout << "Unable to load resource \"" << filename << "\": " << err.what() << std::endl;
            return nullRes<Resource>();
        }

        mResources.emplace(filename, resource);

        return ResPtr<Resource>(resource);
    }

    return ResPtr<Resource>(pos->second);
}

ResPtr<Texture> ResourceManager::createTexture(Texture::Type type)
{
    Texture *texture = mRenderer->createTexture(type);

    std::stringstream name;
    name << "Texture ";
    name << texture;

    mResources.emplace(name.str(), texture);

    return ResPtr<Texture>(texture);
}

ResPtr<Model> ResourceManager::createModel()
{
    Model *model = NEW(Model);

    std::stringstream name;
    name << "Model ";
    name << model;

    mResources.emplace(name.str(), model);

    return ResPtr<Model>(model);
}

ResPtr<Shader> ResourceManager::createShader(CompiledShader::Type type, std::string source)
{
    Shader *shader = NEW(Shader, mRenderer, "", type, source);

    std::stringstream name;
    name << "Shader ";
    name << shader;

    mResources.emplace(name.str(), shader);

    return ResPtr<Shader>(shader);
}

ResPtr<Mesh> ResourceManager::createMesh(ResPtr<Shader> shader,
                                         Mesh::Primitive primitive,
                                         unsigned int numVertices,
                                         unsigned int numIndices)
{
    Mesh *mesh = NEW(Mesh, shader, primitive, numVertices, numIndices);

    std::stringstream name;
    name << "Mesh ";
    name << mesh;

    mResources.emplace(name.str(), mesh);

    return ResPtr<Mesh>(mesh);
}

ResPtr<Material> ResourceManager::createMaterial(ResPtr<Shader> shader)
{
    Material *material = NEW(Material, shader);

    std::stringstream name;
    name << "Material ";
    name << material;

    mResources.emplace(name.str(), material);

    return ResPtr<Material>(material);
}

const Json::Value& getMember(Json::Value object, const char *name)
{
    if (object.type() != Json::objectValue)
    {
        throw std::runtime_error("Value is not an object.");
    }

    if (object.isMember(name))
    {
        return object[name];
    }


    std::stringstream ss;

    ss << "Object does not have a member named \"";
    ss << name;
    ss << "\".";

    throw std::runtime_error(ss.str());
}

Resource *ResourceManager::_load(std::string& filename)
{
    Json::Reader reader(Json::Features::strictMode());

    Json::Value root;

    if (not reader.parse(readFile(filename), root))
    {
        std::cout << reader.getFormattedErrorMessages() << std::endl;
        return NULL;
    }

    std::unordered_map<std::string, ResPtr<Resource> > externalResources;

    if (root.isMember("external resources"))
    {
        Json::Value externResVal = root["external resources"];
        Json::Value::Members members = externResVal.getMemberNames();

        for (Json::Value::Members::iterator it = members.begin(); it != members.end(); ++it)
        {
            std::string name = *it;
            ResPtr<Resource> resource = load(externResVal[name].asString());

            externalResources[name] = resource;
        }
    }

    std::string resourceType = root["type"].asString();

    if (resourceType == "texture")
    {
        return loadTexture(getMember(root, "texture"), externalResources);
    } else if (resourceType == "model")
    {
        return loadModel(getMember(root, "model"), externalResources);
    } else if (resourceType == "mesh")
    {
        return loadMesh(getMember(root, "mesh"), externalResources);
    } else if (resourceType == "shader")
    {
        return loadShader(getMember(root, "shader"), externalResources);
    } else if (resourceType == "material")
    {
        return loadMaterial(getMember(root, "material"), externalResources);
    } else if (resourceType == "redirection")
    {
        return loadRedirection(getMember(root, "redirection"), externalResources, filename);
    } else
    {
        throw std::runtime_error("Unknown resource type.");
    }
}

Texture *ResourceManager::loadTexture(const Json::Value& json,
                                      const std::unordered_map<std::string, ResPtr<Resource> >& externalResources)
{
    std::string textureType = getMember(json, "type").asString();
    Texture::Type type;

    if (textureType == "1d")
    {
        type = Texture::Texture1D;
    } else if (textureType == "2d")
    {
        type = Texture::Texture2D;
    } else if (textureType == "3d")
    {
        type = Texture::Texture3D;
    } else if (textureType == "cubemap")
    {
        type = Texture::TextureCubeMap;
    } else
    {
        throw std::runtime_error("Unknown texture type.");
    }

    Texture *texture;
    bool hasMipmaps = false;
    bool linearMipmapping = false;

    if (type == Texture::TextureCubeMap)
    {
        texture = mRenderer->createCubemap(getMember(json, "positive x").asCString(), getMember(json, "negative x").asCString(),
                                           getMember(json, "positive y").asCString(), getMember(json, "negative y").asCString(),
                                           getMember(json, "positive z").asCString(), getMember(json, "negative z").asCString());
    } else
    {
        texture = mRenderer->createTexture(getMember(json, "filename").asCString());
    }

    if (json.isMember("generate mipmaps"))
    {
        if (json["generate mipmaps"].asBool())
        {
            texture->generateMipmaps();

            hasMipmaps = true;
        }
    }

    if (json.isMember("linear mipmapping"))
    {
        if (json["generate mipmaps"].asBool())
        {
            linearMipmapping = true;
        }
    }

    if (json.isMember("anisotropy"))
    {
        Json::Value anisotropy = json["anisotropy"];

        if (anisotropy.isString())
        {
            if (anisotropy.asString() == "max")
            {
                texture->setAnisotropy(mRenderer->getMaxAnisotropy());
            } else
            {
                throw std::runtime_error("Unknown anisotropy.");
            }
        } else
        {
            texture->setAnisotropy(std::min(mRenderer->getMaxAnisotropy(), anisotropy.asFloat()));
        }
    }

    if (json.isMember("min filter"))
    {
        std::string minFilter = json["min filter"].asString();

        if (minFilter == "linear")
        {
            if (hasMipmaps)
            {
                if (linearMipmapping)
                {
                    texture->setMinFilter(Texture::LinearWithLinearMipmapMinFilter);
                } else
                {
                    texture->setMinFilter(Texture::LinearWithNearestMipmapMinFilter);
                }
            } else
            {
                texture->setMinFilter(Texture::LinearMinFilter);
            }
        } else if (minFilter == "nearest")
        {
            if (hasMipmaps)
            {
                if (linearMipmapping)
                {
                    texture->setMinFilter(Texture::NearestWithLinearMipmapMinFilter);
                } else
                {
                    texture->setMinFilter(Texture::NearestWithNearestMipmapMinFilter);
                }
            } else
            {
                texture->setMinFilter(Texture::NearestMinFilter);
            }
        } else
        {
            throw std::runtime_error("Unknown min filter.");
        }
    }

    if (json.isMember("mag filter"))
    {
        std::string magFilter = json["mag filter"].asString();

        if (magFilter == "linear")
        {
            texture->setMagFilter(Texture::LinearMagFilter);
        } else if (magFilter == "nearest")
        {
            texture->setMagFilter(Texture::NearestMagFilter);
        } else
        {
            throw std::runtime_error("Unknown mag filter.");
        }
    }

    if (json.isMember("wrap u"))
    {
        std::string wrapU = json["wrap u"].asString();

        if (wrapU == "repeat")
        {
            texture->setUWrap(Texture::Repeat);
        } else if (wrapU == "mirrored repeat")
        {
            texture->setUWrap(Texture::MirroredRepeat);
        } else if (wrapU == "clamp")
        {
            texture->setUWrap(Texture::Clamp);
        } else
        {
            throw std::runtime_error("Unknown U wrap.");
        }
    }

    if (json.isMember("wrap v"))
    {
        std::string wrapV = json["wrap v"].asString();

        if (wrapV == "repeat")
        {
            texture->setVWrap(Texture::Repeat);
        } else if (wrapV == "mirrored repeat")
        {
            texture->setVWrap(Texture::MirroredRepeat);
        } else if (wrapV == "clamp")
        {
            texture->setVWrap(Texture::Clamp);
        } else
        {
            throw std::runtime_error("Unknown V wrap.");
        }
    }

    if (json.isMember("wrap w"))
    {
        std::string wrapW = json["wrap w"].asString();

        if (wrapW == "repeat")
        {
            texture->setWWrap(Texture::Repeat);
        } else if (wrapW == "mirrored repeat")
        {
            texture->setWWrap(Texture::MirroredRepeat);
        } else if (wrapW == "clamp")
        {
            texture->setWWrap(Texture::Clamp);
        } else
        {
            throw std::runtime_error("Unknown W wrap.");
        }
    }

    return texture;
}

Model *ResourceManager::loadModel(const Json::Value& value,
                                  const std::unordered_map<std::string, ResPtr<Resource> >& externalResources)
{
    Json::Value lods = getMember(value, "lods");

    if (not lods.isArray())
    {
        throw std::runtime_error("Unable to get the LODs.");
    }

    Model *model = NEW(Model);

    if (value.isMember("stippledLODs"))
    {
        model->mStippledLODs = value["stippledLODs"].asBool();
    }

    for (Json::Value::iterator it = lods.begin(); it != lods.end(); ++it)
    {
        Json::Value lod = *it;

        std::string mesh = getMember(lod, "mesh").asString();
        std::string material = getMember(lod, "material").asString();
        float distance = getMember(lod, "distance").asFloat();

        model->mLODs.push_back(LOD(externalResources.at(mesh).cast<Mesh>(),
                                   externalResources.at(material).cast<Material>(),
                                   distance));
    }

    model->sortLODs();

    return model;
}

Shader *ResourceManager::loadShader(const Json::Value& json,
                                    const std::unordered_map<std::string, ResPtr<Resource> >& externalResources)
{
    std::string type = json["type"].asString();

    std::string filename = json["filename"].asString();

    if (type == "vertex")
    {
        return NEW(Shader, mRenderer, filename, CompiledShader::Vertex, readFile(filename));
    } else if (type == "fragment")
    {
        return NEW(Shader, mRenderer, filename, CompiledShader::Fragment, readFile(filename));
    } else if (type == "geometry")
    {
        return NEW(Shader, mRenderer, filename, CompiledShader::Geometry, readFile(filename));
    } else if (type == "tessellation control")
    {
        return NEW(Shader, mRenderer, filename, CompiledShader::TessControl, readFile(filename));
    } else if (type == "tessellation evaluation")
    {
        return NEW(Shader, mRenderer, filename, CompiledShader::TessEval, readFile(filename));
    } else
    {
        throw std::runtime_error("Unknown shader type.");
    }
}

Mesh *ResourceManager::loadMesh(const Json::Value& json,
                                const std::unordered_map<std::string, ResPtr<Resource> >& externalResources)
{
    ResPtr<Shader> shader = externalResources.at(getMember(json, "vertex shader").asString()).cast<Shader>();

    Mesh *mesh = loadAWM(mRenderer, shader, getMember(json, "awm filename").asCString());

    if (json.isMember("geometry shader"))
    {
        mesh->mGeometryShader = externalResources.at(json["geometry shader"].asString()).cast<Shader>();
    }

    if (json.isMember("tessellation control shader"))
    {
        mesh->mTessControlShader = externalResources.at(json["tessellation control shader"].asString()).cast<Shader>();
    }

    if (json.isMember("tessellation evaluation shader"))
    {
        mesh->mTessEvalShader = externalResources.at(json["tessellation evaluation shader"].asString()).cast<Shader>();
    }

    loadUniforms(getMember(json, "uniforms"), mesh->mUniforms, externalResources);
    loadDefines(getMember(json, "defines"), mesh->mDefines);

    if (json.isMember("cull face"))
    {
        std::string cullFace = json["cull face"].asString();

        if (cullFace == "none")
        {
            mesh->mCullFace = Mesh::None;
        } else if (cullFace == "front")
        {
            mesh->mCullFace = Mesh::Front;
        } else if (cullFace == "back")
        {
            mesh->mCullFace = Mesh::Back;
        } else
        {
            throw std::runtime_error("Unknown cull face.");
        }
    }

    if (json.isMember("front face winding"))
    {
        std::string winding = json["front face winding"].asString();

        if (winding == "clockwise")
        {
            mesh->mFrontFaceWinding = Mesh::Clockwise;
        } else if (winding == "counter clockwise")
        {
            mesh->mFrontFaceWinding = Mesh::CounterClockwise;
        } else
        {
            throw std::runtime_error("Unknown front face winding");
        }
    }

    if (json.isMember("patch size"))
    {
        mesh->mPrimitive = Mesh::Patches;
        mesh->mPatchSize = json["patch size"].asUInt();
    }

    return mesh;
}

Material *ResourceManager::loadMaterial(const Json::Value& json,
                                        const std::unordered_map<std::string, ResPtr<Resource> >& externalResources)
{
    ResPtr<Shader> shader = externalResources.at(json["shader"].asString()).cast<Shader>();

    Material *material = NEW(Material, shader);

    loadUniforms(getMember(json, "uniforms"), material->mUniforms, externalResources);
    loadDefines(getMember(json, "defines"), material->mDefines);

    return material;
}

//TODO: Complete this to support graphics card and drivers.
bool isFeatureSetSupported(const Json::Value& featureSet)
{
    GraphicsCard card = getGraphicsCard();
    GraphicsDriver driver = getGraphicsDriver();
    unsigned int glslMajor, glslMinor;
    getGLSLVersion(glslMajor, glslMinor);

    switch (glslMajor*10+glslMinor)
    {
        case 11:
        {
            if (featureSet.isMember("glsl 1.1 supported"))
            {
                if (not featureSet["glsl 1.1 supported"])
                {
                    return false;
                }
            }
        }
        case 12:
        {
            if (featureSet.isMember("glsl 1.2 supported"))
            {
                if (not featureSet["glsl 1.2 supported"])
                {
                    return false;
                }
            }
        }
        case 13:
        {
            if (featureSet.isMember("glsl 1.3 supported"))
            {
                if (not featureSet["glsl 1.3 supported"])
                {
                    return false;
                }
            }
        }
        case 14:
        {
            if (featureSet.isMember("glsl 1.4 supported"))
            {
                if (not featureSet["glsl 1.4 supported"])
                {
                    return false;
                }
            }
        }
        case 15:
        {
            if (featureSet.isMember("glsl 1.5 supported"))
            {
                if (not featureSet["glsl 1.5 supported"])
                {
                    return false;
                }
            }
        }
        case 33:
        {
            if (featureSet.isMember("glsl 3.3 supported"))
            {
                if (not featureSet["glsl 3.3 supported"])
                {
                    return false;
                }
            }
        }
        case 40:
        {
            if (featureSet.isMember("glsl 4.0 supported"))
            {
                if (not featureSet["glsl 4.0 supported"])
                {
                    return false;
                }
            }
        }
        case 41:
        {
            if (featureSet.isMember("glsl 4.1 supported"))
            {
                if (not featureSet["glsl 4.1 supported"])
                {
                    return false;
                }
            }
        }
        case 42:
        {
            if (featureSet.isMember("glsl 4.2 supported"))
            {
                if (not featureSet["glsl 4.2 supported"])
                {
                    return false;
                }
            }
        }
        case 43:
        {
            if (featureSet.isMember("glsl 4.3 supported"))
            {
                if (not featureSet["glsl 4.3 supported"])
                {
                    return false;
                }
            }
        }
        case 44:
        {
            if (featureSet.isMember("glsl 4.4 supported"))
            {
                if (not featureSet["glsl 4.4 supported"])
                {
                    return false;
                }
            }
        }
        case 45:
        {
            if (featureSet.isMember("glsl 4.5 supported"))
            {
                if (not featureSet["glsl 4.5 supported"])
                {
                    return false;
                }
            }
        }
    }

    return true;
}

Resource *ResourceManager::loadRedirection(const Json::Value& json,
                                           const std::unordered_map<std::string, ResPtr<Resource> >& externalResources,
                                           std::string& filename)
{
    Json::Value targets = getMember(json, "targets");

    GraphicsCard card = getGraphicsCard();
    GraphicsDriver driver = getGraphicsDriver();
    unsigned int glslMajor, glslMinor;
    getGLSLVersion(glslMajor, glslMinor);

    for (Json::Value::iterator it = targets.begin(); it != targets.end(); ++it)
    {
        if (isFeatureSetSupported(getMember(*it, "feature set")))
        {
            filename = getMember(*it, "resource").asString();

            return load(filename).getPointer();
        }
    }

    throw std::runtime_error("Unable to find a feature set that works.");
}

void ResourceManager::loadUniforms(const Json::Value& json, std::map<std::string, UniformValue>& uniforms,
                                   const std::unordered_map<std::string, ResPtr<Resource> >& externalResources)
{
    Json::Value::Members members = json.getMemberNames();

    for (Json::Value::Members::iterator it = members.begin();
         it != members.end(); ++it)
    {
        std::string name = *it;
        Json::Value uniform = json[name];

        std::string type = uniform["type"].asString();

        if (type == "float")
        {
            uniforms[name].setFloat(uniform["value"].asFloat());
        } else if (type == "vec2")
        {
            Json::Value vec2 = uniform["value"];

            uniforms[name].setVec2(glm::vec2(vec2[0].asFloat(),
                                             vec2[1].asFloat()));
        } else if (type == "vec3")
        {
            Json::Value vec3 = uniform["value"];

            uniforms[name].setVec3(glm::vec3(vec3[0].asFloat(),
                                             vec3[1].asFloat(),
                                             vec3[2].asFloat()));
        } else if (type == "vec4")
        {
            Json::Value vec4 = uniform["value"];

            uniforms[name].setVec4(glm::vec4(vec4[0].asFloat(),
                                             vec4[1].asFloat(),
                                             vec4[2].asFloat(),
                                             vec4[3].asFloat()));
        } else if (type == "int")
        {
            uniforms[name].setInt(uniform["value"].asInt());
        } else if (type == "ivec2")
        {
            Json::Value ivec2 = uniform["value"];

            uniforms[name].setIVec2(glm::ivec2(ivec2[0].asInt(),
                                               ivec2[1].asInt()));
        } else if (type == "ivec3")
        {
            Json::Value ivec3 = uniform["value"];

            uniforms[name].setIVec3(glm::ivec3(ivec3[0].asInt(),
                                               ivec3[1].asInt(),
                                               ivec3[2].asInt()));
        } else if (type == "ivec4")
        {
            Json::Value ivec4 = uniform["value"];

            uniforms[name].setIVec4(glm::ivec4(ivec4[0].asInt(),
                                               ivec4[1].asInt(),
                                               ivec4[2].asInt(),
                                               ivec4[3].asInt()));
        } else if (type == "texture")
        {
            uniforms[name].setTexture(externalResources.at(uniform["value"].asString()).cast<Texture>());
        } else
        {
            throw std::runtime_error("Unknown uniform type.");
        }
    }
}

void ResourceManager::loadDefines(const Json::Value& json, std::map<std::string, std::string>& defines)
{
    Json::Value::Members members = json.getMemberNames();

    for (Json::Value::Members::iterator it = members.begin();
         it != members.end(); ++it)
    {
        std::string name = *it;
        std::string value = json[name].asString();

        defines.emplace(name, value);
    }

}
