#ifndef GLUTILS_H
#define GLUTILS_H

void getOpenGLVersion(unsigned int& major, unsigned int& minor);

void getGLSLVersion(unsigned int& major, unsigned int& minor);

bool isExtensionSupported(const char *name);

#endif // GLUTILS_H
