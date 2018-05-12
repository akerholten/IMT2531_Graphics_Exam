#include "skyboxModel.hpp"
#include "stb_image.h"

skyboxModel::skyboxModel() {

}

skyboxModel::skyboxModel(char *path, std::string filetype) {
	init();

	loadModel(path, filetype);
}

void skyboxModel::Draw(Shader shader, glm::mat4 view, glm::mat4 projection) {
	glDepthFunc(GL_LEQUAL); // change depth function so depth test passes when values are equal to depth buffer's content
	shader.use();
	//view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
}

void skyboxModel::loadModel(std::string path, std::string filetype) {
	std::string directory = path.append("/");
	
	faces = {
		"right" + filetype,
		"left" + filetype,
		"top" + filetype,
		"bottom" + filetype,
		"front" + filetype,		// Front and back are
		"back" + filetype		// flipped on some cubemaps.
	};

	cubemapTexture = loadCubeMap(faces, directory);
}

unsigned int skyboxModel::loadCubeMap(std::vector<std::string> faces, std::string directory) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		std::string currentFace = directory;
		currentFace.append(faces[i]);
		unsigned char *data = stbi_load(currentFace.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

void skyboxModel::init() {
	skyboxVertices = initVertices();

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(GLfloat), skyboxVertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLfloat*)0);
}

std::vector<GLfloat> skyboxModel::initVertices() {
	std::vector<GLfloat> vertices {
		// positions          
		-2.0f,  2.0f, -2.0f,
		-2.0f, -2.0f, -2.0f,
		2.0f, -2.0f, -2.0f,
		2.0f, -2.0f, -2.0f,
		2.0f,  2.0f, -2.0f,
		-2.0f,  2.0f, -2.0f,

		-2.0f, -2.0f,  2.0f,
		-2.0f, -2.0f, -2.0f,
		-2.0f,  2.0f, -2.0f,
		-2.0f,  2.0f, -2.0f,
		-2.0f,  2.0f,  2.0f,
		-2.0f, -2.0f,  2.0f,

		2.0f, -2.0f, -2.0f,
		2.0f, -2.0f,  2.0f,
		2.0f,  2.0f,  2.0f,
		2.0f,  2.0f,  2.0f,
		2.0f,  2.0f, -2.0f,
		2.0f, -2.0f, -2.0f,

		-2.0f, -2.0f,  2.0f,
		-2.0f,  2.0f,  2.0f,
		2.0f,  2.0f,  2.0f,
		2.0f,  2.0f,  2.0f,
		2.0f, -2.0f,  2.0f,
		-2.0f, -2.0f,  2.0f,

		-2.0f,  2.0f, -2.0f,
		2.0f,  2.0f, -2.0f,
		2.0f,  2.0f,  2.0f,
		2.0f,  2.0f,  2.0f,
		-2.0f,  2.0f,  2.0f,
		-2.0f,  2.0f, -2.0f,

		-2.0f, -2.0f, -2.0f,
		-2.0f, -2.0f,  2.0f,
		2.0f, -2.0f, -2.0f,
		2.0f, -2.0f, -2.0f,
		-2.0f, -2.0f,  2.0f,
		2.0f, -2.0f,  2.0f
	};

	return vertices;
}