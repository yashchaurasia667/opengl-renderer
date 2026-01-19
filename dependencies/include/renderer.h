#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <vector>
#include <string>

#include <shader.h>
#include <model.h>

// void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
class Renderer
{
private:
  static float main_scale;
  static GLFWwindow *window;
  static std::vector<Model> models;

public:
  static ImGuiIO *io;
  static int width, height;
  static GLFWmousebuttonfun glfw_mouse_button_callback;
  static GLFWmousebuttonfun imgui_mouse_button_callback;

  Renderer(const char *title, int width, int height, const char *object_path, const char *glsl_version, bool vsync);
  ~Renderer();
  void start(void (*game_loop)(GLFWwindow *window, Shader &shader), Shader &shader);
  void addModel(std::string path, glm::vec3 position, glm::vec2 rotation, glm::vec3 scale);

  static GLFWwindow *getWindow();
  static void setCursorMode(unsigned int mode);
  static void setFrameBufferCallback(GLFWframebuffersizefun callback);
  static void setCursorPosCallback(GLFWcursorposfun callback);
  static void setMouseButtonCallback(GLFWmousebuttonfun callback);
  static void setScrollCallback(GLFWscrollfun callback);
  static void setErrorCallback(GLFWerrorfun callback);
};