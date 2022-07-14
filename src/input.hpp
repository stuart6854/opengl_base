#pragma once

#define GLFW_INCLUDE_NONE
#include <glfw/glfw3.h>

#include <glm/ext/vector_float2.hpp>

#include <array>

class Input
{
public:
    static void init(GLFWwindow* glfwWindow);

    static void newFrame();

    static bool isKeyDown(int key);
    static bool isKeyUp(int key);
    static bool isKeyHeld(int key);

    static bool isMouseBtnDown(int btn);
    static bool isMouseBtnUp(int btn);
    static bool isMouseBtnHeld(int btn);

    static auto getCursorPos() -> const glm::vec2&;

private:
    struct State
    {
        std::array<bool, GLFW_KEY_LAST> keys;
        std::array<bool, GLFW_MOUSE_BUTTON_LAST> mouseBtns;
        glm::vec2 cursorPos = {};
    };

    inline static State m_currentState;
    inline static State m_lastState;
};

inline void Input::init(GLFWwindow* glfwWindow)
{
    glfwSetKeyCallback(glfwWindow,
                       [](GLFWwindow* window, const int key, int, const int action, int) { Input::m_currentState.keys[key] = action != GLFW_RELEASE; });

    glfwSetMouseButtonCallback(glfwWindow,
                               [](GLFWwindow* window, const int btn, const int action, int) { Input::m_currentState.mouseBtns[btn] = action != GLFW_RELEASE; });

    glfwSetCursorPosCallback(glfwWindow, [](GLFWwindow* window, double x, double y) { Input::m_currentState.cursorPos = { x, y }; });
}

inline void Input::newFrame()
{
    glfwPollEvents();

    m_lastState = m_currentState;
}

inline bool Input::isKeyDown(const int key)
{
    return m_currentState.keys[key] && !m_lastState.keys[key];
}

inline bool Input::isKeyUp(const int key)
{
    return !m_currentState.keys[key] && m_lastState.keys[key];
}

inline bool Input::isKeyHeld(const int key)
{
    return m_currentState.keys[key] && m_lastState.keys[key];
}

inline bool Input::isMouseBtnDown(const int btn)
{
    return m_currentState.mouseBtns[btn] && !m_lastState.mouseBtns[btn];
}

inline bool Input::isMouseBtnUp(const int btn)
{
    return !m_currentState.mouseBtns[btn] && m_lastState.mouseBtns[btn];
}

inline bool Input::isMouseBtnHeld(const int btn)
{
    return m_currentState.mouseBtns[btn] && m_lastState.mouseBtns[btn];
}

inline auto Input::getCursorPos() -> const glm::vec2&
{
    return m_currentState.cursorPos;
}
