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

    Shader(const char *vertFile, const char *fragFile)
    {
        std::string vertCode = loadShaderFile(vertFile);
        std::string fragCode = loadShaderFile(fragFile);

        const char *vertSource = vertCode.c_str();
        const char *fragSource = fragCode.c_str();

        GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vshader, 1, &vertSource, nullptr);
        glCompileShader(vshader);
        checkCompileErrors(vshader, "VERTEX");

        GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fshader, 1, &fragSource, nullptr);
        glCompileShader(fshader);
        checkCompileErrors(fshader, "FRAGMENT");

        ID = glCreateProgram();

        glAttachShader(ID, vshader);
        glAttachShader(ID, fshader);
        glLinkProgram(ID);
        glValidateProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        glDeleteShader(vshader);
        glDeleteShader(fshader);
    };

    ~Shader() { glDeleteProgram(ID); }

    Shader(const Shader &) = delete;
    Shader &operator=(const Shader &) = delete;

    void Use() { glUseProgram(ID); }

    void setInt(const char *uniform, GLuint unit) { glUniform1i(glGetUniformLocation(ID, uniform), unit); }

    void setMat4(const char *name, const glm::mat4 mat) { glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(mat)); }

  private:
    std::string loadShaderFile(const char *path)
    {
        std::ifstream file;
        std::string content;

        file.open(path);
        if (!file.is_open())
        {
            std::cerr << "ERROR: Could not open shader file: " << path << std::endl;
            return "";
        }

        std::stringstream stream;
        stream << file.rdbuf();
        content = stream.str();
        file.close();

        return content;
    }

    void checkCompileErrors(GLuint shader, const char *type)
    {
        GLint success;
        char infoLog[1024];
        if (std::string(type) != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                std::cerr << "SHADER_COMPILE_ERROR (" << type << "):\n" << infoLog << "\n";
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                std::cerr << "PROGRAM_LINK_ERROR:\n" << infoLog << "\n";
            }
        }
    }
};
