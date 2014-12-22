#include "resourcemanager.h"

#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>

#include "utils/memory.h"
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
    for (std::unordered_map<std::string, Resource *>::iterator it = mResources.begin();
         it != mResources.end(); ++it)
    {
        DELETE(Resource, it->second);
    }
}

void ResourceManager::deleteUnusedResources()
{
    for (std::unordered_map<std::string, Resource *>::iterator it = mResources.begin();
         it != mResources.end();)
    {
        if (it->second->getRefCount() <= 0)
        {
            DELETE(Resource, it->second);
            mResources.erase(it);
            return;
        } else
        {
            ++it;
        }
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

std::string readFile(std::string filename)
{
    FILE *file = std::fopen(filename.c_str(), "r");

    if (file == NULL)
    {
        std::cout << "Unable to find \"" << filename << "\"" << std::endl;
        return "";
    }

    std::fseek(file, 0, SEEK_END);

    std::size_t size = std::ftell(file);

    std::fseek(file, 0, SEEK_SET);

    char *cData = (char *)ALLOCATE(size+1);
    std::memset(cData, 0, size+1);

    std::fread(cData, 1, size, file);

    std::fclose(file);

    std::string data(cData);

    DEALLOCATE(cData);

    return data;
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

Resource *ResourceManager::_load(std::string filename)
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
        return NEW(Shader, mRenderer, CompiledShader::Vertex, readFile(filename));
    } else if (type == "fragment")
    {
        return NEW(Shader, mRenderer, CompiledShader::Fragment, readFile(filename));
    } else if (type == "geometry")
    {
        return NEW(Shader, mRenderer, CompiledShader::Geometry, readFile(filename));
    } else
    {
        throw std::runtime_error("Unknown shader type.");
    }
}

Mesh *ResourceManager::loadMesh(const Json::Value& json,
                                const std::unordered_map<std::string, ResPtr<Resource> >& externalResources)
{
    ResPtr<Shader> shader = externalResources.at(json["shader"].asString()).cast<Shader>();

    Mesh *mesh = loadAWM(mRenderer, shader, json["awm filename"].asCString());

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

    return mesh;
}

Material *ResourceManager::loadMaterial(const Json::Value& json,
                                        const std::unordered_map<std::string, ResPtr<Resource> >& externalResources)
{
    ResPtr<Shader> shader = externalResources.at(json["shader"].asString()).cast<Shader>();

    Material *material = NEW(Material, shader);

    Json::Value uniforms = json["uniforms"];
    Json::Value::Members uniformMembers = uniforms.getMemberNames();

    for (Json::Value::Members::iterator it = uniformMembers.begin();
         it != uniformMembers.end(); ++it)
    {
        std::string name = *it;
        Json::Value uniform = uniforms[name];

        std::string type = uniform["type"].asString();

        if (type == "float")
        {
            material->mUniforms[name].setFloat(uniform["value"].asFloat());
        } else if (type == "vec2")
        {
            Json::Value vec2 = uniform["value"];

            material->mUniforms[name].setVec2(glm::vec2(vec2[0].asFloat(),
                                                        vec2[1].asFloat()));
        } else if (type == "vec3")
        {
            Json::Value vec3 = uniform["value"];

            material->mUniforms[name].setVec3(glm::vec3(vec3[0].asFloat(),
                                                        vec3[1].asFloat(),
                                                        vec3[2].asFloat()));
        } else if (type == "vec4")
        {
            Json::Value vec4 = uniform["value"];

            material->mUniforms[name].setVec4(glm::vec4(vec4[0].asFloat(),
                                                        vec4[1].asFloat(),
                                                        vec4[2].asFloat(),
                                                        vec4[3].asFloat()));
        } else if (type == "int")
        {
            material->mUniforms[name].setInt(uniform["value"].asInt());
        } else if (type == "ivec2")
        {
            Json::Value ivec2 = uniform["value"];

            material->mUniforms[name].setIVec2(glm::ivec2(ivec2[0].asInt(),
                                                          ivec2[1].asInt()));
        } else if (type == "ivec3")
        {
            Json::Value ivec3 = uniform["value"];

            material->mUniforms[name].setIVec3(glm::ivec3(ivec3[0].asInt(),
                                                          ivec3[1].asInt(),
                                                          ivec3[2].asInt()));
        } else if (type == "ivec4")
        {
            Json::Value ivec4 = uniform["value"];

            material->mUniforms[name].setIVec4(glm::ivec4(ivec4[0].asInt(),
                                                          ivec4[1].asInt(),
                                                          ivec4[2].asInt(),
                                                          ivec4[3].asInt()));
        } else if (type == "texture")
        {
            material->mUniforms[name].setTexture(externalResources.at(uniform["value"].asString()).cast<Texture>());
        } else
        {
            throw std::runtime_error("Unknown uniform type.");
        }
    }

    Json::Value defines = json["defines"];
    Json::Value::Members defineMembers = defines.getMemberNames();

    for (Json::Value::Members::iterator it = defineMembers.begin();
         it != defineMembers.end(); ++it)
    {
        std::string name = *it;
        std::string value = defines[name].asString();

        material->mDefines.emplace(name, value);
    }

    return material;
}
