#pragma once

class IndexBuffer
{
private:
  unsigned int ID;

public:
  IndexBuffer();
  IndexBuffer(int count, unsigned int *data, unsigned int usage);
  ~IndexBuffer();

  void bind() const;
  void unbind() const;
  void setData(int count, unsigned int *data, unsigned int usage);
};