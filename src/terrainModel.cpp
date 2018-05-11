#include "terrainModel.hpp"
#include "stb_image.h"
#include <iostream>

terrainModel::terrainModel() {
	// do nothing
	terrainMaxHeight = LEVEL_MAX_HEIGHT;
}

terrainModel::terrainModel(char* path) {
	terrainMaxHeight = LEVEL_MAX_HEIGHT;
	loadModel(path);
}

void terrainModel::loadModel(std::string path) {
	TextureFromFile(path.c_str());
}

unsigned int terrainModel::TextureFromFile(const char *path, bool gamma) {
	

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 2)
			format = GL_RG;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		generateTerrain(data, width, height);
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

void terrainModel::generateTerrain(unsigned char* data, int imageWidth, int imageHeight) {
	std::cout	<< imageWidth	<< " ImageWidth\n"
				<< imageHeight	<< " ImageHeight\n";
	for (int y = 0; y < imageHeight-1; y++) {
		for (int x = 0; x < imageWidth-1; x++) {
			heightMap.push_back(getPixelHeight(data, imageWidth, x, y));
			std::cout << heightMap[x + (imageWidth * y)] << " Height "
						<< x << " x " << y << " y \n";
		}
	}

}

float terrainModel::getPixelHeight(unsigned char* data, int imageWidth, int x, int y) {
	unsigned char* pixelOffset = data + (x + (imageWidth * y)) * 1; //Get the image
	unsigned char r = pixelOffset[0]; //Get R value
	unsigned char g = pixelOffset[1]; //Get G value
	unsigned char b = pixelOffset[2]; //Get B value
	float height = (float)r + (float)g + (float)b; //Put values together to calculate the height
	return height / terrainMaxHeight; //Return the height with smooth in 40
}