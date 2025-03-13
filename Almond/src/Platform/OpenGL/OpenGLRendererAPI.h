#pragma once

#include "Almond/Renderer/RendererAPI.h"

namespace Almond {
	// 实现具体的渲染代码
	class OpenGLRendererAPI : public RendererAPI{
	public:
		virtual void Init() override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;

	};

}