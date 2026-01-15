#include <renderer.h>
#include <shader.h>
#include <camera.h>
#include <utils.h>

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION

void gameLoop();
void mouseCallback(GLFWwindow *window, double xposin, double yposin);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void framebufferSizeCallback(GLFWwindow *window, int width, int height);

bool cursor = false;
Camera camera(glm::vec3(0.0f), 45.0f, 0.1f, 2.5f);
GLFWwindow *window;

int main()
{
  Renderer ren("renderer window", 800, 600, "../resources/square.obj");
  ren.setCursorMode(GLFW_CURSOR_DISABLED);
  ren.setFrameBufferCallback(framebufferSizeCallback);
  ren.setCursorPosCallback(mouseCallback);
  ren.setScrollCallback(scrollCallback);
  window = ren.getWindow();

  Shader def("../shaders/cube.vs", "../shaders/cube.fs");
  ren.addModel("../resources/square.obj");
  ren.start(gameLoop, def);
  return 0;
}

void gameLoop()
{
  std::cout << "inside gameloop" << std::endl;
  camera.updateFrame();
  processInput(window);
  glCall(glClearColor(0.1f, 0.3f, 0.3f, 1.0f));
  glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
  {
    glfwSetInputMode(window, GLFW_CURSOR, cursor ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    cursor = !cursor;
  }
  camera.processMovement(window);
}

void mouseCallback(GLFWwindow *window, double xposin, double yposin)
{
  float x = static_cast<float>(xposin);
  float y = static_cast<float>(yposin);
  camera.updateView(x, y);
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
  float x = static_cast<float>(xoffset);
  float y = static_cast<float>(yoffset);
  camera.updateZoom(x, y);
}