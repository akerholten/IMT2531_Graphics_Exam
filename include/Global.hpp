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

	bool iKey;
	bool jKey;
	bool kKey;
	bool lKey;
	bool yKey;
	bool hKey;

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
	bool minusKey;
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

const float MAX_RENDER_DISTANCE = 10000.0f;
const float MIN_RENDER_DISTANCE = 0.05f;

const int LEVEL_MAX_HEIGHT = 100;
const int MAX_WIDTH = 100;
const int MAX_HEIGHT = 100;
const int LEVELSCALE = 30*2;
const float LERP_RANGE = 0.10f; // Currently using float lerpRange from main instead

const int seasonTimeScaleInSeconds = 30;
const int dayLightTimeScaleInSeconds = 10;