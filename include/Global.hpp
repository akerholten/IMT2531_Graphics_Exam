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

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

const int LEVEL_MAX_HEIGHT = 100;
const int MAX_WIDTH = 100;
const int MAX_HEIGHT = 100;
const int LEVELSCALE = 30;