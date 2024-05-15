#include "RenderCore.h"

#include "Core/Log.h"

namespace sl
{

void RenderCore::Init(GraphicsBackend backend)
{
	m_backend = backend;
	m_pRenderAPI = RenderAPI::Create();
}

void RenderCore::SetDefaultState()
{
	m_pRenderAPI->EnableBlend();
}

void RenderCore::ClearColor(float r, float g, float b, float a)
{
	m_pRenderAPI->SetClearColor(r, g, b, a);
	m_pRenderAPI->ClearColor();
}

void RenderCore::ClearDepth(float depth)
{
	m_pRenderAPI->SetClearDepth(depth);
	m_pRenderAPI->ClearDepth();
}

void RenderCore::ClearStencil(int  stencil)
{
	m_pRenderAPI->SetClearStencil(stencil);
	m_pRenderAPI->ClearStencil();
}

void RenderCore::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	m_pRenderAPI->SetViewport(x, y, width, height);
}

void RenderCore::Submit(VertexArray *pVertexArray, Shader *pShader)
{
	pVertexArray->Bind();
	pShader->Bind();
	m_pRenderAPI->DrawIndexed(pVertexArray->GetIndexBuffer()->GetCount());
}

} // namespace sl
