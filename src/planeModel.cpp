#include "planeModel.hpp"

planeModel::planeModel() {
	velocity = glm::vec3(-5.0f, 0.0f, 0.0f);
	rotationSpeed = 10.0f;
	acceleration = 2.0f;
	maxSpeed = 300.0f;
	minSpeed = 2.0f;
	currentSpeed = 2.0f;
}

planeModel::planeModel(char *path) {
	velocity = glm::vec3(-5.0f, 0.0f, 0.0f);
	rotationSpeed = 10.0f;
	acceleration = 2.0f;
	maxSpeed = 300.0f;
	minSpeed = 2.0f;
	currentSpeed = 2.0f;

	loadModel(path);
}

void planeModel::update(float deltaTime, KeyInput keys) {

	if (keys.wKey) {
		rotate(rotationSpeed * deltaTime, glm::vec3(0.0f, 0.0f, 1.0f));
	}
	if (keys.sKey) {
		rotate(-rotationSpeed * deltaTime, glm::vec3(0.0f, 0.0f, 1.0f));
	}
	if (keys.aKey) {
		rotate(rotationSpeed * deltaTime, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	if (keys.dKey) {
		rotate(-rotationSpeed * deltaTime, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	if (keys.commaKey) {
		if (currentSpeed < maxSpeed) velocity.x += acceleration * deltaTime;
	}
	if (keys.dotKey) {
		if (currentSpeed > minSpeed) velocity.x -= acceleration * deltaTime;
	}


	translate(glm::vec3(-currentSpeed * deltaTime, 0.0f, 0.0f));
}