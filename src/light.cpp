#include "light.hpp"

Light::Light() {
	dirSet = false;
	pointLightCount = 0;
	spotLightCount = 0;
}

void Light::init() {
	dirSet = true;
	worldLight.direction = glm::vec3(1.0f, -1.0f, -0.3f);
	worldLight.ambient = glm::vec3(0.3f, 0.25f, 0.25f);
	worldLight.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	worldLight.specular = glm::vec3(0.4f, 0.4f, 0.4f);
}

void Light::initSpotLight() {
	if (spotLightCount < 9) {
		spotLights[spotLightCount].ambient = glm::vec3(0.2f, 0.2f, 0.2f);
		spotLights[spotLightCount].diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
		spotLights[spotLightCount].specular = glm::vec3(0.6f, 0.6f, 0.6f);
		spotLights[spotLightCount].cutOff = glm::cos(glm::radians(16.0f));
		spotLights[spotLightCount].outerCutOff = glm::cos(glm::radians(20.0f));
		spotLights[spotLightCount].constant = 1.0f;
		spotLights[spotLightCount].linear = 0.007;
		spotLights[spotLightCount].quadratic = 0.0002;

		spotLightCount++;
	}
}

void Light::newDirectionalLight(directionalLight light) {

}

void Light::newPointLight(pointLight light) {

}

void Light::newSpotLight(spotLight light) {

}

void Light::sendLightToShader(Shader shader, Camera camera) {
	shader.setBool("dirSet", dirSet);
	shader.setVec3("dirLight.direction", worldLight.direction);
	shader.setVec3("dirLight.ambient", worldLight.ambient);
	shader.setVec3("dirLight.diffuse", worldLight.diffuse);
	shader.setVec3("dirLight.specular", worldLight.specular);

	shader.setInt("spotCount", spotLightCount);
	for (int i = 0; i < spotLightCount; i++) {
		spotLightToShader(i, shader, camera);
	}
}

void Light::spotLightToShader(int index, Shader shader, Camera camera) {
	std::string spotLight = "spotLight[";
	spotLight.append(std::to_string(index));
	spotLight.append("]");

	shader.setVec3(spotLight + ".position", camera.Position);
	shader.setVec3(spotLight + ".direction", camera.Front);
	shader.setVec3(spotLight + ".ambient", spotLights[index].ambient);
	shader.setVec3(spotLight + ".diffuse", spotLights[index].diffuse);
	shader.setVec3(spotLight + ".specular", spotLights[index].specular);
	shader.setFloat(spotLight + ".cutOff", spotLights[index].cutOff);
	shader.setFloat(spotLight + ".outerCutOff", spotLights[index].outerCutOff);
	shader.setFloat(spotLight + ".constant", 1.0f);
	shader.setFloat(spotLight + ".linear", 0.007);
	shader.setFloat(spotLight + ".quadratic", 0.0002);
}

void Light::setLightProperty(LightType type, glm::vec3 newProperty, LightVariable variable) {

}