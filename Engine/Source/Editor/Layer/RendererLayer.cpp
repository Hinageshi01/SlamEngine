#include "RendererLayer.h"

#include "RenderCore/RenderCore.h"
#include "Resource/TextureResource.h"
#include "Scene/ECSWorld.h"

void RendererLayer::OnAttach()
{

}

void RendererLayer::OnDetach()
{

}

void RendererLayer::OnEvent(sl::Event &event)
{

}

void RendererLayer::BeginFrame()
{

}

void RendererLayer::OnUpdate(float deltaTime)
{

}

void RendererLayer::OnRender()
{
	m_viewProjectionCache = sl::ECSWorld::GetEditorCameraComponent().GetViewProjection();

	BasePass();
	EntityIDPass();
}

void RendererLayer::EndFrame()
{

}

void RendererLayer::BasePass()
{
	sl::RenderCore::GetMainFramebuffer()->Bind();
	sl::RenderCore::ClearColor(0.1f, 0.1f, 0.1f, 1.0);
	sl::RenderCore::ClearDepth(1.0f);

	auto group = sl::ECSWorld::GetRegistry().group<sl::RenderingComponent>(entt::get<sl::TransformComponent>);
	for (auto entity : group)
	{
		auto [rendering, transform] = group.get<sl::RenderingComponent, sl::TransformComponent>(entity);
		if (!rendering.m_pVertexArray || !rendering.m_pShader)
		{
			continue;
		}

		rendering.m_pShader->Bind();
		rendering.m_pShader->UploadUniform("u_modelViewProjection", m_viewProjectionCache * transform.GetTransform());
		rendering.m_pTextureResource->GetTexture()->Bind(0);

		sl::RenderCore::Submit(rendering.m_pVertexArray, rendering.m_pShader);
	}

	sl::RenderCore::GetMainFramebuffer()->Unbind();
}

void RendererLayer::EntityIDPass()
{
	sl::RenderCore::GetEntityIDFramebuffer()->Bind();
	sl::RenderCore::ClearDepth(1.0f);
	int entityIDClearData = -1;
	sl::RenderCore::GetEntityIDFramebuffer()->Clear(0, &entityIDClearData);

	auto view = sl::ECSWorld::GetRegistry().view<sl::RenderingComponent, sl::TransformComponent, sl::EntityIDComponent>();
	for (auto entity : view)
	{
		auto [rendering, transform, entityID] = view.get<sl::RenderingComponent, sl::TransformComponent, sl::EntityIDComponent>(entity);
		if (!rendering.m_pVertexArray)
		{
			continue;
		}

		entityID.m_pShader->Bind();
		entityID.m_pShader->UploadUniform("u_modelViewProjection", m_viewProjectionCache * transform.GetTransform());
		entityID.m_pShader->UploadUniform("u_entityID", (int)entity);

		// Pending: Should we separate mesh data from RenderingComponent?
		sl::RenderCore::Submit(rendering.m_pVertexArray, entityID.m_pShader);
	}

	sl::RenderCore::GetEntityIDFramebuffer()->Unbind();
}
