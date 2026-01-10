#pragma once

class Texture
{
private:
  int width, height, nrChannels;
  unsigned int ID;
public:
  Texture(const char *path);
  ~Texture();

  void bind();
  void unbind();
};