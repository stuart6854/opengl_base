#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

float vertices[] = 
{
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.5f,  0.5f, 0.0f,
};

int indices[] =
{
    0, 1, 2
};

const auto* VERTEX_SRC = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";

const auto* FRAGMENT_SRC = R"(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
} 
)";

int main(int, char**) 
{
    std::cout << "Hello OpenGL!\n";

    glfwInit();

    auto* window = glfwCreateWindow(1280, 720, "OpenGL Window", nullptr, nullptr);
    if(window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) 
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    /* Vertex Input */
    GLuint vao;
    GLuint vbo, ebo;
    {
        glCreateVertexArrays(1, &vao);

        // Vertex buffer
        glCreateBuffers(1, &vbo);
        glNamedBufferData(vbo, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // Element buffer
        glCreateBuffers(1, &ebo);
        glNamedBufferData(ebo, sizeof(indices), indices, GL_STATIC_DRAW);

        // Assign buffers to Vertex Buffer Object
        glVertexArrayElementBuffer(vao, ebo);
        glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(float) * 3);

        // Setup attributes
        glEnableVertexArrayAttrib(vao, 0);
        glVertexArrayAttribBinding(vao, 0, 0);
        glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    }

    /* Pipeline */
    GLuint program;
    {
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &VERTEX_SRC, nullptr);
        glCompileShader(vertexShader);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &FRAGMENT_SRC, nullptr);
        glCompileShader(fragmentShader);

        program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);  

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    glClearColor(0.3912f, 0.5843f, 0.9294f, 1.0f);  // Cornflower Blue
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glBindVertexArray(vbo);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteProgram(program);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
