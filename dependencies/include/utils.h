#ifndef UTILS_H
#define UTILS_H

#include <glad/glad.h>
#include <iostream>

#define ASSERT(x) \
  if (!x)         \
  __debugbreak()

#define glCall(x) \
  glClearError(); \
  x;              \
  ASSERT(glLogError(#x, __FILE__, __LINE__))

inline void glClearError()
{
  while (glGetError() != GL_NO_ERROR)
    ;
}

inline bool glLogError(const char *function, const char *file, int line)
{
  while (GLenum error = glGetError())
  {
    std::cout << "[OpenGL Error]: (" << error << "):" << function << " " << file << ":" << line << std::endl;
    return false;
  }
  return true;
}

#endif