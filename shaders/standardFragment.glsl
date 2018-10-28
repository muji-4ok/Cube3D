#version 330 core
out vec4 fragColor;

uniform mat3 colors_i;
uniform mat3 colors_j;
uniform mat3 colors_k;
uniform vec3 colors[6];

uniform int i;
uniform int j;
uniform int k;

uniform int is_hit;
uniform float hit_index;

flat in int i_inner;
flat in int j_inner;
flat in int k_inner;

flat in int index;

in vec3 outPos;

void main()
{
    if (bool(is_hit) && hit_index == index)
        fragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
    else
        fragColor = vec4(colors[index], 1.0);
}
