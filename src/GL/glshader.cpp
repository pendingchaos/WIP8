#include "GL/glshader.h"

#include <GL/gltexture.h>
#include <GL/glubo.h>

GLCompiledShader::GLCompiledShader(CompiledShader::Type type,
                                   unsigned int numSources,
                                   const char **sources) : CompiledShader(type)
{
    switch (type)
    {
        case Vertex:
        {
            mShader = glCreateShader(GL_VERTEX_SHADER);
            break;
        }
        case Fragment:
        {
            mShader = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        }
        case Geometry:
        {
            mShader = glCreateShader(GL_GEOMETRY_SHADER);
            break;
        }
    }

    glShaderSource(mShader, numSources, sources, NULL);

    glCompileShader(mShader);
}

GLCompiledShader::~GLCompiledShader()
{
    glDeleteShader(mShader);
}
