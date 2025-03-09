#include <ampch.h>
#include <Almond.h>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui/imgui.h"

class ExampleLayer : public Almond::Layer {
public:
	ExampleLayer()
		:Layer("Example"), m_Camera (-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_CameraRotation(0.0f) {
		m_VertexArray.reset(Almond::VertexArray::Create());					// 创建VAO

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			0.5f, -0.5f, 0.0f,  0.2f, 0.3f, 0.4f, 1.0f,
			0.0f,  0.5f, 0.0f,	0.8f, 0.3f, 0.4f, 1.0f
		};

		std::shared_ptr<Almond::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Almond::VertexBuffer::Create(vertices, sizeof(vertices)));	 // 创建vertex buffer

		// 设置顶点属性
		Almond::BufferLayout layout = {
			{ Almond::ShaderDataType::Float3, "a_Position"},
			{ Almond::ShaderDataType::Float4, "a_Color"}
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);				// VAO绑定vertex buffer

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Almond::IndexBuffer> indexBuffer;
		indexBuffer.reset(Almond::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);				// VAO绑定index buffer

		m_SquareVA.reset(Almond::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.5f,  0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f
		};
		std::shared_ptr<Almond::VertexBuffer> squareVB;
		// vertexBuffer.reset(Almond::VertexBuffer::Create(vertices, sizeof(vertices)));
		squareVB.reset(Almond::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		Almond::BufferLayout squareLayout = {
			{ Almond::ShaderDataType::Float3, "a_Position"}
		};
		squareVB->SetLayout(squareLayout);
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Almond::IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(Almond::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
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

			in vec3 v_Position;
			in vec4 v_Color;
			
			void main(){
				color = vec4(0.2f, 0.3f, 0.8f, 1.0f);
			}
		)";

		m_Shader.reset(new Almond::Shader(vertexSrc, fragmentSrc));
		m_BuleShader.reset(new Almond::Shader(blueVertexSrc, blueFragmentSrc));
	}

	void OnUpdate(Almond::Timestep ts) override {

		if (Almond::Input::IsKeypressed(AM_KEY_LEFT)) 
			m_CameraPosition.x -= m_CameraSpeed * ts;
		else if (Almond::Input::IsKeypressed(AM_KEY_RIGHT)) 
			m_CameraPosition.x += m_CameraSpeed * ts;
		
		if (Almond::Input::IsKeypressed(AM_KEY_DOWN)) 
			m_CameraPosition.y -= m_CameraSpeed * ts;
		else if (Almond::Input::IsKeypressed(AM_KEY_UP)) 
			m_CameraPosition.y += m_CameraSpeed * ts;

		if (Almond::Input::IsKeypressed(AM_KEY_A))
			m_CameraRotation -= m_RotationSpeed * ts;
		else if (Almond::Input::IsKeypressed(AM_KEY_D))
			m_CameraRotation += m_RotationSpeed * ts;

		Almond::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Almond::RendererCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Almond::Renderer::BeginScene(m_Camera);								// 负责每帧渲染前的环境设置

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);

				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Almond::Renderer::Submit(m_BuleShader, m_SquareVA, transform);		// 收集场景数据
			}
		}

		Almond::Renderer::Submit(m_Shader, m_VertexArray);

		Almond::Renderer::EndScene();										// 暂时为空
	}

	virtual void OnImGuiRender() override {

	}

	void OnEvent(Almond::Event& event) override{

	}

private:
	std::shared_ptr<Almond::Shader> m_Shader;				// Shader类指针
	std::shared_ptr<Almond::VertexArray> m_VertexArray;		// VAO指针

	std::shared_ptr<Almond::Shader> m_BuleShader;			// Shader类指针
	std::shared_ptr<Almond::VertexArray> m_SquareVA;		// VAO指针

	Almond::OrthographicCamera m_Camera;					// 摄像机
	glm::vec3 m_CameraPosition;								// 相机位置 
	float m_CameraSpeed = 2.0f;								// 相机移动速度
	float m_CameraRotation;									// 相机初始旋转角度
	float m_RotationSpeed = 30.0f;							// 相机旋转速度
};

class Sandbox : public Almond::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());

	}

	~Sandbox() {

	}

};

Almond::Application* Almond::CreateApplication() {
	return new Sandbox();
}