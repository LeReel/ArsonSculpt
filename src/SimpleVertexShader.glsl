#version 330 core

// Input vertex data, different for all executions of this shader.
layout (location = 0) in vec3 aPos;

out vec4 positionColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    //gl_Position is one of the few built-in variables: you have to assign some value to it. 
    //Everything else is optional
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    positionColor = gl_Position;
}