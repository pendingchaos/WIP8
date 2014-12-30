#include res/shaders/lib/glsl compat.glsl

attribute vec3 position;

out vec3 frag_position_worldSpace;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;

void main()
{
    frag_position_worldSpace = vec3(modelMatrix * vec4(position, 1.0));

    gl_Position = projectionMatrix * mat4(mat3(viewMatrix)) * vec4(frag_position_worldSpace, 1.0);
}
