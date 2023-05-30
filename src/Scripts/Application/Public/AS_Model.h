#pragma once
#include "AS_Mesh.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

class AS_Model
{
public:

    AS_Model();
    AS_Model(std::string _path)
    {
        LoadModel(_path);
    }
    void Draw(AS_Shader& _shader);

private:
    // Model Data
    std::vector<AS_Mesh> meshes;
    std::string directory;

    std::vector<AS_Texture> loadedTextures;

    AS_Mesh processedMesh;
    
    void LoadModel(std::string _path);
    std::vector<AS_Texture> LoadMaterialTextures(aiMaterial* _material, aiTextureType _type, std::string _typeName);
    
    void ProcessNode(aiNode* _node, const aiScene* _scene);
    void ProcessMesh(aiMesh* _mesh, const aiScene* _scene);

    unsigned int TextureFromFile(const char* _path, const std::string& _directory, bool _gamma = false);
};
