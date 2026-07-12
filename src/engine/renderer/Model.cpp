#include "Model.hpp"
#include "engine/renderer/Texture.hpp"

#include <iterator>
#include <memory>

void Model::Draw(Shader &shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader);
}

void Model::loadModel(std::string path)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << "\n";
        return;
    };

    directory = path.substr(0, path.find_last_of("/"));
    processNode(scene->mRootNode, scene);
}

// Recursive Function
void Model::processNode(aiNode *node, const aiScene *scene)
{
    // Process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    // Then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
};

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::shared_ptr<Texture>> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        // process vertex positions, normals and texture coordinates
        glm::vec3 tempVec;

        tempVec.x = mesh->mVertices[i].x;
        tempVec.y = mesh->mVertices[i].y;
        tempVec.z = mesh->mVertices[i].z;
        vertex.Position = tempVec;

        tempVec.x = mesh->mNormals[i].x;
        tempVec.y = mesh->mNormals[i].y;
        tempVec.z = mesh->mNormals[i].z;
        vertex.Normal = tempVec;

        if (mesh->mTextureCoords[0]) // checks if mesh contain texture coordinates
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    // process Indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // process material
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        // Using make_move_iterator as texture is not copyable
        std::vector<std::shared_ptr<Texture>> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), std::make_move_iterator(diffuseMaps.begin()), std::make_move_iterator(diffuseMaps.end()));

        std::vector<std::shared_ptr<Texture>> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), std::make_move_iterator(specularMaps.begin()), std::make_move_iterator(specularMaps.end()));
    }

    return Mesh(vertices, indices, textures);
}

std::vector<std::shared_ptr<Texture>> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<std::shared_ptr<Texture>> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;

        for (unsigned int j = 0; j < textureCache.size(); j++)
        {
            if (std::strcmp(textureCache[j]->path.C_Str(), str.C_Str()) == 0)
            {
                textures.push_back(textureCache[j]);
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            std::string filename = directory + "/" + str.C_Str();

            // Shared ptr
            auto tex = std::make_shared<Texture>(filename.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_UNSIGNED_BYTE);
            tex->type = typeName;
            tex->path = str;

            textureCache.push_back(tex);
            textures.push_back(tex);
        }
    }

    return textures;
};
