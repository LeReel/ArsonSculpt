#include "AS_Mesh.h"
#include "AS_Shader.h"

AS_Mesh::AS_Mesh()
{
}

AS_Mesh::AS_Mesh(std::vector<Vertex> _vertices, std::vector<unsigned> _indices, std::vector<Texture> _textures)
{
    vertices = _vertices;
    indices = _indices;
    textures = _textures;

    SetupMesh();
}

void AS_Mesh::Draw(AS_Shader& _shader)
{
    unsigned int _diffuseNr = 1, _specularNr = 1;
    const unsigned long long _size = textures.size();

    for (unsigned int i = 0; i < _size; ++i)
    {
        // Activate proper texture unit before binding
        glActiveTexture(GL_TEXTURE0 + i);

        // Retrieve texture number (the N in diffuse_textureN)
        std::string _number = "";
        std::string _name = textures[i].Type;
        if (_name == "texture_diffuse")
        {
            _number = std::to_string(_diffuseNr++);
        }
        else if (_name == "texture_specular")
        {
            _number = std::to_string(_specularNr++);
        }

        _shader.SetInt(("material." + _name + _number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].Id);
    }
    glActiveTexture(GL_TEXTURE0);

    // Draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES,
                   indices.size(),
                   GL_UNSIGNED_INT,
                   0);
    glBindVertexArray(0);
}

void AS_Mesh::SetupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(Vertex),
                 &vertices[0],
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(unsigned int),
                 &indices[0],
                 GL_STATIC_DRAW);

    // Vertex position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void*)0);

    // Vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void*)offsetof(Vertex, Normal));

    // Vertex textCoords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}
