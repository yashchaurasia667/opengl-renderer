#include "shader.h"
#include <utils.h>

#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

string Shader::getShaderSource(const char *path)
{
  ifstream shader_file(path);
  if (!shader_file)
  {
    cout << "ERROR::SHADER::" << path << "::NOT SUCCESSFULLY READ" << endl;
    return nullptr;
  }

  stringstream buffer;
  buffer << shader_file.rdbuf();
  string source = buffer.str();
  return source;
}

void Shader::checkCompileError(unsigned int id, string type)
{
  int success, len;
  if (type != "PROGRAM")
  {
    glCall(glGetShaderiv(id, GL_COMPILE_STATUS, &success));
    if (!success)
    {
      glCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len));

      char *info = (char *)malloc(len * sizeof(char));
      glCall(glGetShaderInfoLog(id, len, NULL, info));

      cout << "ERROR::SHADER_COMPILATION_ERROR of type " << type << "\n"
           << info << endl;
    }
  }
  else
  {
    glCall(glGetProgramiv(id, GL_LINK_STATUS, &success));
    if (!success)
    {
      glCall(glGetProgramiv(id, GL_INFO_LOG_LENGTH, &len));

      char *info = (char *)malloc(len * sizeof(char));
      glCall(glGetProgramInfoLog(id, len, NULL, info));

      cout << "ERROR::PROGRAM_LINKING_ERROR of type " << type << "\n"
           << info << endl;
    }
  }
}

unsigned int Shader::createShader(GLenum type, const char *path)
{
  unsigned int id = glCreateShader(type);
  string source = getShaderSource(path);
  const char *c_source = source.c_str();

  glCall(glShaderSource(id, 1, &c_source, NULL));
  glCall(glCompileShader(id));
  checkCompileError(id, "SHADER");

  return id;
}

Shader::Shader(const char *vertex_path, const char *fragment_path)
{
  unsigned int vertex_shader = createShader(GL_VERTEX_SHADER, vertex_path);
  unsigned int fragment_shader = createShader(GL_FRAGMENT_SHADER, fragment_path);

  ID = glCreateProgram();
  glCall(glAttachShader(ID, vertex_shader));
  glCall(glAttachShader(ID, fragment_shader));
  glCall(glLinkProgram(ID));
  checkCompileError(ID, "PROGRAM");
  glCall(glValidateProgram(ID));

  glCall(glDeleteShader(vertex_shader));
  glCall(glDeleteShader(fragment_shader));
}

Shader::~Shader()
{
  glCall(glDeleteProgram(ID));
}

void Shader::bind()
{
  glCall(glUseProgram(ID));
}

void Shader::unbind()
{
  glCall(glUseProgram(0));
}
