#pragma once

#include "Almond/Renderer/GraphicsContext.h"

class GLFWwindow;

namespace Almond {
	// OpenGL上下文
	class OpenGLContext : public GraphicsContext {
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};

}