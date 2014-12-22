#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include <glm/glm.hpp>

#include "resource.h"
#include "texture.h"

class Renderer;

class Framebuffer
{
    public:
        virtual ~Framebuffer();

        virtual ResPtr<Texture> addDepth(Texture::InternalFormat format)=0;
        virtual ResPtr<Texture> addColor(Texture::InternalFormat format, const glm::vec2& scale=glm::vec2(1.0f))=0;
        virtual void finish(bool depth=true)=0;

        virtual void setWidth(unsigned int width)=0;
        virtual unsigned int getWidth() const =0;

        virtual void setHeight(unsigned int height)=0;
        virtual unsigned int getHeight() const =0;

        virtual void setWidthAndHeight(unsigned int width, unsigned int height)=0;

        virtual ResPtr<Texture> getColorTexture(unsigned int index) const =0;
        virtual ResPtr<Texture> getDepthTexture() const =0;
        inline bool hasDepthTexture() const {return getDepthTexture() != nullRes<Texture>();}
    protected:
        Framebuffer();
};

class RenderTarget
{
    public:
        RenderTarget(Renderer *renderer);
        virtual ~RenderTarget();

        Framebuffer *addFramebuffer();
        void removeFramebuffer();
        Framebuffer *getFramebuffer() const {return mFramebuffer;}
        inline bool hasFramebuffer() const {return mFramebuffer != NULL;}

        inline Renderer *getRenderer() const {return mRenderer;}

        void setWidth(unsigned int width);
        inline unsigned int getWidth() const {return mWidth;}

        void setHeight(unsigned int height);
        inline unsigned int getHeight() const {return mHeight;}

        void setWidthAndHeight(unsigned int width, unsigned int height);
    private:
        Renderer *mRenderer;

        Framebuffer *mFramebuffer;

        unsigned int mWidth;
        unsigned int mHeight;
};

#endif // RENDERTARGET_H
