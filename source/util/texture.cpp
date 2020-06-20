#pragma once
#include "util/texture.h"
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"


using namespace LOGL;
using namespace Oper;
using namespace std;
 

const Texture Texture::InValid(0, ENUM_TYPE_NOT_VALID);

LOGL::Texture::Texture(unsigned int ID /*= 0*/, ENUM_TYPE type /*= ENUM_TYPE_2D*/) : ID(ID), type(type) 
{
	this->SetWrapping();
	this->SetFiltering();
};

LOGL::Texture::Texture(const std::vector<std::string> & skybox)
{
	type = ENUM_TYPE_NOT_VALID;
	GL_Type = 0;
	Load(skybox);

	this->SetWrapping();
	this->SetFiltering();
}

LOGL::Texture::Texture(ENUM_TYPE type) : Texture(0, type) { }

LOGL::Texture::Texture(const std::string & path, bool flip /*= false*/, bool gammaCorrection /*= false*/)
{
	type = ENUM_TYPE_NOT_VALID;
	GL_Type = 0;
	Load(path, flip, gammaCorrection);

	this->SetWrapping();
	this->SetFiltering();
}

LOGL::Texture::Texture(unsigned int width, unsigned int height, float const* data, unsigned int dataType, unsigned int srcFormat, unsigned int internalFormat)
{
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, srcFormat, dataType, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	type = ENUM_TYPE_2D;
	GL_Type = GL_TEXTURE_2D;

	this->SetWrapping();
	this->SetFiltering();
}

bool LOGL::Texture::Load(const std::string & path, bool flip /*= false*/, bool gammaCorrection /*= false*/, unsigned int desiredChannel /*= 0*/)
{
	if (IsValid())
	{
		cout << " Error :  the texture is valid already \n";
		return false;
	}

	int width;
	int height;

	//加载纹理图片
	stbi_set_flip_vertically_on_load(flip);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channel, desiredChannel);
	if (data == nullptr)
	{
		printf("texture load failed, filepath = %s", path.c_str());
		type = ENUM_TYPE_NOT_VALID;
		GL_Type = 0;
		return false;
	}
	GLenum internalFormat;
	GLenum dataFormat;
	if (channel == 1) {
		internalFormat = GL_RED;
		dataFormat = GL_RED;
	}
	else if (channel == 3) {
		internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
		dataFormat = GL_RGB;
	}
	else if (channel == 4) {
		internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
		dataFormat = GL_RGBA;
	}

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glWrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glWrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glMinfilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glMaxFilter);

	glGenerateMipmap(GL_TEXTURE_2D);
	type = ENUM_TYPE_2D;
	GL_Type = GL_TEXTURE_2D;
	//Opengl时时刻刻记住要解绑， 不然会被坑死， 很难Debug
	this->UnBind();

	return true;
}

bool LOGL::Texture::SetImage(const Oper::Image & image)
{
	if (type != ENUM_TYPE_2D_DYNAMIC) {
		printf("ERROR: type[%s] can't set image\n", Type2Str(type).c_str());
		return false;
	}

	if (ID == 0)
		glGenTextures(1, &ID);

	GLenum format;
	int nrComponents = image.GetChannel();
	if (nrComponents == 1)
		format = GL_RED;
	else if (nrComponents == 3)
		format = GL_RGB;
	else if (nrComponents == 4)
		format = GL_RGBA;

	glBindTexture(GL_TEXTURE_2D, ID);
	glTexImage2D(GL_TEXTURE_2D, 0, format, image.GetWidth(), image.GetHeight(), 0, format, GL_UNSIGNED_BYTE, image.GetConstData());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glMinfilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glMaxFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glWrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glWrapT);
	UnBind();
}

void LOGL::Texture::SetWrapping(unsigned int wrap_s /*= GL_REPEAT*/, unsigned int wrap_t /*= GL_REPEAT*/)
{
	glWrapS = wrap_s;
	wrap_t = wrap_t;
}

void LOGL::Texture::SetFiltering(unsigned int minFilter /*= GL_NEAREST*/, unsigned int maxFilter /*= GL_LINEAR*/)
{
	glMinfilter = minFilter;
	glMaxFilter = maxFilter;
}

bool LOGL::Texture::Use(unsigned int id /* = 0 */) const
{
	if (!IsValid())
		return false;

	glActiveTexture(GL_TEXTURE0 + id);
	glUniform1i(glGetUniformLocation(shaderID, nameInShader.c_str()), id);
	glBindTexture(Type2GL(type), ID);
	return true;
}

void LOGL::Texture::UnBind()
{
	if (!IsValid())
		return;
	glBindTexture(Type2GL(type), 0);
}

bool LOGL::Texture::GetID() const
{
	return ID;
}

unsigned int LOGL::Texture::getGLType() const
{
	return this->GL_Type;
}

bool LOGL::Texture::IsValid() const
{
	return ID != 0 && type != ENUM_TYPE_NOT_VALID;
}


void LOGL::Texture::SetTexNameInShader(unsigned int program, const std::string& texname)
{
	if (IsValid())
	{
		this->nameInShader = texname;
		this->shaderID = program;
	}
}

unsigned int LOGL::Texture::Type2GL(ENUM_TYPE type)
{
	switch (type)
	{
	case ENUM_TYPE_NOT_VALID:
		return 0;
	case ENUM_TYPE_2D:
	case ENUM_TYPE_2D_DYNAMIC:
		return GL_TEXTURE_2D;
	case ENUM_TYPE_CUBE_MAP:
		return GL_TEXTURE_CUBE_MAP;
	default:
		return 0;
	}
}

std::string LOGL::Texture::Type2Str(ENUM_TYPE type)
{
	switch (type)
	{
	case ENUM_TYPE_NOT_VALID:
		return "ENUM_TYPE_NOT_VALID";
	case ENUM_TYPE_2D:
		return "ENUM_TYPE_2D";
	case ENUM_TYPE_CUBE_MAP:
		return "ENUM_TYPE_CUBE_MAP";
	case ENUM_TYPE_2D_DYNAMIC:
		return "ENUM_TYPE_2D_DYNAMIC";
	default:
		return "UNKNOWN_TYPE";
	}
}

bool LOGL::Texture::Load(const std::vector<std::string> & skybox)
{
	if (IsValid()) {
		printf("ERROR: The texture is valid already.\n");

		return false;
	}

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
	
	int width;
	int height;
	for (size_t i = 0; i < skybox.size(); i++)
	{
		//Image image(skybox[i].c_str());
		stbi_set_flip_vertically_on_load(false);
		unsigned char* data = stbi_load(skybox[i].c_str(), &width, &height, &channel, 0);

		if (data == nullptr)
		{
			printf("Cubemap texture failed to load at path: %s\n", skybox[i].c_str());
			type = ENUM_TYPE_NOT_VALID;
			GL_Type = 0;
			return false;
		}
		
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//包围盒设置
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	type = ENUM_TYPE_CUBE_MAP;
	GL_Type = GL_TEXTURE_CUBE_MAP;
	UnBind();
	return true;
}
