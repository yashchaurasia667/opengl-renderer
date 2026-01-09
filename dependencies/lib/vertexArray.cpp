#include "vertexArray.h"

#include <vertexBuffer.h>
#include <vertexBufferLayout.h>
#include <utils.h>
#include <vector>

VertexArray::VertexArray()
{
  glCall(glGenVertexArrays(1, &ID));
  glCall(glBindVertexArray(ID));
}

VertexArray::~VertexArray()
{
  glCall(glDeleteVertexArrays(1, &ID));
}

void VertexArray::bind()
{
  glCall(glBindVertexArray(ID));
}

void VertexArray::unbind()
{
  glCall(glBindVertexArray(0));
}

void VertexArray::addBuffer(VertexBuffer &vb, VertexBufferLayout &layout)
{
  const std::vector<struct Attribute> *elements = layout.getElements();
  unsigned int offset = 0;
  unsigned int stride = layout.getStride();

  for (unsigned int i = 0; i < elements->size(); i++)
  {
    // glVertexAttribPointer(index, size, type, normalized, stride, offset);
    struct Attribute elm = (*elements)[i];
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i, elm.count, elm.type, elm.normalized, stride, (void *)static_cast<uintptr_t>(offset));

    offset += elm.count * elm.getSizeOfType(elm.type);
  }
}
