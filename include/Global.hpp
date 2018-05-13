#pragma once
#include "glm.hpp"

#include <string>



struct Vertex {

	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;

};


struct Texture {
	unsigned int id;
	std::string type;
	std::string path; 
};

struct KeyInput {

	bool wKey;
	bool aKey;
	bool sKey;
	bool dKey;
	bool oKey;

	bool zeroKey;
	bool oneKey;
	bool twoKey;
	bool threeKey;
	bool fourKey;
	bool fiveKey;
	bool sixKey;
	bool sevenKey;
	bool eightKey;
	bool nineKey;

	bool spaceKey;
	bool lCtrlKey;
	bool commaKey;
	bool dotKey;

	// arrows
	bool up;
	bool down;
	bool left;
	bool right;
	bool escKey;
};

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

const int LEVEL_MAX_HEIGHT = 100;
const int MAX_WIDTH = 100;
const int MAX_HEIGHT = 100;
const int LEVELSCALE = 30;

const int seasonTimeScaleInSeconds = 30;
const int dayLightTimeScaleInSeconds = 10;