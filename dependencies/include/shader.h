#pragma once

class Shader
{
private:
  unsigned int ID;
  const char *getShaderSource(const char *path);
  void checkCompileError(unsigned int id, string type);
  unsigned int createShader(GLenum type, const char *path);

public:
  Shader(const char *vertex_path, const char *fragment_path);
  ~Shader();

  void bind();
  void unbind();
};