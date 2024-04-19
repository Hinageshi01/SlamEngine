#include "OpenGLContext.h"

#include "Log/Log.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace sl
{

OpenGLContext::OpenGLContext(GLFWwindow *pWindow) : m_pWindow(pWindow)
{
	glfwMakeContextCurrent(m_pWindow);

	bool success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	SL_ENGINE_ASSERT_INFO(success, "GLAD init context failed!");

	SL_ENGINE_INFO("OpenGL Inof:");
	SL_ENGINE_INFO("  Vendor: {}", (const char *)glGetString(GL_VENDOR));
	SL_ENGINE_INFO("  Renderer: {}", (const char *)glGetString(GL_RENDERER));
	SL_ENGINE_INFO("  Version: {}", (const char *)glGetString(GL_VERSION));
}

void OpenGLContext::Bind()
{
	glfwMakeContextCurrent(m_pWindow);
}

void OpenGLContext::SwapBuffers()
{
	glfwSwapBuffers(m_pWindow);
}

} // namespace sl
