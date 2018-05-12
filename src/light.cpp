#include "light.hpp"

Light::Light() {
	dirSet = false;
	pointLightCount = 0;
	spotLightCount = 0;
}

void Light::init() {

}

void Light::newDirectionalLight(directionalLight light) {

}

void Light::newPointLight(pointLight light) {

}

void Light::newSpotLight(spotLight light) {

}

void Light::sendLightToShader(Shader shader) {

}

void Light::setLightProperty(LightType type, glm::vec3 newProperty, LightVariable variable) {

}