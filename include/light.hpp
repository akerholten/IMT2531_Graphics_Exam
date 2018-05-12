#pragma once
#include "glm.hpp"
#include "shader.hpp"
#include <array>

struct directionalLight {
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	void setDirection(glm::vec3 newDir) {
		direction = newDir;
	};
	void setAmbient(glm::vec3 newAmbient) {
		ambient = newAmbient;
	};
	void setDiffuse(glm::vec3 newDiffuse) {
		diffuse = newDiffuse;
	};
	void setSpecular(glm::vec3 newSpecular) {
		specular = newSpecular;
	};
};

struct pointLight {
	glm::vec3 position;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;

	void setPosition(glm::vec3 newPos) {
		position = newPos;
	};
	void setAmbient(glm::vec3 newAmbient) {
		ambient = newAmbient;
	};
	void setDiffuse(glm::vec3 newDiffuse) {
		diffuse = newDiffuse;
	};
	void setSpecular(glm::vec3 newSpecular) {
		specular = newSpecular;
	};
};

struct spotLight {
	glm::vec3 position;
	glm::vec3 direction;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float cutOff;
	float outerCutOff;

	float constant;
	float linear;
	float quadratic;

	void setPosition(glm::vec3 newPos) {
		position = newPos;
	};
	void setDirection(glm::vec3 newDir) {
		direction = newDir;
	};
	void setAmbient(glm::vec3 newAmbient) {
		ambient = newAmbient;
	};
	void setDiffuse(glm::vec3 newDiffuse) {
		diffuse = newDiffuse;
	};
	void setSpecular(glm::vec3 newSpecular) {
		specular = newSpecular;
	};
};

enum LightType {
	directional, point, spot
};

enum LightVariable {
	pos, dir,
	ambient, diffuse, specular,
	cutOff, outerCutOff,
	constant, linear, quadratic
};

class Light {
public:
	Light();
	void init();

	directionalLight worldLight;

	void newDirectionalLight(directionalLight light);
	void newPointLight(pointLight light);
	void newSpotLight(spotLight light);

	void sendLightToShader(Shader shader);

	void setLightProperty(LightType type, glm::vec3 newProperty, LightVariable variable);
private:
	bool dirSet;
	int pointLightCount;
	int spotLightCount;
	std::array<pointLight, 10> pointLights;
	std::array<spotLight, 10> spotLights;
};