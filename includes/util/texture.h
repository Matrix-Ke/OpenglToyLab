#pragma once
#include <vector>
#include <string>
#include "glad/glad.h"
#include "util/Image.h"

namespace OpenGL
{
	class Texture
	{
	public:
		enum ENUM_TYPE
		{
			ENUM_TYPE_NOT_VALID,
			ENUM_TYPE_2D,
			ENUM_TYPE_CUBE_MAP,
			ENUM_TYPE_2D_DYNAMIC,
		};

		Texture(unsigned int ID = 0, ENUM_TYPE type = ENUM_TYPE_2D);
		Texture(ENUM_TYPE type);
		Texture(unsigned int width, unsigned int height, float const* data, unsigned int dateType, unsigned int srcFormat, unsigned int internalFormat);
		Texture(const std::vector<std::string> & skybox);
		Texture(const std::string & path, bool flip = false, bool gammaCorrection = false);

		bool Load(const std::vector<std::string> & skybox);

		bool Load(const std::string & path, bool flip = false, bool  gammaCorrection = false, unsigned int desiredChannel = 0);

		bool SetImage(const Oper::Image & image);

		void SetWrapping(unsigned int wrap_s = GL_REPEAT, unsigned int wrap_t = GL_REPEAT);

		void SetFiltering(unsigned int minFilter = GL_NEAREST, unsigned int maxFilter = GL_LINEAR);

		bool Use(unsigned int id  = 0 ) const;

		void UnBind();

		bool  GetID() const;

		unsigned int  getGLType() const;

		bool IsValid()  const;

		void SetName(const std::string& texname);

		std::string  getName() const;


	public:
		static const Texture  InValid;

	private:
		static unsigned int Type2GL(ENUM_TYPE type);
		static std::string Type2Str(ENUM_TYPE type);


	private:
		std::string  m_TexName;
		unsigned int textureID;
		int channel;
		ENUM_TYPE type;

		unsigned int glWrapS;
		unsigned int glWrapT;
		unsigned int glMinfilter;
		unsigned int glMaxFilter;

		unsigned int  GL_Type;
	};
}
