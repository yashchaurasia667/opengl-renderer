#pragma once

#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>

#include <shader.h>
#include <vertexArray.h>
#include <vertexBuffer.h>
#include <vertexBufferLayout.h>
#include <indexBuffer.h>

struct Vertex
{
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;
};

struct TextureType
{
  unsigned int id;
  std::string type;
  std::string path;
};

class Mesh
{
public:
  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureType> textures);
  void draw(Shader &shader);

private:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<TextureType> textures;

  VertexArray vao;
  VertexBuffer vbo;
  IndexBuffer ibo;
  VertexBufferLayout layout;

  void setupMesh();
};