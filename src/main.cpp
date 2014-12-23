#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include <cstdio>
#include <cstring>
#include <cmath>

#include "entity.h"

#include "transform.h"

#include "scene.h"

#include "renderer.h"
#include "vertexbuffer.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "material.h"
#include "model.h"
#include "awm.h"
#include "rendertarget.h"
#include "resource.h"
#include "resourcemanager.h"

#include "utils/memory.h"
#include "utils/crash.h"
#include "utils/utils.h"

#include "GL/glbackend.h"

class DeferredShadingMaterial : public Material
{
    public:
        DeferredShadingMaterial(ResourceManager *resMgr,
                                ResPtr<Texture> colorTexture,
                                ResPtr<Texture> depthTexture,
                                ResPtr<Texture> normalTexture,
                                ResPtr<Texture> materialTexture,
                                ResPtr<Texture> ambientTexture,
                                ResPtr<Texture> specularTexture,
                                Scene *scene) : Material(resMgr->load("res/shaders/quad fragment.json").cast<Shader>()),
                                                mScene(scene)
        {
            mUniforms["colorTexture"] = colorTexture;
            mUniforms["depthTexture"] = depthTexture;
            mUniforms["normalTexture"] = normalTexture;
            mUniforms["materialTexture"] = materialTexture;
            mUniforms["ambientTexture"] = ambientTexture;
            mUniforms["specularTexture"] = specularTexture;
            mUniforms["lightDirection"] = glm::vec3(0.0f);
        }

        void setLightDirection(glm::vec3 dir)
        {
            mUniforms["lightDirection"] = glm::vec3(mScene->mViewTransform.getMatrix()*glm::vec4(dir, 1.0f));
        }
    private:
        Scene *mScene;
};

typedef struct Version
{
    unsigned int major;
    unsigned int minor;
} Version;

static const Version versions[] = {{3, 3},
                                   {3, 2},
                                   {3, 1},
                                   {3, 0},
                                   {2, 1}};

SDL_GLContext createContext(SDL_Window *window, unsigned int versionIndex=0)
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, versions[versionIndex].major);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, versions[versionIndex].minor);

    SDL_GLContext context = SDL_GL_CreateContext(window);

    return context;

    if (context == NULL)
    {
        if (versionIndex == sizeof(versions)/sizeof(Version)-1)
        {
            std::cout << "Unable to create an OpenGL context." << std::endl;
            std::exit(1);
        }

        return createContext(window, versionIndex+1);
    }

    std::cout << "Created an OpenGL "
              << versions[versionIndex].major
              << '.'
              << versions[versionIndex].minor
              << " context."
              << std::endl;

    return context;
}

