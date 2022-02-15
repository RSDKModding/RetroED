#version 330 core
in vec2 ex_UV;
out vec4 out_color;

uniform sampler2D source;
uniform sampler2D dest;

void main()
{
    out_color = texture(source, ex_UV);
    if (out_color.a == 0)
        discard;

    out_color = vec4(out_color.rgb, 1.0);
}