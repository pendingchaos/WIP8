#define TO_LINEAR(color) pow((color), vec3(2.2, 2.2, 2.2))
#define FROM_LINEAR(color) pow((color), 1.0/vec3(2.2, 2.2, 2.2))

#include res/shaders/lib/glsl compat.glsl

DECL_RT0(vec4);

in vec2 frag_uv;

uniform sampler2D colorTexture;
uniform sampler2D depthTexture;
uniform sampler2D normalTexture;
uniform sampler2D materialTexture;
uniform sampler2D ambientTexture;
uniform sampler2D specularTexture;

uniform vec3 lightDirection;

vec3 diffuse(vec3 normal, vec3 dir, vec3 albedo)
{
    return max(dot(normal, dir), 0.0) * albedo;
}

vec3 ambient(vec3 albedo, vec3 ambientColor)
{
    return albedo * ambientColor;
}

float specular(vec3 normal, vec3 dir, vec3 position, float specularExponent)
{
    vec3 viewDir = normalize(-position);
    vec3 reflectDir = normalize(reflect(-dir, normal));

    return pow(max(dot(viewDir, reflectDir), 0.0), specularExponent) * max(dot(normal, dir), 0.0);
}

void main()
{
    vec3 albedo = texture2D(colorTexture, frag_uv).rgb;
    float depth = texture2D(depthTexture, frag_uv).r;

    if (depth == 0.0f)
    {
        out0 = vec4(0.0f);
        discard;
    }

    vec2 material = texture2D(materialTexture, frag_uv).xy;

    bool isLit = material.y == 1.0;

    if (isLit)
    {
        float specularExponent = material.x * 256.0;

        vec3 normal = texture2D(normalTexture, frag_uv).xyz * 2.0 - 1.0;

        vec3 specularColor = texture2D(specularTexture, frag_uv).rgb;
        vec3 ambientColor = texture2D(ambientTexture, frag_uv).rgb;

        vec3 position = vec3(frag_uv * 2.0 - 1.0, depth);

        RT0 = vec4(FROM_LINEAR(  diffuse(normal, lightDirection, albedo)
                               + ambient(albedo, ambientColor)
                               + specular(normal, lightDirection, position, specularExponent) * specularColor), 1.0);
    } else
    {
        RT0 = vec4(FROM_LINEAR(albedo), 1.0);
    }
}
