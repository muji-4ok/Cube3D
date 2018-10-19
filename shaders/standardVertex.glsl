#version 330 core
layout (location = 0) in vec4 pos;

flat out int i_inner;
flat out int j_inner;
flat out int k_inner;

flat out int index;

out vec3 outPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    index = int(pos.w);
    i_inner = int((pos.x + 0.5) * 2);
    j_inner = int((pos.y + 0.5) * 2);
    k_inner = int((pos.z + 0.5) * 2);
    outPos = pos.xyz;
    gl_Position = projection * view * model * vec4(pos.xyz, 1.0);
}
