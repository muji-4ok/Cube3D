#version 330 core
layout (location = 0) in vec3 pos;

flat out int i_inner;
flat out int j_inner;
flat out int k_inner;

out vec3 outPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    i_inner = int((pos.x + 0.5) * 2);
    j_inner = int((pos.y + 0.5) * 2);
    k_inner = int((pos.z + 0.5) * 2);
    outPos = pos;
    gl_Position = projection * view * model * vec4(pos, 1.0);
}
