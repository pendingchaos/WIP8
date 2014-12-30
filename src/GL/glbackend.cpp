#include "GL/glbackend.h"

#include <iostream>

#include "utils/memory.h"
#include "scene.h"
#include "GL/glvertexbuffer.h"
#include "GL/glshader.h"
#include "GL/gltexture.h"
#include "GL/glrendertarget.h"

void debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char *message, const void *userParam)
{
    const char *source_str;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API_ARB: {source_str = "Api"; break;}
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB: {source_str = "Window system"; break;}
        case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB: {source_str = "GLSL Compilter"; break;}
        case GL_DEBUG_SOURCE_THIRD_PARTY_ARB: {source_str = "Third Party"; break;}
        case GL_DEBUG_SOURCE_APPLICATION_ARB: {source_str = "Application"; break;}
        case GL_DEBUG_SOURCE_OTHER_ARB: {source_str = "Other;"; break;}
        default: {source_str = "Unknown"; break;}
    }

    const char *type_str;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR_ARB: {type_str = "Error"; break;}
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB: {type_str = "Deprecated"; break;}
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB: {type_str = "Undefined behavior"; break;}
        case GL_DEBUG_TYPE_PORTABILITY_ARB: {type_str = "Portability"; break;}
        case GL_DEBUG_TYPE_PERFORMANCE_ARB: {type_str = "Performance"; break;}
        case GL_DEBUG_TYPE_OTHER_ARB: {type_str = "Other";break;}
        default: {type_str = "Unknown"; break;}
    }

    const char *severity_str;

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH_ARB: {severity_str = "High"; break;}
        case GL_DEBUG_SEVERITY_MEDIUM_ARB: {severity_str = "Medium"; break;}
        case GL_DEBUG_SEVERITY_LOW_ARB: {severity_str = "Low"; break;}
        default: {severity_str = "Unknown"; break;}
    }

    const char *code_str;

    switch (glGetError())
    {
        case GL_INVALID_ENUM: {code_str = "GL_INVALID_ENUM"; break;}
        case GL_INVALID_VALUE: {code_str = "GL_INVALID_VALUE"; break;}
        case GL_INVALID_OPERATION: {code_str = "GL_INVALID_OPERATION"; break;}
        case GL_STACK_OVERFLOW: {code_str = "GL_STACK_OVERFLOW"; break;}
        case GL_STACK_UNDERFLOW: {code_str = "GL_STACK_UNDERFLOW"; break;}
        case GL_OUT_OF_MEMORY: {code_str = "GL_OUT_OF_MEMORY"; break;}
        case GL_INVALID_FRAMEBUFFER_OPERATION: {code_str = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;}
        case 0: {code_str = "None"; break;}
        default: {code_str = "Unknown"; break;}
    }

    std::cout << "OpenGL error:" << std::endl
              << "Severity: "    << severity_str << std::endl
              << "Type: " << type_str << std::endl
              << "Source: " << source_str << std::endl
              << "Message: " << message << std::endl
              << "Code: " << code_str << std::endl
              << std::endl;

    if (severity == GL_DEBUG_SEVERITY_HIGH)
    {
        std::exit(1);
    }
}

GLBackend::GLBackend()
{
    if (mExtensions.extGL_ARB_debug_output)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallbackARB((GLDEBUGPROCARB)debugCallback, NULL);
    } else
    {
        std::cout <<
        "Warning: GL_ARB_debug_output is not supported. Debugging will be a lot harder."
        << std::endl;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glDepthFunc(GL_LESS);
}

GLBackend::~GLBackend()
{
    for (UnorderedNoHashMap<Program, GLuint>::iterator it = mPrograms.begin(); it != mPrograms.end(); ++it)
    {
        glDeleteProgram(it->second);
    }

    for (UnorderedNoHashMap<VAO, GLuint>::iterator it = mVAOs.begin(); it != mVAOs.end(); ++it)
    {
        glDeleteVertexArrays(1, &it->second);
    }
}

VertexBuffer *GLBackend::createVertexBuffer()
{
    return NEW(GLVertexBuffer);
}

