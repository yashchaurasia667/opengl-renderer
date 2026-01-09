#pragma once

class IndexBuffer
{
private:
  unsigned int ID;

public:
  IndexBuffer();
  IndexBuffer(int count, void *data, unsigned int usage);
  ~IndexBuffer();

  void bind() const;
  void unbind() const;
  void setData(int count, void *data, unsigned int usage);
};