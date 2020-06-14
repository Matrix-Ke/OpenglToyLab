#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "util/shader.h"






struct VertexInfo
{
	glm::vec3	Position;
	glm::vec3	Normal;
	glm::vec2	TexCoords;
	glm::vec3	Tangent;
	glm::vec3	Bitangent;
};


struct TextureInfo
{
	unsigned int id;
	std::string	type;
	std::string	path;
};

class Mesh 
{
	
public:

	std::vector<VertexInfo>	vertices;

	std::vector<unsigned int>	indices;
	std::vector<TextureInfo>	   textures;
	unsigned int		VAO;


	Mesh(std::vector<VertexInfo>& vertices, std::vector<unsigned int>& indices, std::vector<TextureInfo>& textures);

	//draw mesh
	void Draw(Shader shader);



private:
	unsigned int VBO, EBO;

	void setupMesh();
};