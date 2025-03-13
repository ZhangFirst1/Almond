#include "ampch.h"
#include "OpenGLArrayVertex.h"

#include <glad/glad.h>

namespace Almond {

	// 根据类型返回对应的OpenGL类型
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
		switch (type)
		{
		case Almond::ShaderDataType::Float:		return GL_FLOAT;
		case Almond::ShaderDataType::Float2:	return GL_FLOAT;
		case Almond::ShaderDataType::Float3:	return GL_FLOAT;
		case Almond::ShaderDataType::Float4:	return GL_FLOAT;
		case Almond::ShaderDataType::Mat3:		return GL_FLOAT;
		case Almond::ShaderDataType::Mat4:		return GL_FLOAT;
		case Almond::ShaderDataType::Int:		return GL_INT;
		case Almond::ShaderDataType::Int2:		return GL_INT;
		case Almond::ShaderDataType::Int3:		return GL_INT;
		case Almond::ShaderDataType::Int4:		return GL_INT;
		case Almond::ShaderDataType::Bool:		return GL_BOOL;
		}
		AM_CORE_ASSERT(false, "Unknow ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray() {
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexbuffer)
	{
		AM_CORE_ASSERT(vertexbuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexbuffer->Bind();

		// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr)
		uint32_t index = 0;
		const BufferLayout& layout = vertexbuffer->GetLayout();
		for (const auto& element : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}
		m_VertexBuffers.push_back(vertexbuffer);	// 加入vertexbuffer指针列表
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexbuffer)
	{
		glBindVertexArray(m_RendererID);
		indexbuffer->Bind();

		m_IndexBuffer = indexbuffer;
	}

}