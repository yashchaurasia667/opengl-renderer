#include "mesh.h"

#include <utils.h>

Mesh::Mesh(std::vector<VertexType> vertices, std::vector<unsigned int> indices, std::vector<TextureType> textures)
{
  this->vertices = vertices;
  this->indices = indices;
  this->textures = textures;
}

void Mesh::setupMesh()
{
  vao.bind();
  vbo.setData(vertices.size() * (3 + 3 + 2), &vertices[0], GL_STATIC_DRAW);
  ibo.setData(indices.size(), &indices[0], GL_STATIC_DRAW);

  layout.push<float>(3);
  layout.push<float>(3);
  layout.push<float>(2);

  vao.addBuffer(vbo, layout);
  vao.unbind();
}

void Mesh::draw(Shader &shader)
{
  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;

  for (unsigned int i = 0; i < textures.size(); i++)
  {
    glCall(glActiveTexture(GL_TEXTURE0 + i));

    std::string number;
    std::string name = textures[i].type;
    if (name == "texture_diffuse")
      number = std::to_string(diffuseNr++);
    if (name == "texture_specular")
      number = std::to_string(specularNr++);

    shader.setInt((name + number).c_str(), i);
    glCall(glBindTexture(GL_TEXTURE_2D, textures[i].id));
  }

  glCall(glActiveTexture(GL_TEXTURE0));

  vao.bind();
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  vao.unbind();
}