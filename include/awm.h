#ifndef AWM_H
#define AWM_H

class Renderer;

template <typename T>
class ResPtr;

class Shader;
class Mesh;

Mesh *loadAWM(Renderer *renderer, ResPtr<Shader> vertexShader, const char *filename);

#endif // AWM_H
