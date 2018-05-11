#include "model.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


/* PUBLIC FUNCTIONS */

//unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

Model::Model(){
	transform = glm::mat4(1.0f);
}

Model::Model(char *path){
	transform = glm::mat4(1.0f);
	loadModel(path);
}

void Model::Draw(Shader shader)
{
	shader.setMat4("model", transform);

	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader);
}

glm::mat4 Model::getTransform() {
	return transform;
}
void Model::translate(glm::vec3 offset) {
	transform = glm::translate(transform, offset);
}
void Model::rotate(float degrees, glm::vec3 rotation) {
	transform = glm::rotate(transform, glm::radians(degrees), rotation);
}
void Model::scale(glm::vec3 scale) {
	transform = glm::scale(transform, scale);
}
void Model::scale(float scale) {
	transform = glm::scale(transform, glm::vec3(scale, scale, scale));
}

/* PRIVATE FUNCTIONS */

void Model::loadModel(std::string path)
{
	// Default load not set
}


unsigned int Model::TextureFromFile(const char *path, const std::string &directory, bool gamma)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
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

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << filename << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}