#include "util/fbo.h"


unsigned int OpenGL::FBO::DefaultBuffer = 0;

OpenGL::FBO::FBO(unsigned int width, unsigned int height, Enum_Type type /*= ENUM_TYPE_BASIC*/)
	: m_type(type), m_width(width), m_height(height)
{
	switch (type)
	{
	case OpenGL::FBO::ENUM_TYPE_BASIC:
		if (!GenFBO_BASIC(width, height))
			printf("GenFBO_BASIC fail\n");
		break;
	case OpenGL::FBO::ENUM_TYPE_RGBF1_DEPTH:
	case OpenGL::FBO::ENUM_TYPE_RGBF2_DEPTH:
	case OpenGL::FBO::ENUM_TYPE_RGBF3_DEPTH: {
		const int colorBufferNum = type - ENUM_TYPE_RGBF1_DEPTH + 1;
		if (!GenFBO_RGBF_DEPTH(width, height, colorBufferNum))
			printf("GenFBO_RGBF_DEPTH fail\n");
		break;
	}
	case OpenGL::FBO::ENUM_TYPE_MSAA:
		if (!GenFBO_MSAA(width, height))
			printf("GenFBO_MSAA fail\n");
		break;
	case OpenGL::FBO::ENUM_TYPE_COLOR:
	case OpenGL::FBO::ENUM_TYPE_COLOR_FLOAT: {
		const bool isFloat = type == ENUM_TYPE_COLOR_FLOAT;
		if (!GenFBO_COLOR(width, height, isFloat))
			printf("GenFBO_COLOR fail\n");
		break;
	}
	case OpenGL::FBO::ENUM_TYPE_RED:
		if (!GenFBO_RED(width, height))
			printf("GenFBO_RED fail\n");
		break;
		break;
	case OpenGL::FBO::ENUM_TYPE_DEPTH:
		if (!GenFBO_DEPTH(width, height))
			printf("GenFBO_DEPTH fail\n");
		break;
	case OpenGL::FBO::ENUM_TYPE_CUBE_DEPTH:
		if (!GenFBO_CUBE_DEPTH(width, height))
			printf("GenFBO_CUBE_DEPTH fail\n");
		break;
	case OpenGL::FBO::ENUM_TYPE_GBUFFER:
		if (!GenFBO_GBUFFER(width, height))
			printf("GenFBO_GBUFFER fail\n");
		break;
	case OpenGL::FBO::ENUM_TYPE_PBR_GBUFFER:
		if (!GenFBO_PBR_GBUFFER(width, height))
			printf("GenFBO_PBR_GBUFFER fail\n");
		break;
	default:
		printf("ERROR: FBO type not know\n");
		m_isValid = false;
		break;
	}
};

bool OpenGL::FBO::Use()
{
	if (!m_isValid)
	{
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_FboID);
	return true;
}

unsigned int OpenGL::FBO::GetID() const
{
	if (!m_isValid) 
	{
		return 0;
	}
	return m_FboID;
}

unsigned int OpenGL::FBO::GetWidth() const
{
	if (!this->m_isValid)
	{
		return 0;
	}
	return this->m_width;
}

unsigned int OpenGL::FBO::GetHeight() const
{
	if (!this->m_height)
	{
		return 0;
	}
	return this->m_height;
}

const OpenGL::Texture & OpenGL::FBO::GetColorTexture(unsigned int idx /*= 0*/) const
{
	if (!m_isValid|| idx > m_colorTextures.size())
	{
		return Texture::InValid;
	}

	//if (m_type != ENUM_TYPE_BASIC
	//	&& m_type != ENUM_TYPE_MSAA
	//	&& m_type != ENUM_TYPE_COLOR
	//	&& m_type != ENUM_TYPE_RED
	//	&& m_type != ENUM_TYPE_COLOR_FLOAT
	//	&& m_type != ENUM_TYPE_RGBF1_DEPTH
	//	&& m_type != ENUM_TYPE_RGBF2_DEPTH
	//	&& m_type != ENUM_TYPE_RGBF3_DEPTH
	//	&& m_type != ENUM_TYPE_GBUFFER
	//	&& m_type != ENUM_TYPE_PBR_GBUFFER)
	//	return Texture::InValid;

	return m_colorTextures[idx];
}

