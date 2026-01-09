#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include <shader.h>
#include <camera.h>
#include <vertexBuffer.h>
#include <indexBuffer.h>
#include <vertexBufferLayout.h>
#include <vertexArray.h>
#include <utils.h>

const unsigned int SCR_WIDTH = 800, SCR_HEIGHT = 600;
Camera camera(glm::vec3(0.0f), 45.0f, 0.1f, 2.5f);

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouseCallback(GLFWwindow *window, double xposin, double yposin);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifndef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Renderer", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create a GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to load OpenGL function pointers" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouseCallback);
  glfwSetScrollCallback(window, scrollCallback);

  glCall(glEnable(GL_DEPTH_TEST));
  glCall(glClearColor(0.5f, 0.3f, 0.2f, 1.0f));

  {
    float vertices[] = {
        -0.5f, -0.5f, -0.5,
        0.5f, -0.5f, -0.5,
        0.5f, 0.5f, -0.5,
        -0.5f, 0.5f, -0.5};

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3};

    Shader def("../shaders/cube.vs", "../shaders/default.fs");
    VertexArray vao;
    VertexBuffer vbo(4 * 3, vertices, GL_STATIC_DRAW);
    IndexBuffer ibo(3 * 2, indices, GL_STATIC_DRAW);
    VertexBufferLayout layout;

    layout.push<float>(3);
    vao.addBuffer(vbo, layout);

    while (!glfwWindowShouldClose(window))
    {
      camera.updateFrame();
      processInput(window);
      glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

      def.bind();
      glm::mat4 model = glm::mat4(1.0f);
      glm::mat4 view = camera.getViewMatrix();
      glm::mat4 projection = glm::perspective(camera.getFov(), 16.0f / 9.0f, 0.1f, 100.0f);

      def.setMat4("model", model);
      def.setMat4("view", view);
      def.setMat4("projection", projection);

      glCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
      // glCall(glDrawArrays(GL_TRIANGLES, 0, 3));

      glfwPollEvents();
      glfwSwapBuffers(window);
    }
  }

  glfwTerminate();
  return 0;
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
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