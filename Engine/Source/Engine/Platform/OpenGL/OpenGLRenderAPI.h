#pragma once

#include "RenderCore/RenderAPI.h"

namespace sl
{

class OpenGLRenderAPI : public RenderAPI
{
public:
	OpenGLRenderAPI() = default;
	virtual ~OpenGLRenderAPI() = default;

	virtual void SetClearColor(float r, float g, float b, float a) override;
	virtual void SetClearDepth(float depth) override;
	virtual void SetClearStencil(int  stencil) override;
	virtual void Clear(uint8_t attachments) override;

	virtual void DefaultBlend() override;

	virtual void DrawIndexed(uint32_t count) override;
};

} // namespace sl
