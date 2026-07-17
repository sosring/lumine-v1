#include "Texture.hpp"

Texture::Texture(const char *filepath, GLenum texType, GLenum slot, GLenum pixelType) : target(texType), path(filepath), unit(slot)
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
