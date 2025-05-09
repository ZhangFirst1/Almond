#pragma once

#include "ampch.h"

#include "Almond/Core/Core.h"
#include "Almond/Events/Event.h"

namespace Almond {

	// 窗口属性
	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Almond Engine",
			uint32_t width = 1600, uint32_t height = 900)
			:Title(title), Width(width), Height(height){}
	};

	// Window窗口接口
	class ALMOND_API Window {
	public:
		// 每个窗口都有函数指针，用来接收事件
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		// 设置窗口属性
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enable = 0) = 0;							// Vertical Sync（垂直同步）
		virtual bool IsSyne() const = 0;

		// 返回窗口（void*因为未必是GLFWwindow）
		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());	// 创建窗口
	};


}