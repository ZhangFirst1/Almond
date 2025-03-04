#include "ampch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Almond {
	
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle) {
		AM_CORE_ASSERT(windowHandle, "windowHandle was no existed!");
	}

	void OpenGLContext::Init() {
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);	// 初始化glad
		AM_CORE_ASSERT(status, "Failed to initialize GLAD!");

		AM_CORE_INFO("OpenGL Info:");
		AM_CORE_INFO("	Vendor : {0}", glGetString(GL_VENDOR));
		AM_CORE_INFO("	Renderer : {0}", glGetString(GL_RENDERER));
		AM_CORE_INFO("	version : {0}", glGetString(GL_VERSION));

	}

	void OpenGLContext::SwapBuffers() {
		glfwSwapBuffers(m_WindowHandle);
	}

}