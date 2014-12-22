#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>

#include <map>
#include <string>

#include "texture.h"
#include "resource.h"

class Renderer;
class UBO;

class CompiledShader
{
    friend Renderer;

    public:
        typedef enum Type
        {
            Vertex,
            Fragment,
            Geometry
        } Type;

        virtual ~CompiledShader();

        inline Type getType() const {return mType;}
    protected:
        CompiledShader(Type type);

        Type mType;
};


class Shader : public Resource
{
    public:
        Shader(Renderer *renderer, CompiledShader::Type type, std::string source);
        virtual ~Shader();

        CompiledShader *getShader(std::map<std::string, std::string> defines
                                  =std::map<std::string, std::string>());

        inline CompiledShader::Type getType() const {return mType;}

        inline Renderer *getRenderer() const {return mRenderer;}
    private:
        Renderer *mRenderer;

        CompiledShader::Type mType;
        std::string mSource;

        std::map<std::map<std::string, std::string>, CompiledShader *> mShaders;
};

#endif // SHADER_H
