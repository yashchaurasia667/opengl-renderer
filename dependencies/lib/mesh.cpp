#include "mesh.h"
#include <utils.h>

void Mesh::setupMesh()
{
  if (vertices.empty() || indices.empty())
  {
    std::cout << "Mesh has no data" << std::endl;
    return;
  }
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  vbo.setData(vertices.size() * (3 + 3 + 2), &vertices[0], GL_STATIC_DRAW);
  ibo.setData(indices.size(), &indices[0], GL_STATIC_DRAW);

  // layout.push<float>(3); // position
  // layout.push<float>(3); // normals
  // layout.push<float>(2); // texture coords

  // vao.addBuffer(vbo, layout);
  // vao.unbind();

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoords));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureType> textures)
{
  this->vertices = vertices;
  this->indices = indices;
  this->textures = textures;

  setupMesh();
}

void Mesh::draw(Shader &shader)
{
  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;
  unsigned int normalNr = 1;
  unsigned int heightNr = 1;

  for (unsigned int i = 0; i < textures.size(); i++)
  {
    glCall(glActiveTexture(GL_TEXTURE0 + i));

    std::string number;
    std::string name = textures[i].type;

    if (name == "texture_diffuse")
      number = std::to_string(diffuseNr++);
    else if (name == "texture_specular")
      number = std::to_string(specularNr++);
    else if (name == "texture_normal")
      number = std::to_string(normalNr++);
    else if (name == "texture_height")
      number = std::to_string(heightNr++);

    shader.setInt((name + number).c_str(), i);
    glCall(glBindTexture(GL_TEXTURE_2D, textures[i].id));
  }

  // vao.bind();      //<---- breaks here
  glBindVertexArray(VAO);
  glCall(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0));
  // vao.unbind();
  glBindVertexArray(0);

  glCall(glActiveTexture(GL_TEXTURE0));
}