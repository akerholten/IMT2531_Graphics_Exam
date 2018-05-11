#version 330 core

out vec4 FragColor;
struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};
uniform Material material;

/*      LIGHT EMITTER DEFINITIONS       */

void main()
{
    // Note/todo material.ambient is ignore atm
    FragColor = vec4(1.0f, 0.5f, 0.5f, 1.0f);
}
