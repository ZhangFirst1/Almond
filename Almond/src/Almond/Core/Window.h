#pragma once

#include "ampch.h"

#include "Almond/Core/Core.h"
#include "Almond/Events/Event.h"

namespace Almond {

	// 窗口属性
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Almond Engine",
			unsigned int width = 1280, unsigned int height = 720)
			:Title(title), Width(width), Height(height){}
	};

	// Window窗口接口
	class ALMOND_API Window {
	public:
		// 每个窗口都有函数指针，用来接收事件
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// 设置窗口属性
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enable = 0) = 0;							// Vertical Sync（垂直同步）
		virtual bool IsSyne() const = 0;

		// 返回窗口（void*因为未必是GLFWwindow）
		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());	// 创建窗口
	};


}