#version 330 core
in vec3 in_pos;
in vec3 in_norm;
in vec4 in_color;
in vec2 in_UV;

out vec3 ex_norm;
out vec4 ex_color;
out vec2 ex_UV;
out vec3 ex_fragP;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 light             = vec3(1.0, 1.0, 1.0);
uniform float ambient_strength = 0.1;
uniform vec3 light_pos         = vec3(0.0, 32.0, -32.0);

void main()
{
    gl_Position = projection * view * model * vec4(in_pos, 1.0);
    ex_fragP    = vec3(model * vec4(in_pos, 1.0));
    ex_norm     = in_norm;
    ex_color    = in_color;
    ex_UV       = in_UV;

    vec3 ambient = ambient_strength * light;

    vec3 norm     = normalize(ex_norm);
    vec3 lightDir = normalize(light_pos - ex_fragP);

    float diff   = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light;
    vec3 result  = (ambient + diffuse) * in_color.rgb;
    //ex_color     = vec4(result, in_color.w);
}
