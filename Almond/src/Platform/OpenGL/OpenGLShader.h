#pragma once
#include <string>
#include <glm/glm.hpp>

#include "Almond/Renderer/Shader.h"

typedef unsigned int GLenum;

namespace Almond {

	class OpenGLShader : public Shader{
	public:
		OpenGLShader(const std::string& filePath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const;
		virtual void UnBind() const;

		virtual const std::string& GetName() const { return m_Name; }

		void UploadUniformInt(const std::string& name, const int value);
		void UploadUniformFloat(const std::string& name, const float value);	// 绑定Shader的Uniform变量
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);	
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);	
	private:
		std::string ReadFile(const std::string& filePath);		// 读取着色器代码
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);	// 预处理着色器文件，分离不同的着色器代码
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);		
	private:
		uint32_t m_RendererID;
		std::string m_Name;
	};


}