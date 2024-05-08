#pragma once

#include "Layer/Layer.h"

namespace sl
{

class MouseScrollEvent;
class SceneViewportResizeEvent;
class CameraControllerEvent;
class MouseButtonReleaseEvent;

}

class CameraControllerLayer : public sl::Layer
{
public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(sl::Event &event) override;

	virtual void BeginFrame() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void EndFrame() override;

private:
	void UpdateMainCamera(float deltaTime);
	void UpdateFPSCamera(float deltaTime);
	void UpdateEditorCamera(float deltaTime);

	bool OnMouseScroll(sl::MouseScrollEvent &event);
	bool OnSceneViewportResize(sl::SceneViewportResizeEvent &event);
	bool OnCameraController(sl::CameraControllerEvent &event);
	bool OnMouseButtonRelease(sl::MouseButtonReleaseEvent &event);
};
