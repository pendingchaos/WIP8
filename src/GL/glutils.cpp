#include "GL/glutils.h"

#include <GL/gl.h>

#include <cstdio>
#include <cstring>
#include <ctype.h>
#include <string>
#include <algorithm>

void getOpenGLVersion(unsigned int& major, unsigned int& minor)
{
    std::sscanf((const char *)glGetString(GL_VERSION), "%u.%u", &major, &minor);
}

void getGLSLVersion(unsigned int& major, unsigned int& minor)
{
    std::sscanf((const char *)glGetString(GL_SHADING_LANGUAGE_VERSION), "%u.%u", &major, &minor);

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

GraphicsCard getGraphicsCard()
{
    std::string vendor = (const char *)glGetString(GL_VENDOR);
    std::string renderer = (const char *)glGetString(GL_RENDERER);

    std::transform(vendor.begin(), vendor.end(), vendor.begin(), tolower);
    std::transform(renderer.begin(), renderer.end(), renderer.begin(), tolower);

    if (vendor.find("nouveau") != std::string::npos)
    {
        return NvidiaCard;
    } else if (renderer.find("software") != std::string::npos)
    {
        return SoftwareCard;
    } else if (vendor.find("nvidia") != std::string::npos)
    {
        return NvidiaCard;
    } else if (vendor.find("ati") != std::string::npos)
    {
        return AMDCard;
    } else if (vendor.find("amd") != std::string::npos)
    {
        return AMDCard;
    } else if (vendor.find("intel") != std::string::npos)
    {
        return IntelCard;
    } else
    {
        return UnknownCard;
    }
}

GraphicsDriver getGraphicsDriver()
{
    std::string vendor = (const char *)glGetString(GL_VENDOR);
    std::string renderer = (const char *)glGetString(GL_RENDERER);

    std::transform(vendor.begin(), vendor.end(), vendor.begin(), tolower);
    std::transform(renderer.begin(), renderer.end(), renderer.begin(), tolower);

    if (vendor.find("nouveau") != std::string::npos)
    {
        return MesaDriver;
    } else if (vendor.find("mesa") != std::string::npos)
    {
        return MesaDriver;
    } else if (renderer.find("mesa") != std::string::npos)
    {
        return MesaDriver;
    } else
    {
        GraphicsCard card = getGraphicsCard();

        switch (card)
        {
            case NvidiaCard:
            {
                return NvidiaDriver;
            }
            case AMDCard:
            {
                return AMDDriver;
            }
            case IntelCard:
            {
                return IntelDriver;
            }
            default:
            {
                return UnknownDriver;
            }
        }
    }

}
