#pragma once

#include "RenderCore/BackendInfo.h"
#include "RenderCore/FrameBuffer.h"
#include "RenderCore/RenderAPI.h"
#include "RenderCore/Shader.h"
#include "RenderCore/Texture.h"
#include "RenderCore/UniformBuffer.h"
#include "RenderCore/VertexArray.h"

#include <memory>

namespace sl
{

class RenderCore final
{
public:
	RenderCore() = delete;
	RenderCore(const RenderCore &) = delete;
	RenderCore &operator=(const RenderCore &) = delete;
	RenderCore(RenderCore &&) = delete;
	RenderCore &operator=(RenderCore &&) = delete;
	~RenderCore() = delete;

	static void SetBackend(GraphicsBackend backend);
	static GraphicsBackend GetBackend() { return m_backend; }

	static void Init();

	static uint32_t GetMaxTextureSize() { return m_info.m_maxTextureSize; }
	static uint32_t GetMaxFramebufferSize() { return m_info.m_maxFramebufferSize; }
	static uint32_t GetMaxFramebufferColorAttachmentCount() { return m_info.m_maxFramebufferColorAttachmentCount; }

	static void SetMainFramebuffer(FrameBuffer *pBuffer) { m_pMainFrameBuffer.reset(pBuffer); }
	static FrameBuffer *GetMainFramebuffer() { return m_pMainFrameBuffer.get(); }

	static void SetEntityIDFramebuffer(FrameBuffer *pBuffer) { m_pEntityIDFrameBuffer.reset(pBuffer); }
	static FrameBuffer *GetEntityIDFramebuffer() { return m_pEntityIDFrameBuffer.get(); }

	static void SetUniformBuffer(uint32_t bindingPoint, UniformBuffer *pUniformBuffer);
	static UniformBuffer *GetUniformBuffer(uint32_t bindingPoint);

	static void SetDefaultState();

	static void ClearColor(float r, float g, float b, float a);
	static void ClearDepth(float depth);
	static void ClearStencil(int stencil);

	static void Culling(CullingMode mode);

	static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

	static void Submit(VertexArray *pVertexArray, Shader *pShader);

private:
	inline static GraphicsBackend m_backend = GraphicsBackend::None;
	inline static BackendInfo m_info = {};
	
	inline static std::unique_ptr<RenderAPI> m_pRenderAPI;
	inline static std::unique_ptr<FrameBuffer> m_pMainFrameBuffer;
	inline static std::unique_ptr<FrameBuffer> m_pEntityIDFrameBuffer;
	inline static std::map<uint32_t, std::unique_ptr<UniformBuffer>> m_UniformBuffers;
};

} // namespace sl
