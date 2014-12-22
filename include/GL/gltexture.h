#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include <GL/gl.h>

#include "texture.h"

class GLTexture : public Texture
{
    public:
        GLTexture(Texture::Type type);
        virtual ~GLTexture();

        virtual void allocData1D(unsigned int width,
                                 InternalFormat internalFormat,
                                 Format format,
                                 const void *data);

        virtual void allocData2D(unsigned int width,
                                 unsigned int height,
                                 InternalFormat internalFormat,
                                 Format format,
                                 const void *data);

        virtual void allocFaceData(unsigned int width,
                                   unsigned int height,
                                   InternalFormat internalFormat,
                                   Format format,
                                   const void **data);

        virtual void allocData3D(unsigned int width,
                                 unsigned int height,
                                 unsigned int depth,
                                 InternalFormat internalFormat,
                                 Format format,
                                 const void *data);

        virtual void generateMipmaps();

        virtual void setMinFilter(Texture::MinFilter filter);
        virtual Texture::MinFilter getMinFilter();

        virtual void setMagFilter(Texture::MagFilter filter);
        virtual Texture::MagFilter getMagFilter();

        virtual void setUWrap(Texture::Wrap wrap);
        virtual Texture::Wrap getUWrap();

        virtual void setVWrap(Texture::Wrap wrap);
        virtual Texture::Wrap getVWrap();

        virtual void setWWrap(Texture::Wrap wrap);
        virtual Texture::Wrap getWWrap();

        virtual void setAnisotropy(float anisotropy);
        virtual float getAnisotropy();

        GLenum mTarget;
        GLuint mTexture;

        MinFilter mMinFilter;
        MagFilter mMagFilter;
        Wrap mUWrap;
        Wrap mVWrap;
        Wrap mWWrap;
        float mAnisotropy;

        GLuint getBoundTexture();

        GLint internalFormatToGL(InternalFormat format);
        void formatToGL(Format format, GLenum& glFormat, GLenum& glType);
};

#endif // GLTEXTURE_H
