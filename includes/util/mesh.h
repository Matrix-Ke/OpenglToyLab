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
#include "util/texture.h"
#include "util/VAO.h"


namespace Oper
{
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
		Mesh(bool mIsVaild = false);
		Mesh(std::vector<VertexInfo>& vertices, std::vector<unsigned int>& indices, std::vector<TextureInfo>& mTexs);

		Mesh(LOGL::VAO & mVao, std::vector< LOGL::Texture>  &  mTextures);

		//draw mesh
		void Draw(Shader shader);

		bool indexValid() const;


	private:
		void setupMesh();


	public:
		std::vector<VertexInfo>		vertices;
		std::vector<unsigned int>	indices;
		std::vector<TextureInfo>	textures;


	private:
		std::vector<LOGL::Texture>	mTextures;

		unsigned int	VAO;
		unsigned int	EBO;
		unsigned int	pointNum;

		bool			hasIndex;
		bool			isValid;
		bool			isCommonTextureName;

	};
}



