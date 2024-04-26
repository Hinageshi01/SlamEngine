#include "Input.h"

#include "Window/Window.h"

#include <GLFW/glfw3.h>

namespace sl
{

bool Input::IsKeyPressed(int key)
{
	GLFWwindow *pWindow = Window::GetInstance().GetWindow();
	auto state = glfwGetKey(pWindow, key);
	return GLFW_PRESS == state;
}

bool Input::IsMouseButtonPressed(int button)
{
	GLFWwindow *pWindow = Window::GetInstance().GetWindow();
	auto state = glfwGetMouseButton(pWindow, button);
	return GLFW_PRESS == state;
}

glm::vec2 Input::GetMousePos()
{
	GLFWwindow *pWindow = Window::GetInstance().GetWindow();
	double posX, posY;
	glfwGetCursorPos(pWindow, &posX, &posY);
	return { float(posX), float(posY)};
}

} // namespace sl
