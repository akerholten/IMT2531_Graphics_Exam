#pragma once
#include "model.hpp"


class terrainModel : public Model {
public:
	terrainModel();
	terrainModel(char* path);
private:

	float getPixelHeight();
	virtual void loadModel(std::string path);
};