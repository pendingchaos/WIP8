#include "material.h"

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
