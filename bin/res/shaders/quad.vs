#if CORE
#define attribute in
#else
#define out varying
#endif

attribute vec2 position;

out vec2 frag_uv;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);

    frag_uv = (position + 1.0) / 2.0;
}