CompiledShader *GLBackend::createShader(std::string filename,
                                        CompiledShader::Type type,
                                        unsigned int numSources,
                                        const char **sources)
{
    return NEW(GLCompiledShader, filename, type, numSources, sources);
}

Texture *GLBackend::createTexture(Texture::Type type)
{
    return NEW(GLTexture, type);
}

Framebuffer *GLBackend::createFramebuffer()
{
    return NEW(GLFramebuffer, this);
}

float GLBackend::getMaxAnisotropy()
{
    float anisotropy;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy);
    return anisotropy;
}

bool GLBackend::isGeometryShadersSupported()
{
    return mExtensions.extGL_ARB_geometry_shader4;
}

bool GLBackend::isTessellationSupported()
{
    return mExtensions.extGL_ARB_tessellation_shader;
}

unsigned int GLBackend::getMaxPatchSize()
{
    GLint maxPatchSize;
    glGetIntegerv(GL_MAX_PATCH_VERTICES, &maxPatchSize);

    return maxPatchSize;
}

void GLBackend::submitDrawCall(const DrawCall& drawCall)
{
    mDrawCalls.push_back(drawCall);
}

void GLBackend::executeDrawCalls(RenderTarget *target)
{
    if (target->getFramebuffer() != NULL)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, ((GLFramebuffer *)target->getFramebuffer())->mFramebuffer);
    } else
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    glViewport(0, 0, target->getWidth(), target->getHeight());

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

    for (std::vector<DrawCall>::iterator it = mDrawCalls.begin(); it != mDrawCalls.end(); ++it)
    {
        executeDrawCall(*it);
    }

    mDrawCalls.clear();
}

static const GLenum toGLDepthFunc[]
{
    GL_NEVER,
    GL_LESS,
    GL_EQUAL,
    GL_LEQUAL,
    GL_GREATER,
    GL_NOTEQUAL,
    GL_GEQUAL,
    GL_ALWAYS
};

static const GLenum toGLBlendEquation[]
{
    GL_FUNC_ADD,
    GL_FUNC_SUBTRACT,
    GL_FUNC_REVERSE_SUBTRACT,
    GL_MIN,
    GL_MAX
};

static const GLenum toGLBlendFactor[]
{
    GL_ZERO,
    GL_ONE,
    GL_SRC_COLOR,
    GL_ONE_MINUS_SRC_COLOR,
    GL_DST_COLOR,
    GL_ONE_MINUS_DST_COLOR,
    GL_SRC_ALPHA,
    GL_ONE_MINUS_SRC_ALPHA,
    GL_DST_ALPHA,
    GL_ONE_MINUS_DST_ALPHA
};

