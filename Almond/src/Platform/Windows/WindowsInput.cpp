#include "ampch.h"
#include "Almond/Core/Input.h"
#include "Almond/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Almond {

	// 轮询是否按下keycode按键
	bool Input::IsKeypressed(int keycode) {
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(int button) {
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> Input::GetMousePosition() {
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return std::make_pair<float, float>((float)xpos, (float)ypos);
	}

	float Input::GetMouseX() {
		auto [x, y] = GetMousePosition();
		return x;
	}

	float Input::GetMouseY() {
		auto [x, y] = GetMousePosition();
		return y;
	}

}