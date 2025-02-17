#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Window.h"
#include "Events/ApplicationEvent.h"

/*
	我们希望降低模块间耦合度，在Application类的实现中不应有Event，同样Window类也不应有Event和Application。
	在Application类中创建窗口类并绑定该窗口的回调函数，使用回调函数来触发相应的事件。
*/

namespace Almond {

	class ALMOND_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	Application* CreateApplication();

}

