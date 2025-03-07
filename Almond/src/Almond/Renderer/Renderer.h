#pragma once

#include "RendererAPI.h"

namespace Almond {

	class Renderer {
	public:
		static void BeginScene();
		static void EndScene();
		// 提交vertexArray并绑定，由RendererCommand::DrawIndexed具体实现DrawElement
		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }	// 获取RendererAPI
	};

}
