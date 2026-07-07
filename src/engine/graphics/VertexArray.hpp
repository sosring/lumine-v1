#pragma once
#include <glad.h>
#include "VertexBuffer.hpp"

class VertexArray
{
  public:
    GLuint ID;

    VertexArray()
    {
        glGenVertexArrays(1, &ID);
        glBindVertexArray(ID);
    }

    ~VertexArray() { glDeleteVertexArrays(1, &ID); }

    VertexArray(const VertexArray &) = delete;
    VertexArray &operator=(const VertexArray &) = delete;

    void LinkVertexBuffer(VertexBuffer &VertexBuffer, GLuint layout, GLuint size, GLenum type, GLsizeiptr stripe, void *offset)
    {
        VertexBuffer.Bind();
        glVertexAttribPointer(layout, size, type, GL_FALSE, stripe, offset);
        glEnableVertexAttribArray(layout);
        VertexBuffer.Unbind();
    }

    void Bind() { glBindVertexArray(ID); };

    void Unbind() { glBindVertexArray(0); };
};
