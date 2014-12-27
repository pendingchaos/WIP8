#include res/shaders/lib/glsl compat.glsl

attribute vec2 position;

out vec2 frag_uv;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);

    frag_uv = (position + 1.0) / 2.0;
}
