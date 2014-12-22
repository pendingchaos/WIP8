#include "shader.h"

#include "utils/memory.h"
#include "renderer.h"

CompiledShader::CompiledShader(Type type) : mType(type) {}

CompiledShader::~CompiledShader() {}


Shader::Shader(Renderer *renderer,
               CompiledShader::Type type,
               std::string source) : Resource(typeid(Shader)),
                                     mRenderer(renderer),
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
        CompiledShader *shader = mRenderer->createShader(mType, mSource.c_str(), defines);

        mShaders.emplace(defines, shader);

        return shader;
    }

    return pos->second;
}
