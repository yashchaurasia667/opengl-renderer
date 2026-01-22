#pragma once

#include <string>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

class Shader
{
private:
  unsigned int ID;
  std::string getShaderSource(const char *path);
  void checkCompileError(unsigned int id, std::string type);
  unsigned int createShader(GLenum type, const char *path);

public:
  Shader() = default;
  Shader(const char *vertex_path, const char *fragment_path);
  ~Shader();

  void bind();
  void unbind();

  void setInt(const char *name, int data);
  void setFloat(const char *name, float data);
  void setBool(const char *name, bool data);
  void setVec3(const char *name, glm::vec3 data);
  void setMat4(const char *name, glm::mat4 data);
};