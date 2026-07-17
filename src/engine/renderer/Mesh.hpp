#pragma once

#include <cstddef>
#include <glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Texture.hpp"
#include "Shader.hpp"

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh
{
  public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<std::shared_ptr<Texture>> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture>> textures);
    // ~Mesh() = default;

    void Draw(Shader &shader) const;

  private:
    std::unique_ptr<VertexArray> vao;
    std::unique_ptr<VertexBuffer> vbo;
    std::unique_ptr<IndexBuffer> ebo;

    void setupMesh();
};
