#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

#include <shader.h>
#include <model.h>

typedef void (*frameBufferSizeCallbackFun)(GLFWwindow *window, int width, int height);
typedef void (*mouseCallbackFun)(GLFWwindow *window, double xpos, double ypos);
typedef void (*keyboardCallbackFun)(GLFWwindow *window);

class Renderer
{
private:
  static GLFWwindow *window;
  static std::vector<Model> models;

public:
  Renderer(const char *title, int width, int height, const char *object_path);
  ~Renderer();
  void start(void (*game_loop)(GLFWwindow *window, Shader &shader), Shader &shader);

  static GLFWwindow *getWindow();
  static void setCursorMode(unsigned int mode);
  static void setFrameBufferCallback(frameBufferSizeCallbackFun callback);
  static void setCursorPosCallback(mouseCallbackFun callback);
  static void setScrollCallback(mouseCallbackFun callback);
  void addModel(std::string path);
};