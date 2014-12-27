#include "GL/glshader.h"

#include <iostream>

#include "utils/memory.h"
#include "GL/gltexture.h"

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

    GLint logLength;
    glGetShaderiv(mShader, GL_INFO_LOG_LENGTH, &logLength);
    char *infoLog = (char *)ALLOCATE(logLength);

    glGetShaderInfoLog(mShader, logLength, NULL, infoLog);

    std::cout << infoLog << std::endl;

    DEALLOCATE(infoLog);
}

GLCompiledShader::~GLCompiledShader()
{
    glDeleteShader(mShader);
}
