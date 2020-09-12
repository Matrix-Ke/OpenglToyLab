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
		Texture(ENUM_TYPE type, unsigned int width , unsigned int height);
		Texture(const std::vector<std::string> & skybox);
		Texture(unsigned int width, unsigned int height, float const* data, unsigned int dateType, unsigned int srcFormat, unsigned int internalFormat);
		Texture(const std::string & path,  bool flip = false, bool gammaCorrection = false, std::string texName = std::string("texture_diffuse1"));

		Texture(const Texture& rhs);
		Texture& operator=(const Texture& rhs);

		bool Load(const std::vector<std::string> & skybox);

		bool Load(const std::string & path, bool flip = false, bool  gammaCorrection = false, unsigned int desiredChannel = 0);

		bool SetImage(const Oper::Image & image);

		void SetWrapping(unsigned int wrap_s = GL_REPEAT, unsigned int wrap_t = GL_REPEAT, unsigned int wrap_r = GL_CLAMP_TO_EDGE);

		void SetFiltering(unsigned int minFilter = GL_NEAREST, unsigned int maxFilter = GL_LINEAR);

		bool Use(unsigned int id  = 0 ) const;

		void UnBind();

		unsigned int  GetID() const;

		unsigned int  getGLType() const;

		bool IsValid()  const;

		void SetName(const std::string& texname);

		std::string  getName() const;

	protected:
		void init();

		void constructEmptyCubemap(unsigned int internalFormat, unsigned int widthSize, unsigned int heightSize);

	public:
		static const Texture InValid;
	private:
		static unsigned int Type2GL(ENUM_TYPE type);
		static std::string Type2Str(ENUM_TYPE type);


	private:
		std::string  m_TexName;
		unsigned int m_TextureID;
		int m_Channel;
		ENUM_TYPE m_Type;

		unsigned int m_GlWrapS;
		unsigned int m_GlWrapT;
		unsigned int m_GlWrapR;
		unsigned int m_GlMinfilter;
		unsigned int m_GlMaxFilter;

		unsigned int m_GLtexType;
	};
}
