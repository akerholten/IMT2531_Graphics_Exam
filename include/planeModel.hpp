#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include "objectModel.hpp"
#include "gtx/matrix_decompose.hpp"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


class planeModel : public objectModel {
public:
	planeModel();
	planeModel(char *path);
	
	void update(float deltaTime, KeyInput keys);
	void setNewPosition();
	void resetToOriginalPosition();
	void getMidPoint(glm::vec3 mid);
	std::string currentSpeedAsText();
	glm::vec3 currentPosition();
private:
	glm::vec3 velocity;
	glm::vec3 position;

	glm::vec3 Front;
	glm::vec3 Right;

	glm::vec3 MidPoint;

	float Yaw;
	float Pitch;

	float currentSpeed;
	float acceleration;
	float rotationSpeed;
	float maxSpeed;
	float minSpeed;
};