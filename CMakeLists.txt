cmake_minimum_required(VERSION 3.29)
project(OpenGLPBR)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG latest)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(glm GIT_REPOSITORY https://github.com/g-truc/glm.git GIT_TAG 1.0.1)
FetchContent_MakeAvailable(glm)

add_executable(OpenGLPBR
        source/main.cpp
        src/glad.c
        source/shader.cpp
)

target_link_libraries(OpenGLPBR glfw ${CMAKE_DL_LIBS})
target_include_directories(OpenGLPBR PRIVATE include)
target_include_directories(OpenGLPBR PRIVATE include ${glm_SOURCE_DIR})
