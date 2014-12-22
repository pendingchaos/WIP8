#define TO_LINEAR(color) pow((color), vec4(2.2, 2.2, 2.2, 1.0))

in vec3 frag_position_worldSpace;

layout (location = 0) out vec4 out_color;
layout (location = 1) out float out_depth;
layout (location = 2) out vec3 out_normal;
layout (location = 3) out vec2 out_material;
layout (location = 4) out vec3 out_ambient;
layout (location = 5) out vec3 out_specular;

uniform samplerCube skyBoxTexture;

void main()
{
    out_color = TO_LINEAR(texture(skyBoxTexture, frag_position_worldSpace));

    out_material = vec2(0.0);
}
