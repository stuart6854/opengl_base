#include "input.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "window.hpp"

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>

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

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

int run()
{
    std::cout << "Hello OpenGL!\n";

    Window::init(WIN_WIDTH, WIN_HEIGHT);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialise OpenGL context" << std::endl;
        return -1;
    }

    Input::init(Window::get());

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
    ImGui_ImplGlfw_InitForOpenGL(Window::get(), true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    /* Vertex Input */
    Mesh<int> triangleMesh;
    triangleMesh.setVertices(vertices, sizeof(vertices));
    triangleMesh.setIndices(indices, sizeof(indices));
    triangleMesh.apply(GL_TRIANGLES, sizeof(glm::vec3), { { 0, 3, GL_FLOAT, GL_FALSE, 0 } });

    /* Pipeline */
    Shader shader;
    shader.init(VERTEX_SRC, FRAGMENT_SRC);

    bool showDemo = false;
    bool showDebug = true;

    glClearColor(0.3912f, 0.5843f, 0.9294f, 1.0f); // Cornflower Blue

    double lastTime = glfwGetTime();
    while (!Window::shouldClose())
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

        shader.bind();
        triangleMesh.bind();
        triangleMesh.draw();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(Window::get());
    }

    /* Shutdown ImGui */
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}

int main(int, char**)
{
    return run();
}
