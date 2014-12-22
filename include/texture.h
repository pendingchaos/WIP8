#ifndef TEXTURE_H
#define TEXTURE_H

#include "resource.h"

class Texture : public Resource
{
    public:
        typedef enum Type
        {
            Texture1D,
            Texture2D,
            Texture3D,
            TextureCubeMap
        } Type;

        typedef enum InternalFormat
        {
            RedU8_Norm_InternalFormat,
            RedS8_Norm_InternalFormat,

            RedU16_Norm_InternalFormat,
            RedS16_Norm_InternalFormat,

            RedU8_InternalFormat,
            RedS8_InternalFormat,

            RedU16_InternalFormat,
            RedS16_InternalFormat,

            RedU32_InternalFormat,
            RedS32_InternalFormat,

            Red16F_InternalFormat,
            Red32F_InternalFormat,

            RedGreenU8_Norm_InternalFormat,
            RedGreenS8_Norm_InternalFormat,

            RedGreenU16_Norm_InternalFormat,
            RedGreenS16_Norm_InternalFormat,

            RedGreenU8_InternalFormat,
            RedGreenS8_InternalFormat,

            RedGreenU16_InternalFormat,
            RedGreenS16_InternalFormat,

            RedGreenU32_InternalFormat,
            RedGreenS32_InternalFormat,

            RedGreen16F_InternalFormat,
            RedGreen32F_InternalFormat,

            RGBU8_Norm_InternalFormat,
            RGBS8_Norm_InternalFormat,

            RGBU16_Norm_InternalFormat,
            RGBS16_Norm_InternalFormat,

            RGBU8_InternalFormat,
            RGBS8_InternalFormat,

            RGBU16_InternalFormat,
            RGBS16_InternalFormat,

            RGBU32_InternalFormat,
            RGBS32_InternalFormat,

            RGB16F_InternalFormat,
            RGB32F_InternalFormat,

            RGBAU8_Norm_InternalFormat,
            RGBAS8_Norm_InternalFormat,

            RGBAU16_Norm_InternalFormat,
            RGBAS16_Norm_InternalFormat,

            RGBAU8_InternalFormat,
            RGBAS8_InternalFormat,

            RGBAU16_InternalFormat,
            RGBAS16_InternalFormat,

            RGBAU32_InternalFormat,
            RGBAS32_InternalFormat,

            RGBA16F_InternalFormat,
            RGBA32F_InternalFormat,

            RGB_Norm_DXT1_InternalFormat,
            RGBA_Norm_DXT1_InternalFormat,

            RGBA_Norm_DXT3_InternalFormat,

            RGBA_Norm_DXT5_InternalFormat,

            DepthI16_Norm_InternalFormat,
            DepthI24_Norm_InternalFormat,
            DepthI32_Norm_InternalFormat,
            Depth32F_InternalFormat
        } InternalFormat;

        typedef enum Format
        {
            RedU8_Format,
            RedS8_Format,

            RedU16_Format,
            RedS16_Format,

            RedU32_Format,
            RedS32_Format,

            Red32F_Format,

            RedGreenU8_Format,
            RedGreenS8_Format,

            RedGreenU16_Format,
            RedGreenS16_Format,

            RedGreenU32_Format,
            RedGreenS32_Format,

            RedGreen32F_Format,

            RGBU8_Format,
            RGBS8_Format,

            RGBU16_Format,
            RGBS16_Format,

            RGBU32_Format,
            RGBS32_Format,

            RGB32F_Format,

            RGBAU8_Format,
            RGBAS8_Format,

            RGBAU16_Format,
            RGBAS16_Format,

            RGBAU32_Format,
            RGBAS32_Format,

            RGBA32F_Format,

            Depth32F_Format
        } Format;

        typedef enum MinFilter
        {
            LinearMinFilter,
            NearestMinFilter,

            LinearWithNearestMipmapMinFilter,
            NearestWithNearestMipmapMinFilter,

            LinearWithLinearMipmapMinFilter,
            NearestWithLinearMipmapMinFilter
        } MinFilter;

        typedef enum MagFilter
        {
            LinearMagFilter,
            NearestMagFilter
        } MagFilter;

        typedef enum Wrap
        {
            Repeat,
            MirroredRepeat,
            Clamp
        } Wrap;

        virtual ~Texture();

        virtual void allocData1D(unsigned int width,
                                 InternalFormat internalFormat,
                                 Format format,
                                 const void *data)=0;

        virtual void allocData2D(unsigned int width,
                                 unsigned int height,
                                 InternalFormat internalFormat,
                                 Format format,
                                 const void *data)=0;

        virtual void allocFaceData(unsigned int width,
                                   unsigned int height,
                                   InternalFormat internalFormat,
                                   Format format,
                                   const void **data)=0;

        virtual void allocData3D(unsigned int width,
                                 unsigned int height,
                                 unsigned int depth,
                                 InternalFormat internalFormat,
                                 Format format,
                                 const void *data)=0;

        virtual void generateMipmaps()=0;

        virtual void setMinFilter(MinFilter filter)=0;
        virtual MinFilter getMinFilter()=0;

        virtual void setMagFilter(MagFilter filter)=0;
        virtual MagFilter getMagFilter()=0;

        virtual void setUWrap(Wrap wrap)=0;
        virtual Wrap getUWrap()=0;

        virtual void setVWrap(Wrap wrap)=0;
        virtual Wrap getVWrap()=0;

        virtual void setWWrap(Wrap wrap)=0;
        virtual Wrap getWWrap()=0;

        virtual void setAnisotropy(float anisotropy)=0;
        virtual float getAnisotropy()=0;

        inline Type getType() const {return mType;}
    protected:
        Texture(Type type);

        Type mType;
};

#endif // TEXTURE_H
