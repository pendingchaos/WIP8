#include "shader.h"

#include "utils/memory.h"
#include "renderer.h"

UniformValue::UniformValue() : mType(Nothing)
{
    mTexture = nullRes<Texture>();
}

UniformValue::UniformValue(float value) : mType(Float)
{
    mFloatData[0] = value;
}

UniformValue::UniformValue(const glm::vec2& value) : mType(Vec2)
{
    mFloatData[0] = value.x;
    mFloatData[1] = value.y;
}

UniformValue::UniformValue(const glm::vec3& value) : mType(Vec3)
{
    mFloatData[0] = value.x;
    mFloatData[1] = value.y;
    mFloatData[2] = value.z;
}

UniformValue::UniformValue(const glm::vec4& value) : mType(Vec4)
{
    mFloatData[0] = value.x;
    mFloatData[1] = value.y;
    mFloatData[2] = value.z;
    mFloatData[3] = value.w;
}

UniformValue::UniformValue(int value) : mType(Int)
{
    mIntData[0] = value;
}

UniformValue::UniformValue(const glm::ivec2& value) : mType(IVec2)
{
    mIntData[0] = value.x;
    mIntData[1] = value.y;
}

UniformValue::UniformValue(const glm::ivec3& value) : mType(IVec3)
{
    mIntData[0] = value.x;
    mIntData[1] = value.y;
    mIntData[2] = value.z;
}

UniformValue::UniformValue(const glm::ivec4& value) : mType(IVec4)
{
    mIntData[0] = value.x;
    mIntData[1] = value.y;
    mIntData[2] = value.z;
    mIntData[3] = value.w;
}

UniformValue::UniformValue(ResPtr<Texture> value) : mType(TextureType)
{
    mTexture = value;
}


CompiledShader::CompiledShader(Type type) : mType(type) {}

CompiledShader::~CompiledShader() {}


Shader::Shader(Renderer *renderer,
               std::string filename,
               CompiledShader::Type type,
               std::string source) : Resource(typeid(Shader)),
                                     mRenderer(renderer),
                                     mFilename(filename),
                                     mType(type),
                                     mSource(source) {}

Shader::~Shader()
{
    for (std::map<std::map<std::string, std::string>, CompiledShader *>::iterator it
         = mShaders.begin(); it != mShaders.end(); ++it)
    {
        DELETE(CompiledShader, it->second);
    }
}

CompiledShader *Shader::getShader(std::map<std::string, std::string> defines)
{
    std::map<std::map<std::string, std::string>, CompiledShader *>::iterator pos
    = mShaders.find(defines);

    if (pos == mShaders.end())
    {
        CompiledShader *shader = mRenderer->createShader(mFilename, mType, mSource.c_str(), defines);

        mShaders.emplace(defines, shader);

        return shader;
    }

    return pos->second;
}
