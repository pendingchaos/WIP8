#define TO_LINEAR(color) pow((color), vec4(2.2, 2.2, 2.2, 1.0))

#include res/shaders/lib/glsl compat.glsl

DECL_RT0(vec4);
DECL_RT1(float);
DECL_RT2(vec3);
DECL_RT3(vec2);
DECL_RT4(vec3);
DECL_RT5(vec3);

in vec3 frag_position_worldSpace;

uniform samplerCube skyBoxTexture;

void main()
{
    RT0 = TO_LINEAR(textureCube(skyBoxTexture, frag_position_worldSpace));
    RT1 = frag_position_worldSpace.z;

    RT3.xy = vec2(0.0);
}
