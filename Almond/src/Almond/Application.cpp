#include "ampch.h"
#include "Application.h"

#include "Almond/Events/ApplicationEvent.h"
#include "Almond/Log.h"

namespace Almond {

	Application::Application() {
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application() {

	}

	void Application::Run() {
		while (m_Running) {
			m_Window->OnUpdate();
		}
	}

}