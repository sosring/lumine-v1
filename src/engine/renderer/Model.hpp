#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Texture.hpp"
#include "Mesh.hpp"
#include <vector>
#include <string>

class Model
{
  public:
    Model(const char *path) { loadModel(path); }
    void Draw(Shader &shader) const;
    void DrawWithOutline(Shader &mainShader, Shader &outlineShader, const glm::mat4 &modelMatrix, float outlineScale = 1.02f) const;

  private:
    // Model data
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<std::shared_ptr<Texture>> textureCache;

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};
