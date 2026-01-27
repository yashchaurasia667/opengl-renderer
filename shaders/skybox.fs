#version 330 core

in vec2 TexCoords;
uniform samplerCube skybox;

out vec4 FragColor;
void main() {
  FragColor = textuxe(skybox, TexCoords);
}