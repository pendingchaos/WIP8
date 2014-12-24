#include "GL/glutils.h"

#include <GL/gl.h>

#include <cstdio>
#include <cstring>

void getOpenGLVersion(unsigned int& major, unsigned int& minor)
{
    std::sscanf((const char *)glGetString(GL_VERSION), "%d.%d", &major, &minor);
}

void getGLSLVersion(unsigned int& major, unsigned int& minor)
{
    std::sscanf((const char *)glGetString(GL_SHADING_LANGUAGE_VERSION), "%d.%d", &major, &minor);

    minor /= 10;
}

bool isExtensionSupported(const char *name)
{
    unsigned int major, minor;

    getOpenGLVersion(major, minor);

    if (major >= 3)
    {
        unsigned int numExtensions;

        glGetIntegerv(GL_NUM_EXTENSIONS, (GLint *)&numExtensions);

        for (unsigned int i=0; i<numExtensions; ++i)
        {
            if (strcmp(name, (const char *)glGetStringi(GL_EXTENSIONS, i)) == 0)
            {
                return true;
            }
        }
    } else
    {
        const char *extensions = (const char *)glGetString(GL_EXTENSIONS);

        bool found = true;

        const char *cmpName = name;

        while (*extensions != '\x00')
        {
            if (*extensions == ' ')
            {
                if (found)
                {
                    return true;
                }

                cmpName = name;
            } else if (*cmpName == '\x00')
            {
                found = false;
            } else if (*extensions != *cmpName)
            {
                found = false;
                ++cmpName;
            }
        }

        return true;
    }

    return false;
}
