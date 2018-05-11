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