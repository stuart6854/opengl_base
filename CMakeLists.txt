cmake_minimum_required(VERSION 3.0.0)
project(OpenGL_Base VERSION 0.1.0)

add_executable(OpenGL_Base 
    libs/glad/src/glad.c
    src/main.cpp
)

# Link Glad
target_include_directories(OpenGL_Base PUBLIC libs/glad/include)

# Link GLFW
target_link_directories(OpenGL_Base PUBLIC libs/glfw/lib)
target_link_libraries(OpenGL_Base PUBLIC glfw3)
target_include_directories(OpenGL_Base PUBLIC libs/glfw/include)

# Include GLM
target_include_directories(OpenGL_Base PUBLIC libs/glm/include)