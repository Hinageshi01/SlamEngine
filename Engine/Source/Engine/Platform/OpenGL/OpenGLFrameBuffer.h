#pragma once

#include "RenderCore/Attachment.h"
#include "RenderCore/FrameBuffer.h"

namespace sl
{

class OpenGLFrameBuffer : public FrameBuffer
{
public:
	OpenGLFrameBuffer(std::vector<Texture2D *> textures, bool destroy = false);
	~OpenGLFrameBuffer();

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual void Resize(uint32_t width, uint32_t height) override;

	virtual uint32_t GetWidth() const override { return m_width; }
	virtual uint32_t GetHeight() const override { return m_height; }

	virtual void Clear(uint32_t attachmentIndex, const void *pClearData) const override;
	virtual int ReadPixel(uint32_t attachmentIndex, uint32_t x, uint32_t y) override;

	virtual uint32_t GetAttachmentHandle(size_t i = 0) const override;

private:
	void Create();

	uint32_t m_width = 0;
	uint32_t m_height = 0;
	uint32_t m_colorAttachmentCount = 0;

	std::vector<Attachment> m_attachments;
	bool m_destroyTextureWithFramebuffer;

	uint32_t m_handle = 0;
};

} // namespace sl
