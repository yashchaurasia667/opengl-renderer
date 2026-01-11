#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>
#include <mesh.h>
#include <shader.h>

class Model
{
private:
  std::vector<TextureType> textures_loaded;
  std::vector<Mesh> meshes;
  std::string directory;
  bool gammaCorrection;

  void loadModel(const char *path);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<TextureType> loadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName);
  unsigned int textureFromFile(const char *path, std::string &directory, bool gamma);

public:
  Model(const char *path);
  void draw(Shader &shader);
};