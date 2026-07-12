#pragma once

#include <glad.h>
#include <stb_image.h>
#include <SDL3/SDL.h>
#include <assimp/types.h>

#include <string>

class Texture
{
  public:
    GLuint ID = 0;
    std::string type{};
    aiString path;

    GLenum target = GL_TEXTURE_2D;
    GLenum unit = GL_TEXTURE0;

    Texture(const char *filepath, GLenum texType, GLenum slot, GLenum pixelType) : target(texType), path(filepath), unit(slot)
    {
        glGenTextures(1, &ID);
        glActiveTexture(slot);
        glBindTexture(target, ID);

        glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_set_flip_vertically_on_load(true);

        int imgW, imgH, nrChannels;
        unsigned char *data = stbi_load(filepath, &imgW, &imgH, &nrChannels, 0);
        if (data)
        {
            GLenum fmt = (nrChannels == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(texType, 0, fmt, imgW, imgH, 0, fmt, pixelType, data);
            glGenerateMipmap(texType);
        }
        else
        {
            SDL_Log("Failed to load texture");
        }

        stbi_image_free(data);
    }

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
