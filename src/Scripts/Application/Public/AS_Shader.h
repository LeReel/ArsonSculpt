#pragma once

#ifndef SHADER_H
#define SHADER_H

//! Always include glew.h before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
  
// Standard headers
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

class AS_Shader
{
public:
    //ProgramID
    unsigned int ProgramID = 0;

    AS_Shader();    
    //Constructor reads and builds the shader 
    AS_Shader(const char* _vertexPath, const char* _fragmentPath);

    //Use/activate shader
    void Use();

    //Utility uniform functions
    void SetBool(const std::string& _name, bool _value) const;
    void SetInt(const std::string& _name, int _value) const;
    void SetFloat(const std::string& _name, float _value) const;
    void SetVec4(const std::string& _name, float _x, float _y, float _z, float _w);
    void SetMat4(const std::string& _name, const glm::mat4 &mat);
};

#endif