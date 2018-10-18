#version 330 core
in vec3 vertColor;

out vec4 fragColor;

uniform float tweenVal;

void main()
{
    fragColor = vec4(0.2, 0.5, tweenVal, 1.0);
}
