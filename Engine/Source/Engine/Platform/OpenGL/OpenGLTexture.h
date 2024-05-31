#pragma once

#include "Core/Defines.h"
#include "RenderCore/Texture.h"

namespace sl
{

class OpenGLTexture2D : public Texture2D
{
public:
	OpenGLTexture2D(uint32_t width, uint32_t height, bool mipmap, TextureFormat format, uint32_t flags = 0, const void *pData = nullptr);
	virtual ~OpenGLTexture2D() override;

	virtual uint32_t GetHandle() const override { return m_handle; }

	virtual uint32_t GetWidth() const override { return m_width; }
	virtual uint32_t GetHeight() const override { return m_height; }
	virtual TextureFormat GetFormat() const  override { return m_format; }

	virtual void Clear(const void *pClearData) const override;
	virtual void Resize(uint32_t width, uint32_t height, const void *pData = nullptr) override;

	virtual void Bind(uint32_t slot) const override;

private:
	void Create(const void *pData = nullptr);

	uint32_t m_width = 0;
	uint32_t m_height = 0;

	bool m_hasMip;
	TextureFormat m_format;
	uint32_t m_flags = 0;

	uint32_t m_handle = 0;
};

} // namespace sl
