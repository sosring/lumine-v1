#pragma once
#include <glad.h>
#include <stb_image.h>

#include <iostream>

class Texture
{
  public:
    GLuint ID;
    GLenum type;

    Texture(const char *image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
    {
        type = texType;

        glGenTextures(1, &ID);
        glActiveTexture(slot);
        glBindTexture(type, ID);

        // glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
        // glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_set_flip_vertically_on_load(true);

        int imgW, imgH, nrChannels;
        unsigned char *data = stbi_load(image, &imgW, &imgH, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(texType, 0, format, imgW, imgH, 0, format, pixelType, data);
            glGenerateMipmap(texType);
        }
        else
        {
            std::cout << "Failed to load texture" << "\n";
        }

        stbi_image_free(data);
    }

    ~Texture() { glDeleteTextures(1, &ID); }

    Texture(const Texture &) = delete;
    Texture &operator=(const Texture &) = delete;

    void Bind() { glBindTexture(type, ID); }

    void Unbind() { glBindTexture(type, 0); }
};
