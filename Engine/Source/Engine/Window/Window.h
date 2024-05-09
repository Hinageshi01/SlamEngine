#pragma once

#include "Event/Event.h"
#include "Layer/Layer.h"
#include "RenderCore/RenderContext.h"

namespace sl
{

// TODO: WindowLayer
class Window final
{
public:
	Window() = delete;

	Window(std::string title, uint32_t width, uint32_t height);
	~Window();

	void BegineFrame();
	void EndFrame();

	void *GetNativeWindow() const { return m_pNativeWindow; }

	void SetWidth(uint32_t width) { m_width = width; }
	uint32_t &GetWidth() { return m_width; }
	uint32_t GetWidth() const { return m_width; }

	void SetHeight(uint32_t height) { m_height = height; }
	uint32_t &GetHeight() { return m_height; }
	uint32_t GetHeight() const { return m_height; }

	void SetVSync(bool VSync);
	bool &GetIsVSync() { return m_isVSync; }
	bool GetIsVSync() const { return m_isVSync; }

	void CaptureCursor();
	void ReleaseCursor();

	void SetEventCallback(EventCallback fun) { m_eventCallback = std::move(fun); }
	void DespatchEvent(Event &event) { m_eventCallback(event); }

private:
	void SetCallbacks();

	void *m_pNativeWindow = nullptr;
	RenderContext *m_pRenderContext = nullptr;

	std::string m_title = "Default Title";
	uint32_t m_width = 1280;
	uint32_t m_height = 720;
	bool m_isVSync = true;
	bool m_isCursorCaptured = false;

	EventCallback m_eventCallback = nullptr;
};

} // namespace sl
