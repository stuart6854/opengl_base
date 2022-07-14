cmake_minimum_required(VERSION 3.0.0)
project(OpenGL_Base VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(OpenGL_Base 
    "libs/glad/src/glad.c"
    "src/main.cpp"
    "src/imgui/imgui_impl_glfw.cpp" 
    "src/imgui/imgui_impl_opengl3.cpp" 
)

# Link Glad
target_include_directories(OpenGL_Base PUBLIC libs/glad/include)

# Link GLFW
target_link_directories(OpenGL_Base PUBLIC libs/glfw/lib)
target_link_libraries(OpenGL_Base PUBLIC glfw3)
target_include_directories(OpenGL_Base PUBLIC libs/glfw/include)
target_compile_definitions(OpenGL_Base PUBLIC "GLFW_INCLUDE_NONE")

# Include GLM
target_include_directories(OpenGL_Base PUBLIC libs/glm/include)

# Link IMGUI
add_subdirectory(libs/imgui)
target_link_libraries(OpenGL_Base PUBLIC imgui)