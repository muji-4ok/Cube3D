#version 330 core
out vec4 color;

uniform vec3 rectangleColor;

void main()
{
    color = vec4(rectangleColor, 1.0);
}