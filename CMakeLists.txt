# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.31)

# Project name
project(OpenGLProject)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm CONFIG REQUIRED)

include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})

file(GLOB SOURCES "src/*.cpp" "src/external/*.cpp")

add_executable(OpenGLApp ${SOURCES})

target_link_libraries(OpenGLApp PRIVATE OpenGL::GL glfw glm::glm-header-only)