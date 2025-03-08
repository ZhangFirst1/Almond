#pragma once

#include "RendererAPI.h"
#include "RendererCommand.h"

#include "Almond/Renderer/Shader.h"
#include "Almond/Renderer/OrthographicCamera.h"

namespace Almond {

	class Renderer {
	public:
		static void BeginScene(OrthographicCamera& camera);			// 负责每帧渲染前的环境设置
		static void EndScene();										// 对收集到的场景数据进行优化
		// 收集场景数据，同时收集渲染命令，提交渲染命令到队列里
		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }	// 获取RendererAPI

	private:
		struct SceneData {
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};	

}
