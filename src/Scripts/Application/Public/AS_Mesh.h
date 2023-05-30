#pragma once
#include "AS_Camera.h"

struct AS_Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct AS_Texture
{
    unsigned int Id;
    std::string Type; //Eg: diffuse or specular
    std::string Path; // Store the path of the texture to compare with other textures
};

class AS_Mesh
{
    // Mesh Data
    std::vector<AS_Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<AS_Texture> textures;

    // Render data
    unsigned int VAO = 0, VBO = 0, EBO = 0;

public:
    AS_Mesh();
    AS_Mesh(std::vector<AS_Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<AS_Texture> _textures);
    void Draw(AS_Shader& _shader);

private:
    void SetupMesh();
};
