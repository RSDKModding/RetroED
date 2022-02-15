#version 330 core
in vec2 ex_UV;
in vec4 ex_color;
out vec4 out_color;

uniform vec2 screenSize;
uniform vec2 viewportSize;

uniform sampler2DRect tiles;

void main()
{
    ivec3 placed  = ivec3(texture(tiles, ex_UV).rgb * 255);
    int index = (placed.b >> 3) | ((placed.g >> 2) << 5) | ((placed.r >> 3) << 11);
    if (index == 0)
        discard;
    out_color = vec4(placed, 1.0);
}