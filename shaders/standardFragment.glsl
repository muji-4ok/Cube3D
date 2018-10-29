#version 330 core
out vec4 fragColor;

uniform vec3 colors[6];

uniform int i;
uniform int j;
uniform int k;

uniform int is_hit;
uniform float hit_index;

flat in int index;

void main()
{
    if (bool(is_hit) && hit_index == index)
        fragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
    else
        fragColor = vec4(colors[index], 1.0);
}
