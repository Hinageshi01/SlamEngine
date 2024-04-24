#include "Camera.h"

#include "Core/Log.h"
#include "Window/Input.h"

#include <glm/trigonometric.hpp>

#include <array>

namespace sl
{

namespace
{

static std::array<uint32_t, 6> CamraMoveKey =
{
	SL_KEY_W, SL_KEY_A, SL_KEY_S, SL_KEY_D,
	SL_KEY_E, SL_KEY_Q,
};

}

void Camera::Update(float deltaTime)
{
	// TODO: Check is mouse in scene view.

	// Camera FPS mode.
	if (Input::IsMouseButtonPressed(SL_MOUSE_BUTTON_2))
	{
		UpdateFPSCamera(deltaTime);
	}
	// Camera editor mode.
	else if (Input::IsKeyPressed(SL_KEY_LEFT_ALT) && Input::IsMouseButtonPressed(SL_MOUSE_BUTTON_1))
	{
		UpdateEditorCamera(deltaTime);
	}
	// Camera unused.
	else
	{
		if (m_isActive)
		{
			m_pWindow->EnableCursor();
			m_isActive = false;
		}
	}
}

void Camera::UpdateFPSCamera(float deltaTime)
{
	if (!m_isActive)
	{
		m_mouseLastPos = Input::GetMousePos();
		m_lastMoveDir = m_data.GetFrontDir();
		m_pWindow->DisableCursor();
		m_isActive = true;
	}

	// Rotation
	{
		glm::vec2 crtPos = Input::GetMousePos();
		float offsetX = crtPos.x - m_mouseLastPos.x;
		float offsetY = m_mouseLastPos.y - crtPos.y;
		m_mouseLastPos = Input::GetMousePos();

		m_data.GetRotation() += glm::vec3{ offsetY, offsetX, 0.0f }  * m_rotateSpeed * deltaTime;
		m_data.GetRotation().x = std::min(m_data.GetRotation().x, glm::radians(89.9f));
		m_data.GetRotation().x = std::max(m_data.GetRotation().x, glm::radians(-89.9f));
	}

	// TODO: It's better to use bit mask and shift to avoid repeated call of Input::IsKeyPressed
	bool isMoving = false;
	for (auto key : CamraMoveKey)
	{
		if (Input::IsKeyPressed(key))
		{
			isMoving = true;
			break;
		}
	}
	
	glm::vec3 finalMoveDir;
	if (isMoving)
	{
		if (!m_isMoving)
		{
			// Start moving
			m_acceleration = m_maxMoveSpeed / 50.0f;
			m_isMoving = true;
		}

		finalMoveDir = glm::vec3{ 0.0f, 0.0f, 0.0f };
		if (Input::IsKeyPressed(SL_KEY_W))
		{
			finalMoveDir += m_data.GetFrontDir();
		}
		if (Input::IsKeyPressed(SL_KEY_A))
		{
			finalMoveDir -= m_data.GetRightDir();
		}
		if (Input::IsKeyPressed(SL_KEY_S))
		{
			finalMoveDir -= m_data.GetFrontDir();
		}
		if (Input::IsKeyPressed(SL_KEY_D))
		{
			finalMoveDir += m_data.GetRightDir();
		}
		if (Input::IsKeyPressed(SL_KEY_E))
		{
			finalMoveDir += CameraData::WorldUp;
		}
		if (Input::IsKeyPressed(SL_KEY_Q))
		{
			finalMoveDir -= CameraData::WorldUp;
		}
		finalMoveDir = glm::normalize(finalMoveDir);
		m_lastMoveDir = finalMoveDir;
	}
	else
	{
		if (m_isMoving)
		{
			// Stop moving
			m_acceleration = -m_maxMoveSpeed / 50.0f;
			m_isMoving = false;
		}

		finalMoveDir = m_lastMoveDir;
	}

	m_moveSpeed += m_acceleration;
	m_moveSpeed = std::max(0.0f, m_moveSpeed);
	m_moveSpeed = std::min(m_maxMoveSpeed, m_moveSpeed);
	m_moveSpeedKeyShiftMultiplier = Input::IsKeyPressed(SL_KEY_LEFT_SHIFT) ? 2.5f : 1.0f;
	float finalMoveSpeed = m_moveSpeed * m_moveSpeedMouseScrollMultiplier * m_moveSpeedKeyShiftMultiplier * deltaTime;

	m_data.GetPosition() += finalMoveDir * finalMoveSpeed;
	m_data.Dirty();
}

void Camera::UpdateEditorCamera(float deltaTime)
{
	// TODO
	if (!m_isActive)
	{
		m_mouseLastPos = Input::GetMousePos();
		m_pWindow->DisableCursor();
		m_isActive = true;
	}
}

void Camera::OnEvent(Event &event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_CALLBACK(Camera::OnMouseScrolled));
	dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_CALLBACK(Camera::OnWindowResized));
}

bool Camera::OnMouseScrolled(MouseScrolledEvent &event)
{
	m_moveSpeedMouseScrollMultiplier += event.GetOffsetY() * 0.2f;
	m_moveSpeedMouseScrollMultiplier = std::max(0.125f, m_moveSpeedMouseScrollMultiplier);
	m_moveSpeedMouseScrollMultiplier = std::min(8.0f, m_moveSpeedMouseScrollMultiplier);

	return true;
}

bool Camera::OnWindowResized(WindowResizeEvent &event)
{
	m_data.SetAspect((float)event.GetWidth() / (float)event.GetHeight());
	m_data.Dirty();
	return true;
}

} // namespace sl
