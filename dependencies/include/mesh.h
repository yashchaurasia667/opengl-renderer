#pragma once

#include <string>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

#include <vertexArray.h>
#include <vertexBuffer.h>
#include <vertexBufferLayout.h>
#include <indexBuffer.h>
#include <shader.h>

struct VertexType
{
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;
};

struct TextureType
{
  unsigned int id;
  std::string type;
};

class Mesh
{
private:
  std::vector<VertexType> vertices;
  std::vector<unsigned int> indices;
  std::vector<TextureType> textures;

  VertexArray vao;
  VertexBuffer vbo;
  IndexBuffer ibo;
  VertexBufferLayout layout;

  void setupMesh();

public:
  Mesh(std::vector<VertexType> vertices, std::vector<unsigned int> indices, std::vector<TextureType> textures);
  void draw(Shader &shader);
};