#pragma once
#include <string>

namespace Almond {

	class Shader {
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void UnBind() const;

	private: 
		unsigned int m_RendererID;
	};


}