#version 330 core

#define MAX_POINT_LIGHTS 64
#define MAX_DIRECTIONAL_LIGHTS 4
#define MAX_SPOT_LIGHTS 8

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
uniform sampler2D texture_specular1;

uniform int numPointLights;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int numDirectionalLights;
uniform DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform int numSpotLights;
uniform SpotLight spotlights[MAX_SPOT_LIGHTS];

out vec4 FragColor;

vec3 calculateDirectionalLighting(DirectionalLight light, vec3 norm, vec3 viewDir) {
  vec3 lightDir = normalize(-light.direction);
  float diff = max(dot(norm, lightDir), 0);

  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);
  vec3 tex = vec3(texture(material.diffuse, TexCoords));

  vec3 ambient = light.ambient * tex;
  vec3 diffuse = light.diffuse * diff * tex;
  vec3 specular = light.specular * spec * tex;

  return ambient + diffuse + specular;
}

vec3 calculatePointLighting(PointLight light, vec3 norm, vec3 viewDir) {
  float distance = length(light.position - FragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

  vec3 lightDir = normalize(light.position - FragPos);
  float diff = max(dot(norm, lightDir), 0);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);

  vec3 tex = vec3(texture(material.diffuse, TexCoords));
  vec3 ambient = light.ambient * tex;
  vec3 diffuse = light.diffuse * diff * tex;
  vec3 specular = light.specular * spec * tex;

  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;

  return ambient + diffuse + specular;
}

vec3 calculateSpotLighting(SpotLight light, vec3 norm, vec3 viewDir) {
  vec3 lightDir = normalize(light.position - FragPos);
  float diff= max(dot(norm, lightDir), 0);

  vec3 reflectDir = reflect(-lightDir, norm);
  float spec= pow(max(dot(viewDir, reflectDir), 0), material.shininess);

  float theta = dot(lightDir, normalize(-light.direction));
  float epsilon = light.cutOff - light.outerCutOff;
  float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);  
  vec3 result = vec3(0.0);

  vec3 tex = vec3(texture(material.diffuse, TexCoords));
    
  if(theta > light.cutOff) 
  {       
    vec3 ambient = light.ambient * tex;
    vec3 diffuse = light.diffuse * diff * tex;
    vec3 specular = light.specular * spec * tex;
    diffuse *= intensity;
    specular *= intensity;

    result = ambient + diffuse + specular; 
  }
  else  
    result = light.ambient * tex;
  
  return result;
}

void main()
{    
  vec3 point = vec3(0.0);
  vec3 directional = vec3(0.0);
  vec3 spot = vec3(0.0);

  vec3 norm = normalize(Normal);
  vec3 viewDir = normalize(viewPos - FragPos);

  for(int i=0; i < numPointLights; i++)
    point += calculatePointLighting(pointLights[i], norm, viewDir);

  for(int i=0; i < numDirectionalLights; i++)
    directional += calculateDirectionalLighting(directionalLights[i], norm, viewDir);

  for(int i=0; i < numSpotLights; i++)
    spot += calculateSpotLighting(spotlights[i], norm, viewDir);

  vec3 result = point + directional + spot;
  FragColor = vec4(result, 0.0);
}