#include "light.hpp"

Light::Light() {
	dirSet = false;
	pointLightCount = 0;
	spotLightCount = 0;
}

void Light::init() {
	dirSet = true;

	nightLight.direction = glm::vec3(0.0f, 0.20f, 1.0f);
	nightLight.ambient = glm::vec3(0.3f, 0.25f, 0.25f);
	nightLight.diffuse = glm::vec3(0.15f, 0.05f, 0.1f);
	nightLight.specular = glm::vec3(0.1f, 0.1f, 0.1f);

	morningLight.direction = glm::vec3(1.0f, -0.10f, 0.0f);
	morningLight.ambient = glm::vec3(0.3f, 0.25f, 0.25f);
	morningLight.diffuse = glm::vec3(0.8f, 0.4f, 0.4f);
	morningLight.specular = glm::vec3(0.4f, 0.4f, 0.4f);

	noonLight.direction = glm::vec3(0.0f, -1.00f, -1.0f);
	noonLight.ambient = glm::vec3(0.3f, 0.25f, 0.25f);
	noonLight.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	noonLight.specular = glm::vec3(0.6f, 0.6f, 0.6f);

	middayLight.direction = glm::vec3(-1.0f, -0.60f, 1.0f);
	middayLight.ambient = glm::vec3(0.3f, 0.25f, 0.25f);
	middayLight.diffuse = glm::vec3(0.6f, 0.6f, 0.6f);
	middayLight.specular = glm::vec3(0.5f, 0.5f, 0.5f);

	worldLight = nightLight;
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

void Light::lerpLight(int currentTime, float lerpPos) {
	
	if (currentTime == 0) {
		worldLight = lerpCurrent(nightLight, morningLight, lerpPos);
	}
	else if (currentTime == 1) {
		worldLight = lerpCurrent(morningLight, noonLight, lerpPos);
	}
	else if (currentTime == 2) {
		worldLight = lerpCurrent(noonLight, middayLight, lerpPos);
	}
	else if (currentTime == 3) {
		worldLight = lerpCurrent(middayLight, nightLight, lerpPos);
	}
}

directionalLight Light::lerpCurrent(directionalLight start, directionalLight end, float lerpPos) {
	directionalLight resultLight;
	
	resultLight.direction.x = lerp(start.direction.x, end.direction.x, lerpPos);
	resultLight.direction.y = lerp(start.direction.y, end.direction.y, lerpPos);
	resultLight.direction.z = lerp(start.direction.z, end.direction.z, lerpPos);

	resultLight.ambient.x = lerp(start.ambient.x, end.ambient.x, lerpPos);
	resultLight.ambient.y = lerp(start.ambient.y, end.ambient.y, lerpPos);
	resultLight.ambient.z = lerp(start.ambient.z, end.ambient.z, lerpPos);

	resultLight.diffuse.x = lerp(start.diffuse.x, end.diffuse.x, lerpPos);
	resultLight.diffuse.y = lerp(start.diffuse.y, end.diffuse.y, lerpPos);
	resultLight.diffuse.z = lerp(start.diffuse.z, end.diffuse.z, lerpPos);

	resultLight.specular.x = lerp(start.specular.x, end.specular.x, lerpPos);
	resultLight.specular.y = lerp(start.specular.y, end.specular.y, lerpPos);
	resultLight.specular.z = lerp(start.specular.z, end.specular.z, lerpPos);

	return resultLight;
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

float Light::lerp(float v0, float v1, float t) {
	return (1 - t) * v0 + t * v1;
}