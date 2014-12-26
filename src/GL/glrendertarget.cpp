#include "GL/glrendertarget.h"

#include <algorithm>

#include "utils/memory.h"
#include "renderer.h"
#include <GL/gltexture.h>

GLFramebuffer::GLFramebuffer(Backend *backend) : mBackend(backend),
                                                 mFinished(false),
                                                 mDepthTexture(NULL),
                                                 mDepthRBO(0),
                                                 mWidth(1),
                                                 mHeight(1)
{
    glGenFramebuffers(1, &mFramebuffer);
}

GLFramebuffer::~GLFramebuffer()
{
    if (glIsRenderbuffer(mDepthRBO))
    {
        glDeleteRenderbuffers(1, &mDepthRBO);
    }

    glDeleteFramebuffers(1, &mFramebuffer);
}

ResPtr<Texture> GLFramebuffer::addDepth(ResPtr<Texture> texture, Texture::InternalFormat format)
{
    if (mDepthTexture == NULL and not mFinished)
    {
        mDepthTexture = texture;
        mDepthFormat = format;

        mDepthTexture->setMinFilter(Texture::NearestMinFilter);
        mDepthTexture->setMagFilter(Texture::NearestMagFilter);
        mDepthTexture->setUWrap(Texture::Clamp);
        mDepthTexture->setVWrap(Texture::Clamp);
    }

    return mDepthTexture;
}

ResPtr<Texture> GLFramebuffer::addColor(ResPtr<Texture> texture, Texture::InternalFormat format, const glm::vec2& scale)
{
    if (not mFinished)
    {
        texture->setMinFilter(Texture::NearestMinFilter);
        texture->setMagFilter(Texture::NearestMagFilter);
        texture->setUWrap(Texture::Clamp);
        texture->setVWrap(Texture::Clamp);

        mColorTextures.push_back((ColorTexture){texture, format, scale});

        return texture;
    }

    return nullRes<Texture>();
}

void GLFramebuffer::finish(bool depth)
{
    if (mFinished)
    {
        return;
    }

    mFinished = true;

    glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);

    if (depth and mDepthTexture == NULL)
    {
        glGenRenderbuffers(1, &mDepthRBO);

        glBindRenderbuffer(GL_RENDERBUFFER, mDepthRBO);
    }

    setWidthAndHeight(mWidth, mHeight);
}

void GLFramebuffer::setWidth(unsigned int width)
{
    setWidthAndHeight(width, mHeight);
}

unsigned int GLFramebuffer::getWidth() const
{
    return mWidth;
}

void GLFramebuffer::setHeight(unsigned int height)
{
    setWidthAndHeight(mWidth, height);
}

unsigned int GLFramebuffer::getHeight() const
{
    return mHeight;
}

void GLFramebuffer::setWidthAndHeight(unsigned int width, unsigned int height)
{
    mWidth = std::max(width, 1u);
    mHeight = std::max(height, 1u);

    if (not mFinished)
    {
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);

    if (mDepthTexture != NULL)
    {
        GLTexture *glDepthTex = (GLTexture *)mDepthTexture.cast<Texture>().getPointer();

        glBindTexture(GL_TEXTURE_2D, glDepthTex->mTexture);

        glDepthTex->allocData2D(mWidth, mHeight, mDepthFormat, Texture::Depth32F_Format, NULL);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, glDepthTex->mTexture, 0);
    }

    if (glIsRenderbuffer(mDepthRBO) and mDepthTexture == NULL)
    {
        glBindRenderbuffer(GL_RENDERBUFFER, mDepthRBO);

        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mWidth, mHeight);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthRBO);
    }

    unsigned int i=0;
    GLenum drawBuffers[mColorTextures.size()];

    for (std::vector<ColorTexture>::iterator it = mColorTextures.begin();
         it != mColorTextures.end(); ++it)
    {
        GLTexture *glTexture = (GLTexture *)it->texture.cast<Texture>().getPointer();

        glBindTexture(GL_TEXTURE_2D, glTexture->mTexture);

        glTexture->allocData2D(mWidth*it->scale.x, mHeight*it->scale.y, it->format, Texture::RGBU8_Format, NULL);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, glTexture->mTexture, 0);
        drawBuffers[i] = GL_COLOR_ATTACHMENT0+i;

        ++i;
    }

    glDrawBuffers(mColorTextures.size(), drawBuffers);
}

ResPtr<Texture> GLFramebuffer::getColorTexture(unsigned int index) const
{
    return ResPtr<Texture>(mColorTextures[index].texture);
}

ResPtr<Texture> GLFramebuffer::getDepthTexture() const
{
    return ResPtr<Texture>(mDepthTexture);
}
