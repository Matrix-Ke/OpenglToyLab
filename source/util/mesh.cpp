#pragma once
#include "util/mesh.h"

using namespace std;
using namespace Oper;

Mesh::Mesh(vector<VertexInfo>& vertices, vector<unsigned int>& indices, vector<TextureInfo>& textures)
{
	if (vertices.empty())
	{
		this->isValid = false;
	}
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	this->pointNum = indices.size();


	setupMesh();
}

Oper::Mesh::Mesh(OpenGL::VAO & mVao, const std::vector< OpenGL::Texture> & mTexs)
{
	if (!mVao.IsValid())
	{
		return;
	}

	this->VAO = mVao.GetID();
	this->EBO = 0;
	this->pointNum = mVao.Size();
	this->hasIndex = mVao.indexValid();

	this->isValid = true;

	//setting texture
	this->isCommonTextureName = false;
	this->mTextures = mTexs;
}

Oper::Mesh::Mesh(OpenGL::VAO & mVao, const OpenGL::Texture & mTexs)
{
	if (!mVao.IsValid())
	{
		return;
	}

	this->VAO = mVao.GetID();
	this->EBO = 0;
	this->pointNum = mVao.Size();
	this->hasIndex = mVao.indexValid();

	this->isValid = true;

	//setting texture
	this->isCommonTextureName = false;
	this->mTextures = std::vector< OpenGL::Texture>{ mTexs };
}

bool Oper::Mesh::indexValid() const
{
	return this->hasIndex;
}

Oper::Mesh::Mesh(bool mIsVaild /*= false*/) : isValid(mIsVaild) { };

void Mesh::Draw(Shader shader)
{
	if (this->isCommonTextureName)
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			string number;
			string name = textures[i].type;
			if (name == "texture_specular")
				number = std::to_string(specularNr++);
			else if (name == "texture_normal")
				number = std::to_string(normalNr++);
			else if (name == "texture_height")
				number = std::to_string(heightNr++);
			else
				number = std::to_string(diffuseNr++);//default  name == "texture_diffuse"
				//throw exception("undefined texture type");

			glActiveTexture(GL_TEXTURE0 + i);
			glUniform1i(glGetUniformLocation(shader.getID(), (name + number).c_str()), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}

	}
	else
	{
		for (unsigned int i = 0; i < mTextures.size(); i++)
		{
			auto&  tTex = mTextures[i];
			shader.setInt(tTex.getName(), i);
			tTex.Use(i);
		}
	}


	glBindVertexArray(VAO);
	if (hasIndex)
	{
		glDrawElements(GL_TRIANGLES, pointNum, GL_UNSIGNED_INT, NULL);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, pointNum);
	}

	//必须要解除绑定
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);

	unsigned int	VBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, pointNum * sizeof(VertexInfo), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, pointNum * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (void*)offsetof(VertexInfo, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (void*)offsetof(VertexInfo, TexCoords));
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (void*)offsetof(VertexInfo, Tangent));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (void*)offsetof(VertexInfo, Bitangent));

	glBindVertexArray(0);
}

