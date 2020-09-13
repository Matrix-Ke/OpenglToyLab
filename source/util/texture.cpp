#pragma once
#include "util/texture.h"
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"


using namespace OpenGL;
using namespace Oper;
using namespace std;
 

const Texture Texture::InValid(0, ENUM_TYPE_NOT_VALID);

OpenGL::Texture::Texture(unsigned int ID /*= 0*/, ENUM_TYPE type /*= ENUM_TYPE_2D*/)
{
	this->init();
	this->m_TextureID = ID;
	this->m_Type = type;
};


OpenGL::Texture::Texture(ENUM_TYPE type, unsigned int width, unsigned int height, unsigned int minfilter)
{
	this->init();


	switch (type)
	{
	case OpenGL::Texture::ENUM_TYPE_NOT_VALID:
		break;
	case OpenGL::Texture::ENUM_TYPE_2D:
		break;
	case OpenGL::Texture::ENUM_TYPE_CUBE_MAP:
		this->SetWrapping(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
		this->SetFiltering(GL_LINEAR, GL_LINEAR);
		this->m_GLtexType = GL_TEXTURE_CUBE_MAP;
		this->m_Type = ENUM_TYPE_CUBE_MAP;
		this->m_GlMinfilter = minfilter;
		this->constructEmptyCubemap(GL_RGB16F, width, height);

		break;
	case OpenGL::Texture::ENUM_TYPE_2D_DYNAMIC:
		break;
	default:
		break;
	}
}

OpenGL::Texture::Texture(const std::vector<std::string> & skybox)
{
	this->init();

	m_GlMinfilter = GL_LINEAR;
	m_GlMaxFilter = GL_LINEAR;

	m_Type = ENUM_TYPE_CUBE_MAP;
	m_GLtexType = GL_TEXTURE_CUBE_MAP;

	Load(skybox);
}

OpenGL::Texture::Texture(const std::string & path,  bool flip /*= false*/, bool gammaCorrection /*= false*/, std::string texName /*= std::string("texture_diffuse1")*/)
{
	this->init();

	m_Type = ENUM_TYPE_2D;
	m_GLtexType = GL_TEXTURE_2D;

	Load(path, flip, gammaCorrection);
}

OpenGL::Texture::Texture(unsigned int width, unsigned int height, float const* data, unsigned int dataType, unsigned int srcFormat, unsigned int internalFormat)
{
	this->init();
	this->SetWrapping(GL_REPEAT, GL_REPEAT);
	this->SetFiltering(GL_NEAREST, GL_LINEAR);

	glGenTextures(1, &m_TextureID);
	glBindTexture(m_GLtexType, m_TextureID);
	glTexImage2D(m_GLtexType, 0, internalFormat, width, height, 0, srcFormat, dataType, data);
	glTexParameteri(m_GLtexType, GL_TEXTURE_WRAP_S, m_GlWrapS);
	glTexParameteri(m_GLtexType, GL_TEXTURE_WRAP_T, m_GlWrapT);
	glTexParameteri(m_GLtexType, GL_TEXTURE_MIN_FILTER, m_GlMinfilter);
	glTexParameteri(m_GLtexType, GL_TEXTURE_MAG_FILTER, m_GlMaxFilter);
	m_Type = ENUM_TYPE_2D;
	m_GLtexType = m_GLtexType;
}

Texture& OpenGL::Texture::operator=(const Texture& rhs)
{
	this->m_TexName = rhs.m_TexName;
	this->m_TextureID = rhs.m_TextureID;
	this->m_Channel = rhs.m_Channel;
	this->m_Type = rhs.m_Type;
	this->m_GlWrapS = rhs.m_GlWrapS;
	this->m_GlWrapT = rhs.m_GlWrapT;
	this->m_GlMinfilter = rhs.m_GlMaxFilter;
	this->m_GlMaxFilter = rhs.m_GlMaxFilter;
	this->m_GLtexType = rhs.m_GLtexType;
	return *this;
};

OpenGL::Texture::Texture(const Texture& rhs)
{
	this->m_TexName = rhs.m_TexName;
	this->m_TextureID = rhs.m_TextureID;
	this->m_Channel = rhs.m_Channel;
	this->m_Type = rhs.m_Type;
	this->m_GlWrapS = rhs.m_GlWrapS;
	this->m_GlWrapT = rhs.m_GlWrapT;
	this->m_GlMinfilter = rhs.m_GlMaxFilter;
	this->m_GlMaxFilter = rhs.m_GlMaxFilter;
	this->m_GLtexType = rhs.m_GLtexType;
}

bool OpenGL::Texture::Load(const std::string & path, bool flip /*= false*/, bool gammaCorrection /*= false*/, unsigned int desiredChannel /*= 0*/)
{
	if (!IsValid())
	{
		cout << " Error :  the texture is valid already \n";
		return false;
	}

	int width;
	int height;

	//加载纹理图片
	int dataFormat;
	stbi_set_flip_vertically_on_load(flip);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &dataFormat, desiredChannel);
	if (data == nullptr)
	{
		printf("texture load failed, filepath = %s \n", path.c_str());
		m_Type = ENUM_TYPE_NOT_VALID;
		m_GLtexType = 0;
		return false;
	}
	GLenum internalFormat;
	if (dataFormat == 1) {
		internalFormat = GL_RED;
		m_Channel = GL_RED;
	}
	else if (dataFormat == 3) {
		internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
		m_Channel = GL_RGB;
	}
	else if (dataFormat == 4) {
		internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
		m_Channel = GL_RGBA;
	}

	glGenTextures(1, &m_TextureID);
	glBindTexture(m_GLtexType, m_TextureID);
	glTexImage2D(m_GLtexType, 0, internalFormat, width, height, 0, m_Channel, GL_UNSIGNED_BYTE, data);

	glTexParameteri(m_GLtexType, GL_TEXTURE_WRAP_S, m_GlWrapS);
	glTexParameteri(m_GLtexType, GL_TEXTURE_WRAP_T, m_GlWrapT);
	glTexParameteri(m_GLtexType, GL_TEXTURE_MIN_FILTER, m_GlMinfilter);
	glTexParameteri(m_GLtexType, GL_TEXTURE_MAG_FILTER, m_GlMaxFilter);

	glGenerateMipmap(m_GLtexType);

	//Opengl时时刻刻记住要解绑， 不然会被坑死， 很难Debug
	this->UnBind();

	return true;
}

