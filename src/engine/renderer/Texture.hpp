#pragma once

#include <glad.h>
#include <stb_image.h>
#include <SDL3/SDL.h>
#include <assimp/types.h>
#include <assimp/texture.h>

#include <string>

class Texture
{
  public:
    GLuint ID = 0;
    std::string type{};
    aiString path;

    GLenum target = GL_TEXTURE_2D;
    GLenum unit = GL_TEXTURE0;

    Texture(const char *filepath, GLenum texType, GLenum slot, GLenum pixelType);
    ~Texture() { glDeleteTextures(1, &ID); }

    // Copy constructor & assignment
    Texture(Texture &other) = delete;
    Texture &operator=(const Texture &) = delete;

    // Move constructor & assignment
    Texture(Texture &&other) noexcept : ID(other.ID), target(other.target), unit(other.unit), type(std::move(other.type)), path(other.path)
    {
        other.ID = 0;
    }

    Texture &operator=(Texture &&other) noexcept
    {
        if (this != &other)
        {
            if (ID != 0)
                glDeleteTextures(1, &ID);

            ID = other.ID;
            target = other.target;
            unit = other.unit;
            type = std::move(other.type);
            path = other.path;

            other.ID = 0;
        }

        return *this;
    }

    void Bind()
    {
        glActiveTexture(unit);
        glBindTexture(target, ID);
    }

    void Unbind() { glBindTexture(target, 0); }
};
