#pragma once
#include <string>
#include <vector>
#include <glm/vec3.hpp>

class AS_Shader;

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec3 TexCoords;
};

struct Texture
{
    unsigned int Id;
    std::string Type; //Eg: diffuse or specular
};

class AS_Mesh
{
    // Mesh Data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // Render data
    unsigned int VAO = 0, VBO = 0, EBO = 0;

public:
    AS_Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, std::vector<Texture> _textures);
    void Draw(AS_Shader& _shader);

private:
    void SetupMesh();
};
