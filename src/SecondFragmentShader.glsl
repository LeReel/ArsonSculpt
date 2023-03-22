#version 330 core

//in vec4 vertexColor; //Input variable from vertexShader (same name and type)

//Uniforms are global, meaning that the variable is unique per shader program object.
//They can be accessed from any shader at any stage in the shader program.
//Whatever their value is set to, they will keep it until reset or updated
uniform vec4 vertexColor;

out vec4 outColor;

void main() {
    outColor = vertexColor;
}