bool OpenGL::Texture::SetImage(const Oper::Image & image)
{
	if (m_Type != ENUM_TYPE_2D_DYNAMIC) {
		printf("ERROR: type[%s] can't set image\n", Type2Str(m_Type).c_str());
		return false;
	}

	if (m_TextureID == 0)
		glGenTextures(1, &m_TextureID);

	GLenum format;
	int nrComponents = image.GetChannel();
	if (nrComponents == 1)
		format = GL_RED;
	else if (nrComponents == 3)
		format = GL_RGB;
	else if (nrComponents == 4)
		format = GL_RGBA;

	glBindTexture(m_GLtexType, m_TextureID);
	glTexImage2D(m_GLtexType, 0, format, image.GetWidth(), image.GetHeight(), 0, format, GL_UNSIGNED_BYTE, image.GetConstData());
	glTexParameteri(m_GLtexType, GL_TEXTURE_MIN_FILTER, m_GlMinfilter);
	glTexParameteri(m_GLtexType, GL_TEXTURE_MAG_FILTER, m_GlMaxFilter);
	glTexParameteri(m_GLtexType, GL_TEXTURE_WRAP_S, m_GlWrapS);
	glTexParameteri(m_GLtexType, GL_TEXTURE_WRAP_T, m_GlWrapT);
	UnBind();
}

void OpenGL::Texture::SetWrapping(unsigned int wrap_s /*= GL_REPEAT*/, unsigned int wrap_t /*= GL_REPEAT*/, unsigned int wrap_r /* =GL_CLAMP_TO_EDGE*/ )
{
	m_GlWrapS = wrap_s;
	m_GlWrapT = wrap_t;
	m_GlWrapR = wrap_r;
}

void OpenGL::Texture::SetFiltering(unsigned int minFilter /*= GL_NEAREST*/, unsigned int maxFilter /*= GL_LINEAR*/)
{
	m_GlMinfilter = minFilter;
	m_GlMaxFilter = maxFilter;
}

bool OpenGL::Texture::Use(unsigned int id /* = 0 */) const
{
	if (!IsValid())
		return false;

	glActiveTexture(GL_TEXTURE0 + id);
	glBindTexture(Type2GL(m_Type), m_TextureID);
	return true;
}

void OpenGL::Texture::UnBind()
{
	if (!IsValid())
		return;
	glBindTexture(Type2GL(m_Type), 0);
}

unsigned int OpenGL::Texture::GetID() const
{
	return m_TextureID;
}

unsigned int OpenGL::Texture::getGLType() const
{
	return this->m_GLtexType;
}

bool OpenGL::Texture::IsValid() const
{
	return m_TextureID != 0 && m_Type != ENUM_TYPE_NOT_VALID;
}


void OpenGL::Texture::SetName(const std::string& texname)
{
	if (IsValid())
	{
		this->m_TexName = texname;
	}
}

std::string OpenGL::Texture::getName() const
{
	return m_TexName;
}

void OpenGL::Texture::init()
{
	this->m_TexName = "defuseTexture";
	this->m_Channel = GL_RGB;
	this->m_Type = ENUM_TYPE_NOT_VALID;
	this->m_GlWrapS = GL_CLAMP_TO_EDGE;
	this->m_GlWrapT = GL_CLAMP_TO_EDGE;
	this->m_GlWrapR = GL_CLAMP_TO_EDGE;

	this->m_GlMinfilter = GL_NEAREST;
	this->m_GlMaxFilter = GL_LINEAR;
	this->m_GLtexType = GL_TEXTURE_2D;
}

void OpenGL::Texture::constructEmptyCubemap(unsigned int internalFormat, unsigned int widthSize, unsigned int heightSize)
{
	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
	this->m_Channel = GL_RGB;
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, widthSize, widthSize, 0, m_Channel, GL_FLOAT, nullptr);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, m_GlWrapS);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, m_GlWrapT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, m_GlWrapR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, m_GlMinfilter);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, m_GlMaxFilter);
}




bool OpenGL::Texture::Load(const std::vector<std::string> & skybox)
{
	if (IsValid()) {
		printf("ERROR: The texture is valid already.\n");

		return false;
	}

	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
	
	int width;
	int height;
	for (size_t i = 0; i < skybox.size(); i++)
	{
		//Image image(skybox[i].c_str());
		stbi_set_flip_vertically_on_load(false);
		unsigned char* data = stbi_load(skybox[i].c_str(), &width, &height, &m_Channel, 0);

		if (data == nullptr)
		{
			printf("Cubemap texture failed to load at path: %s\n", skybox[i].c_str());
			m_Type = ENUM_TYPE_NOT_VALID;
			m_GLtexType = 0;
			return false;
		}
		
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, m_GlMinfilter);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, m_GlMaxFilter);
	//包围盒设置
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, m_GlWrapS);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, m_GlWrapT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, m_GlWrapR);

	UnBind();
	return true;
}


unsigned int OpenGL::Texture::Type2GL(ENUM_TYPE type)
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

std::string OpenGL::Texture::Type2Str(ENUM_TYPE type)
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