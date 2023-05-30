#include "AS_Model.h"

#define STB_IMAGE_IMPLEMENTATION
#include "src/Scripts/stb_image.h"

AS_Model::AS_Model()
{
    
}

void AS_Model::Draw(AS_Shader& _shader)
{
    const unsigned long long _size = meshes.size();

    for (unsigned int i = 0; i < _size; ++i)
    {
        meshes[i].Draw(_shader);
    }
}

void AS_Model::LoadModel(std::string _path)
{
    Assimp::Importer _importer;
    const aiScene* _scene = _importer.ReadFile(_path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!_scene || _scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !_scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << _importer.GetErrorString() << std::endl;
        return;
    }
    directory = _path.substr(0, _path.find_last_of('/'));

    ProcessNode(_scene->mRootNode, _scene);
}

std::vector<AS_Texture> AS_Model::LoadMaterialTextures(aiMaterial* _material,
                                                       aiTextureType _type,
                                                       std::string _typeName)
{
    std::vector<AS_Texture> _textures;

    unsigned _texCount = _material->GetTextureCount(_type);
    for (unsigned int i = 0; i < _texCount; ++i)
    {
        aiString _string;
        _material->GetTexture(_type, i, &_string);
        bool _skip = false;

        unsigned _loadedTexSize = loadedTextures.size();
        for (unsigned int j = 0; j < _loadedTexSize; ++j)
        {
            if (std::strcmp(loadedTextures[j].Path.data(), _string.C_Str()) == 0)
            {
                _textures.push_back(loadedTextures[j]);
                _skip = true;
                break;
            }
        }

        // If texture hasn't been loaded yet
        if (!_skip)
        {
            AS_Texture _texture;
            _texture.Id = TextureFromFile(_string.C_Str(), directory);
            _texture.Type = _typeName;
            _texture.Path = _string.C_Str();

            _textures.push_back(_texture);
            loadedTextures.push_back(_texture); //Add to loaded textures
        }
    }
    return _textures;
}

void AS_Model::ProcessNode(aiNode* _node, const aiScene* _scene)
{
    // Process all the node's meshes (if any)
    const unsigned _numMeshes = _node->mNumMeshes;
    for (unsigned int i = 0; i < _numMeshes; ++i)
    {
        aiMesh* _mesh = _scene->mMeshes[_node->mMeshes[i]];
        ProcessMesh(_mesh, _scene);
        meshes.push_back(processedMesh);
    }

    // Then do the same for each of its children
    unsigned _numChildren = _node->mNumChildren;
    for (unsigned int i = 0; i < _numChildren; ++i)
    {
        ProcessNode(_node->mChildren[i], _scene);
    }
}

void AS_Model::ProcessMesh(aiMesh* _mesh, const aiScene* _scene)
{
    std::vector<AS_Vertex> _vertices;
    std::vector<unsigned int> _indices;
    std::vector<AS_Texture> _textures;

    unsigned _numVertices = _mesh->mNumVertices;

    for (unsigned int i = 0; i < _numVertices; ++i)
    {
        AS_Vertex _vertex;
        glm::vec3 _vector;

        // Process vertex position, normals and texture coordinates
        aiVector3D _position = _mesh->mVertices[i];
        _vector.x = _position.x;
        _vector.y = _position.y;
        _vector.z = _position.z;
        _vertex.Position = _vector;

        aiVector3D _normal = _mesh->mNormals[i];
        _vector.x = _normal.x;
        _vector.y = _normal.y;
        _vector.z = _normal.z;
        _vertex.Normal = _vector;

        if (_mesh->mTextureCoords[0]) // Does the mesh contains texCoords ?
        {
            glm::vec2 _vec;
            _vec.x = _mesh->mTextureCoords[0][i].x;
            _vec.y = _mesh->mTextureCoords[0][i].y;
            _vertex.TexCoords = _vec;
        }
        else
        {
            _vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        _vertices.push_back(_vertex);
    }
    // Process indices
    unsigned _numFaces = _mesh->mNumFaces;
    for (unsigned int i = 0; i < _numFaces; ++i)
    {
        aiFace _face = _mesh->mFaces[i];

        unsigned _numIndices = _face.mNumIndices;
        for (unsigned int j = 0; j < _numIndices; ++j)
        {
            _indices.push_back(_face.mIndices[j]);
        }
    }

    // Process material
    if (_mesh->mMaterialIndex >= 0)
    {
        aiMaterial* _material = _scene->mMaterials[_mesh->mMaterialIndex];

        // Diffuse
        std::vector<AS_Texture> _diffuseMaps = LoadMaterialTextures(_material,
                                                                    aiTextureType_DIFFUSE,
                                                                    "texture_diffuse");
        _textures.insert(_textures.end(), _diffuseMaps.begin(), _diffuseMaps.end());

        // Specular
        std::vector<AS_Texture> _specularMaps = LoadMaterialTextures(_material,
                                                                     aiTextureType_SPECULAR,
                                                                     "texture_specular");
        _textures.insert(_textures.end(), _specularMaps.begin(), _specularMaps.end());
    }
    const AS_Mesh _processedMesh(_vertices, _indices, _textures);
    processedMesh = _processedMesh;
}

unsigned AS_Model::TextureFromFile(const char* _path, const std::string& _directory, bool _gamma)
{
    std::string _fileName = std::string(_path);
    _fileName = _directory + '/' + _fileName;

    unsigned int _textureID;
    glGenTextures(1, &_textureID);

    int _width, _height, _nrComponents;
    unsigned char* _data = stbi_load(_fileName.c_str(), &_width, &_height, &_nrComponents, 0);

    if (_data)
    {
        GLenum _format;
        switch (_nrComponents)
        {
        case 1:
            _format = GL_RED;
            break;
        case 3:
            _format = GL_RGB;
            break;
        case 4:
            _format = GL_RGBA;
            break;
        default:
            break;
        }

        glBindTexture(GL_TEXTURE_2D, _textureID);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     _format,
                     _width,
                     _height,
                     0,
                     _format,
                     GL_UNSIGNED_BYTE, _data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(_data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << _path << std::endl;
        stbi_image_free(_data);
    }

    return _textureID;
}
