#include "ampch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Almond {

	// 根据不同平台返回不同的vertex buffer
	VertexBuffer* VertexBuffer::Create(float* vertices, unsigned int size) {
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None: AM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::OpenGL: return new OpenGLVertexBuffer(vertices, size);
		}

		AM_CORE_ASSERT(false, "RendererAPI is still unkonw!");
		return nullptr;
	}

	// 根据不同平台返回不同的index buffer
	IndexBuffer* IndexBuffer::Create(unsigned int* indices, unsigned int size) {
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None: AM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::OpenGL: return new OpenGLIndexBuffer(indices, size);
		}

		AM_CORE_ASSERT(false, "RendererAPI is still unkonw!");
		return nullptr;
	}

}