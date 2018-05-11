#pragma once
#include "model.hpp"
#include "Global.hpp"
#include <array>


class terrainModel : public Model {
public:
	terrainModel();
	terrainModel(char* path);
private:
	int terrainMaxHeight;
	
	std::vector<float> heightMap;
	float getPixelHeight(unsigned char* data, int imageWidth, int x, int y);

	void generateTerrain(unsigned char* data, int imageWidth, int imageHeight);
	virtual void loadModel(std::string path);
	virtual unsigned int TextureFromFile(const char *path, bool gamma = false);
};