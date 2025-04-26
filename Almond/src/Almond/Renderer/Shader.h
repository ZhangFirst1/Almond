#pragma once
#include <string>
#include <glm/glm.hpp>

namespace Almond {

	class Shader {
	public:
		virtual ~Shader() = default;

		// 绑定 / 解绑 着色器 在OpenGL中是glUseProgram
		virtual void Bind() const = 0;			
		virtual void UnBind() const = 0;

		// 设置shader中的Uniform变量
		virtual void SetInt(const std::string& name, const int value) = 0;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) = 0;
		virtual void SetFloat(const std::string& name, const float value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

		virtual const std::string& GetName() const = 0;

		// 可以通过着色器代码文件路径 或 直接通过源码创建shader
		static Ref<Shader> Create(const std::string& filePath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
	};

	// 用于管理多个着色器的类
	class ShaderLibrary {
	public:
		void Add(const Ref<Shader>& shader);				// 添加着色器
		void Add(const std::string& name, const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& filepath);		// 加载着色器，创建并添加
		Ref<Shader> Load(const std::string& name, const std::string& filepath);

		Ref<Shader> Get(const std::string& name);			// 根据名称获取着色器

		bool Exists(const std::string& name) const;			// 名称是否重复
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};

}