#ifndef GLSHADER_H
#define GLSHADER_H

#include <GL/gl.h>

#include "resource.h"
#include "shader.h"
#include "texture.h"

class UBO;

class GLCompiledShader : public CompiledShader
{
    public:
        GLCompiledShader(Type type,
                         unsigned int numSources,
                         const char **sources);

        virtual ~GLCompiledShader();

        GLuint mShader;
};

#endif // GLSHADER_H
