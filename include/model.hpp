#pragma once

#include "shader.hpp"
#include "mesh.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"


#include <vector>
#include <iostream>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"



class Model
{
public:
	/*  Functions   */
	Model();
	Model(char *path);
	void Draw(Shader shader);
	glm::mat4 getTransform();
	void translate(glm::vec3 offset);
	void rotate(float degrees, glm::vec3 rotation);
	void scale(glm::vec3 scale);
	void scale(float scale);
protected:
	std::vector<Texture> textures_loaded;
	virtual unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);
	/*  Model Data  */
	glm::mat4 transform;
	std::vector<Mesh> meshes;
	std::string directory;
	/*  Functions   */
	virtual void loadModel(std::string path);
};