#include res/shaders/lib/glsl compat.glsl

attribute vec3 position;

out vec3 frag_position_worldSpace;
out vec3 frag_position_viewSpace;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;

void main()
{
    frag_position_worldSpace = vec3(modelMatrix * vec4(position, 1.0));
    frag_position_viewSpace = mat3(viewMatrix) * frag_position_worldSpace;

    gl_Position = projectionMatrix * vec4(frag_position_viewSpace, 1.0);
}
