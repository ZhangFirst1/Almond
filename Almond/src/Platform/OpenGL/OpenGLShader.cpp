#include "ampch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>


namespace Almond {
	// 工具函数，用于将glsl中的标记（如vertex）抓换为OpenGL类型
	static GLenum ShaderTypeFromString(const std::string& type) {
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		AM_CORE_ASSERT(false, "Unkonw shader type!");
	}

	OpenGLShader::OpenGLShader(const std::string& filePath) {
		std::string source = ReadFile(filePath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		// 从路径中提取文件名
		// ../Texture.glsl
		auto lastSlash = filePath.find_last_of("\//");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filePath.rfind('.');
		auto count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filePath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		:m_Name(name){
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}

	OpenGLShader::~OpenGLShader() {
		glDeleteShader(m_RendererID);
	}

	std::string OpenGLShader::ReadFile(const std::string& filePath) {
		std::string result;
		std::ifstream in(filePath, std::ios::in | std::ios::binary);	// 以读取模式、二进制模式打开文件
		if (in) {
			in.seekg(0, std::ios::end);					// 设置文件读取位置为末尾，且不设方向。
			result.resize(in.tellg());					// 调整result大小，以容纳整个文件
			in.seekg(0, std::ios::beg);					// 调整回文件开头
			in.read(&result[0], result.size());			// 读取文件
			in.close();									// 关闭文件流
		}
		else {
			AM_CORE_ERROR("Could not open file '{0}'", filePath);
		}
		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source) {
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLen = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);					  // 找到第一个标记处
		while (pos != std::string::npos) {
			size_t eol = source.find_first_of("\r\n", pos);		  // 下一行
			AM_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + 1 + typeTokenLen;				  // #type vertex 中间还有一个空格
			std::string type = source.substr(begin, eol - begin); // 判断是vertex还是fragment
			AM_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type");

			size_t nextLinePos = source.find_first_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);			  // 寻找下一个标记
			// 若找到，添加映射
			shaderSources[ShaderTypeFromString(type)] =
				source.substr(nextLinePos,
					pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}
		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources) {
		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		GLuint program = glCreateProgram();
		AM_CORE_ASSERT(shaderSources.size() <= 2, "We just support 2 shaders now!");
		std::array<GLenum, 2> glShaderIDs;			// 着色器ID数组
		int glShaderIDIndex = 0;
		for (auto&& [type, source] : shaderSources) {
			// Create an empty vertex shader handle
			GLuint shader = glCreateShader(type);

			// Send the vertex shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			// 检查着色器编译是否出错
			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				AM_CORE_ERROR("{0}", infoLog.data());
				AM_CORE_ASSERT(false, "Shader compilation failure!");
				return;
			}
			// Attach our shaders to our program
			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		// 检查着色器连接是否正确
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			// 此处好像有些问题，maxLength并未正确赋值
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);

			for (auto id : glShaderIDs) {
				// Don't leak shaders either.
				glDeleteShader(id);
			}

			AM_CORE_ERROR("{0}", infoLog.data());
			AM_CORE_ASSERT(false, "Shader Link failure!");
			return;
		}

		for (auto id : glShaderIDs) {
			// Always detach shaders after a successful link.
			glDetachShader(program, id);
		}
		
		m_RendererID = program;
	}

	void OpenGLShader::Bind() const {
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::UnBind() const {
		glUseProgram(0);
	}
	void OpenGLShader::UploadUniformInt(const std::string& name, const int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}
	void OpenGLShader::UploadUniformFloat(const std::string& name, const float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}
	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}
	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}
	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}
	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}