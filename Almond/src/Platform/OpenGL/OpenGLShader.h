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
		
		// glUseProgram
		virtual void Bind() const;
		virtual void UnBind() const;

		// 绑定Shader的Uniform变量（调用UploadUniformxxx）
		virtual void SetInt(const std::string& name, const int value) override;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override;
		virtual void SetFloat(const std::string& name, const float value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;

		virtual const std::string& GetName() const { return m_Name; }

		// 绑定Shader的Uniform变量
		void UploadUniformInt(const std::string& name, const int value);
		void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);
		void UploadUniformFloat(const std::string& name, const float value);	
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);	
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);	
	private:
		std::string ReadFile(const std::string& filePath);								// 读取着色器代码
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);	// 预处理着色器文件，分离不同的着色器代码
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);		// 编译着色器
	private:
		uint32_t m_RendererID;
		std::string m_Name;
	};

}