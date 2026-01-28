cmake_minimum_required(VERSION 3.21)
project(renderer LANGUAGES C CXX)

find_package(OpenGL REQUIRED)

set(INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/include")
set(LIB     "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lib")
set(SRC     "${CMAKE_CURRENT_SOURCE_DIR}/src")

# ImGui sources
file(GLOB IMGUI_SOURCES "${LIB}/imgui/*.cpp")
file(GLOB IMGUI_BACKEND_SOURCES "${LIB}/imgui/backends/*.cpp")

# ---- Library with actual compiled code ----
add_library(depends STATIC
    "${LIB}/glad.c"
    "${LIB}/shader.cpp"
    "${LIB}/vertexBuffer.cpp"
    "${LIB}/vertexBufferLayout.cpp"
    "${LIB}/indexBuffer.cpp"
    "${LIB}/vertexArray.cpp"
    "${LIB}/camera.cpp"
    "${LIB}/texture.cpp"
    "${LIB}/utils.cpp"
    "${LIB}/mesh.cpp"
    "${LIB}/model.cpp"
    "${LIB}/renderer.cpp"
    ${IMGUI_SOURCES}
    ${IMGUI_BACKEND_SOURCES}
)

target_include_directories(depends
    PUBLIC
        "${INCLUDE}"
        "${INCLUDE}/imgui"
        "${INCLUDE}/imgui/backends"
)

target_compile_options(depends
    PRIVATE
        -Wall
        -Wextra
        -g
)

target_link_libraries(depends
    PUBLIC
        OpenGL::GL
        "${LIB}/libglfw3.a"
        "${LIB}/libassimp.a"
        "${LIB}/libzlibstatic.a"
        kernel32
        user32
        gdi32
)

# ---- Executable ----
add_executable(renderer
    "${SRC}/main.cpp"
)

target_link_libraries(renderer
    PRIVATE
        depends
)
