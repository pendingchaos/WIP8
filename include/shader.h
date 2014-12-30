#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>

#include <map>
#include <string>

#include "resource.h"
#include "texture.h"

class UniformValue
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

        UniformValue();
        UniformValue(float value);
        UniformValue(const glm::vec2& value);
        UniformValue(const glm::vec3& value);
        UniformValue(const glm::vec4& value);
        UniformValue(int value);
        UniformValue(const glm::ivec2& value);
        UniformValue(const glm::ivec3& value);
        UniformValue(const glm::ivec4& value);
        UniformValue(ResPtr<Texture> value);

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


class Renderer;

class CompiledShader
{
    friend Renderer;

    public:
        typedef enum Type
        {
            Vertex,
            Fragment,
            Geometry,
            TessControl,
            TessEval
        } Type;

        virtual ~CompiledShader();

        inline Type getType() const {return mType;}
    protected:
        CompiledShader(Type type);

        Type mType;
};


class Shader : public Resource
{
    public:
        Shader(Renderer *renderer, std::string filename, CompiledShader::Type type, std::string source);
        virtual ~Shader();

        CompiledShader *getShader(std::map<std::string, std::string> defines
                                  =std::map<std::string, std::string>());

        inline CompiledShader::Type getType() const {return mType;}
        inline const std::string& getSource() const {return mSource;}

        inline Renderer *getRenderer() const {return mRenderer;}
    private:
        Renderer *mRenderer;

        std::string mFilename;

        CompiledShader::Type mType;
        std::string mSource;

        std::map<std::map<std::string, std::string>, CompiledShader *> mShaders;
};

#endif // SHADER_H
