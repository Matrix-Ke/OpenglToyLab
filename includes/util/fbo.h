#pragma once

#include <vector>
#include "util/texture.h"

namespace OpenGL
{
	class FBO
	{
	public:
		enum Enum_Type
		{
			ENUM_TYPE_BASIC,
			ENUM_TYPE_RGBF1_DEPTH,
			ENUM_TYPE_RGBF2_DEPTH,
			ENUM_TYPE_RGBF3_DEPTH,
			ENUM_TYPE_MSAA,
			ENUM_TYPE_COLOR,
			ENUM_TYPE_RED,
			ENUM_TYPE_COLOR_FLOAT,
			ENUM_TYPE_DEPTH,
			ENUM_TYPE_CUBE_DEPTH,
			ENUM_TYPE_GBUFFER,
		};


		FBO(unsigned int width, unsigned int height, Enum_Type type = ENUM_TYPE_BASIC);

		bool Use();

		
		const Texture & GetColorTexture(unsigned int  idx = 0) const;
		const Texture & GetDepthTexture() const;
		bool IsValid() const;
		
		unsigned int GetID() const;
		unsigned int GetWidth() const;
		unsigned int GetHeight() const;

		bool PassTo(const FBO& fbo, unsigned int passType = GL_COLOR_BUFFER_BIT) const;
		bool PassTo(unsigned int fboID, unsigned int width, unsigned int height, unsigned int passType = GL_COLOR_BUFFER_BIT) const;


	public:
		static void UseDefault();
		static unsigned int DefaultBuffer;


	private:
		bool GenFBO_BASIC(unsigned int width, unsigned int height);
		bool GenFBO_RGBF_DEPTH(unsigned int width, unsigned int height, unsigned int colorBufferNum = 1);
		bool GenFBO_MSAA(unsigned int width, unsigned int height);
		bool GenFBO_COLOR(unsigned int width, unsigned int height, bool isFloat = false);
		bool GenFBO_RED(unsigned int width, unsigned int height);
		bool GenFBO_DEPTH(unsigned int width, unsigned int height);
		bool GenFBO_CUBE_DEPTH(unsigned int width, unsigned int height);
		bool GenFBO_GBUFFER(unsigned int width, unsigned int height);

		bool IsComplete() const;

		FBO&  operator=(const FBO& fbo) = delete;

		Enum_Type  m_type;
		unsigned int m_FboID;
		std::vector<Texture> m_colorTextures;
		Texture  m_depthTexture;
		unsigned int m_width; 
		unsigned int m_height;
		bool m_isValid;
	};
}