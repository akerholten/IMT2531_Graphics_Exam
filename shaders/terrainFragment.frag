#version 330 core

out vec4 FragColor;
struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};
uniform Material material;

in vec3 pos;

/*      LIGHT EMITTER DEFINITIONS       */

void main()
{
    // Note/todo material.ambient is ignore atm
    float colorModifier = pos.y/100;
    vec3 color = vec3(1.0f)*colorModifier;
    FragColor = vec4(color, 1.0f);
}
