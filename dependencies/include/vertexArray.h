#pragma once

class VertexArray
{
private:
  unsigned int ID;

public:
  VertexArray();
  ~VertexArray();

  void addBuffer(VertexBuffer &vb, VertexBufferLayout &layout);
};