#pragma once

#include "LayerStack/Layer.h"

class ResourceManagerLayer : public sl::Layer
{
public:
	ResourceManagerLayer();
	virtual ~ResourceManagerLayer() override;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(sl::Event &event) override;

	virtual void BeginFrame() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void EndFrame() override;
};
