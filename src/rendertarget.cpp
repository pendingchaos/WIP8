#include "rendertarget.h"

#include "utils/memory.h"
#include "renderer.h"

Framebuffer::Framebuffer() {}
Framebuffer::~Framebuffer() {}


RenderTarget::RenderTarget(Renderer *renderer) : mRenderer(renderer),
                                                 mFramebuffer(NULL),
                                                 mWidth(0),
                                                 mHeight(0) {}

RenderTarget::~RenderTarget()
{
    removeFramebuffer();
}

Framebuffer *RenderTarget::addFramebuffer()
{
    removeFramebuffer();

    mFramebuffer = mRenderer->createFramebuffer();

    mFramebuffer->setWidthAndHeight(mWidth, mHeight);

    return mFramebuffer;
}

void RenderTarget::removeFramebuffer()
{
    DELETE(Framebuffer, mFramebuffer);
}

void RenderTarget::setWidth(unsigned int width)
{
    mWidth = width;

    if (mFramebuffer != NULL)
    {
        mFramebuffer->setWidth(width);
    }
}

void RenderTarget::setHeight(unsigned int height)
{
    mHeight = height;

    if (mFramebuffer != NULL)
    {
        mFramebuffer->setHeight(height);
    }
}

void RenderTarget::setWidthAndHeight(unsigned int width, unsigned int height)
{
    mWidth = width;
    mHeight = height;

    if (mFramebuffer != NULL)
    {
        mFramebuffer->setWidthAndHeight(width, height);
    }
}
