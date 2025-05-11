#pragma once
#include "Almond/Renderer/VertexArray.h"

namespace Almond {
	// OpenGL VAO
	class OpenGLVertexArray : public VertexArray{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		// 为VAO绑定VBO和EBO
		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexbuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexbuffer) override;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const  override { return m_VertexBuffers; }
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }

		static VertexArray* Create();
	private:
		uint32_t m_RendererID;
		uint32_t m_VertexBufferIndex = 0;

		std::vector<Ref<VertexBuffer>> m_VertexBuffers;		// 添加到VAO的所有VBO的指针
		Ref<IndexBuffer> m_IndexBuffer;						// 添加到VAO的EBO的指针
	};

}