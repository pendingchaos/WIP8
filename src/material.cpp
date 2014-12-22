#include "material.h"

MaterialUniform::MaterialUniform() : mType(Nothing)
{
    mTexture = nullRes<Texture>();
}

MaterialUniform::MaterialUniform(float value) : mType(Float)
{
    mFloatData[0] = value;
}

MaterialUniform::MaterialUniform(const glm::vec2& value) : mType(Vec2)
{
    mFloatData[0] = value.x;
    mFloatData[1] = value.y;
}

MaterialUniform::MaterialUniform(const glm::vec3& value) : mType(Vec3)
{
    mFloatData[0] = value.x;
    mFloatData[1] = value.y;
    mFloatData[2] = value.z;
}

MaterialUniform::MaterialUniform(const glm::vec4& value) : mType(Vec4)
{
    mFloatData[0] = value.x;
    mFloatData[1] = value.y;
    mFloatData[2] = value.z;
    mFloatData[3] = value.w;
}

MaterialUniform::MaterialUniform(int value) : mType(Int)
{
    mIntData[0] = value;
}

MaterialUniform::MaterialUniform(const glm::ivec2& value) : mType(IVec2)
{
    mIntData[0] = value.x;
    mIntData[1] = value.y;
}

MaterialUniform::MaterialUniform(const glm::ivec3& value) : mType(IVec3)
{
    mIntData[0] = value.x;
    mIntData[1] = value.y;
    mIntData[2] = value.z;
}

MaterialUniform::MaterialUniform(const glm::ivec4& value) : mType(IVec4)
{
    mIntData[0] = value.x;
    mIntData[1] = value.y;
    mIntData[2] = value.z;
    mIntData[3] = value.w;
}

MaterialUniform::MaterialUniform(ResPtr<Texture> value) : mType(TextureType)
{
    mTexture = value;
}


Material::Material(ResPtr<Shader> fragmentShader) : Resource(typeid(Material)),
                                                    mFragmentShader(fragmentShader),
                                                    mWriteDepth(true),
                                                    mWriteRed(true),
                                                    mWriteGreen(true),
                                                    mWriteBlue(true),
                                                    mWriteAlpha(true),
                                                    mDepthFunc(Less),
                                                    mRGBBlendEquation(Add),
                                                    mAlphaBlendEquation(Add),
                                                    mRGBSrcBlendFactor(SrcAlpha),
                                                    mRGBDstBlendFactor(OneMinusSrcAlpha),
                                                    mAlphaSrcBlendFactor(SrcAlpha),
                                                    mAlphaDstBlendFactor(OneMinusSrcAlpha) {}
