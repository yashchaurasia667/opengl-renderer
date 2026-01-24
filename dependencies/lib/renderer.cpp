#include "renderer.h"
#include <iostream>
#include <stdexcept>
#include <utils.h>

GLFWwindow *Renderer::window = nullptr;
std::vector<Model> Renderer::models;
int Renderer::width = 0;
int Renderer::height = 0;
float Renderer::ambient = 0.3f, Renderer::diffuse = 1.0f, Renderer::specular = 1.0f;

std::vector<LightType> Renderer::lights;
Shader Renderer::lightShader;

float Renderer::main_scale = 0.0f;
ImGuiIO *Renderer::io = nullptr;

GLFWmousebuttonfun Renderer::glfw_mouse_button_callback = nullptr;
GLFWkeyfun Renderer::glfw_key_callback = nullptr;

void GLFWMouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
  ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
  if (Renderer::glfw_mouse_button_callback && !ImGui::GetIO().WantCaptureMouse)
    Renderer::glfw_mouse_button_callback(window, button, action, mods);
}

void GLFWKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

  // Only process game input if ImGui isn't capturing keyboard
  if (ImGui::GetIO().WantCaptureKeyboard)
    return;
  Renderer::glfw_key_callback(window, key, scancode, action, mods);
}

void GLFWCharCallback(GLFWwindow *window, unsigned int c)
{
  ImGui_ImplGlfw_CharCallback(window, c);
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

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  io = &ImGui::GetIO();
  io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

  ImGui_ImplGlfw_InitForOpenGL(window, false);
  ImGui_ImplOpenGL3_Init(glsl_version);

  glfwSetCharCallback(window, ImGui_ImplGlfw_CharCallback);
  glfwSetKeyCallback(window, GLFWKeyCallback);
  glfwSetMouseButtonCallback(window, GLFWMouseButtonCallback);

  lightShader = Shader("../shaders/light.vs", "../shaders/light.fs");
  GLint success;
  glGetProgramiv(lightShader.getId(), GL_LINK_STATUS, &success);
  if (!success)
  {
    char infoLog[1024];
    glGetProgramInfoLog(lightShader.getId(), 1024, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }
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

void Renderer::drawLights(glm::mat4 view, glm::mat4 projection, Shader &shader)
{
  for (unsigned int i = 0; i < lights.size(); i++)
  {
    if (lights[i].type == POINT)
    {
      glm::mat4 model = lights[i].model.getModelMatrix();

      lightShader.bind();
      lightShader.setMat4("model", model);
      lightShader.setMat4("view", view);
      lightShader.setMat4("projection", projection);
      lightShader.setVec3("color", lights[i].color);
      lights[i].model.draw(lightShader);
    }

    std::string lightstr = "pointLights";
    lightstr += "[" + std::to_string(i) + "]";

    shader.bind();
    shader.setVec3((lightstr + ".position").c_str(), lights[i].model.position);
    shader.setVec3((lightstr + ".ambient").c_str(), glm::vec3(ambient));
    shader.setVec3((lightstr + ".diffuse").c_str(), glm::vec3(diffuse));
    shader.setVec3((lightstr + ".specular").c_str(), glm::vec3(specular));
    shader.setVec3((lightstr + ".color").c_str(), lights[i].color);
    shader.setFloat((lightstr + ".constant").c_str(), 1.0f);
    shader.setFloat((lightstr + ".linear").c_str(), 0.22f);
    shader.setFloat((lightstr + ".quadratic").c_str(), 0.20f);
  }
}

void Renderer::start(void (*game_loop)(GLFWwindow *window, Shader &shader), Shader &shader, Camera &camera)
{
  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // ADD MODEL WINDOW
    {
      static char path[128] = "../resources/monkey/monkey.obj";
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
      ImGui::Begin("Loaded Stuff");

      if (ImGui::CollapsingHeader("Models"))
      {
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
      }

      if (ImGui::CollapsingHeader("Lights"))
      {
        for (unsigned int i = 0; i < lights.size(); i++)
        {
          if (ImGui::CollapsingHeader(std::to_string(i).c_str()))
          {
            ImGui::Text("Position");
            ImGui::Text("(%.1f, %.1f, %.1f)", lights[i].model.position.x, lights[i].model.position.y, lights[i].model.position.z);

            ImGui::Text("Color");
            ImGui::Text("(%.1f, %.1f, %.1f)", lights[i].color.x, lights[i].color.y, lights[i].color.z);
          }
        }
      }

      ImGui::End();
    }
    // ADD LIGHTS
    {
      static float px = 0, py = 0, pz = 0;
      static float r = 1.0f, g = 1.0f, b = 1.0f;
      static float strength = 1.0;
      static int index = 0;
      const char *type[] = {"POINT", "DIRECTIONAL", "SPOT"};

      ImGui::Begin("Add Lights");

      ImGui::Text("Position");
      ImGui::InputFloat("px", &px);
      ImGui::InputFloat("py", &py);
      ImGui::InputFloat("pz", &pz);

      ImGui::Text("Color");
      ImGui::InputFloat("r", &r);
      ImGui::InputFloat("g", &g);
      ImGui::InputFloat("b", &b);

      // ImGui::Text("Type");
      ImGui::Combo("Type", &index, type, IM_ARRAYSIZE(type));

      ImGui::InputFloat("Strength", &strength);

      if (ImGui::Button("Add Light"))
      {
        std::cout << "Adding light: " << type[index] << std::endl;

        LightTypeList lightType = POINT;
        if (strncmp(type[index], "DIRECTIONAL", 11) == 0)
          lightType = DIRECTIONAL;
        else if (strncmp(type[index], "SPOT", 4) == 0)
          lightType = SPOT;

        addLight(glm::vec3(px, py, pz), glm::vec3(r, g, b), strength, lightType);
      }

      ImGui::End();
    }

    if (game_loop && window)
      game_loop(window, shader);

    shader.setInt("numPointLights", lights.size());
    shader.setInt("numDirectionalLights", 0);
    shader.setInt("numSpotLights", 0);

    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(camera.getFov(), (float)Renderer::width / (float)Renderer::height, 0.1f, 100.0f);
    drawLights(view, projection, shader);

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

void Renderer::addLight(glm::vec3 position, glm::vec3 color, float strength, LightTypeList type)
{
  Model lightModel("../resources/light.obj", position, glm::vec2(0.0f), glm::vec3(1.0f), false);
  LightType light = {
      lightModel,
      color,
      strength,
      type,
  };
  lights.push_back(light);
}

GLFWwindow *Renderer::getWindow()
{
  return window;
}

Shader &Renderer::getLightShader()
{
  return lightShader;
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
  glfw_mouse_button_callback = callback;
}

void Renderer::setKeyCallback(GLFWkeyfun callback)
{
  glfw_key_callback = callback;
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