#pragma once

#include <glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <string>

class Shader
{
  public:
    GLuint ID;

    Shader(const char *vertFile, const char *fragFile);

    ~Shader() { glDeleteProgram(ID); }

    Shader(const Shader &) = delete;
    Shader &operator=(const Shader &) = delete;

    void Use() { glUseProgram(ID); }

    void setInt(const char *uniform, GLuint v) { glUniform1i(glGetUniformLocation(ID, uniform), v); }

    void setFloat(const char *uniform, GLfloat v) { glUniform1f(glGetUniformLocation(ID, uniform), v); }

    void setBool(const char *uniform, bool v) { glUniform1i(glGetUniformLocation(ID, uniform), (int)v); }

    void setVec3(const char *name, const glm::vec3 &v) { glUniform3fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(v)); }

    void setVec4(const char *name, const glm::vec4 &v) { glUniform4fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(v)); }

    void setMat4(const char *name, const glm::mat4 mat) { glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(mat)); }

  private:
    std::string loadShaderFile(const char *path);

    void checkCompileErrors(GLuint shader, const char *type, const char *filename);
};
