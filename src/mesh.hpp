#pragma once

#include <glad/glad.h>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>

#include <vector>

struct Vertex
{
    glm::vec2 pos = {};
    glm::vec2 texCoord = {};
    glm::vec3 color = { 1, 0, 0 };
};

struct Attrib
{
    GLuint index = 0;
    GLint size = 0;
    GLenum type = 0;
    GLboolean normalised = GL_FALSE;
    GLuint offset = 0;
};

template <typename Index>
class Mesh
{
public:
    ~Mesh();

    void setVertices(const void* data, std::size_t size);
    void setIndices(const void* data, std::size_t count);
    void apply(GLenum topology, const std::size_t stride, const std::vector<Attrib>& attribs);

    void bind() const;
    void draw() const;

private:
    GLuint m_vao = 0;
    GLuint m_vbo = 0, m_ebo = 0;
    GLsizei m_indexCount = 0;

    GLenum m_topology = 0;
};

template <typename Index>
Mesh<Index>::~Mesh()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
}

template <typename Index>
void Mesh<Index>::setVertices(const void* data, const std::size_t size)
{
    if (m_vbo == 0)
    {
        glCreateBuffers(1, &m_vbo);
    }

    glNamedBufferData(m_vbo, size, data, GL_STATIC_DRAW);
}

template <typename Index>
void Mesh<Index>::setIndices(const void* data, const std::size_t count)
{
    if (m_ebo == 0)
    {
        glCreateBuffers(1, &m_ebo);
    }

    glNamedBufferData(m_ebo, count * sizeof(Index), data, GL_STATIC_DRAW);

    m_indexCount = count;
}

template <typename Index>
void Mesh<Index>::apply(const GLenum topology, const std::size_t stride, const std::vector<Attrib>& attribs)
{
    m_topology = topology;

    if (m_vao == 0)
    {
        glCreateVertexArrays(1, &m_vao);
    }

    // Assign buffers to Vertex Buffer Object
    glVertexArrayElementBuffer(m_vao, m_ebo);
    glVertexArrayVertexBuffer(m_vao, 0, m_vbo, 0, stride);

    // Setup attributes
    for (const auto& attrib : attribs)
    {
        glEnableVertexArrayAttrib(m_vao, attrib.index);
        glVertexArrayAttribBinding(m_vao, attrib.index, 0);
        glVertexArrayAttribFormat(m_vao, attrib.index, attrib.size, attrib.type, attrib.normalised, attrib.offset);
    }
}

template <typename Index>
void Mesh<Index>::bind() const
{
    glBindVertexArray(m_vao);
}

template <typename Index>
void Mesh<Index>::draw() const
{
    glDrawElements(m_topology, m_indexCount, GL_UNSIGNED_INT, nullptr);
}
