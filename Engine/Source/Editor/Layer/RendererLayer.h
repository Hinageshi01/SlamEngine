#pragma once

#include "Layer/Layer.h"

#include <glm/mat4x4.hpp>

class RendererLayer : public sl::Layer
{
public:
	RendererLayer() = default;
	virtual ~RendererLayer() override = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(sl::Event &event) override;

	virtual void BeginFrame() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void EndFrame() override;

private:
	void BasePass();
	void EntityIDPass();

	glm::mat4 m_viewProjectionCache;
};
