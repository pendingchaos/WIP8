#ifndef MATERIAL_H
#define MATERIAL_H

#include <map>
#include <cstring>

#include <glm/glm.hpp>

#include "shader.h"
#include "resource.h"

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
        std::map<std::string, UniformValue> mUniforms;

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
