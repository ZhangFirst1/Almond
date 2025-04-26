#include "ampch.h"
#include "Application.h"
#include "Input.h"
#include "Almond/Renderer/Renderer.h"
#include "Almond/Core/Timestep.h"

#include <GLFW/glfw3.h>

namespace Almond {
	// 成员函数指针需要附加一个对象实例来调用，使用bind将成员函数和对象实例绑定
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application() {
		AM_PROFILE_FUNCTION();

		AM_CORE_ASSERT(!s_Instance, "Application has existed!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());		// 创建窗口
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();							// 初始化ImGuiLayer
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application() {
		AM_PROFILE_FUNCTION();

		// Renderer::Shutdonw();
	}

	void Application::PushLayer(Layer* layer) {
		AM_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer) {
		AM_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e) {
		AM_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);				// 创建事件分发器
		// 如果传进的事件类e是WindowCloseEvent类，使用分发器内部的回调函数调用OnWindowClose
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));	
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
		// dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(OnMouseMoved));

		// 事件处理，从栈顶（vector尾）到栈底（vector头）
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run() {
		AM_PROFILE_FUNCTION();

		while (m_Running) {
			AM_PROFILE_SCOPE("Run Loop");
			// 计算帧间隔时间
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized) {									// 最小化后不应该遍历图层栈
				{
					AM_PROFILE_SCOPE("LayerStack OnUpdate");
					for (Layer* layer : m_LayerStack)				// 遍历图层栈
						layer->OnUpdate(timestep);
				}
				m_ImGuiLayer->Begin();
				{
					AM_PROFILE_SCOPE("LayerStack OnImGuiRenderer");
					for (Layer* layer : m_LayerStack)				// ImGui栈
						layer->OnImGuiRender();
				}
				m_ImGuiLayer->End();
			}

			m_Window->OnUpdate();
		}
	}

	// 接收关闭事件，更改运行状态，退出循环
	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		AM_PROFILE_FUNCTION();

		// 最小化
		if (m_Window->GetHeight() == 0 || m_Window->GetWidth() == 0) {
			m_Minimized = true;
			return false;
		}
	
		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

}