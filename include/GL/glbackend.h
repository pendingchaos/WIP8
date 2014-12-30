#ifndef GLBACKEND_H
#define GLBACKEND_H

#include <vector>
#include <string>

#include <GL/gl.h>

#include "backend.h"
#include "GL/glextensions.h"

class GLBackend : public Backend
{
    public:
        GLBackend();
        virtual ~GLBackend();

        virtual VertexBuffer *createVertexBuffer();

        virtual CompiledShader *createShader(std::string filename,
                                             CompiledShader::Type type,
                                             unsigned int numSources,
                                             const char **sources);

        virtual Texture *createTexture(Texture::Type type);

        virtual Framebuffer *createFramebuffer();

        virtual float getMaxAnisotropy();

        virtual bool isGeometryShadersSupported();

        virtual bool isTessellationSupported();
        virtual unsigned int getMaxPatchSize();

        virtual void submitDrawCall(const DrawCall& drawCall);
        virtual void executeDrawCalls(RenderTarget *target);

        void executeDrawCall(const DrawCall& drawCall);

        GLExtensions mExtensions;

        std::vector<DrawCall> mDrawCalls;

        template <typename keyType, typename valueType>
        class UnorderedNoHashMap
        {
            public:
                typedef typename std::vector<std::pair<keyType, valueType> >::iterator iterator;

                typename std::vector<std::pair<keyType, valueType> >::iterator begin()
                {
                    return mValues.begin();
                }

                typename std::vector<std::pair<keyType, valueType> >::iterator end()
                {
                    return mValues.end();
                }

                typename std::vector<std::pair<keyType, valueType> >::iterator find(keyType key)
                {
                    for (typename std::vector<std::pair<keyType, valueType> >::iterator it
                         = mValues.begin(); it != mValues.end(); ++it)
                    {
                        if (it->first == key)
                        {
                            return it;
                        }
                    }

                    return end();
                }

                void emplace(const keyType& key, const valueType& value)
                {
                    mValues.push_back(std::pair<keyType, valueType>(key, value));
                }
            private:
                std::vector<std::pair<keyType, valueType> > mValues;
        };

        typedef struct Program
        {
            CompiledShader *vertex;
            CompiledShader *fragment;
            CompiledShader *geometry;
            CompiledShader *tessControl;
            CompiledShader *tessEval;

            bool operator == (const Program& other) const
            {
                return     vertex      == other.vertex
                       and fragment    == other.fragment
                       and geometry    == other.geometry
                       and tessControl == other.tessControl
                       and tessEval    == other.tessEval;
            }
        } Program;

        typedef struct VAO
        {
            GLuint program;
            ResPtr<Mesh> mesh;

            bool operator == (const VAO& other) const
            {
                return     program == other.program
                       and mesh    == other.mesh;
            }
        } VAO;

        UnorderedNoHashMap<Program, GLuint> mPrograms;
        UnorderedNoHashMap<VAO, GLuint> mVAOs;

        GLuint getProgram(CompiledShader *vertex,
                          CompiledShader *fragment,
                          CompiledShader *geometry,
                          CompiledShader *tessControl,
                          CompiledShader *tessEval);
        GLuint createProgram(const Program& program);

        GLuint getVAO(GLuint program, ResPtr<Mesh> mesh);
        GLuint createVAO(const VAO& vao);

        void vertexAttrib(GLuint program, const char *name, const MeshComponent *component);
        void setUniforms(GLuint program, const std::map<std::string, UniformValue>& uniforms);

        unsigned int mTextureUnit;

};

#endif // GLBACKEND_H
