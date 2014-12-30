#ifndef GLSHADER_H
#define GLSHADER_H

#include <string>

#include <GL/gl.h>

#include "resource.h"
#include "shader.h"
#include "texture.h"

class GLCompiledShader : public CompiledShader
{
    public:
        GLCompiledShader(std::string filename,
                         Type type,
                         unsigned int numSources,
                         const char **sources);

        virtual ~GLCompiledShader();

        GLuint mShader;
};

#endif // GLSHADER_H