int main()
{
    initCrashSystem();
    initMemorySystem();

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          640,
                                          640,
                                           SDL_WINDOW_OPENGL
                                          |SDL_WINDOW_RESIZABLE
                                          |SDL_WINDOW_SHOWN);

    SDL_GLContext context = createContext(window);

    SDL_GL_SetSwapInterval(0);

    bool running = true;
    float frametime = 0.0f;

    Renderer *renderer = NEW(Renderer, NEW(GLBackend));
    ResourceManager *resMgr = renderer->getResourceManager();
    Scene *scene = NEW(Scene, renderer);
    Scene *quadScene = NEW(Scene, renderer);

    RenderTarget *target = NEW(RenderTarget, renderer);

    RenderTarget *textureTarget = NEW(RenderTarget, renderer);
    Framebuffer *framebuffer = textureTarget->addFramebuffer();
    framebuffer->addColor(Texture::RGBAU8_Norm_InternalFormat);
    framebuffer->addColor(Texture::Red32F_InternalFormat);
    framebuffer->addColor(Texture::RGBU8_Norm_InternalFormat);
    framebuffer->addColor(Texture::RGBAU8_Norm_InternalFormat);
    framebuffer->addColor(Texture::RGBU8_Norm_InternalFormat);
    framebuffer->addColor(Texture::RGBU8_Norm_InternalFormat);
    framebuffer->finish();

    ResPtr<Model> model = resMgr->load("res/models/dragon.json").cast<Model>();


    ResPtr<Mesh> quadMesh = resMgr->createMesh(resMgr->load("res/shaders/quad vertex.json").cast<Shader>(), Mesh::Triangles, 6);
    VertexBuffer *quadVB = quadMesh->addPositions(renderer, MeshComponent(2, MeshComponent::Float32))->getVertexBuffer();

    quadVB->alloc(sizeof(glm::vec2)*6);
    glm::vec2 *quadPositions = (glm::vec2 *)quadVB->map(false, true);
    quadPositions[0] = glm::vec2(-1.0f, -1.0f);
    quadPositions[1] = glm::vec2( 1.0f, -1.0f);
    quadPositions[2] = glm::vec2(-1.0f,  1.0f);
    quadPositions[3] = glm::vec2(-1.0f,  1.0f);
    quadPositions[4] = glm::vec2( 1.0f, -1.0f);
    quadPositions[5] = glm::vec2( 1.0f,  1.0f);
    quadVB->unmap();


    ResPtr<Material> quadMaterial = resMgr->createMaterial(
    resMgr->load("res/shaders/quad fragment.json").cast<Shader>());
    quadMaterial->mUniforms["colorTexture"] = framebuffer->getColorTexture(0);
    quadMaterial->mUniforms["depthTexture"] = framebuffer->getColorTexture(1);
    quadMaterial->mUniforms["normalTexture"] = framebuffer->getColorTexture(2);
    quadMaterial->mUniforms["materialTexture"] = framebuffer->getColorTexture(3);
    quadMaterial->mUniforms["ambientTexture"] = framebuffer->getColorTexture(4);
    quadMaterial->mUniforms["specularTexture"] = framebuffer->getColorTexture(5);
    quadMaterial->mUniforms["lightDirection"] = glm::vec3(0.0f);


    ResPtr<Model> quadModel = resMgr->createModel();
    quadModel->mLODs.push_back(LOD(quadMesh, quadMaterial, 0.0f));
    quadModel->sortLODs();

    quadScene->createEntity(quadModel);


    scene->mSkyboxTexture = resMgr->load("res/textures/enviroment texture.json").cast<Texture>();


    Entity *entity = scene->createEntity(model);

    float t = 0.0f;

    bool fullscreen = false;

    glm::vec3 cameraPosition(0.0f, 0.0f, 5.0f);
    glm::vec2 cameraAngle(3.1415f, 0.0f);
    float cameraSpeed = 3.0f;
    float cameraRotateSpeed = 1.0f;

    while (running)
    {
        Uint64 start = SDL_GetPerformanceCounter();

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                {
                    running = false;
                    break;
                }
                case SDL_KEYDOWN:
                {
                    switch (event.key.keysym.scancode)
                    {
                        case SDL_SCANCODE_ESCAPE:
                        {
                            running = false;
                            break;
                        }
                        case SDL_SCANCODE_F1:
                        {
                            fullscreen = not fullscreen;

                            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP*fullscreen);
                        }
                        default: {}
                    }
                }
            }
        }

        const Uint8 *pressed = SDL_GetKeyboardState(NULL);

        glm::vec3 direction(std::cos(cameraAngle.y) * std::sin(cameraAngle.x),
                            std::sin(cameraAngle.y),
                            std::cos(cameraAngle.y) * std::cos(cameraAngle.x));

        glm::vec3 right(std::sin(cameraAngle.x - 3.1415f / 2.0f),
                        0.0f,
                        std::cos(cameraAngle.x - 3.1415f / 2.0f));

        glm::vec3 up = glm::cross(right, direction);

        if (pressed[SDL_SCANCODE_LEFT])
        {
            cameraAngle.x += cameraRotateSpeed * frametime;
        } else if (pressed[SDL_SCANCODE_RIGHT])
        {
            cameraAngle.x -= cameraRotateSpeed * frametime;
        } else if (pressed[SDL_SCANCODE_UP])
        {
            cameraAngle.y += cameraRotateSpeed * frametime;
        } else if (pressed[SDL_SCANCODE_DOWN])
        {
            cameraAngle.y -= cameraRotateSpeed * frametime;
        } else if (pressed[SDL_SCANCODE_A])
        {
            cameraPosition -= right * frametime * cameraSpeed;
        } else if (pressed[SDL_SCANCODE_D])
        {
            cameraPosition += right * frametime * cameraSpeed;
        } else if (pressed[SDL_SCANCODE_W])
        {
            cameraPosition += direction * frametime * cameraSpeed;
        } else if (pressed[SDL_SCANCODE_S])
        {
            cameraPosition -= direction * frametime * cameraSpeed;
        }

        scene->mProjectionTransform.reset();

        int windowWidth;
        int windowHeight;
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
        target->setWidthAndHeight(windowWidth, windowHeight);
        textureTarget->setWidthAndHeight(windowWidth, windowHeight);

        scene->mProjectionTransform.perspective(45.0f, float(windowWidth)/float(windowHeight), 0.1f, 100.0f);

        scene->mViewTransform.reset();
        scene->mViewTransform.lookAt(cameraPosition,
                                     cameraPosition+direction,
                                     up);

        t += frametime;

        entity->mTransform.reset();
        entity->mTransform.scale(glm::vec3(0.5f));

        entity->mTransform.rotate(45.0f*t, glm::vec3(0.0f, 1.0f, 0.0f));

        renderer->render(textureTarget, scene);

        quadMaterial->mUniforms["lightDirection"]
        = glm::mat3(scene->mViewTransform.getMatrix()) * glm::vec3(-1.0f, 1.0f, -1.0f);

        renderer->render(target, quadScene);

        SDL_GL_SwapWindow(window);

        resMgr->deleteUnusedResources();

        Uint64 end = SDL_GetPerformanceCounter();

        frametime = float(end - start) / float(SDL_GetPerformanceFrequency());

        char title[256];
        std::memset(title, 0, 256);
        std::snprintf(title, 256, "Frametime: %.4f, Framerate: %.0f", frametime, 1.0f/frametime);
        SDL_SetWindowTitle(window, title);
    }

    DELETE(RenderTarget, textureTarget);
    DELETE(RenderTarget, target);

    DELETE(Scene, quadScene);
    DELETE(Scene, scene);

    DELETE(Renderer, renderer);

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);

    SDL_Quit();

    deinitMemorySystem();

    return 0;
}
