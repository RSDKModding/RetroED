#version 330 core
in vec2 ex_UV;
in vec4 ex_color;
out vec4 out_color;

uniform sampler2D sprite;
uniform vec3 transColor;

uniform bool useColor;

void main()
{
    if (useColor) out_color = vec4(ex_color.rgb, 1.0);
    else if (transColor != texture(sprite, ex_UV).rgb)
        out_color = vec4(texture(sprite, ex_UV).rgb, 1.0);
    else discard;
}