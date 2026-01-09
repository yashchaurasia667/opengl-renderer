#pragma once

class VertexBuffer
{
private:
  unsigned int ID;

public:
  VertexBuffer();
  VertexBuffer(int count, void *data, unsigned int usage);
  ~VertexBuffer();

  void bind() const;
  void unbind() const;
  void setData(int count, void *data, unsigned int usage);
};