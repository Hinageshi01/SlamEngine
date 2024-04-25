#pragma once

// TEMPORARY
#include "Camera/Camera.h"
#include "RenderCore/Shader.h"
#include "RenderCore/Texture.h"
#include "RenderCore/VertexArray.h"

#include "Core/Defines.h"
#include "Core/Timmer.h"
#include "Layer/LayerStack.h"
#include "Window/Window.h"

struct EditorInitor
{
	std::string title;
	uint32_t m_width;
	uint32_t m_height;
	sl::GraphicsBackend m_backend;
};

class Editor final
{
public:
	Editor() = delete;
	Editor(const Editor &) = delete;
	Editor &operator=(const Editor &) = delete;
	Editor(Editor &&) = delete;
	Editor &operator=(Editor &&) = delete;

	Editor(EditorInitor initor);
	~Editor();

	void Update();

private:
	void Init(EditorInitor initor);
	void Shutdown();

	void BegineFrame();
	void Render();
	void EndFrame();

	void OnEvent(sl::Event &event);
	bool OnWindowClose(sl::WindowCloseEvent &event);
	bool OnWindowResized(sl::WindowResizeEvent &event);

	bool m_isRunning = true;
	bool m_isMinimized = false;

	sl::Timmer m_timmer;
	sl::LayerStack m_layerStack;
	sl::Window *m_pWindow = nullptr;

	// TEMPORARY
	sl::VertexArray *m_pVertexArray;
	sl::Texture2D *m_pTextureJoucho;
	sl::Texture2D *m_pTextureLogo;
	sl::Shader *m_pShader;
	sl::Camera m_camera;
};
