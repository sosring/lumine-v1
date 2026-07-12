#pragma once

#include <glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

class Shader
{
  public:
    GLuint ID;

    Shader(const char *vertFile, const char *fragFile);

    ~Shader() { glDeleteProgram(ID); }

    Shader(const Shader &) = delete;
    Shader &operator=(const Shader &) = delete;

    void Use() { glUseProgram(ID); }

    void setInt(const char *uniform, GLuint unit) { glUniform1i(glGetUniformLocation(ID, uniform), unit); }

    void setVec3(const char *name, const glm::vec3 &v) { glUniform3fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(v)); }

    void setMat4(const char *name, const glm::mat4 mat) { glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(mat)); }

  private:
    std::string loadShaderFile(const char *path);

    void checkCompileErrors(GLuint shader, const char *type);
};
