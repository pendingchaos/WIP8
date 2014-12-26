#define TO_LINEAR(color) pow((color), vec4(2.2, 2.2, 2.2, 1.0))

#if CORE
layout (location = 0) out vec4 out0; //out_color;
layout (location = 1) out float out1; //out_depth;
layout (location = 2) out vec3 out2; //out_normal;
layout (location = 3) out vec2 out3; //out_material;
layout (location = 4) out vec3 out4; //out_ambient;
layout (location = 5) out vec3 out5; //out_specular;

#define texture2D(tex, uv) texture(tex, uv)
#define textureCube(tex, dir) texture(tex, dir)

#define texture2DLod(tex, uv, lod) textureLod(tex, uv, lod)
#define textureCubeLod(tex, dir, lod) textureLod(tex, dir, lod)
#else
#define in varying

#define out0 gl_FragData[0]
#define out1 gl_FragData[1].x
#define out2 gl_FragData[2].xyz
#define out3 gl_FragData[3].xy
#define out4 gl_FragData[4].xyz
#define out5 gl_FragData[5].xyz
#endif

in vec3 frag_position_worldSpace;

uniform samplerCube skyBoxTexture;

void main()
{
    out0 = TO_LINEAR(textureCube(skyBoxTexture, frag_position_worldSpace));
    out1 = frag_position_worldSpace.z;

    out3 = vec2(0.0);
}
