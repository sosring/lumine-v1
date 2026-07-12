#pragma once
#include <glad.h>

class VertexBuffer
{
  public:
    GLuint ID;

    VertexBuffer(const void *vertices, GLsizeiptr size)
    {
        glGenBuffers(1, &ID);
        glBindBuffer(GL_ARRAY_BUFFER, ID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    ~VertexBuffer() { glDeleteBuffers(1, &ID); };

    VertexBuffer(const VertexBuffer &) = delete;
    VertexBuffer &operator=(const VertexBuffer &) = delete;

    void Bind() { glBindBuffer(GL_ARRAY_BUFFER, ID); };

    void Unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); };
};
