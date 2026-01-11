#include "vertexBuffer.h"
#include <utils.h>

#include <glad/glad.h>

VertexBuffer::VertexBuffer() : ID(0)
{
  glCall(glGenBuffers(1, &ID));
  glCall(glBindBuffer(GL_ARRAY_BUFFER, ID));
}

VertexBuffer::VertexBuffer(int count, void *data, unsigned int usage) : VertexBuffer()
{
  glCall(glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), data, usage));
}

VertexBuffer::~VertexBuffer()
{
  glCall(glDeleteBuffers(1, &ID));
}

void VertexBuffer::bind() const
{
  glCall(glBindBuffer(GL_ARRAY_BUFFER, ID));
}

void VertexBuffer::unbind() const
{
  glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::setData(int count, void *data, unsigned int usage)
{
  glCall(glBindBuffer(GL_ARRAY_BUFFER, ID));
  glCall(glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), data, usage));
}
