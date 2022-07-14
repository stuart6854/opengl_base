#pragma once

#include <glad/glad.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#define CHECK_SHADER(id)                                                                                                                                       \
    do                                                                                                                                                         \
    {                                                                                                                                                          \
        int success;                                                                                                                                           \
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);                                                                                                        \
        if (!success)                                                                                                                                          \
        {                                                                                                                                                      \
            char infoLog[512];                                                                                                                                 \
            glGetShaderInfoLog(id, 512, nullptr, infoLog);                                                                                                     \
            std::cerr << "[OpenGL][Shader] " << infoLog << std::endl;                                                                                          \
        }                                                                                                                                                      \
    } while (false)

class Shader
{
public:
    ~Shader();

    void init(const char* vertexSrc, const char* fragmentSrc);

    void bind() const;

    void setFloat(const char* name, float value) const;
    void setFloat2(const char* name, const glm::vec2& value) const;
    void setFloat3(const char* name, const glm::vec3& value) const;
    void setFloat4(const char* name, const glm::vec4& value) const;
    void setMat4(const char* name, const glm::mat4& value) const;

private:
    GLuint m_program = 0;
};

inline Shader::~Shader()
{
    glDeleteProgram(m_program);
}

inline void Shader::init(const char* vertexSrc, const char* fragmentSrc)
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSrc, nullptr);
    glCompileShader(vertexShader);
    CHECK_SHADER(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSrc, nullptr);
    glCompileShader(fragmentShader);
    CHECK_SHADER(fragmentShader);

    m_program = glCreateProgram();
    glAttachShader(m_program, vertexShader);
    glAttachShader(m_program, fragmentShader);
    glLinkProgram(m_program);
    {
        int success;
        glGetProgramiv(m_program, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetProgramInfoLog(m_program, 512, nullptr, infoLog);
            std::cerr << "[OpenGL][Shader] " << infoLog << std::endl;
        }
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

inline void Shader::bind() const
{
    glUseProgram(m_program);
}

inline void Shader::setFloat(const char* name, const float value) const
{
    auto loc = glGetUniformLocation(m_program, name);
    glUniform1f(loc, value);
}

inline void Shader::setFloat2(const char* name, const glm::vec2& value) const
{
    auto loc = glGetUniformLocation(m_program, name);
    glUniform2f(loc, value.x, value.y);
}

inline void Shader::setFloat3(const char* name, const glm::vec3& value) const
{
    auto loc = glGetUniformLocation(m_program, name);
    glUniform3f(loc, value.x, value.y, value.z);
}

inline void Shader::setFloat4(const char* name, const glm::vec4& value) const
{
    auto loc = glGetUniformLocation(m_program, name);
    glUniform4f(loc, value.x, value.y, value.z, value.w);
}

inline void Shader::setMat4(const char* name, const glm::mat4& value) const
{
    auto loc = glGetUniformLocation(m_program, name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}
