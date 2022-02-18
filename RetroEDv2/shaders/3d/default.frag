#version 330 core

in vec3 ex_norm;
in vec4 ex_color;
in vec2 ex_UV;
out vec4 out_color;

uniform bool useColor;
uniform bool useNormals;
uniform bool useTextures;

uniform sampler2D tex;

void main()
{
    if (useTextures)
        out_color = vec4(texture(tex, ex_UV).rgb, 1.0);
    else if (useColor)
        out_color = vec4(ex_color.rgb, 1.0);
    else
        out_color = vec4(0.5, 0.5, 0.5, 1.0);
}