#pragma once

#include <glfw/glfw3.h>

void glfwErrorCallback(const int error, const char* msg)
{
    std::cerr << "[GLFW] [" << error << "] " << msg << std::endl;
}

class Window
{
public:
    static void init(int width, int height);
    static void shutdown();

    static auto width() -> int;
    static auto height() -> int;
    static auto aspect() -> float;

    static bool shouldClose();

    static auto get() -> GLFWwindow*;

private:
    inline static GLFWwindow* m_window;

    inline static int m_width, m_height;
};

inline void Window::init(const int width, const int height)
{
    glfwSetErrorCallback(&glfwErrorCallback);

    if (!glfwInit())
    {
        std::cerr << "Failed to initialise GLFW" << std::endl;
        return;
    }

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    m_window = glfwCreateWindow(width, height, "OpenGL Window", nullptr, nullptr);
    if (m_window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_window);

    m_width = width;
    m_height = height;
}

inline void Window::shutdown()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

inline auto Window::width() -> int
{
    return m_width;
}

inline auto Window::height() -> int
{
    return m_height;
}

inline auto Window::aspect() -> float
{
    return static_cast<float>(m_width) / static_cast<float>(m_height);
}

inline bool Window::shouldClose()
{
    return glfwWindowShouldClose(m_window);
}

inline auto Window::get() -> GLFWwindow*
{
    return m_window;
}
