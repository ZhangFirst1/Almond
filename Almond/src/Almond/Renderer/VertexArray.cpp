#include "ampch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLArrayVertex.h"

namespace Almond {

	VertexArray* VertexArray::Create() {
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: AM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLVertexArray();
		}

		AM_CORE_ASSERT(false, "RendererAPI is still unkonw!");
		return nullptr;
	}

}