#ifndef GLUTILS_H
#define GLUTILS_H

typedef enum GraphicsCard
{
    NvidiaCard,
    AMDCard,
    IntelCard,
    SoftwareCard,
    UnknownCard
} GraphicsCard;

typedef enum GraphicsDriver
{
    MesaDriver,
    NvidiaDriver,
    AMDDriver,
    IntelDriver,
    UnknownDriver
} GraphicsDriver;

void getOpenGLVersion(unsigned int& major, unsigned int& minor);

void getGLSLVersion(unsigned int& major, unsigned int& minor);

bool isExtensionSupported(const char *name);

GraphicsCard getGraphicsCard();

GraphicsDriver getGraphicsDriver();

#endif // GLUTILS_H
