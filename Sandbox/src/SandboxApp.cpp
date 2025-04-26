#include <ampch.h>
#include <Almond.h>
#include <Almond/Core/EntryPoint.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"
#include "imgui/imgui.h"
#include "Sandbox2D.h"

class ExampleLayer : public Almond::Layer {
public:
	ExampleLayer()
		:Layer("Example"), m_CameraController (1280.0f / 720.0f, true) {
		m_VertexArray = Almond::VertexArray::Create();					// 创建VAO

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			0.5f, -0.5f, 0.0f,  0.2f, 0.3f, 0.4f, 1.0f,
			0.0f,  0.5f, 0.0f,	0.8f, 0.3f, 0.4f, 1.0f
		};

		Almond::Ref<Almond::VertexBuffer> vertexBuffer = Almond::VertexBuffer::Create(vertices, sizeof(vertices));	 // 创建vertex buffer

		// 设置顶点属性
		Almond::BufferLayout layout = {
			{ Almond::ShaderDataType::Float3, "a_Position"},
			{ Almond::ShaderDataType::Float4, "a_Color"}
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);				// VAO绑定vertex buffer

		uint32_t indices[3] = { 0, 1, 2 };
		Almond::Ref<Almond::IndexBuffer> indexBuffer = Almond::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);				// VAO绑定index buffer

		m_SquareVA = Almond::VertexArray::Create();

		float squareVertices[4 * 5] = {
			-0.5f,-0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
		};

		Almond::Ref <Almond::VertexBuffer> squareVB = Almond::VertexBuffer::Create(squareVertices, sizeof(squareVertices));

		Almond::BufferLayout squareLayout = {
			{ Almond::ShaderDataType::Float3, "a_Position"},
			{ Almond::ShaderDataType::Float2, "a_TexCoord"}
		};
		squareVB->SetLayout(squareLayout);
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Almond::Ref<Almond::IndexBuffer> squareIndexBuffer = Almond::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_SquareVA->SetIndexBuffer(squareIndexBuffer);				// VAO绑定index buffer

		// Shader
		std::string vertexSrc = R"(
			#version 330 core			
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;
			
			void main(){
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0f);
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
			uniform mat4 u_Transform;
			

			out vec3 v_Position;
			
			void main(){
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0f);
				
			}
		)";

		std::string blueFragmentSrc = R"(
			#version 330 core			
			
			layout(location = 0) out vec4 color;
			
			uniform vec3 u_Color;

			in vec3 v_Position;
			
			void main(){
				color = vec4(u_Color, 1.0f);
			}
		)";

		m_Shader = Almond::Shader::Create("VertexPosShader", vertexSrc, fragmentSrc);
		m_BlueShader = Almond::Shader::Create("FlatPosShader", blueVertexSrc, blueFragmentSrc);
		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = Almond::Texture2D::Create("assets/textures/wood.png");
		m_QUTTexture = Almond::Texture2D::Create("assets/textures/QUT.png");

		std::dynamic_pointer_cast<Almond::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Almond::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Almond::Timestep ts) override {
		// Update
		m_CameraController.OnUpdate(ts);

		// Render
		Almond::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Almond::RendererCommand::Clear();

		Almond::Renderer::BeginScene(m_CameraController.GetCamera());						// 负责每帧渲染前的环境设置

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Almond::OpenGLShader>(m_BlueShader)->Bind();
		std::dynamic_pointer_cast<Almond::OpenGLShader>(m_BlueShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Almond::Renderer::Submit(m_BlueShader, m_SquareVA, transform);		// 收集场景数据
			}
		}

		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		Almond::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));		// 收集场景数据
		m_QUTTexture->Bind();
		Almond::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));		// 收集场景数据

		// Triangle
		// Almond::Renderer::Submit(m_Shader, m_VertexArray);

		Almond::Renderer::EndScene();										// 暂时为空
	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Almond::Event& event) override{
		m_CameraController.OnEvent(event);
	}

private:
	Almond::ShaderLibrary m_ShaderLibrary;				// ShaderLibrary类指针
	Almond::Ref<Almond::Shader> m_Shader;				// Shader类指针
	Almond::Ref<Almond::VertexArray> m_VertexArray;		// VAO指针

	Almond::Ref<Almond::Shader> m_BlueShader;			// Shader类指针
	Almond::Ref<Almond::VertexArray> m_SquareVA;		// VAO指针

	Almond::Ref<Almond::Texture> m_Texture;				// 纹理指针
	Almond::Ref<Almond::Texture> m_QUTTexture;			// 纹理指针

	Almond::OrthographicCameraController m_CameraController;// 摄像机控制器

	glm::vec3 m_SquareColor = { 0.1f, 0.2f, 0.8f };
};

class Sandbox : public Almond::Application {
public:
	Sandbox() {
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}

	~Sandbox() {

	}

};

Almond::Application* Almond::CreateApplication() {
	return new Sandbox();
}