#pragma once

#include "Core/Defines.h"
#include "Layer/Layer.h"

namespace sl
{

class MouseScrollEvent;
class SceneViewportResizeEvent;
class CameraActivateEvent;
class MouseButtonReleaseEvent;
class KeyReleaseEvent;

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
	bool OnCameraActivate(sl::CameraActivateEvent &event);
	bool OnMouseButtonRelease(sl::MouseButtonReleaseEvent &event);
	bool OnKeyRelease(sl::KeyReleaseEvent &event);

	sl::CameraControllerMode m_controllerMode = sl::CameraControllerMode::None;
	bool m_isRotating = false;
	bool m_isMoving = false;
};
