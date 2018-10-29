#version 330 core
layout (location = 0) in vec4 pos;

flat out int index;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    index = int(pos.w);
    gl_Position = projection * view * model * vec4(pos.xyz, 1.0);
}
