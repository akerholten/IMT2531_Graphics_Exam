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

const int LEVEL_MAX_HEIGHT = 100;
const int MAX_WIDTH = 100;
const int MAX_HEIGHT = 100;
const int LEVELSCALE = 30;