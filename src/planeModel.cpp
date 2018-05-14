#include "planeModel.hpp"

planeModel::planeModel() {
	velocity = glm::vec3(-5.0f, 0.0f, 0.0f);
	rotationSpeed = 30.0f;
	acceleration = 2.0f;
	maxSpeed = 50.0f;
	minSpeed = 2.5f;
	currentSpeed = 2.5f;

	std::srand(time(NULL));
}

planeModel::planeModel(char *path) {
	velocity = glm::vec3(-5.0f, 0.0f, 0.0f);
	rotationSpeed = 30.0f;
	acceleration = 2.0f;
	maxSpeed = 30.0f;
	minSpeed = 2.5f;
	currentSpeed = 2.5f;

	std::srand(time(NULL));	// Rand seed for spawning plane in different positions
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
		if (currentSpeed < maxSpeed) currentSpeed += acceleration * deltaTime;
	}
	if (keys.dotKey) {
		if (currentSpeed > minSpeed) currentSpeed -= acceleration * deltaTime;
	}

	translate(glm::vec3(-currentSpeed * deltaTime, 0.0f, 0.0f));
}

void planeModel::setNewPosition() {
	transform = glm::mat4(1.0f);
	int maxX = MidPoint.x * 2;		// Retrieve the max values from midpoint to
	int maxZ = MidPoint.z * 2;		// ensure plane spawns within the terrain.
	glm::vec3 newPos = glm::vec3((float)(rand() % maxX), 
								(float)(LEVEL_MAX_HEIGHT * 0.90f),
								(float)(rand() % maxZ));
	translate(newPos);

	// Rotate positions to point up or down (on z) on the terrain, so the plane doesn't point outside
	if (newPos.z < MidPoint.z) {
		rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else {
		rotate(-90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	}
}

void planeModel::resetToOriginalPosition() {
	transform = glm::mat4(1.0f);
	translate(MidPoint + glm::vec3(0.0f, (float)(LEVEL_MAX_HEIGHT/2), 0.0f));
	rotate(-90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}

void planeModel::getMidPoint(glm::vec3 mid) {
	MidPoint = mid;
}

std::string planeModel::currentSpeedAsText() {
	std::string returnSpeed = std::to_string((int)(currentSpeed*10));
	returnSpeed.append(" km/h");

	return returnSpeed;
}

glm::vec3 planeModel::currentPosition() {
	glm::vec3 returnPosition;
	glm::decompose(transform, glm::vec3(), glm::quat(), returnPosition, glm::vec3(), glm::vec4());
	position = returnPosition;
	return returnPosition;
}