void GLBackend::executeDrawCall(const DrawCall& drawCall)
{
    Scene *scene = drawCall.mScene;
    const glm::mat4& modelMatrix = drawCall.mModelMatrix;
    ResPtr<Mesh> mesh = drawCall.mMesh;
    ResPtr<Material> material = drawCall.mMaterial;

    switch (mesh->mCullFace)
    {
        case Mesh::None:
        {
            glDisable(GL_CULL_FACE);
            break;
        }
        case Mesh::Front:
        {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            break;
        }
        case Mesh::Back:
        {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            break;
        }
    }

    switch (mesh->mFrontFaceWinding)
    {
        case Mesh::Clockwise:
        {
            glFrontFace(GL_CW);
            break;
        }
        case Mesh::CounterClockwise:
        {
            glFrontFace(GL_CCW);
            break;
        }
    }

    glDepthMask(material->mWriteDepth);
    glColorMask(material->mWriteRed,
                material->mWriteGreen,
                material->mWriteBlue,
                material->mWriteAlpha);

    glDepthFunc(toGLDepthFunc[material->mDepthFunc]);

    glBlendFuncSeparate(toGLBlendFactor[material->mRGBSrcBlendFactor],
                        toGLBlendFactor[material->mRGBDstBlendFactor],
                        toGLBlendFactor[material->mAlphaSrcBlendFactor],
                        toGLBlendFactor[material->mAlphaDstBlendFactor]);

    glBlendEquationSeparate(toGLBlendEquation[material->mRGBBlendEquation],
                            toGLBlendEquation[material->mAlphaBlendEquation]);

    std::map<std::string, std::string> defines;
    defines.insert(material->mDefines.begin(), material->mDefines.end());
    defines.insert(mesh->mDefines.begin(), mesh->mDefines.end());

    CompiledShader *fragmentShader = material->mFragmentShader->getShader(defines);
    CompiledShader *vertexShader = mesh->mVertexShader->getShader(defines);
    CompiledShader *geometryShader = NULL;
    CompiledShader *tessControlShader = NULL;
    CompiledShader *tessEvalShader = NULL;

    if (mesh->mGeometryShader != nullRes<Shader>() and isGeometryShadersSupported())
    {
        geometryShader = mesh->mGeometryShader->getShader(defines);
    }

    if (mesh->mTessControlShader != nullRes<Shader>()
        and mesh->mTessEvalShader != nullRes<Shader>()
        and isTessellationSupported())
    {
        tessControlShader = mesh->mTessControlShader->getShader(defines);
        tessEvalShader = mesh->mTessEvalShader->getShader(defines);

        glPatchParameteri(GL_PATCH_VERTICES, mesh->mPatchSize);

        //TODO: A wireframe option for meshes.
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else
    {
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    GLuint program = getProgram(vertexShader,
                                fragmentShader,
                                geometryShader,
                                tessControlShader,
                                tessEvalShader);

    glUseProgram(program);

    GLuint vao = getVAO(program, mesh);

    if (mExtensions.extGL_ARB_vertex_array_object)
    {
        glBindVertexArray(vao);
    }

    mTextureUnit = 0;
    setUniforms(program, material->mUniforms);
    setUniforms(program, mesh->mUniforms);

    GLint modelMatrixIndex = glGetUniformLocation(program, "modelMatrix");
    GLint viewMatrixIndex = glGetUniformLocation(program, "viewMatrix");
    GLint projectionMatrixIndex = glGetUniformLocation(program, "projectionMatrix");
    GLint viewNormalMatrixIndex = glGetUniformLocation(program, "viewNormalMatrix");
    GLint modelNormalMatrixIndex = glGetUniformLocation(program, "modelNormalMatrix");
    GLint projectionNormalMatrixIndex = glGetUniformLocation(program, "projectionNormalMatrix");
    GLint lodStippleIndex = glGetUniformLocation(program, "lodStipple");

    if (modelMatrixIndex != -1)
    {
        glUniformMatrix4fv(modelMatrixIndex, 1, GL_FALSE, (const GLfloat *)&modelMatrix);
    }
    if (viewMatrixIndex != -1)
    {
        glm::mat4 viewMatrix = scene->mViewTransform.getMatrix();

        glUniformMatrix4fv(viewMatrixIndex, 1, GL_FALSE, (const GLfloat *)&viewMatrix);
    }
    if (projectionMatrixIndex != -1)
    {
        glm::mat4 modelMatrix = scene->mProjectionTransform.getMatrix();

        glUniformMatrix4fv(projectionMatrixIndex, 1, GL_FALSE, (const GLfloat *)&modelMatrix);
    }
    if (modelNormalMatrixIndex != -1)
    {
        glm::mat4 modelNormalMatrix = glm::transpose(glm::inverse(modelMatrix));

        glUniformMatrix4fv(modelNormalMatrixIndex, 1, GL_FALSE, (const GLfloat *)&modelNormalMatrix);
    }
    if (viewNormalMatrixIndex != -1)
    {
        glm::mat4 viewNormalMatrix = glm::transpose(glm::inverse(scene->mViewTransform.getMatrix()));

        glUniformMatrix4fv(viewNormalMatrixIndex, 1, GL_FALSE, (const GLfloat *)&viewNormalMatrix);
    }
    if (projectionNormalMatrixIndex != -1)
    {
        glm::mat4 projectionNormalMatrix = glm::transpose(glm::inverse(scene->mProjectionTransform.getMatrix()));

        glUniformMatrix4fv(projectionNormalMatrixIndex, 1, GL_FALSE, (const GLfloat *)&projectionNormalMatrix);
    }
    if (lodStippleIndex != -1)
    {
        glUniform1f(lodStippleIndex, drawCall.mLodStipple);
    }

    GLenum glPrimitive = 0;

    switch (mesh->mPrimitive)
    {
        case Mesh::Points: {glPrimitive = GL_POINTS; break;}
        case Mesh::LineStrip: {glPrimitive = GL_LINE_STRIP; break;}
        case Mesh::LineLoop: {glPrimitive = GL_LINE_LOOP; break;}
        case Mesh::TriangleStrip: {glPrimitive = GL_TRIANGLE_STRIP; break;}
        case Mesh::TriangleFan: {glPrimitive = GL_TRIANGLE_FAN; break;}
        case Mesh::Triangles: {glPrimitive = GL_TRIANGLES; break;}
        case Mesh::Patches: {glPrimitive = GL_PATCHES; break;}
    }

    if (mesh->hasIndices())
    {
        GLenum glIndexType;

        switch (mesh->getIndices()->getType())
        {
            case MeshComponent::UInt8: {glIndexType = GL_UNSIGNED_BYTE; break;}
            case MeshComponent::UInt16: {glIndexType = GL_UNSIGNED_SHORT; break;}
            case MeshComponent::UInt32: {glIndexType = GL_UNSIGNED_INT; break;}
            default: {return;}
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ((GLVertexBuffer *)mesh->getIndices()->getVertexBuffer())->mBuffer);

        glDrawElements(glPrimitive, mesh->mNumIndices, glIndexType, (char *)0 + mesh->getIndices()->getOffset());
    } else
    {
        glDrawArrays(glPrimitive, 0, mesh->mNumVertices);
    }
}

GLuint GLBackend::getProgram(CompiledShader *vertex,
                             CompiledShader *fragment,
                             CompiledShader *geometry,
                             CompiledShader *tessControl,
                             CompiledShader *tessEval)
{
    Program program = (Program){vertex, fragment, geometry, tessControl, tessEval};

    UnorderedNoHashMap<Program, GLuint>::iterator pos = mPrograms.find(program);

    if (pos == mPrograms.end())
    {
        GLuint glProgram = createProgram(program);
        mPrograms.emplace(program, glProgram);

        return glProgram;
    }

    return pos->second;
}

GLuint GLBackend::createProgram(const Program& program)
{
    GLuint glProgram = glCreateProgram();

    if (program.vertex != NULL)
    {
        glAttachShader(glProgram, ((GLCompiledShader *)program.vertex)->mShader);
    }

    if (program.fragment != NULL)
    {
        glAttachShader(glProgram, ((GLCompiledShader *)program.fragment)->mShader);
    }

    if (program.geometry != NULL and isGeometryShadersSupported())
    {
        glAttachShader(glProgram, ((GLCompiledShader *)program.geometry)->mShader);
    }

    if (program.tessControl != NULL and isTessellationSupported())
    {
        glAttachShader(glProgram, ((GLCompiledShader *)program.tessControl)->mShader);
    }

    if (program.tessEval != NULL and isTessellationSupported())
    {
        glAttachShader(glProgram, ((GLCompiledShader *)program.tessEval)->mShader);
    }

    glLinkProgram(glProgram);
    glValidateProgram(glProgram);

    return glProgram;
}

GLuint GLBackend::getVAO(GLuint program, ResPtr<Mesh> mesh)
{
    VAO vao = (VAO){program, mesh};

    if (not mExtensions.extGL_ARB_vertex_array_object)
    {
        createVAO(vao);

        return 0;
    }

    UnorderedNoHashMap<VAO, GLuint>::iterator pos = mVAOs.find(vao);

    if (pos == mVAOs.end())
    {
        GLuint glVAO = createVAO(vao);
        mVAOs.emplace(vao, glVAO);

        return glVAO;
    }

    return pos->second;
}

GLuint GLBackend::createVAO(const VAO& vao)
{
    GLuint program = vao.program;
    ResPtr<Mesh> mesh = vao.mesh;

    GLuint glVAO;

    if (mExtensions.extGL_ARB_vertex_array_object)
    {
        glGenVertexArrays(1, &glVAO);

        glUseProgram(program);
        glBindVertexArray(glVAO);
    }

    if (mesh->hasPositions())
    {
        vertexAttrib(program, "position", mesh->getPositions());
    }
    if (mesh->hasNormals())
    {
        vertexAttrib(program, "normal", mesh->getNormals());
    }
    if (mesh->hasUVs())
    {
        vertexAttrib(program, "uv", mesh->getUVs());
    }
    if (mesh->hasColors())
    {
        vertexAttrib(program, "color", mesh->getColors());
    }
    if (mesh->hasTangents())
    {
        vertexAttrib(program, "tangent", mesh->getTangents());
    }

    return glVAO;
}

void GLBackend::vertexAttrib(GLuint program, const char *name, const MeshComponent *component)
{
    glBindBuffer(GL_ARRAY_BUFFER, ((GLVertexBuffer *)component->getVertexBuffer())->mBuffer);

    GLint index = glGetAttribLocation(program, name);

    if (index != -1)
    {
        glEnableVertexAttribArray(index);

        GLenum glType = 0;

        switch (component->getType())
        {
            case MeshComponent::SInt8: {glType = GL_BYTE; break;}
            case MeshComponent::UInt8: {glType = GL_UNSIGNED_BYTE; break;}

            case MeshComponent::SInt16: {glType = GL_SHORT; break;}
            case MeshComponent::UInt16: {glType = GL_UNSIGNED_SHORT; break;}

            case MeshComponent::SInt32: {glType = GL_INT; break;}
            case MeshComponent::UInt32: {glType = GL_UNSIGNED_INT; break;}

            case MeshComponent::Float16: {glType = GL_HALF_FLOAT; break;}
            case MeshComponent::Float32: {glType = GL_FLOAT; break;}
            case MeshComponent::Float64: {glType = GL_DOUBLE; break;}
        }

        if (component->getType() < MeshComponent::Float16)
        {
            glVertexAttribIPointer(index,
                                   component->getNumValues(),
                                   glType,
                                   component->getStride(),
                                   (char *)0+component->getOffset());
        } else
        {
            glVertexAttribPointer(index,
                                  component->getNumValues(),
                                  glType,
                                  component->getNormalized(),
                                  component->getStride(),
                                  (char *)0+component->getOffset());
        }
    }
}

void GLBackend::setUniforms(GLuint program, const std::map<std::string, UniformValue>& uniforms)
{
    for (std::map<std::string, UniformValue>::const_iterator it = uniforms.begin();
         it != uniforms.end(); ++it)
    {
        const UniformValue& uniform = it->second;

        GLint index = glGetUniformLocation(program, it->first.c_str());

        if (index == -1)
        {
            continue;
        }

        switch (uniform.getType())
        {
            case UniformValue::Nothing:
            {
                break;
            }

            case UniformValue::Float:
            {
                float value = uniform.getFloat();

                glUniform1f(index, value);
                break;
            }
            case UniformValue::Vec2:
            {
                glm::vec2 value = uniform.getVec2();

                glUniform2f(index, value.x, value.y);
                break;
            }
            case UniformValue::Vec3:
            {
                glm::vec3 value = uniform.getVec3();

                glUniform3f(index, value.x, value.y, value.z);
                break;
            }
            case UniformValue::Vec4:
            {
                glm::vec4 value = uniform.getVec4();

                glUniform4f(index, value.x, value.y, value.z, value.w);
                break;
            }

            case UniformValue::Int:
            {
                int value = uniform.getInt();

                glUniform1i(index, value);
                break;
            }
            case UniformValue::IVec2:
            {
                glm::ivec2 value = uniform.getIVec2();

                glUniform2i(index, value.x, value.y);
                break;
            }
            case UniformValue::IVec3:
            {
                glm::ivec3 value = uniform.getIVec3();

                glUniform3i(index, value.x, value.y, value.z);
                break;
            }
            case UniformValue::IVec4:
            {
                glm::ivec4 value = uniform.getIVec4();

                glUniform4i(index, value.x, value.y, value.z, value.w);
                break;
            }

            case UniformValue::TextureType:
            {
                GLTexture *texture = (GLTexture *)uniform.getTexture().getPointer();

                glActiveTexture(GL_TEXTURE0+mTextureUnit);
                glBindTexture(texture->mTarget, texture->mTexture);
                glUniform1i(index, mTextureUnit);

                ++mTextureUnit;
                break;
            }
        }
    }
}
