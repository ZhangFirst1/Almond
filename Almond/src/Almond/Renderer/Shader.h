#pragma once
#include <string>
#include <glm/glm.hpp>

namespace Almond {

	class Shader {
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void UnBind() const;

		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);	// 绑定Shader的Uniform变量
	private: 
		uint32_t m_RendererID;
	};


}