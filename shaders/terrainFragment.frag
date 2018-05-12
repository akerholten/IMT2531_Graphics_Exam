#version 330 core

out vec4 FragColor;

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

uniform Material material[4];

/*      LIGHT EMITTER DEFINITIONS       */
struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;
uniform bool dirSet;

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define MAX_NR_POINT_LIGHTS 10
uniform PointLight pointLights[MAX_NR_POINT_LIGHTS];
uniform int pointCount;

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define MAX_NR_SPOT_LIGHTS 10
uniform SpotLight spotLight [MAX_NR_SPOT_LIGHTS];
uniform int spotCount;

/*      FRAG VARIABLES        */
in vec3 pos;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;
//vec3 diffuseColor;
//vec3 specularColor;


/*      FUNCTION DECLARATIONS       */
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


vec3 materialAmbient;
vec3 materialDiffuse;
vec3 materialSpecular;
float materialShininess;

void main()
{
    // Note/todo material.ambient is ignore atm
    /*    TEXTURE PASS    */


    float vegetationModifier = pos.y/100.0f;

    if(vegetationModifier < 0.18f){
      materialAmbient = material[0].ambient;
      materialDiffuse = material[0].diffuse;
      materialSpecular = material[0].specular;
      materialShininess = material[0].shininess;
    }
    else if(vegetationModifier < 0.35f){
      materialAmbient = material[1].ambient;
      materialDiffuse = material[1].diffuse;
      materialSpecular = material[1].specular;
      materialShininess = material[1].shininess;
    }
    else if(vegetationModifier < 0.70f){
      materialAmbient = material[2].ambient;
      materialDiffuse = material[2].diffuse;
      materialSpecular = material[2].specular;
      materialShininess = material[2].shininess;
    }
    else if(vegetationModifier < 1.00f){
      materialAmbient = material[3].ambient;
      materialDiffuse = material[3].diffuse;
      materialSpecular = material[3].specular;
      materialShininess = material[3].shininess;
    }
    //vec3 color = vec3(1.0f)*colorModifier;
    //vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    vec3 result = vec3(0.0f);
    // phase 1: directional lighting
    if(dirSet == true){
      result += CalcDirLight(dirLight, norm, viewDir);
    }

    // phase 2: point lights
    for(int i = 0; i < pointCount; i++){
      result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }

    // phase 3: spot light
    for(int i = 0; i < spotCount; i++){
      result += CalcSpotLight(spotLight[i], norm, FragPos, viewDir);
    }

    FragColor = vec4(result, 1.0f);
}
// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess); // mat.shininess
    // combine results
    vec3 ambient = light.ambient * materialDiffuse;
    vec3 diffuse = light.diffuse * diff * materialDiffuse;
    vec3 specular = light.specular * spec * materialSpecular;
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient * materialDiffuse;
    vec3 diffuse = light.diffuse * diff * materialDiffuse;
    vec3 specular = light.specular * spec * materialSpecular;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * materialDiffuse;
    vec3 diffuse = light.diffuse * diff * materialDiffuse;
    vec3 specular = light.specular * spec * materialSpecular;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}
