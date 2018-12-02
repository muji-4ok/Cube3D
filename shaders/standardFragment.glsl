#version 330 core
out vec4 fragColor;

uniform vec3 colors[6];

flat in int index;

void main()
{
    fragColor = vec4(colors[index], 1.0);
}
