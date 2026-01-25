#version 330 core
in vec2 in_pos;
in vec2 in_UV;
in vec4 in_color;
out vec2 ex_UV;
out vec4 ex_color;

uniform mat4 view; //set to identity for screen based           
uniform mat4 projection;

void main()
{
    gl_Position = view * projection * vec4(in_pos, 0.0, 1.0);
    ex_UV       = in_UV;
    ex_color    = in_color;
}
