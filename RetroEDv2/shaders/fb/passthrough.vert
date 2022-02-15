#version 330 core
layout(location = 0) in vec2 in_pos;
layout(location = 1) in vec2 in_UV;
out vec2 ex_UV;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = view * projection * vec4(in_pos, 0.0, 1.0);
    ex_UV       = in_UV;
}