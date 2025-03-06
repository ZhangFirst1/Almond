#pragma once

namespace Almond {

	// 表示当前使用的是何种Renderer API
	enum class RendererAPI {
		None = 0, OpenGL = 1
	};

	class Renderer {
	public:
		inline static RendererAPI GetAPI() { return s_RendererAPI; }
	private:
		static RendererAPI s_RendererAPI;
	};


}