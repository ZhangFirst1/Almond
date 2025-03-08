﻿#include "ampch.h"
#include "Application.h"
#include "Input.h"
#include "Almond/Renderer/Renderer.h"


namespace Almond {
	// 成员函数指针需要附加一个对象实例来调用，使用bind将成员函数和对象实例绑定
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application() 
		:m_Camera(-1.6f, 1.6f, -0.9f, 0.9f){
		AM_CORE_ASSERT(!s_Instance, "Application has existed!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());		// 创建窗口，此处m_Window是Application类中的，与WindowsWindow类中不同
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();							// 初始化ImGuiLayer
		PushOverlay(m_ImGuiLayer);
		
		m_VertexArray.reset(VertexArray::Create());					// 创建VAO

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			0.5f, -0.5f, 0.0f,  0.2f, 0.3f, 0.4f, 1.0f,
			0.0f,  0.5f, 0.0f,	0.8f, 0.3f, 0.4f, 1.0f
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));	 // 创建vertex buffer

		// 设置顶点属性
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position"},
			{ ShaderDataType::Float4, "a_Color"}
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);				// VAO绑定vertex buffer

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);				// VAO绑定index buffer

		m_SquareVA.reset(VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f, 
			0.5f,  0.5f, 0.0f,	
			-0.5f, 0.5f, 0.0f
		};
		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));	 // 创建vertex buffer

		BufferLayout squareLayout = {
			{ ShaderDataType::Float3, "a_Position"}
		};
		squareVB->SetLayout(squareLayout);
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIndexBuffer);				// VAO绑定index buffer

		// Shader
		std::string vertexSrc = R"(
			#version 330 core			
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;
			
			void main(){
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0f);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core			
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;
			
			void main(){
				color = vec4(v_Position * 0.5 + 0.5, 1.0f);
				color = v_Color;
			}
		)";

		std::string blueVertexSrc = R"(
			#version 330 core			
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			
			void main(){
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0f);
			}
		)";

		std::string blueFragmentSrc = R"(
			#version 330 core			
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;
			
			void main(){
				color = vec4(0.2f, 0.3f, 0.8f, 1.0f);
			}
		)";

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
		m_BuleShader.reset(new Shader(blueVertexSrc, blueFragmentSrc));
	}

	Application::~Application() {

	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer) {
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);				// 创建事件分发器
		// 如果传进的事件类e是WindowCloseEvent类，使用分发器内部的回调函数调用OnWindowClose
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));	

		// 事件处理，从栈顶（vector尾）到栈底（vector头）
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}

	}

	void Application::Run() {
		while (m_Running) {
			RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			RendererCommand::Clear();
			
			m_Camera.SetRotation(45.0f);

			Renderer::BeginScene(m_Camera);					// 负责每帧渲染前的环境设置

			Renderer::Submit(m_BuleShader, m_SquareVA);		// 收集场景数据
			Renderer::Submit(m_Shader, m_VertexArray);

			Renderer::EndScene();							// 暂时为空

			for (Layer* layer : m_LayerStack)				// 遍历图层栈
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)				// ImGui栈
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	// 接收关闭事件，更改运行状态，退出循环
	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}

}