#pragma once
#include "model.hpp"


class objectModel : public Model {
public:
	objectModel();
	objectModel(char* path);

private:
	virtual void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
		std::string typeName);
};