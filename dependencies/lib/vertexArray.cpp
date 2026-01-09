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

void VertexArray::addBuffer(VertexBuffer &vb, VertexBufferLayout &layout)
{
  const std::vector<struct Attribute> *elements = layout.getElements();
  unsigned int offset = 0;

  for (unsigned int i = 0; i < elements->size(); i++)
  {
    // glVertexAttribPointer(index, size, type, normalized, stride, offset);
    struct Attribute elm = (*elements)[i];
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(i, elm.count, elm.type, elm.normalized, layout.getStride(), (void *)offset);

    offset += elm.count * elm.getSizeOfType(elm.type);
  }
}
