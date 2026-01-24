#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <vector>
#include <string>

#include <shader.h>
#include <camera.h>
#include <model.h>

enum LightTypeList
{
  DIRECTIONAL,
  POINT,
  SPOT
};

struct LightType
{
  Model model;
  glm::vec3 color;
  float strength;
  LightTypeList type;
};

class Renderer
{
private:
  static float main_scale;
  static GLFWwindow *window;
  static std::vector<Model> models;
  static std::vector<LightType> lights;
  static ImGuiIO *io;
  static Shader lightShader;
  static void drawLights(glm::mat4 view, glm::mat4 projection, Shader &shader);

public:
  static int width, height;
  static float ambient, diffuse, specular;
  static GLFWmousebuttonfun glfw_mouse_button_callback;
  static GLFWkeyfun glfw_key_callback;

  Renderer(const char *title, int width, int height, const char *object_path, const char *glsl_version, bool vsync);
  ~Renderer();
  static void start(void (*game_loop)(GLFWwindow *window, Shader &shader), Shader &shader, Camera &camera);
  static void addModel(std::string path, glm::vec3 position, glm::vec2 rotation, glm::vec3 scale);
  static void addLight(glm::vec3 position, glm::vec3 color, float strength, LightTypeList type);

  static GLFWwindow *getWindow();
  static Shader &getLightShader();

  static void setMouseButtonCallback(GLFWmousebuttonfun callback);
  static void setKeyCallback(GLFWkeyfun callback);

  static void setCursorMode(unsigned int mode);
  static void setFrameBufferCallback(GLFWframebuffersizefun callback);
  static void setCursorPosCallback(GLFWcursorposfun callback);
  static void setScrollCallback(GLFWscrollfun callback);
  static void setErrorCallback(GLFWerrorfun callback);
};