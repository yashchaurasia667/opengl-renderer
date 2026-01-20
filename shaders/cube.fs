#version 330 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct PointLight {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

struct DirectionalLight {
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct SpotLight {
  vec3 position;
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float cutOff;
  float outerCutOff;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 cubeColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

uniform Material material;
uniform sampler2D texture_diffuse1;
// uniform sampler2D texture_diffuse2;
// uniform sampler2D texture_diffuse3;
// uniform sampler2D texture_diffuse4;

uniform sampler2D texture_specular1;
// uniform sampler2D texture_specular2;
// uniform sampler2D texture_specular3;
// uniform sampler2D texture_specular4;

out vec4 FragColor;

void main()
{    
  FragColor = texture(texture_diffuse1, TexCoords);
  // FragColor = vec4(TexCoords, 0.0, 1.0);
}