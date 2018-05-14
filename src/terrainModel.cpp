#include "terrainModel.hpp"
#include "stb_image.h"
#include <iostream>

terrainModel::terrainModel() {
	terrainMaxHeight = LEVEL_MAX_HEIGHT;
	blockScale = float(LEVEL_MAX_HEIGHT / LEVELSCALE);
}

terrainModel::terrainModel(char* path) {
	terrainMaxHeight = LEVEL_MAX_HEIGHT;
	blockScale = float(LEVEL_MAX_HEIGHT / LEVELSCALE);
	loadModel(path);
}

glm::vec3 terrainModel::calculateMidPoint() {
	// To calculate proper midpoint
	// Halfway down in Y multiplied with X-Pixels in each row + one row/2
	return vertices[(imageHeight / 2) * imageWidth + (imageWidth / 2)].Position;
}

void terrainModel::loadModel(std::string path) {
	HeightmapFromFile(path.c_str());
}

unsigned int terrainModel::HeightmapFromFile(const char *path, bool gamma) {

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

		// Call to generate terrain before image data is freed
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
	for (int y = 0; y < imageHeight; y++) {		// Iterating on Y last
		for (int x = 0; x < imageWidth; x++) {	// So we calculate each row of data first
			heightMap.push_back(getPixelHeight(data, nrComponents, x, y));
			
			Vertex vboData;
			vboData.Position.x	= x * blockScale;
			vboData.Position.y	= heightMap[x + (imageWidth * y)] * terrainMaxHeight;	// Using correct calculated height
			vboData.Position.z	= y * blockScale;
			vboData.Normal		= glm::vec3(0.0f);
			vboData.TexCoords	= glm::vec2(0.0f);

			vertices.push_back(vboData);
		}
	}

	// Generate indices
	generateIndexBuffer();

	// Generate normals, 6 times, as each vertex is connected to 6 other vertices
	// This is to ensure that they get somewhat correct normal-data
	for (int i = 0; i < 6; i++) {
		generateNormals(i);
	}

	// Then normalize the calculated normals on faces
	for (int i = 0; i < indices.size(); i++) {
		vertices[indices[i]].Normal = glm::normalize(vertices[indices[i]].Normal);
	}

	// Then normalize the calculated normals on vertices
	for (int i = 0; i < vertices.size(); i++) {
		vertices[i].Normal = glm::normalize(vertices[i].Normal);
	}

	std::vector<Texture> texture;
	Material terrainMaterial;
	terrainMaterial.ambient = glm::vec3(0.5f);
	terrainMaterial.diffuse = glm::vec3(0.5f);
	terrainMaterial.specular = glm::vec3(0.5f);
	terrainMaterial.shininess = 16;

	Mesh terrainMesh(vertices, indices, texture, terrainMaterial);
	meshes.push_back(terrainMesh);

}

void terrainModel::generateIndexBuffer() {
	// 2 triangles for each quad
	// 3 indices for each triangle
	int index = 0;
	for (int y = 0; y < imageHeight - 1; y++) {
		for (int x = 0; x < imageWidth - 1; x++) {
			int vertexIndex = x + (y * imageWidth);
			// Top triangle
			indices.push_back(vertexIndex);
			indices.push_back(vertexIndex + imageWidth);
			indices.push_back(vertexIndex + 1);

			// Bottom triangle
			indices.push_back(vertexIndex + 1);
			indices.push_back(vertexIndex + imageWidth + 1);
			indices.push_back(vertexIndex + imageWidth);
		}
	}
}

void terrainModel::generateNormals(int offset) {
	for (int i = offset; i < indices.size() - 3; i += 3) {
		glm::vec3 v0 = vertices[indices[i+0]].Position;
		glm::vec3 v1 = vertices[indices[i+1]].Position;
		glm::vec3 v2 = vertices[indices[i+2]].Position;

		glm::vec3 normal = glm::cross(v2 - v0, v1 - v0);

		vertices[indices[i+0]].Normal += normal;
		vertices[indices[i+1]].Normal += normal;
		vertices[indices[i+2]].Normal += normal;
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
	return (height / 255);  // Divided by 255 to give a value between 0 and 1
}