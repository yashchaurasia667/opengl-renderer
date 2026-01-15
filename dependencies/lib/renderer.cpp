#include "renderer.h"
#include <iostream>
#include <stdexcept>
#include <utils.h>

GLFWwindow *Renderer::window = nullptr;
std::vector<Model> Renderer::models;

Renderer::Renderer(const char *title, int width, int height, const char *object_path)
{
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
}

Renderer::~Renderer()
{
  if (window)
  {
    glfwDestroyWindow(window);
    window = nullptr;
  }
  glfwTerminate();
}

void Renderer::start(void (*game_loop)())
{
  while (!glfwWindowShouldClose(window))
  {
    if (game_loop)
      game_loop();

    glfwPollEvents();
    glfwSwapBuffers(window);
  }
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

void Renderer::setFrameBufferCallback(frameBufferSizeCallbackFun callback)
{
  if (window)
    glfwSetFramebufferSizeCallback(window, callback);
}

void Renderer::setCursorPosCallback(mouseCallbackFun callback)
{
  if (window)
    glfwSetCursorPosCallback(window, callback);
}

void Renderer::setScrollCallback(mouseCallbackFun callback)
{
  if (window)
    glfwSetScrollCallback(window, callback);
}
