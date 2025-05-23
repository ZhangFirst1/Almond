﻿#pragma once

#include "Core.h"
#include "Almond/Events/Event.h"
#include "Almond/Core/Window.h"
#include "Almond/Events/ApplicationEvent.h"
#include "LayerStack.h"
#include "Almond/ImGui/ImGuiLayer.h"


/*
	我们希望降低模块间耦合度，在Application类的实现中不应有Event，同样Window类也不应有Event和Application。
	在Application类中创建窗口类并绑定该窗口的回调函数，使用回调函数来触发相应的事件。
*/

namespace Almond {

	class ALMOND_API Application
	{
	public:
		Application(const std::string& name = "Almond App");
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);							// 处理事件

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		void Close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		inline Window& GetWindow(){ return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		std::unique_ptr<Window> m_Window;				// 窗口指针
		ImGuiLayer* m_ImGuiLayer;						// ImGui指针
		bool m_Running = true;							// 运行状态
		bool m_Minimized = false;						// 最小化
		LayerStack m_LayerStack;						// 图层栈帧

		float m_LastFrameTime = 0.0f;					// 渲染上一帧的时间

		static Application* s_Instance;					// Application类单例
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}

