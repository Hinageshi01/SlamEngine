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

	uint32_t GetSceneViewportSizeX() const { return m_viewportSizeX; }
	uint32_t GetSceneViewportSizeY() const { return m_viewportSizeY; }

	void SetEventCallback(sl::EventCallback fun) { m_eventCallback = std::move(fun); }

private:
	void ShowDebugPanels();
	void ShowMenuBar();
	void ShowEntityList();
	void ShowLog();
	void ShowInfo(float deltaTime);

	// Just used inside ImGuiLayer
	template<class T, class Fun>
	void DrawComponent(const char *label, Fun uiFunction);
	template<class T>
	void AddComponent(const char *label);
	void StartWithText(std::string text);

	void ShowDetails();
	void ShowSceneViewport();

	// Dock space
	// ImGuiDockNodeFlags m_dockSpaceFlag = ImGuiDockNodeFlags_None
	int m_dockSpaceFlag = 0;
	
	// Menubar
	bool m_debugImGuiDemo = false;
	bool m_styleEditor = false;
	bool m_debugItemPicker = false;
	bool m_debugIDStack = false;

	// Entity list
	sl::Entity m_selectedEntity;

	// Details
	float m_maxTextSize = 0.0f;

	// Scene viewport
	uint32_t m_viewportSizeX = 0;
	uint32_t m_viewportSizeY = 0;
	
	sl::EventCallback m_eventCallback;
};
