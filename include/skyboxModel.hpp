#pragma once
#include "model.hpp"


class skyboxModel : public Model {
public:
	skyboxModel();
	skyboxModel(char *path, std::string filetype);
	virtual void Draw(Shader shader, glm::mat4 view, glm::mat4 projection);
	void init();


private:
	std::vector<std::string> faces;

	unsigned int cubemapTexture;
	unsigned int skyboxVAO;
	unsigned int skyboxVBO;

	std::vector<GLfloat> skyboxVertices;
	std::vector<GLfloat> initVertices();
	virtual void loadModel(std::string path, std::string filetype);
	unsigned int loadCubeMap(std::vector<std::string> faces, std::string directory);
};