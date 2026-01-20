#include "renderer.h"
#include <iostream>
#include <stdexcept>
#include <utils.h>

GLFWwindow *Renderer::window = nullptr;
std::vector<Model> Renderer::models;
int Renderer::width = 0;
int Renderer::height = 0;

float Renderer::main_scale = 0.0f;
ImGuiIO *Renderer::io = nullptr;
GLFWmousebuttonfun Renderer::glfw_mouse_button_callback = nullptr;

void GLFWMouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
  if (Renderer::glfw_mouse_button_callback)
    Renderer::glfw_mouse_button_callback(window, button, action, mods);
}

Renderer::Renderer(const char *title, int width, int height, const char *object_path, const char *glsl_version, bool vsync = false)
{
  if (window)
    throw std::runtime_error("window is already initialized");

  this->width = width;
  this->height = height;

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
  if (vsync)
    glfwSwapInterval(1);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    throw std::runtime_error("Failed to load OpenGL function pointers");

  glCall(glEnable(GL_DEPTH_TEST));
  glfwSetMouseButtonCallback(window, GLFWMouseButtonCallback);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  io = &ImGui::GetIO();
  io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

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

    // ADD MODEL WINDOW
    {
      static char path[128] = "../resources/donut/donut.obj";
      static float px = 0, py = 0, pz = 0;
      static float rx = 0, ry = 0;
      static float scale = 1.0;

      ImGui::Begin("OpenGL Renderer");
      ImGui::InputText("path", path, 128);

      ImGui::Text("Position");
      ImGui::InputFloat("px", &px);
      ImGui::InputFloat("py", &py);
      ImGui::InputFloat("pz", &pz);

      ImGui::Text("Rotation");
      ImGui::InputFloat("rx", &rx);
      ImGui::InputFloat("ry", &ry);

      ImGui::InputFloat("Scale", &scale);

      if (ImGui::Button("Add Model") && strncmp(path, "\0", 1) != 0)
      {
        std::cout << "Loading model" << std::endl;
        models.push_back(Model(path, glm::vec3(px, py, pz), glm::vec2(rx, ry), glm::vec3(scale), false));
        // models.push_back(Model(path, glm::vec3(0.0f), glm::vec2(1.0f), glm::vec3(1.0f), false));
      }

      ImGui::End();
    }
    // LOADED MODELS WINDOW
    {
      ImGui::Begin("Loaded Models");
      for (unsigned int i = 0; i < models.size(); i++)
      {
        if (ImGui::CollapsingHeader(std::to_string(i).c_str()))
        {

          ImGui::Text("Position");
          ImGui::Text(std::to_string(models[i].position.x).c_str());
          ImGui::Text(std::to_string(models[i].position.y).c_str());
          ImGui::Text(std::to_string(models[i].position.z).c_str());

          ImGui::Text("Rotation");
          ImGui::Text(std::to_string(models[i].rotation.x).c_str());
          ImGui::Text(std::to_string(models[i].rotation.y).c_str());

          ImGui::Text(std::to_string(models[i].scale.x).c_str());
        }
      }
      ImGui::End();
    }

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
  // glfwSetMouseButtonCallback(window, callback);
  glfw_mouse_button_callback = callback;
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