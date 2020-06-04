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

unsigned int TextureFormFile(const char* path, const std::string & directory, bool gamma = false);

class Model
{

public:
	std::vector<Texture>  texture_loaded;
	std::vector<Mesh>     meshes;
	std::string     directory;

	bool    gammaCorrection;
	Model();
	~Model();

private:

};