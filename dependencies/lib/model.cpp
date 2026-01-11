#include <stb_image.h>
#include "model.h"
#include <texture.h>

Model::Model(const char *path)
{
  loadModel(path);
}

void Model::draw(Shader &shader)
{
  for (unsigned int i = 0; i < meshes.size(); i++)
  {
    meshes[i].draw(shader);
  }
}

void Model::loadModel(const char *path)
{
  Assimp::Importer importer;
  // const aiScene *scene = importer.ReadFile(path, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
  const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
  {
    std::cout << "ERROR::MODEL: " << importer.GetErrorString() << std::endl;
    return;
  }
  std::string sPath = std::string(path);
  directory = sPath.substr(0, sPath.find_last_of('/'));
  processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
  for (unsigned int i = 0; i < node->mNumMeshes; i++)
  {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(mesh, scene));
  }

  for (unsigned int i = 0; i < node->mNumChildren; i++)
    processNode(node->mChildren[i], scene);
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
  std::vector<Vertex> vertices;
  std::vector<TextureType> textures;
  std::vector<unsigned int> indices;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++)
  {
    Vertex v;
    glm::vec3 vec;

    vec.x = mesh->mVertices[i].x;
    vec.y = mesh->mVertices[i].y;
    vec.z = mesh->mVertices[i].z;
    v.position = vec;

    if (mesh->HasNormals())
    {
      vec.x = mesh->mNormals[i].x;
      vec.y = mesh->mNormals[i].y;
      vec.z = mesh->mNormals[i].z;
      v.normal = vec;
    }

    if (mesh->mTextureCoords[0])
    {
      glm::vec2 tex;
      tex.x = mesh->mTextureCoords[0][i].x;
      tex.y = mesh->mTextureCoords[0][i].y;
      v.texCoords = tex;
    }
    else
    {
      v.texCoords = glm::vec2(0.0f);
    }

    vertices.push_back(v);
  }

  for (unsigned int i = 0; i < mesh->mNumFaces; i++)
  {
    aiFace face = mesh->mFaces[i];
    for (unsigned int i = 0; i < face.mNumIndices; i++)
      indices.push_back(face.mIndices[i]);
  }

  aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

  std::vector<TextureType> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
  textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

  std::vector<TextureType> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
  textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

  std::vector<TextureType> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
  textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

  std::vector<TextureType> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
  textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

  return Mesh(vertices, indices, textures);
}

std::vector<TextureType> Model::loadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName)
{
  std::vector<TextureType> textures;
  for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
  {
    aiString str;
    material->GetTexture(type, i, &str);
    bool skip = false;

    for (unsigned int j = 0; j < textures_loaded.size(); j++)
    {
      if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
      {
        textures.push_back(textures_loaded[j]);
        skip = true;
        break;
      }
    }
    if (!skip)
    {
      TextureType tex;
      tex.id = textureFromFile(str.C_Str(), directory, gammaCorrection);
      tex.type = typeName;
      tex.path = str.C_Str();

      textures.push_back(tex);
      textures_loaded.push_back(tex);
    }
  }
  return textures;
}

unsigned int Model::textureFromFile(const char *path, std::string &directory, bool gamma)
{
  // std::string fileName = std::string(path);
  // fileName = directory + '/' + fileName;

  // Texture texture(path);
  // return texture.ID;

  std::string fileName = std::string(path);
  fileName = directory + '/' + fileName;

  unsigned int textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  unsigned char *data = stbi_load(fileName.c_str(), &width, &height, &nrComponents, 0);
  if (data)
  {
    GLenum format;
    if (nrComponents == 1)
      format = GL_RED;
    else if (nrComponents == 3)
      format = GL_RGB;
    else if (nrComponents == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  }
  else
  {
    std::cout << "Texture failed to load at path: " << path << std::endl;
    stbi_image_free(data);
  }

  return textureID;
}
