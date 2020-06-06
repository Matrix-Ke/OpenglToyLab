#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "util/shader.h"
#include "util/mesh.h"


class Model
{

public:
	std::vector<Texture>  textures_loaded;
	std::vector<Mesh>     meshes;
	std::string     directory;

	bool    gammaCorrection;
	Model(std::string const & path, bool gamma = false);
	~Model();

	void Draw(Shader shader);

private:

	void loadModel(std::string const &path);

	void processNode(aiNode* node, const aiScene*scene);

	Mesh processMesh(aiMesh*  mesh, const aiScene* scene);

	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

};
