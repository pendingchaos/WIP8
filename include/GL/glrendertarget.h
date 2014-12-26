#ifndef GLRENDERTARGET_H
#define GLRENDERTARGET_H

#include <GL/gl.h>

#include <vector>

#include "rendertarget.h"

class Backend;

class GLFramebuffer : public Framebuffer
{
    public:
        GLFramebuffer(Backend *backend);
        virtual ~GLFramebuffer();

        virtual ResPtr<Texture> addDepth(ResPtr<Texture> texture, Texture::InternalFormat format);
        virtual ResPtr<Texture> addColor(ResPtr<Texture> texture, Texture::InternalFormat format, const::glm::vec2& scale=glm::vec2(1.0f));
        virtual void finish(bool depth=true);

        virtual void setWidth(unsigned int width);
        virtual unsigned int getWidth() const;

        virtual void setHeight(unsigned int height);
        virtual unsigned int getHeight() const;

        virtual void setWidthAndHeight(unsigned int width, unsigned int height);

        virtual ResPtr<Texture> getColorTexture(unsigned int index) const;
        virtual ResPtr<Texture> getDepthTexture() const;

        GLuint mFramebuffer;
        Backend *mBackend;

        bool mFinished;

        ResPtr<Texture> mDepthTexture;
        Texture::InternalFormat mDepthFormat;

        GLuint mDepthRBO;

        typedef struct ColorTexture
        {
            ResPtr<Texture> texture;
            Texture::InternalFormat format;
            glm::vec2 scale;
        } ColorTexture;

        std::vector<ColorTexture> mColorTextures;

        unsigned int mWidth;
        unsigned int mHeight;
};

#endif // GLRENDERTARGET_H
