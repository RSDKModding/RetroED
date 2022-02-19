#version 330 core

in vec3 ex_norm;
in vec4 ex_color;
in vec2 ex_UV;
in vec3 ex_fragP;
out vec4 out_color;

uniform bool useColor;
uniform bool useNormals;
uniform bool useTextures;

uniform vec4 default_color;

uniform sampler2D tex;

void main()
{
    vec3 color;
    if (useTextures)
        color = texture(tex, ex_UV).rgb;
    else if (useColor)
        color = ex_color.rgb;
    else
        color = default_color.xyz;

    // vec3 result = (ambient + diffuse) * color;
    vec3 result = color;
    out_color   = vec4(result, 1.0);
}