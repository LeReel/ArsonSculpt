#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
  
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class AS_Shader
{
public:
    //ProgramID
    unsigned int ProgramID;

    //Constructor reads and builds the shader 
    AS_Shader(const char* _vertexPath, const char* _fragmentPath);

    //Use/activate shader
    void Use();

    //Utility uniform functions
    void SetBool(const std::string& _name, bool _value) const;
    void SetInt(const std::string& _name, int _value) const;
    void SetFloat(const std::string& _name, float _value) const;
    void SetVec4(const std::string& _name, float _x, float _y, float _z, float _w);
};

#endif