const OpenGL::Texture & OpenGL::FBO::GetDepthTexture() const
{
	if (!m_isValid)
		return Texture::InValid;

	if (m_type != ENUM_TYPE_DEPTH
		&& m_type != ENUM_TYPE_CUBE_DEPTH)
		return Texture::InValid;;

	return m_depthTexture;
}

bool OpenGL::FBO::IsValid() const
{
	return this->m_isValid;
}

bool OpenGL::FBO::PassTo(const FBO& fbo, unsigned int passType /*= GL_COLOR_BUFFER_BIT*/) const
{
	if (!m_isValid || fbo.IsValid())
	{
		return false;
	}

	return PassTo(fbo.GetID(), fbo.GetWidth(), fbo.GetHeight(), passType);
}

void OpenGL::FBO::UseDefault()
{
	glBindFramebuffer(GL_FRAMEBUFFER, DefaultBuffer);
}

bool OpenGL::FBO::GenFBO_BASIC(unsigned int width, unsigned int height)
{
	glGenFramebuffers(1, &m_FboID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FboID);

	unsigned int colorbufferID;
	glGenTextures(1, &colorbufferID);
	glBindTexture(GL_TEXTURE_2D, colorbufferID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorbufferID, 0);

	unsigned int RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width,height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	//完成之后必须记得要解绑
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_isValid = this->IsComplete();
	if (!m_isValid)
	{
		printf("framebuffer is not complete\n");
		return false;
	}

	m_colorTextures.push_back(Texture(colorbufferID, Texture::ENUM_TYPE_2D));
	return true;
}

bool OpenGL::FBO::GenFBO_RGBF_DEPTH(unsigned int width, unsigned int height, unsigned int colorBufferNum /*= 1*/)
{
	glGenFramebuffers(1, &m_FboID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FboID);

	for (unsigned int i = 0; i < colorBufferNum; i++) {
		unsigned int colorBufferID;
		glGenTextures(1, &colorBufferID);
		glBindTexture(GL_TEXTURE_2D, colorBufferID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBufferID, 0);
		m_colorTextures.push_back(Texture(colorBufferID));
	}

	if (colorBufferNum > 1) {
		unsigned int * attachments = new unsigned int[colorBufferNum];
		for (unsigned int i = 0; i < colorBufferNum; i++)
			attachments[i] = GL_COLOR_ATTACHMENT0 + i;
		glDrawBuffers(colorBufferNum, attachments);
		delete[] attachments;
	}

	unsigned int RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_isValid = IsComplete();
	if (!m_isValid) {
		printf("Framebuffer is not complete!\n");
		m_colorTextures.clear();
		return false;
	}

	return true;
}

bool OpenGL::FBO::GenFBO_MSAA(unsigned int width, unsigned int height)
{
	const unsigned int  samples = 4;
	glGenFramebuffers(1, &m_FboID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FboID);

	unsigned int  colorBufferID;
	glGenTextures(1, &colorBufferID);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, colorBufferID);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width, height, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, colorBufferID, 0);

	unsigned int  RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_isValid = IsComplete();
	if (!m_isValid) {
		printf("Framebuffer is not complete!\n");
		return false;
	}

	m_colorTextures.push_back(Texture(colorBufferID));
	return true;
}

bool OpenGL::FBO::GenFBO_COLOR(unsigned int width, unsigned int height, bool isFloat /*= false*/)
{
	glGenFramebuffers(1, &m_FboID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FboID);
	// create a color attachment texture
	unsigned int  colorBufferID;
	glGenTextures(1, &colorBufferID);
	glBindTexture(GL_TEXTURE_2D, colorBufferID);
	glTexImage2D(GL_TEXTURE_2D, 0, isFloat ? GL_RGB16F : GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBufferID, 0);	// we only need a color buffer

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_isValid = IsComplete();
	if (!m_isValid) {
		printf("Framebuffer is not complete!\n");
		return false;
	}

	m_colorTextures.push_back(Texture(colorBufferID));
	return true;
}

bool OpenGL::FBO::GenFBO_RED(unsigned int width, unsigned int height)
{
	glGenFramebuffers(1, &m_FboID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FboID);
	// create a color attachment texture
	unsigned int  colorBufferID;
	glGenTextures(1, &colorBufferID);
	glBindTexture(GL_TEXTURE_2D, colorBufferID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBufferID, 0);	// we only need a color buffer

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_isValid = IsComplete();
	if (!m_isValid) {
		printf("Framebuffer is not complete!\n");
		return false;
	}

	m_colorTextures.push_back(Texture(colorBufferID));
	return true;
}

