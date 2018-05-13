#pragma once
#include "model.hpp"
#include "Global.hpp"
#include <array>


class terrainModel : public Model {
public:
	terrainModel();
	terrainModel(char* path);

	glm::vec3 calculateMidPoint();
private:
	float blockScale;
	int terrainMaxHeight;
	int imageWidth;
	int imageHeight;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<float>	heightMap;

	virtual void loadModel(std::string path);
	void generateTerrain(unsigned char* data, int nrComponents);
	void generateIndexBuffer();
	void generateNormals(int offset);
	float getPixelHeight(unsigned char* data, int nrComponents, int x, int y);
	unsigned int HeightmapFromFile(const char *path, bool gamma = false);
};