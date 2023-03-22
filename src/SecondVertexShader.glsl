#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 aPos;

void main()
{
    //gl_Position is one of the few built-in variables: you have to assign some value to it. 
    //Everything else is optional
    gl_Position.xyz = aPos;
    gl_Position.w = 1.0;
}