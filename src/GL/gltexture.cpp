#include "GL/gltexture.h"

GLTexture::GLTexture(Texture::Type type) : Texture(type),
                                           mMinFilter(Texture::LinearMinFilter),
                                           mMagFilter(Texture::LinearMagFilter),
                                           mUWrap(Texture::Repeat),
                                           mVWrap(Texture::Repeat),
                                           mWWrap(Texture::Repeat)
{
    glGenTextures(1, &mTexture);

    switch (type)
    {
        case Texture::Texture1D: {mTarget = GL_TEXTURE_1D; break;}
        case Texture::Texture2D: {mTarget = GL_TEXTURE_2D; break;}
        case Texture::Texture3D: {mTarget = GL_TEXTURE_3D; break;}
        case Texture::TextureCubeMap: {mTarget = GL_TEXTURE_CUBE_MAP; break;}
    }

    GLuint lastTexture = getBoundTexture();
    glBindTexture(mTarget, mTexture);

    glTexParameteri(mTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    mMagFilter = Texture::LinearMagFilter;

    glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    mMinFilter = Texture::LinearMinFilter;

    glTexParameteri(mTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
    mUWrap = Texture::Repeat;

    glTexParameteri(mTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
    mVWrap = Texture::Repeat;

    glTexParameteri(mTarget, GL_TEXTURE_WRAP_R, GL_REPEAT);
    mWWrap = Texture::Repeat;

    mAnisotropy = 1.0f;

    glBindTexture(mTarget, lastTexture);

}

GLTexture::~GLTexture()
{
    glDeleteTextures(1, &mTexture);
}

void GLTexture::allocData1D(unsigned int width,
                          InternalFormat internalFormat,
                          Format format,
                          const void *data)
{
    GLuint lastTexture = getBoundTexture();
    glBindTexture(mTarget, mTexture);

    GLenum glFormat;
    GLenum glType;

    formatToGL(format, glFormat, glType);

    glTexImage1D(mTarget,
                 0,
                 internalFormatToGL(internalFormat),
                 width,
                 0,
                 glFormat,
                 glType,
                 data);

    glBindTexture(mTarget, lastTexture);
}

void GLTexture::allocData2D(unsigned int width,
                          unsigned int height,
                          InternalFormat internalFormat,
                          Format format,
                          const void *data)
{
    GLuint lastTexture = getBoundTexture();
    glBindTexture(mTarget, mTexture);

    GLenum glFormat;
    GLenum glType;

    formatToGL(format, glFormat, glType);

    glTexImage2D(mTarget,
                 0,
                 internalFormatToGL(internalFormat),
                 width,
                 height,
                 0,
                 glFormat,
                 glType,
                 data);

    glBindTexture(mTarget, lastTexture);
}

void GLTexture::allocFaceData(unsigned int width,
                              unsigned int height,
                              InternalFormat internalFormat,
                              Format format,
                              const void **data)
{
    GLuint lastTexture = getBoundTexture();
    glBindTexture(mTarget, mTexture);

    GLenum glFormat;
    GLenum glType;

    formatToGL(format, glFormat, glType);

    GLint glInternalFormat = internalFormatToGL(internalFormat);

    for (unsigned int i=0; i<6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,
                     0,
                     glInternalFormat,
                     width,
                     height,
                     0,
                     glFormat,
                     glType,
                     data[i]);
    }

    glBindTexture(mTarget, lastTexture);
}

void GLTexture::allocData3D(unsigned int width,
                          unsigned int height,
                          unsigned int depth,
                          InternalFormat internalFormat,
                          Format format,
                          const void *data)
{
    GLuint lastTexture = getBoundTexture();
    glBindTexture(mTarget, mTexture);

    GLenum glFormat;
    GLenum glType;

    formatToGL(format, glFormat, glType);

    glTexImage3D(mTarget,
                 0,
                 internalFormatToGL(internalFormat),
                 width,
                 height,
                 depth,
                 0,
                 glFormat,
                 glType,
                 data);

    glBindTexture(mTarget, lastTexture);
}

void GLTexture::generateMipmaps()
{
    GLuint lastTexture = getBoundTexture();
    glBindTexture(mTarget, mTexture);
    glGenerateMipmap(mTarget);
    glBindTexture(mTarget, lastTexture);
}

void GLTexture::setMinFilter(Texture::MinFilter filter)
{
    mMinFilter = filter;

    GLuint lastTexture = getBoundTexture();
    glBindTexture(mTarget, mTexture);

    switch (filter)
    {
        case Texture::LinearMinFilter:
        {
            glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            break;
        }
        case Texture::NearestMinFilter:
        {
            glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            break;
        }
        case Texture::LinearWithNearestMipmapMinFilter:
        {
            glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            break;
        }
        case Texture::NearestWithNearestMipmapMinFilter:
        {
            glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            break;
        }
        case Texture::LinearWithLinearMipmapMinFilter:
        {
            glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            break;
        }
        case Texture::NearestWithLinearMipmapMinFilter:
        {
            glTexParameteri(mTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
            break;
        }
    }

    glBindTexture(mTarget, lastTexture);
}

Texture::MinFilter GLTexture::getMinFilter()
{
    return mMinFilter;
}

void GLTexture::setMagFilter(Texture::MagFilter filter)
{
    mMagFilter = filter;

    GLuint lastTexture = getBoundTexture();
    glBindTexture(mTarget, mTexture);

    switch (filter)
    {
        case Texture::LinearMinFilter:
        {
            glTexParameteri(mTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        }
        case Texture::NearestMinFilter:
        {
            glTexParameteri(mTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;
        }
    }

    glBindTexture(mTarget, lastTexture);
}

Texture::MagFilter GLTexture::getMagFilter()
{
    return mMagFilter;
}

void GLTexture::setUWrap(Texture::Wrap wrap)
{
    mUWrap = wrap;

    GLuint lastTexture = getBoundTexture();
    glBindTexture(mTarget, mTexture);

    switch (wrap)
    {
        case Texture::Repeat:
        {
            glTexParameteri(mTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
            break;
        }
        case Texture::MirroredRepeat:
        {
            glTexParameteri(mTarget, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            break;
        }
        case Texture::Clamp:
        {
            glTexParameteri(mTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            break;
        }
    }

    glBindTexture(mTarget, lastTexture);
}

Texture::Wrap GLTexture::getUWrap()
{
    return mUWrap;
}

void GLTexture::setVWrap(Texture::Wrap wrap)
{
    mVWrap = wrap;

    GLuint lastTexture = getBoundTexture();
    glBindTexture(mTarget, mTexture);

    switch (wrap)
    {
        case Texture::Repeat:
        {
            glTexParameteri(mTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
            break;
        }
        case Texture::MirroredRepeat:
        {
            glTexParameteri(mTarget, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
            break;
        }
        case Texture::Clamp:
        {
            glTexParameteri(mTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            break;
        }
    }

    glBindTexture(mTarget, lastTexture);
}

Texture::Wrap GLTexture::getVWrap()
{
    return mVWrap;
}

void GLTexture::setWWrap(Texture::Wrap wrap)
{
    mWWrap = wrap;

    GLuint lastTexture = getBoundTexture();
    glBindTexture(mTarget, mTexture);

    switch (wrap)
    {
        case Texture::Repeat:
        {
            glTexParameteri(mTarget, GL_TEXTURE_WRAP_R, GL_REPEAT);
            break;
        }
        case Texture::MirroredRepeat:
        {
            glTexParameteri(mTarget, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);
            break;
        }
        case Texture::Clamp:
        {
            glTexParameteri(mTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            break;
        }
    }

    glBindTexture(mTarget, lastTexture);
}

Texture::Wrap GLTexture::getWWrap()
{
    return mWWrap;
}

void GLTexture::setAnisotropy(float anisotropy)
{
    mAnisotropy = anisotropy;

    GLuint lastTexture = getBoundTexture();
    glBindTexture(mTarget, mTexture);

    glTexParameterf(mTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);

    glBindTexture(mTarget, lastTexture);
}

float GLTexture::getAnisotropy()
{
    return mAnisotropy;
}

GLuint GLTexture::getBoundTexture()
{
    GLuint tex;

    switch (mType)
    {
        case Texture1D: {glGetIntegerv(GL_TEXTURE_BINDING_1D, (GLint *)&tex); break;}
        case Texture2D: {glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint *)&tex); break;}
        case Texture3D: {glGetIntegerv(GL_TEXTURE_BINDING_3D, (GLint *)&tex); break;}
        case TextureCubeMap: {glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, (GLint *)&tex); break;}
    }

    return tex;
}

GLint GLTexture::internalFormatToGL(InternalFormat format)
{
    switch (format)
    {
        case RedU8_Norm_InternalFormat: {return GL_R8;}
        case RedS8_Norm_InternalFormat: {return GL_R8_SNORM;}

        case RedU16_Norm_InternalFormat: {return GL_R16;}
        case RedS16_Norm_InternalFormat: {return GL_R16_SNORM;}

        case RedU8_InternalFormat: {return GL_R8UI;}
        case RedS8_InternalFormat: {return GL_R8I;}

        case RedU16_InternalFormat: {return GL_R16UI;}
        case RedS16_InternalFormat: {return GL_R16I;}

        case RedU32_InternalFormat: {return GL_R32UI;}
        case RedS32_InternalFormat: {return GL_R32I;}

        case Red16F_InternalFormat: {return GL_R16F;}
        case Red32F_InternalFormat: {return GL_R32F;}

        case RedGreenU8_Norm_InternalFormat: {return GL_RG8;}
        case RedGreenS8_Norm_InternalFormat: {return GL_RG8_SNORM;}

        case RedGreenU16_Norm_InternalFormat: {return GL_RG16;}
        case RedGreenS16_Norm_InternalFormat: {return GL_RG16_SNORM;}

        case RedGreenU8_InternalFormat: {return GL_RG8UI;}
        case RedGreenS8_InternalFormat: {return GL_RG8I;}

        case RedGreenU16_InternalFormat: {return GL_RG16UI;}
        case RedGreenS16_InternalFormat: {return GL_RG16I;}

        case RedGreenU32_InternalFormat: {return GL_RG32UI;}
        case RedGreenS32_InternalFormat: {return GL_RG32I;}

        case RedGreen16F_InternalFormat: {return GL_RG16F;}
        case RedGreen32F_InternalFormat: {return GL_RG32F;}

        case RGBU8_Norm_InternalFormat: {return GL_RGB8;}
        case RGBS8_Norm_InternalFormat: {return GL_RGB8_SNORM;}

        case RGBU16_Norm_InternalFormat: {return GL_RGB16;}
        case RGBS16_Norm_InternalFormat: {return GL_RGB16_SNORM;}

        case RGBU8_InternalFormat: {return GL_RGB8UI;}
        case RGBS8_InternalFormat: {return GL_RGB8I;}

        case RGBU16_InternalFormat: {return GL_RGB16UI;}
        case RGBS16_InternalFormat: {return GL_RGB16I;}

        case RGBU32_InternalFormat: {return GL_RGB32UI;}
        case RGBS32_InternalFormat: {return GL_RGB32I;}

        case RGB16F_InternalFormat: {return GL_RGB16F;}
        case RGB32F_InternalFormat: {return GL_RGB32F;}

        case RGBAU8_Norm_InternalFormat: {return GL_RGBA8;}
        case RGBAS8_Norm_InternalFormat: {return GL_RGBA8_SNORM;}

        case RGBAU16_Norm_InternalFormat: {return GL_RGBA16;}
        case RGBAS16_Norm_InternalFormat: {return GL_RGBA16_SNORM;}

        case RGBAU8_InternalFormat: {return GL_RGBA8UI;}
        case RGBAS8_InternalFormat: {return GL_RGBA8I;}

        case RGBAU16_InternalFormat: {return GL_RGBA16UI;}
        case RGBAS16_InternalFormat: {return GL_RGBA16I;}

        case RGBAU32_InternalFormat: {return GL_RGBA32UI;}
        case RGBAS32_InternalFormat: {return GL_RGBA32I;}

        case RGBA16F_InternalFormat: {return GL_RGBA16F;}
        case RGBA32F_InternalFormat: {return GL_RGBA32F;}

        case RGB_Norm_DXT1_InternalFormat: {return GL_COMPRESSED_RGB_S3TC_DXT1_EXT;}
        case RGBA_Norm_DXT1_InternalFormat: {return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;}

        case RGBA_Norm_DXT3_InternalFormat: {return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;}

        case RGBA_Norm_DXT5_InternalFormat: {return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;}

        case DepthI16_Norm_InternalFormat: {return GL_DEPTH_COMPONENT16;}
        case DepthI24_Norm_InternalFormat: {return GL_DEPTH_COMPONENT24;}
        case DepthI32_Norm_InternalFormat: {return GL_DEPTH_COMPONENT32;}
        case Depth32F_InternalFormat: {return GL_DEPTH_COMPONENT32F;}
    }

    return 0;
}

void GLTexture::formatToGL(Format format, GLenum& glFormat, GLenum& glType)
{
    switch (format)
    {
        case RedU8_Format: {glFormat = GL_R; glType = GL_UNSIGNED_BYTE; break;}
        case RedS8_Format: {glFormat = GL_R; glType = GL_BYTE; break;}

        case RedU16_Format: {glFormat = GL_R; glType = GL_UNSIGNED_SHORT; break;}
        case RedS16_Format: {glFormat = GL_R; glType = GL_SHORT; break;}

        case RedU32_Format: {glFormat = GL_R; glType = GL_UNSIGNED_INT; break;}
        case RedS32_Format: {glFormat = GL_R; glType = GL_INT; break;}

        case Red32F_Format: {glFormat = GL_R; glType = GL_FLOAT; break;}

        case RedGreenU8_Format: {glFormat = GL_RG; glType = GL_UNSIGNED_BYTE; break;}
        case RedGreenS8_Format: {glFormat = GL_RG; glType = GL_BYTE; break;}

        case RedGreenU16_Format: {glFormat = GL_RG; glType = GL_UNSIGNED_SHORT; break;}
        case RedGreenS16_Format: {glFormat = GL_RG; glType = GL_SHORT; break;}

        case RedGreenU32_Format: {glFormat = GL_RG; glType = GL_UNSIGNED_INT; break;}
        case RedGreenS32_Format: {glFormat = GL_RG; glType = GL_INT; break;}

        case RedGreen32F_Format: {glFormat = GL_RG; glType = GL_FLOAT; break;}

        case RGBU8_Format: {glFormat = GL_RGB; glType = GL_UNSIGNED_BYTE; break;}
        case RGBS8_Format: {glFormat = GL_RGB; glType = GL_BYTE; break;}

        case RGBU16_Format: {glFormat = GL_RGB; glType = GL_UNSIGNED_SHORT; break;}
        case RGBS16_Format: {glFormat = GL_RGB; glType = GL_SHORT; break;}

        case RGBU32_Format: {glFormat = GL_RGB; glType = GL_UNSIGNED_INT; break;}
        case RGBS32_Format: {glFormat = GL_RGB; glType = GL_INT; break;}

        case RGB32F_Format: {glFormat = GL_RGB; glType = GL_FLOAT; break;}

        case RGBAU8_Format: {glFormat = GL_RGBA; glType = GL_UNSIGNED_BYTE; break;}
        case RGBAS8_Format: {glFormat = GL_RGBA; glType = GL_BYTE; break;}

        case RGBAU16_Format: {glFormat = GL_RGBA; glType = GL_UNSIGNED_SHORT; break;}
        case RGBAS16_Format: {glFormat = GL_RGBA; glType = GL_SHORT; break;}

        case RGBAU32_Format: {glFormat = GL_RGBA; glType = GL_UNSIGNED_INT; break;}
        case RGBAS32_Format: {glFormat = GL_RGBA; glType = GL_INT; break;}

        case RGBA32F_Format: {glFormat = GL_RGBA; glType = GL_FLOAT; break;}

        case Depth32F_Format: {glFormat = GL_DEPTH_COMPONENT; glType = GL_FLOAT; break;}
    }
}
