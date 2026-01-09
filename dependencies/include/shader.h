#pragma once

#include <string>
#include <glad/glad.h>

class Shader
{
private:
  unsigned int ID;
  std::string getShaderSource(const char *path);
  void checkCompileError(unsigned int id, std::string type);
  unsigned int createShader(GLenum type, const char *path);

public:
  Shader(const char *vertex_path, const char *fragment_path);
  ~Shader();

  void bind();
  void unbind();
};