in vec3 position;

out vec3 frag_position_worldSpace;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;

void main()
{
    frag_position_worldSpace = vec3(modelMatrix * vec4(position, 1.0));

    gl_Position = projectionMatrix * vec4(mat3(viewMatrix) * position, frag_position_worldSpace);
    gl_Position = gl_Position.xyww;
}
