#include "AS_Model.h"

#include "AS_Mesh.h"

void AS_Model::Draw(AS_Shader& _shader)
{
    const unsigned long long _size = meshes.size();

    for (unsigned int i = 0; i < _size; ++i)
    {
        meshes[i].Draw(_shader);
    }
}

void AS_Model::LoadModel(char* _path)
{
}

std::vector<Texture> AS_Model::LoadMaterialTextures(aiMaterial* _material, aiTextureType* _type, std::string _typeName)
{
    std::vector<Texture> _v;
    return _v;
}

void AS_Model::ProcessNode(aiNode* _node, const aiScene* _scene)
{
}

AS_Mesh AS_Model::ProcessMesh(aiMesh* _mesh, const aiScene* _scene)
{
    AS_Mesh _m();

    return _m();
}
