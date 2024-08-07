#include "Input.h"

#include "Window/Window.h"

#include <GLFW/glfw3.h>

namespace sl
{

using WindowTypePtr = GLFWwindow *;

bool Input::IsKeyPressed(int key)
{
	auto state = glfwGetKey(static_cast<WindowTypePtr>(m_pWindow), key);
	return state == GLFW_PRESS;
}

bool Input::IsMouseButtonPressed(int button)
{
	auto state = glfwGetMouseButton(static_cast<WindowTypePtr>(m_pWindow), button);
	return state == GLFW_PRESS;
}

glm::vec2 Input::GetMousePos()
{
	double posX, posY;
	glfwGetCursorPos(static_cast<WindowTypePtr>(m_pWindow), &posX, &posY);
	return glm::vec2{ float(posX), float(posY)};
}

glm::vec2 Input::GetGlobalMousePos()
{
	int windowPosX, windowPosY;
	double cursorPosX, cursorPosY;
	glfwGetWindowPos(static_cast<WindowTypePtr>(m_pWindow), &windowPosX, &windowPosY);
	glfwGetCursorPos(static_cast<WindowTypePtr>(m_pWindow), &cursorPosX, &cursorPosY);

	return glm::vec2{ (float)windowPosX + (float)cursorPosX, (float)windowPosY + (float)cursorPosY };
}

} // namespace sl
