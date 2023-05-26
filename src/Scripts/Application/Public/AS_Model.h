#pragma once
#include <vector>
#include <string>

enum aiTextureType : int;
struct aiMaterial;
// Assimp
struct aiMesh;
struct aiScene;
struct aiNode;

struct Texture;

class AS_Mesh;
class AS_Shader;

class AS_Model
{
public:

    AS_Model(char* _path)
    {
        LoadModel(_path);
    }
    void Draw(AS_Shader& _shader);

private:
    // Model Data
    std::vector<AS_Mesh> meshes;
    std::string directory;
    
    void LoadModel(char* _path);
    std::vector<Texture> LoadMaterialTextures(aiMaterial* _material, aiTextureType* _type, std::string _typeName);
    
    void ProcessNode(aiNode* _node, const aiScene* _scene);
    AS_Mesh ProcessMesh(aiMesh* _mesh, const aiScene* _scene);
};
