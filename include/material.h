#ifndef MATERIAL_H
#define MATERIAL_H

#include <map>
#include <cstring>

#include <glm/glm.hpp>

#include "shader.h"
#include "resource.h"
#include "texture.h"

class MaterialUniform
{
    public:
        typedef enum type
        {
            Nothing,

            Float,
            Vec2,
            Vec3,
            Vec4,

            Int,
            IVec2,
            IVec3,
            IVec4,

            TextureType
        } Type;

        MaterialUniform();
        MaterialUniform(float value);
        MaterialUniform(const glm::vec2& value);
        MaterialUniform(const glm::vec3& value);
        MaterialUniform(const glm::vec4& value);
        MaterialUniform(int value);
        MaterialUniform(const glm::ivec2& value);
        MaterialUniform(const glm::ivec3& value);
        MaterialUniform(const glm::ivec4& value);
        MaterialUniform(ResPtr<Texture> value);

        inline Type getType() const
        {
            return mType;
        }

        inline float getFloat() const
        {
            if (mType == Float)
            {
                return mFloatData[0];
            }

            return 0.0f;
        }

        inline glm::vec2 getVec2() const
        {
            if (mType == Vec2)
            {
                return glm::vec2(mFloatData[0], mFloatData[1]);
            }

            return glm::vec2(0.0f);
        }

        inline glm::vec3 getVec3() const
        {
            if (mType == Vec3)
            {
                return glm::vec3(mFloatData[0], mFloatData[1], mFloatData[2]);
            }

            return glm::vec3(0.0f);
        }

        inline glm::vec4 getVec4() const
        {
            if (mType == Vec4)
            {
                return glm::vec4(mFloatData[0], mFloatData[1], mFloatData[2], mFloatData[3]);
            }

            return glm::vec4(0.0f);
        }

        inline int getInt() const
        {
            if (mType == Int)
            {
                return mIntData[0];
            }

            return 0;
        }

        inline glm::ivec2 getIVec2() const
        {
            if (mType == IVec2)
            {
                return glm::ivec2(mIntData[0], mIntData[1]);
            }

            return glm::ivec2(0);
        }

        inline glm::ivec3 getIVec3() const
        {
            if (mType == IVec3)
            {
                return glm::ivec3(mIntData[0], mIntData[1], mIntData[2]);
            }

            return glm::ivec3(0);
        }

        inline glm::ivec4 getIVec4() const
        {
            if (mType == IVec4)
            {
                return glm::ivec4(mIntData[0], mIntData[1], mIntData[2], mIntData[3]);
            }

            return glm::ivec4(0);
        }

        inline ResPtr<Texture> getTexture() const
        {
            if (mType == TextureType)
            {
                return mTexture;
            }

            return NULL;
        }

        inline void setNothing()
        {
            mType = Nothing;
        }

        inline void setFloat(float value)
        {
            mType = Float;

            mFloatData[0] = value;
        }

        inline void setVec2(const glm::vec2& value)
        {
            mType = Vec2;

            mFloatData[0] = value.x;
            mFloatData[1] = value.y;
        }

        inline void setVec3(const glm::vec3& value)
        {
            mType = Vec3;

            mFloatData[0] = value.x;
            mFloatData[1] = value.y;
            mFloatData[2] = value.z;
        }

        inline void setVec4(const glm::vec4& value)
        {
            mType = Vec4;

            mFloatData[0] = value.x;
            mFloatData[1] = value.y;
            mFloatData[2] = value.z;
            mFloatData[3] = value.w;
        }

        inline void setInt(int value)
        {
            mType = Int;

            mIntData[0] = value;
        }

        inline void setIVec2(const glm::ivec2& value)
        {
            mType = IVec2;

            mIntData[0] = value.x;
            mIntData[1] = value.y;
        }

        inline void setIVec3(const glm::ivec3& value)
        {
            mType = IVec3;

            mIntData[0] = value.x;
            mIntData[1] = value.y;
            mIntData[2] = value.z;
        }

        inline void setIVec4(const glm::ivec4& value)
        {
            mType = IVec4;

            mIntData[0] = value.x;
            mIntData[1] = value.y;
            mIntData[2] = value.z;
            mIntData[3] = value.w;
        }

        inline void setTexture(ResPtr<Texture> texture)
        {
            mType = TextureType;
            mTexture = texture;
        }
    private:
        Type mType;

        union
        {
            float mFloatData[4];
            int mIntData[4];
        };

        ResPtr<Texture> mTexture;

};


class Material : public Resource
{
    public:
        typedef enum DepthFunc
        {
            Never,
            Less,
            Equal,
            LessEqual,
            Greater,
            NotEqual,
            GreaterEqual,
            Always
        } DepthFunc;

        typedef enum BlendEquation
        {
            Add,
            Subtract,
            ReverseSubtract,
            Min,
            Max
        } BlendEquation;

        typedef enum BlendFactor
        {
            Zero,
            One,
            SrcColor,
            OneMinusSrcColor,
            DstColor,
            OneMinusDstColor,
            SrcAlpha,
            OneMinusSrcAlpha,
            DstAlpha,
            OneMinusDstAlpha
        } BlendFactor;

        Material(ResPtr<Shader> fragmentShader);

        ResPtr<Shader> mFragmentShader;
        std::map<std::string, std::string> mDefines;
        std::map<std::string, MaterialUniform> mUniforms;

        bool mWriteDepth:1;

        //TODO: Support multiple color buffers. Use glColorMaski.
        bool mWriteRed:1;
        bool mWriteGreen:1;
        bool mWriteBlue:1;
        bool mWriteAlpha:1;

        DepthFunc mDepthFunc;
        BlendEquation mRGBBlendEquation;
        BlendEquation mAlphaBlendEquation;
        BlendFactor mRGBSrcBlendFactor;
        BlendFactor mRGBDstBlendFactor;
        BlendFactor mAlphaSrcBlendFactor;
        BlendFactor mAlphaDstBlendFactor;
};

#endif // MATERIAL_H
