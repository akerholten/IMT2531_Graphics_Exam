#pragma once

#include "objectModel.hpp"

class planeModel : public objectModel {
public:
	planeModel();
	planeModel(char *path);
	
	void update(float deltaTime, KeyInput keys);
	void setNewPosition();
private:
	glm::vec3 velocity;
	float currentSpeed;
	float acceleration;
	float rotationSpeed;
	float maxSpeed;
	float minSpeed;
};