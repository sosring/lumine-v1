#include "Mesh.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture>> textures)
{
    this->vertices = std::move(vertices);
    this->indices = std::move(indices);
    this->textures = textures;

    setupMesh();
};

void Mesh::Draw(Shader &shader)
{
    unsigned int diffuseNr = 0;
    unsigned int specularNr = 0;

    for (unsigned int i = 0; i < textures.size(); i++)
    {
        std::string uniformName;
        if (textures[i]->type == "texture_diffuse")
            uniformName = "texture_diffuse" + std::to_string(diffuseNr++);
        else if (textures[i]->type == "texture_specular")
            uniformName = "texture_specular" + std::to_string(specularNr++);
        else
            continue;

        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]->ID);
        shader.setInt(uniformName.c_str(), i);
    }

    vao->Bind();
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
};

void Mesh::setupMesh()
{
    vao = std::make_unique<VertexArray>();
    vbo = std::make_unique<VertexBuffer>(vertices.data(), vertices.size() * sizeof(Vertex));
    ebo = std::make_unique<IndexBuffer>(indices.data(), indices.size() * sizeof(GLuint));

    vao->LinkVertexBuffer(*vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)0);
    vao->LinkVertexBuffer(*vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void *)offsetof(Vertex, Normal));
    vao->LinkVertexBuffer(*vbo, 2, 2, GL_FLOAT, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));
};