bool OpenGL::FBO::GenFBO_DEPTH(unsigned int width, unsigned int height)
{
	glGenFramebuffers(1, &m_FboID);
	// create depth texture
	unsigned int  depthBufferID;
	glGenTextures(1, &depthBufferID);
	glBindTexture(GL_TEXTURE_2D, depthBufferID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_FboID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBufferID, 0);
	//从光的透视图下渲染场景的时候深度信息，所以颜色缓冲没有用。
	//然而，不包含颜色缓冲的帧缓冲对象是不完整的，所以我们需要显式告诉OpenGL我们不适用任何颜色数据进行渲染
	//所以我们设置glDrawBuffer(GL_NONE), glReadBuffer(GL_NONE);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	m_isValid = IsComplete();
	if (!m_isValid) {
		printf("Framebuffer is not complete!\n");
		return false;
	}

	m_depthTexture = Texture(depthBufferID);
	return true;

}

bool OpenGL::FBO::GenFBO_CUBE_DEPTH(unsigned int width, unsigned int height)
{
	glGenFramebuffers(1, &m_FboID);

	unsigned int  depthBufferID;
	glGenTextures(1, &depthBufferID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthBufferID);
	for (GLuint i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
			width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, m_FboID);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthBufferID, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_isValid = IsComplete();
	if (!m_isValid) {
		printf("Framebuffer is not complete!\n");
		return false;
	}

	m_depthTexture = Texture(depthBufferID, Texture::ENUM_TYPE_CUBE_MAP);

	return true;
}

bool OpenGL::FBO::GenFBO_GBUFFER(unsigned int width, unsigned int height)
{
	glGenFramebuffers(1, &m_FboID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FboID);
	unsigned int  gPosition, gNormal, gAlbedoSpec;

	// position color buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	// normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	// color + specular color buffer
	glGenTextures(1, &gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);

	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	unsigned int  attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	// create and attach depth buffer (renderbuffer)
	unsigned int  rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	// finally check if framebuffer is complete
	m_isValid = IsComplete();
	if (!m_isValid) {
		printf("Framebuffer is not complete!\n");
		return false;
	}

	m_colorTextures.push_back(gPosition);
	m_colorTextures.push_back(gNormal);
	m_colorTextures.push_back(gAlbedoSpec);

	return true;
}



bool OpenGL::FBO::GenFBO_PBR_GBUFFER(unsigned int width, unsigned int height)
{
	glGenFramebuffers(1, &m_FboID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FboID);
	unsigned int  gPosition, gNormalAo, gAlbedoSpec, gNormalmapRoughness;

	// position color buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	// normal color buffer
	glGenTextures(1, &gNormalAo);
	glBindTexture(GL_TEXTURE_2D, gNormalAo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormalAo, 0);

	// color + specular color buffer
	glGenTextures(1, &gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);

	//metellic map
	glGenTextures(1, &gNormalmapRoughness);
	glBindTexture(GL_TEXTURE_2D, gNormalmapRoughness);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gNormalmapRoughness, 0);


	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	unsigned int  attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, attachments);

	// create and attach depth buffer (renderbuffer)
	unsigned int  rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	// finally check if framebuffer is complete
	m_isValid = IsComplete();
	if (!m_isValid) {
		printf("Framebuffer is not complete!\n");
		return false;
	}

	m_colorTextures.push_back(gPosition);
	m_colorTextures.push_back(gNormalAo);
	m_colorTextures.push_back(gAlbedoSpec);
	m_colorTextures.push_back(gNormalmapRoughness);

	return true;

}

bool OpenGL::FBO::IsComplete() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FboID);
	auto rst = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (rst != GL_FRAMEBUFFER_COMPLETE) {
		printf("Framebuffer is not complete!\n");
		return false;
	}

	return true;
}

bool OpenGL::FBO::PassTo(unsigned int fboID, unsigned int width, unsigned int height, unsigned int passType /*= GL_COLOR_BUFFER_BIT*/) const
{
	if (!this->m_isValid)
		return false;

	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->m_FboID);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboID);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, passType, GL_NEAREST);
	return true;
}


