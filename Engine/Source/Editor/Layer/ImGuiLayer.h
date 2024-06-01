#pragma once

#include "Event/Event.h"
#include "Layer/Layer.h"
#include "Scene/ECSWorld.h"

#include <glm/vec2.hpp>

class ImGuiLayer : public sl::Layer
{
public:
	ImGuiLayer();
	virtual ~ImGuiLayer() override;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(sl::Event &event) override;

	virtual void BeginFrame() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void EndFrame() override;

	void SetEventCallback(sl::EventCallback fun) { m_eventCallback = std::move(fun); }

private:
	void ShowDebugPanel();
	void ShowToolOverlay();
	void ShowOrientationOverlay();

	void ShowMenuBar();
	void ShowInfo(float deltaTime);
	void ShowLog();
	void ShowEntityList();

	// Just used inside ImGuiLayer
	template<class T, class Fun>
	void DrawComponent(const char *label, Fun uiFunction);
	template<class T>
	void AddComponent(const char *label);
	void StartWithText(std::string text);
	void ShowDetails();

	void ShowSceneViewport();
	void ShowImGuizmoTransform();
	void SetCameraControllerMode();
	void MousePick();

	// Dock space
	int m_dockSpaceFlag = 0; // ImGuiDockNodeFlags m_dockSpaceFlag = ImGuiDockNodeFlags_None
	
	// Debug panel
	bool m_debugImGuiDemo = false;
	bool m_debugStyleEditor = false;
	bool m_debugItemPicker = false;
	bool m_debugIDStack = false;

	// Entity list and many other panels
	sl::Entity m_selectedEntity;

	// Details
	float m_maxTextSize = 0.0f;

	// Scene viewport
	uint32_t m_sceneViewportWindowPosX = 0;
	uint32_t m_sceneViewportWindowPosY = 0;
	uint32_t m_sceneViewportSizeX = 0;
	uint32_t m_sceneViewportSizeY = 0;

	// ImGuizmo
	int m_imguizmoMode = 7; // ImGuizmo::OPERATION m_imguizmoMode = ImGuizmo::OPERATION::TRANSLATE
	
	sl::EventCallback m_eventCallback;
};
