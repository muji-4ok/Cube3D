#version 330 core
layout (location = 0) in vec3 pos;

out vec3 vertColor;

void main()
{
    vertColor = pos;
    gl_Position = vec4(pos, 1.0);
}
