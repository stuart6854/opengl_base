#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>

void glfwErrorCallback(int error, const char* msg)
{
    std::cerr << "[GLFW] [" << error << "] " << msg << std::endl;
}

void openglDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    if (id == 131185)
        return;

    std::cout << "[OpenGL] type=" << type << ", severity=" << severity << ", msg=" << message << std::endl;
}

glm::vec3 vertices[] = {
    { -0.5f, -0.5f, 0.0f },
    { 0.5f, -0.5f, 0.0f },
    { 0.5f, 0.5f, 0.0f },
};

int indices[] = { 0, 1, 2 };

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

    glfwSetErrorCallback(&glfwErrorCallback);

    if (!glfwInit())
    {
        std::cerr << "Failed to initialise GLFW" << std::endl;
        return -1;
    }

    auto* window = glfwCreateWindow(1280, 720, "OpenGL Window", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }
    
    glViewport(0, 0, 1280, 720);

    // OpenGL Debug Callback
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(&openglDebugCallback, nullptr);

    /* Init ImGui */
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

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

    bool showDemo = false;
    bool showDebug = true;

    glClearColor(0.3912f, 0.5843f, 0.9294f, 1.0f); // Cornflower Blue

    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        auto time = glfwGetTime();
        float deltaTime = time - lastTime;
        lastTime = time;

        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Insert Update code here...
        // Insert ImGui code here...

        ImGui::ShowDemoWindow(&showDemo);

        ImGui::Begin("Debug", &showDebug);
        ImGui::Text("DeltaTime: %fs", deltaTime);
        ImGui::Separator();
        ImGui::End();

        ImGui::Render();

        glClear(GL_COLOR_BUFFER_BIT);

        // Insert Rendering code here...

        glUseProgram(program);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteProgram(program);

    /* Shutdown ImGui */
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
