#version 330 core

in vec4 positionColor;

out vec4 outColor;

void main() 
{
    outColor = positionColor;
}