#include "OpenGLTexture.h"

#include "Core/Log.h"
#include "Platform/OpenGL/OpenGLDefines.h"
#include "RenderCore/RenderCore.h"

#include <glad/glad.h>
#include <stb/stb_image.h>

namespace sl
{

OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, bool mipmap, TextureFormat format, uint32_t flags, const void *pData) :
	m_width(width), m_height(height), m_hasMip(mipmap), m_format(format), m_flags(flags)
{
	Create(pData);
}

OpenGLTexture2D::~OpenGLTexture2D()
{
	glDeleteTextures(1, &m_handle);
}

void OpenGLTexture2D::Clear(const void *pClearData) const
{
	glClearTexImage(m_handle, 0, GLTextureFormat[(size_t)m_format], GLDataType[(size_t)m_format], pClearData);
}

void OpenGLTexture2D::Resize(uint32_t width, uint32_t height, const void *pData)
{
	if (width <= 0 || height <= 0 || width > RenderCore::GetMaxTextureSize() || height > RenderCore::GetMaxTextureSize())
	{
		SL_ENGINE_ERROR("Invalid texture size!");
		return;
	}

	if (width == m_width && height == m_height)
	{
		return;
	}

	glDeleteTextures(1, &m_handle);
	m_handle = 0;

	m_width = width;
	m_height = height;
	Create(pData);
}

void OpenGLTexture2D::Bind(uint32_t slot) const
{
	glBindTextureUnit(slot, m_handle);
}

void OpenGLTexture2D::Create(const void *pData)
{
	if (m_handle)
	{
		return;
	}

	glCreateTextures(GL_TEXTURE_2D, 1, &m_handle);
	glTextureStorage2D(m_handle, 1, GLInternalTextureFormat[(size_t)m_format], m_width, m_height);
	if (pData)
	{
		glTextureSubImage2D(m_handle, 0, 0, 0, m_width, m_height, GLTextureFormat[(size_t)m_format], GLDataType[(size_t)m_format], pData);
	}

	// Wraps
	glTextureParameteri(m_handle, GL_TEXTURE_WRAP_S, GLTextureWrap[(m_flags & SL_SAMPLER_U_MASK) >> SL_SAMPLER_U_SHIFT]);
	glTextureParameteri(m_handle, GL_TEXTURE_WRAP_T, GLTextureWrap[(m_flags & SL_SAMPLER_V_MASK) >> SL_SAMPLER_V_SHIFT]);
	if ((m_flags & SL_SAMPLER_BORDER) == SL_SAMPLER_BORDER)
	{
		// TODO: Parameterlize it.
		float borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glTextureParameterfv(m_handle, GL_TEXTURE_BORDER_COLOR, borderColor);
	}

	// Filters
	if (m_hasMip)
	{
		glGenerateTextureMipmap(m_handle);
		glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, GLTextureMipmapFilter[(m_flags & SL_SAMPLER_MIPMAP_MASK) >> SL_SAMPLER_MIPMAP_SHIFT]);
	}
	else
	{
		glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, GLTextureFilter[(m_flags & SL_SAMPLER_MIN_MASK) >> SL_SAMPLER_MIN_SHIFT]);
	}
	glTextureParameteri(m_handle, GL_TEXTURE_MAG_FILTER, GLTextureFilter[(m_flags & SL_SAMPLER_MAG_MASK) >> SL_SAMPLER_MAG_SHIFT]);
}

} // namespace sl
