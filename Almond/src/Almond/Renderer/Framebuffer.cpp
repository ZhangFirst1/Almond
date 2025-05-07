#include "ampch.h"
#include "Framebuffer.h"

#include "Almond/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Almond {

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec) {
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: AM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLFramebuffer>(spec);
		}

		AM_CORE_ASSERT(false, "RendererAPI is still unkonw!");
		return nullptr;
	}

}