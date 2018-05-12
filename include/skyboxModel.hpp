#pragma once
#include "model.hpp"


class skyboxModel : public Model {
public:
	skyboxModel();
	skyboxModel(char *path);
	virtual void Draw(Shader shader, glm::mat4 view, glm::mat4 projection);
	void init();


private:
	std::vector<std::string> faces;

	unsigned int cubemapTexture;
	unsigned int skyboxVAO;
	unsigned int skyboxVBO;

	std::vector<GLfloat> initVertices();
	virtual void loadModel(std::string path);
	unsigned int loadCubeMap(std::vector<std::string> faces, std::string directory);
};