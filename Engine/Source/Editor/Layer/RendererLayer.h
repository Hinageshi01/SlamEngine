#pragma once

#include "LayerStack/Layer.h"

#include <glm/mat4x4.hpp>

namespace sl
{

class SceneViewportResizeEvent;
class UniformBuffer;

}

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

	void SetUniformBuffer(sl::UniformBuffer *pBuffer) { m_pUniformBuffer = pBuffer; }
	sl::UniformBuffer *GetUniformBuffer() { return m_pUniformBuffer; }

private:
	void BasePass();
	void EntityIDPass();

	bool OnSceneViewportResize(sl::SceneViewportResizeEvent &event);

	sl::UniformBuffer *m_pUniformBuffer = nullptr;
};
