cmake_minimum_required(VERSION 4.1.2)
project(renderer)

find_package(OpenGL REQUIRED)

set(INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/include")
set(LIB "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lib")
set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_library(depends INTERFACE)
target_include_directories(depends INTERFACE "${INCLUDE}")
target_sources(depends INTERFACE "${LIB}/glad.c" "${LIB}/shader.cpp" "${LIB}/vertexBuffer.cpp" "${LIB}/vertexBufferLayout.cpp" "${LIB}/indexBuffer.cpp" "${LIB}/vertexArray.cpp")

add_executable(${PROJECT_NAME} "${SRC}/main.cpp")
target_link_libraries(depends INTERFACE "${LIB}/libglfw3.a" "${LIB}/libassimp.dll.a" "${LIB}/libzlibstatic.a" OpenGL::GL kernel32 user32 gdi32)
target_link_libraries(${PROJECT_NAME} PRIVATE depends)