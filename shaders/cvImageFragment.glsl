#version 330 core
in vec2 texCoords;
out vec4 color;

uniform sampler2D samp;

void main()
{
    color = texture2D(samp, texCoords);
}