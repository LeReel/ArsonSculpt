#include "AS_Shader.h"

AS_Shader::AS_Shader(const char* _vertexPath, const char* _fragmentPath)
{
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    //Ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        //Open files
        vShaderFile.open(_vertexPath);
        fShaderFile.open(_fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        //Read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        //Close file handlers
        vShaderFile.close();
        fShaderFile.close();
        //Convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. Compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //Shader Program
    ProgramID = glCreateProgram();
    glAttachShader(ProgramID, vertex);
    glAttachShader(ProgramID, fragment);
    glLinkProgram(ProgramID);
    //Print linking errors if any
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ProgramID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    //Delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void AS_Shader::Use()
{
    glUseProgram(ProgramID);
}

void AS_Shader::SetBool(const std::string& _name, bool _value) const
{
    glUniform1i(glGetUniformLocation(ProgramID, _name.c_str()), (int)_value);
}

void AS_Shader::SetInt(const std::string& _name, int _value) const
{
    glUniform1i(glGetUniformLocation(ProgramID, _name.c_str()), _value);
}

void AS_Shader::SetFloat(const std::string& _name, float _value) const
{
    glUniform1f(glGetUniformLocation(ProgramID, _name.c_str()), _value);
}

void AS_Shader::SetVec4(const std::string& _name, float _x, float _y, float _z, float _w)
{
    glUniform4f(glGetUniformLocation(ProgramID, _name.c_str()), _x, _y, _z, _w);
}

void AS_Shader::SetMat4(const std::string& _name, const glm::mat4 &mat)
{
    glUniformMatrix4fv(glGetUniformLocation(ProgramID, _name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
