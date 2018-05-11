#include "terrainModel.hpp"
#include "stb_image.h"
#include <iostream>

terrainModel::terrainModel() {
	// do nothing
	terrainMaxHeight = LEVEL_MAX_HEIGHT;
	blockScale = float(LEVEL_MAX_HEIGHT / 250.0f);
}

terrainModel::terrainModel(char* path) {
	terrainMaxHeight = LEVEL_MAX_HEIGHT;
	blockScale = float(LEVEL_MAX_HEIGHT / 250.0f);
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
		imageWidth = width;
		imageHeight = height;
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

		generateTerrain(data, nrComponents);
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

void terrainModel::generateTerrain(unsigned char* data, int nrComponents) {
	std::cout	<< imageWidth	<< " ImageWidth\n"
				<< imageHeight	<< " ImageHeight\n";
	for (int y = 0; y < imageHeight; y++) {
		for (int x = 0; x < imageWidth; x++) {
			heightMap.push_back(getPixelHeight(data, nrComponents, x, y));
			// Size of the terrain in world units
			float terrainWidth = (imageWidth - 1) * blockScale;		// X-axis
			float terrainHeight = (imageHeight - 1) * blockScale;	// Z-axis

			float halfTerrainWidth = terrainWidth * 0.5f;
			float halfTerrainHeight = terrainHeight * 0.5f;
			
			Vertex vboData;
			vboData.Position.x	= x * blockScale;
			vboData.Position.y	= heightMap[x + (imageWidth * y)] * terrainMaxHeight;
			vboData.Position.z	= y * blockScale;
			vboData.Normal		= glm::vec3(0.0f);
			vboData.TexCoords	= glm::vec2(0.0f);

			vertices.push_back(vboData);
			
			
			/*if (heightMap[x + (imageWidth * y)] >= 0.95f || heightMap[x + (imageWidth * y)] <= 0.05f) {
				std::cout << heightMap[x + (imageWidth * y)] << " Height "
					<< x << " x " << y << " y \n";
			}*/
		}
	}

	generateIndexBuffer();

	std::vector<Texture> texture;
	Material terrainMaterial;

	Mesh terrainMesh(vertices, indices, texture, terrainMaterial);
	meshes.push_back(terrainMesh);

}

void terrainModel::generateIndexBuffer() {
	/*
	Reducing numTriangles with one in width and height
	as I am calculating the triangles from top to bottom
	*/
	
	// 2 triangles for each quad
	int numTriangles = (imageWidth - 1) * (imageHeight - 1) * 2;

	// 3 indices for each triangle
	indices.resize(numTriangles * 3);

	int index = 0;
	for (int y = 0; y < imageHeight - 1; y++) {
		for (int x = 0; x < imageWidth - 1; x++) {
			int vertexIndex = x + (y * imageWidth);
			// Top triangle
			indices[index++] = vertexIndex;
			indices[index++] = vertexIndex + imageWidth;
			indices[index++] = vertexIndex + 1;
			// Bottom triangle
			indices[index++] = vertexIndex + 1;
			indices[index++] = vertexIndex + imageWidth + 1;
			indices[index++] = vertexIndex + imageWidth;
		}
	}

}

float terrainModel::getPixelHeight(unsigned char* data, int nrComponents, int x, int y) {
	float height;
	unsigned char* pixelOffset = data + (x + (imageWidth * y)); //Get the image
	unsigned char r = pixelOffset[0]; //Get R value
	unsigned char g = pixelOffset[1]; //Get G value
	unsigned char b = pixelOffset[2]; //Get B value


	//nrComponents in this case is 1, and use R + G because it gives correct value
	if		(nrComponents == 1)	height = (float)r + (float)g;
	else if (nrComponents == 2)	height = (float)r + (float)g;	//Put values together to calculate the height
	else if (nrComponents > 2)	height = (float)r + (float)g + (float)b;
	return (height / 255); //* terrainMaxHeight; //Return the height with smooth in 40
}