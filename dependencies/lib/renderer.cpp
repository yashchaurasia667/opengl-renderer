#include "renderer.h"
#include <iostream>
#include <stdexcept>
#include <utils.h>

GLFWwindow *Renderer::window = nullptr;
std::vector<Model> Renderer::models;
float Renderer::main_scale = 0.0f;
int Renderer::width = 0;
int Renderer::height = 0;
ImGuiIO *io = nullptr;

Renderer::Renderer(const char *title, int width, int height, const char *object_path, const char *glsl_version)
{
  Renderer::width = width;
  Renderer::height = height;

  if (window)
    throw std::runtime_error("window is already initialized");
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifndef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  window = glfwCreateWindow(width, height, title, nullptr, nullptr);
  if (window == NULL)
    throw std::runtime_error("Failed to create a GLFW window");
  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    throw std::runtime_error("Failed to load OpenGL function pointers");

  glCall(glEnable(GL_DEPTH_TEST));

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
}

Renderer::~Renderer()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  if (window)
  {
    glfwDestroyWindow(window);
    window = nullptr;
  }
  glfwTerminate();
}

void Renderer::start(void (*game_loop)(GLFWwindow *window, Shader &shader), Shader &shader)
{
  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow(); // Show demo window! :)

    if (game_loop && window)
      game_loop(window, shader);

    for (unsigned int i = 0; i < models.size(); i++)
      models[i].draw(shader);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
  }
}

void Renderer::addModel(std::string path, glm::vec3 position, glm::vec2 rotation, glm::vec3 scale)
{
  models.push_back(Model(path, position, rotation, scale, false));
}

GLFWwindow *Renderer::getWindow()
{
  return window;
}

glm::vec2 Renderer::getWindowSize()
{
  return glm::vec2();
}

void Renderer::setCursorMode(unsigned int mode)
{
  if (window)
    glfwSetInputMode(window, GLFW_CURSOR, mode);
}

void Renderer::setFrameBufferCallback(GLFWframebuffersizefun callback)
{
  if (window)
    glfwSetFramebufferSizeCallback(window, callback);
}

void Renderer::setCursorPosCallback(GLFWcursorposfun callback)
{
  if (window)
    glfwSetCursorPosCallback(window, callback);
}

void Renderer::setMouseButtonCallback(GLFWmousebuttonfun callback)
{
  glfwSetMouseButtonCallback(window, callback);
}

void Renderer::setScrollCallback(GLFWscrollfun callback)
{
  if (window)
    glfwSetScrollCallback(window, callback);
}

void Renderer::setErrorCallback(GLFWerrorfun callback)
{
  if (window)
    glfwSetErrorCallback(callback);
}
