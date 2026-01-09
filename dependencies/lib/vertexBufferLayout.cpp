#include "vertexBufferLayout.h"

inline unsigned int VertexBufferLayout::getStride() const
{
  return stride;
}

template <typename T>
void VertexBufferLayout::push(int count)
{
  static_assert(false);
}

template <>
void VertexBufferLayout::push<float>(int count)
{
  struct Attribute v = {count, GL_FLOAT, GL_FALSE};
  layout.push_back(v);
  stride += count * sizeof(float);
}

template <>
void VertexBufferLayout::push<unsigned int>(int count)
{
  struct Attribute v = {count, GL_UNSIGNED_INT, GL_FALSE};
  layout.push_back(v);
  stride += count * sizeof(unsigned int);
}

template <>
void VertexBufferLayout::push<unsigned char>(int count)
{
  struct Attribute v = {count, GL_UNSIGNED_BYTE, GL_FALSE};
  layout.push_back(v);
  stride += count * sizeof(unsigned char);
}