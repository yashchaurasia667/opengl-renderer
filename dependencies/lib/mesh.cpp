#include "mesh.h"
#include <utils.h>

Mesh::Mesh(std::vector<VertexType> vertices, std::vector<unsigned int> indices, std::vector<TextureType> textures)
{
  this->vertices = vertices;
  this->indices = indices;
  this->textures = textures;

  setupMesh();
}

void Mesh::draw(Shader &shader)
{
  // bind appropriate textures
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
      number = std::to_string(specularNr++); // transfer unsigned int to string
    else if (name == "texture_normal")
      number = std::to_string(normalNr++); // transfer unsigned int to string
    else if (name == "texture_height")
      number = std::to_string(heightNr++); // transfer unsigned int to string

    // now set the sampler to the correct texture unit
    shader.setInt((name + number).c_str(), i);
    // and finally bind the texture
    glCall(glBindTexture(GL_TEXTURE_2D, textures[i].id));
  }

  // draw mesh
  glCall(glBindVertexArray(VAO));
  glCall(glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0));
  glCall(glBindVertexArray(0));

  // always good practice to set everything back to defaults once configured.
  glCall(glActiveTexture(GL_TEXTURE0));
}

void Mesh::setupMesh()
{
  // create buffers/arrays
  glCall(glGenVertexArrays(1, &VAO));
  glCall(glGenBuffers(1, &VBO));
  glCall(glGenBuffers(1, &EBO));

  glCall(glBindVertexArray(VAO));
  glCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
  glCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexType), &vertices[0], GL_STATIC_DRAW));

  glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
  glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW));

  // vertex Positions
  glCall(glEnableVertexAttribArray(0));
  glCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexType), (void *)0));
  // vertex normals
  glCall(glEnableVertexAttribArray(1));
  glCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexType), (void *)offsetof(VertexType, normal)));
  // vertex texture coords
  glCall(glEnableVertexAttribArray(2));
  glCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexType), (void *)offsetof(VertexType, texCoords)));
  // vertex tangent
  glCall(glEnableVertexAttribArray(3));
  glCall(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexType), (void *)offsetof(VertexType, tangent)));
  // vertex bitangent
  glCall(glEnableVertexAttribArray(4));
  glCall(glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexType), (void *)offsetof(VertexType, bitangent)));
  // ids
  glCall(glEnableVertexAttribArray(5));
  glCall(glVertexAttribIPointer(5, 4, GL_INT, sizeof(VertexType), (void *)offsetof(VertexType, m_BoneIDs)));

  // weights
  glCall(glEnableVertexAttribArray(6));
  glCall(glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(VertexType), (void *)offsetof(VertexType, m_Weights)));
  glCall(glBindVertexArray(0));
}