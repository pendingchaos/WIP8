#include res/shaders/lib/glsl compat.glsl

attribute vec3 position;
attribute vec3 normal;
attribute vec2 uv;
attribute vec4 color;
attribute vec3 tangent;

#ifdef TESSELLATION
out vec4 control_color;
out vec2 control_uv;
out vec3 control_normal_worldSpace;
out vec3 control_position_worldSpace;
out vec3 control_tangent_worldSpace;

#define frag_color control_color
#define frag_uv control_uv
#define frag_normal_worldSpace control_normal_worldSpace
#define frag_position_worldSpace control_position_worldSpace
#define frag_tangent_worldSpace control_tangent_worldSpace
#else
out vec4 frag_color;
out vec2 frag_uv;
out vec3 frag_normal_worldSpace;
out vec3 frag_position_viewSpace;
out vec3 frag_position_worldSpace;
out vec3 frag_tangent_worldSpace;
#endif

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelNormalMatrix;
uniform mat4 viewNormalMatrix;
uniform mat4 projectionNormalMatrix;

void main()
{
    frag_position_worldSpace = vec3(modelMatrix * vec4(position, 1.0));

    #ifdef TESSELLATION
    gl_Position = modelMatrix * vec4(position, 1.0);
    #else
    gl_Position = projectionMatrix * viewMatrix * vec4(frag_position_worldSpace, 1.0);
    #endif

    frag_color = color;

    frag_uv = uv;

    frag_normal_worldSpace = normalize(vec3(modelNormalMatrix * vec4(normal, 1.0)));

    frag_tangent_worldSpace = normalize(vec3(modelNormalMatrix * vec4(tangent, 1.0)));

    #ifndef TESSELLATION
    frag_position_viewSpace = vec3(viewMatrix * vec4(frag_position_worldSpace, 1.0));
    #endif
}
