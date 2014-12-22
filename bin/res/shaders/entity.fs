#define TO_LINEAR(color) pow((color), vec4(2.2, 2.2, 2.2, 1.0))

in vec4 frag_color;
in vec2 frag_uv;
in vec3 frag_normal_worldSpace;
in vec3 frag_position_viewSpace;
in vec3 frag_position_worldSpace;
in vec3 frag_tangent_worldSpace;

layout (location = 0) out vec4 out_color;
layout (location = 1) out float out_depth;
layout (location = 2) out vec3 out_normal;
layout (location = 3) out vec2 out_material;
layout (location = 4) out vec3 out_ambient;
layout (location = 5) out vec3 out_specular;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewNormalMatrix;
uniform mat4 modelNormalMatrix;


#if ALBEDO_TEXTURE
uniform sampler2D albedoTexture;
#endif

#if NORMAL_TEXTURE
uniform sampler2D normalTexture;
#endif

#if SPECULAR_TEXTURE
uniform sampler2D specularTexture;
#endif

#if STIPPLED_ALPHA
uniform float alpha = 1.0;
#endif

#if ENVIROMENT_TEXTURE
uniform samplerCube enviromentMap;
#endif

uniform float lodStipple = 1.0;

uniform vec3 diffuseColor = vec3(1.0);
uniform float ambientAmount = 0.1;

uniform float specularExponent = 32.0;

vec3 getNormalWorldSpace()
{
    #if NORMAL_TEXTURE
    vec3 tangent = normalize(frag_tangent_worldSpace
                             - dot(frag_tangent_worldSpace,
                                   frag_normal_worldSpace)
                             * frag_normal_worldSpace);

    mat3 TBN = transpose(mat3(tangent,
                              cross(tangent, frag_normal_worldSpace),
                              frag_normal_worldSpace));

    return normalize(TBN * (texture(normalTexture, frag_uv).rgb * 2.0 - 1.0));
    #else
    return frag_normal_worldSpace;
    #endif
}

void stipple(float alpha)
{
    int index = int(gl_FragCoord.x) % 8 +
                int(gl_FragCoord.y) % 8 * 8;

    /*mat4 thresholdMatrix = mat4
    (
       1.0 / 17.0,  9.0 / 17.0,  3.0 / 17.0, 11.0 / 17.0,
      13.0 / 17.0,  5.0 / 17.0, 15.0 / 17.0,  7.0 / 17.0,
       4.0 / 17.0, 12.0 / 17.0,  2.0 / 17.0, 10.0 / 17.0,
      16.0 / 17.0,  8.0 / 17.0, 14.0 / 17.0,  6.0 / 17.0
    );

    if (alpha - thresholdMatrix[int(gl_FragCoord.x) % 4][int(gl_FragCoord.y) % 4] < 0)*/

    int thresholdMatrix[64] = int[64](1, 49, 13, 61, 4, 52, 16, 64,
                                      33, 17, 45, 29, 36, 20, 48, 32,
                                      9, 57, 5, 53, 12, 60, 8, 56,
                                      41, 25, 37, 21, 44, 28, 40, 24,
                                      3, 51, 15, 63, 2, 50, 14, 62,
                                      35, 19, 47, 31, 34, 18, 46, 30,
                                      11, 59, 7, 55, 10, 58, 6, 54,
                                      43, 27, 39, 23, 42, 26, 38, 22);


    if (alpha - float(thresholdMatrix[index]) / 65.0 < 0.0)
    {
        discard;
    }
}

void main()
{
    stipple(lodStipple);

    #ifdef ALBEDO_TEXTURE
    out_color = TO_LINEAR(texture(albedoTexture, frag_uv)) * frag_color * vec4(diffuseColor, 1.0);
    #else
    out_color = frag_color * vec4(diffuseColor, 1.0);
    #endif

    #if STIPPLED_ALPHA
    out_color.a *= alpha;

    if (out_color.a != 1.0f)
    {
        stipple(out_color.a);
    }
    #endif

    out_depth = frag_position_viewSpace.z;

    vec3 normal_worldSpace = getNormalWorldSpace();
    vec3 normal_viewSpace = normalize(vec3(viewNormalMatrix * vec4(normal_worldSpace, 1.0)));

    out_normal = (normal_viewSpace + 1.0) / 2.0;

    #if SPECULAR_TEXTURE
    float specular = texture(specularTexture, frag_uv).r;
    #else
    float specular = 1.0f;
    #endif

    #ifdef ENVIROMENT_TEXTURE
    vec3 viewDir = normalize(frag_position_worldSpace - vec3(inverse(viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)));

    //out_color = TO_LINEAR(texture(enviromentMap, normalize(reflect(viewDir, normal_worldSpace))));

    float IOR = 1.05;

    out_color = TO_LINEAR(texture(enviromentMap, normalize(refract(viewDir, normal_worldSpace, 1.0 / IOR))));
    #endif

    out_material = vec2(specularExponent / 256.0, 1.0);

    #ifdef ENVIROMENT_TEXTURE
    out_ambient = vec3(ambientAmount);

    out_specular = mix(TO_LINEAR(textureLod(enviromentMap, normalize(reflect(viewDir, normal_worldSpace)), 15.0)).rgb, vec3(1.0), 0.25) * specular;
    #else
    out_ambient = vec3(ambientAmount);

    out_specular = vec3(specular);
    #endif
}
